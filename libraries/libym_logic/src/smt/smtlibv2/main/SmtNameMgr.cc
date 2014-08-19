
/// @file SmtNameMgr.cc
/// @brief SmtNameMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtNameMgr.h"
#include "SmtVarFunImpl.h"
#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtNameMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] level スタックレベル
// @param[in] parent_mgr 上位のマネージャ
SmtNameMgr::SmtNameMgr(Alloc& alloc,
		       ymuint level,
		       SmtNameMgr* parent_mgr) :
  mAlloc(alloc),
  mLevel(level),
  mParent(parent_mgr)
{
  mNum = 0;
  mTableSize = 0;
  mHashTable = NULL;

  expand_table(1024);
}

// @brief デストラクタ
SmtNameMgr::~SmtNameMgr()
{
  delete [] mHashTable;
}

// @brief 変数を登録する．
// @param[in] name_id 名前を表す識別子
// @param[in] sort 型
// @return 登録した変数を返す．
// @note エラーが起きたら NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id と同名の定義が既に存在している．
const SmtVarFun*
SmtNameMgr::reg_var(const SmtId* name_id,
		    const SmtSort* sort)
{
  if ( find_obj(name_id) != NULL ) {
    // 同名のオブジェクトが登録されている．
    return NULL;
  }

  void* p = mAlloc.get_memory(sizeof(SmtGlobalVar));
  SmtVarFunImpl* var = new (p) SmtGlobalVar(name_id, sort);

  reg_sub(var);

  return var;
}

// @brief 宣言のみの関数を登録する．
// @param[in] name_id 名前を表す識別子
// @param[in] input_list 入力の型のリスト
// @param[in] output_sort 出力の型
// @param[in] fun_attr 属性
// @return 登録した関数を返す．
// @note エラーが起きたら NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id と同名の関数がすでに存在している．
const SmtVarFun*
SmtNameMgr::reg_fun(const SmtId* name_id,
		    const vector<const SmtSort*>& input_list,
		    const SmtSort* output_sort,
		    SmtVarFun::tFunAttr fun_attr)
{
  if ( find_obj(name_id) != NULL ) {
    // 同名のオブジェクトが登録されている．
    return NULL;
  }

  SmtVarFunImpl* fun = NULL;
  ymuint n = input_list.size();
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtDeclFun1));
    fun = new (p) SmtDeclFun1(name_id, output_sort);
  }
  else {
    // input_list/output_sort をスキャンしてパラメータの数を数える．
    vector<bool> param_array;
    param_array.reserve(n);
    for (ymuint i = 0; i < n; ++ i) {
      const SmtSort* sort = input_list[i];
      if ( sort->is_param() ) {
	ymuint pid = sort->param_id();
	while ( param_array.size() <= pid ) {
	  param_array.push_back(false);
	}
	param_array[pid] = true;
      }
    }
    if ( output_sort->is_param() ) {
      ymuint pid = output_sort->param_id();
      while ( param_array.size() <= pid ) {
	param_array.push_back(false);
      }
      param_array[pid] = true;
    }
    ymuint param_num = param_array.size();
    for (ymuint i = 0; i < param_num; ++ i) {
      if ( !param_array[i] ) {
	// 歯抜けなのでエラー
	return NULL;
      }
    }
    void* p = mAlloc.get_memory(sizeof(SmtDeclFun2) + sizeof(const SmtSort*) * (n - 1));
    fun = new (p) SmtDeclFun2(name_id, input_list, output_sort, fun_attr, param_num);
  }

  reg_sub(fun);

  return fun;
}

// @brief 実体のある関数を登録する．
// @param[in] name_id 名前を表す識別子
// @param[in] output_sort 出力の型
// @param[in] input_list 入力の型と変数のリスト
// @param[in] body 本体
// @return 登録した関数を返す．
// @note エラーが起きたら NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id と同名の関数がすでに存在している．
const SmtVarFun*
SmtNameMgr::reg_fun(const SmtId* name_id,
		    const vector<const SmtVarFun*>& input_list,
		    const SmtSort* output_sort,
		    const SmtTerm* body)
{
  if ( find_obj(name_id) != NULL ) {
    // 同名のオブジェクトが登録されている．
    return NULL;
  }

  SmtVarFunImpl* fun = NULL;
  ymuint n = input_list.size();
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SmtDefFun1));
    fun = new (p) SmtDefFun1(name_id, output_sort, body);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtDefFun2) + sizeof(const SmtVarFun*) * (n - 1));
    fun = new (p) SmtDefFun2(name_id, input_list, output_sort, body);
  }

  reg_sub(fun);

  return fun;
}

// @brief 名前からオブジェクトを探す．
// @param[in] name_id 名前を表す識別子
// @return name_id という名のオブジェクトを返す．
// @note 存在しなければ NULL を返す．
const SmtVarFun*
SmtNameMgr::find_obj(const SmtId* name_id) const
{
  if ( mParent != NULL ) {
    // 親のレベルで探す．
    const SmtVarFun* obj = mParent->find_obj(name_id);
    if ( obj != NULL ) {
      return obj;
    }
  }

  ymuint h = name_id->id();
  ymuint idx = h % mTableSize;
  for (SmtVarFunImpl* obj = mHashTable[idx];
       obj != NULL; obj = obj->mLink) {
    if ( obj->name() == name_id ) {
      return obj;
    }
  }
  return NULL;
}

// @brief reg_fun() の下請け関数
// @param[in] obj 登録するオブジェクト
void
SmtNameMgr::reg_sub(SmtVarFunImpl* obj)
{
  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
  }

  ++ mNum;

  ymuint h = obj->name()->id() % mTableSize;
  obj->mLink = mHashTable[h];
  mHashTable[h] = obj;
}

// @brief ハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SmtNameMgr::expand_table(ymuint req_size)
{
  ymuint old_size = mTableSize;
  SmtVarFunImpl** old_table = mHashTable;

  mTableSize = req_size;
  mHashTable = new SmtVarFunImpl*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (SmtVarFunImpl* obj = old_table[i]; obj != NULL; ) {
	SmtVarFunImpl* tmp_obj = obj;
	obj = obj->mLink;

	ymuint h = tmp_obj->name()->id() % mTableSize;
	tmp_obj->mLink = mHashTable[h];
	mHashTable[h] = tmp_obj;
      }
    }
    delete [] old_table;
  }
}

END_NAMESPACE_YM_SMTLIBV2
