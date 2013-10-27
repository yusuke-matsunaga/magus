
/// @file NameMgr.cc
/// @brief NameMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "NameMgr.h"
#include "NameObjImpl.h"
#include "ym_smtlibv2/SmtId.h"
//#include "ym_smtlibv2/SmtSort.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス NameMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] level スタックレベル
// @param[in] parent_mgr 上位のマネージャ
NameMgr::NameMgr(Alloc& alloc,
		 ymuint level,
		 NameMgr* parent_mgr) :
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
NameMgr::~NameMgr()
{
  delete [] mHashTable;
}

// @brief 組み込み関数を登録する．
// @param[in] name_id 名前を表す識別子
// @param[in] fun_type 関数の型
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - name_id と同名の関数がすでに存在している．
bool
NameMgr::reg_builtin_fun(const SmtId* name_id,
			 tSmtFun fun_type)
{
  if ( find_obj(name_id) != NULL ) {
    // 同名のオブジェクトが登録されている．
    return false;
  }

  void* p = mAlloc.get_memory(sizeof(BuiltinFunObj));
  NameObjImpl* obj = new (p) BuiltinFunObj(name_id, fun_type);

  reg_sub(obj);

  return true;
}

// @brief 関数を登録する．
// @param[in] name_id 名前を表す識別子
// @param[in] fun 関数
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - name_id と同名の定義がすでに存在している．
bool
NameMgr::reg_fun(const SmtId* name_id,
		 const SmtFun* fun)
{
  if ( find_obj(name_id) != NULL ) {
    // 同名のオブジェクトが登録されている．
    return false;
  }

  void* p = mAlloc.get_memory(sizeof(FunObj));
  NameObjImpl* obj = new (p) FunObj(name_id, fun);

  reg_sub(obj);

  return true;
}

// @brief 変数を登録する．
// @param[in] name_id 名前を表す識別子
// @param[in] var 変数
// @retval true 処理が成功した．
// @retval false 処理が失敗した．
//
// エラーの原因は以下のとおり
//  - name_id と同名の定義が既に存在している．
bool
NameMgr::reg_var(const SmtId* name_id,
		 const SmtVar* var)
{
  if ( find_obj(name_id) != NULL ) {
    // 同名のオブジェクトが登録されている．
    return false;
  }

  void* p = mAlloc.get_memory(sizeof(VarObj));
  NameObjImpl* obj = new (p) VarObj(name_id, var);

  reg_sub(obj);

  return true;
}

// @brief 名前からオブジェクトを探す．
// @param[in] name_id 名前を表す識別子
// @return name_id という名のオブジェクトを返す．
// @note 存在しなければ NULL を返す．
const NameObj*
NameMgr::find_obj(const SmtId* name_id) const
{
  if ( mParent != NULL ) {
    // 親のレベルで探す．
    const NameObj* obj = mParent->find_obj(name_id);
    if ( obj != NULL ) {
      return obj;
    }
  }

  ymuint h = name_id->id();
  ymuint idx = h % mTableSize;
  for (NameObjImpl* obj = mHashTable[idx];
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
NameMgr::reg_sub(NameObjImpl* obj)
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
NameMgr::expand_table(ymuint req_size)
{
  ymuint old_size = mTableSize;
  NameObjImpl** old_table = mHashTable;

  mTableSize = req_size;
  mHashTable = new NameObjImpl*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (NameObjImpl* obj = old_table[i]; obj != NULL; ) {
	NameObjImpl* tmp_obj = obj;
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
