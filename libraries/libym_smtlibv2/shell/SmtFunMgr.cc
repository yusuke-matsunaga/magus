
/// @file SmtFunMgr.cc
/// @brief SmtFunMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtFunMgr.h"
#include "SmtFunImpl.h"
#include "ym_smtlibv2/SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtFunMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] level スタックレベル
// @param[in] parent_mgr 上位のマネージャ
SmtFunMgr::SmtFunMgr(Alloc& alloc,
		     ymuint level,
		     SmtFunMgr* parent_mgr) :
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
SmtFunMgr::~SmtFunMgr()
{
  delete [] mHashTable;
}

BEGIN_NONAMESPACE

// 関数が等しいかチェックする
bool
check_fun(const SmtFun* fun,
	  const vector<const SmtSort*>& input_list,
	  const SmtSort* sort)
{
  if ( fun->body() != NULL ) {
    return false;
  }
  ymuint n = input_list.size();
  if ( fun->sort() != sort ||
       fun->input_num() != n ) {
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( fun->input_sort(i) != input_list[i] ) {
      return false;
    }
  }
  return true;
}

END_NONAMESPACE

// @brief 宣言のみの関数を返す．
// @param[in] name 名前
// @param[in] sort 出力の型
// @param[in] input_list 入力の型のリスト
// @param[in] attr 属性
// @param[in] param_num パラメータの数
const SmtFun*
SmtFunMgr::reg_fun(const SmtId* name,
		   const vector<const SmtSort*>& input_list,
		   const SmtSort* sort,
		   SmtFun::tAttr attr,
		   ymuint param_num)
{
  {
    const SmtFun* fun = find_fun(name);
    if ( fun != NULL ) {
      if ( check_fun(fun, input_list, sort) ) {
	return fun;
      }
      // 同名で形の異なる関数が登録されている．
      return NULL;
    }
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
  }

  ymuint n = input_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtFun1) + sizeof(const SmtSort*) * (n - 1));
  SmtFun1* fun = new (p) SmtFun1(name, sort, n, attr, param_num);

  fun->mId = mNum;
  ++ mNum;

  for (ymuint i = 0; i < n; ++ i) {
    fun->mInputList[i] = input_list[i];
  }

  ymuint h = name->id() % mTableSize;
  fun->mLink = mHashTable[h];
  mHashTable[h] = fun;

  return fun;
}

BEGIN_NONAMESPACE

// 関数が等しいかチェックする
bool
check_fun(const SmtFun* fun,
	  const vector<SmtSortedVar>& input_list,
	  const SmtSort* sort,
	  const SmtTerm* body)
{
  if ( fun->body() != body ) {
    return false;
  }

  ymuint n = input_list.size();
  if ( fun->sort() != sort ||
       fun->input_num() != n ) {
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( fun->input_sort(i) != input_list[i].mSort ||
	 fun->input_var(i) != input_list[i].mVar ) {
      return false;
    }
  }
  return true;
}

END_NONAMESPACE

// @brief 宣言のみの関数を返す．
// @param[in] name 名前
// @param[in] sort 出力の型
// @param[in] input_list 入力の型と変数のリスト
// @param[in] body 本体
// @note input_list と input_var_list のサイズは同じ
const SmtFun*
SmtFunMgr::reg_fun(const SmtId* name,
		   const vector<SmtSortedVar>& input_list,
		   const SmtSort* sort,
		   const SmtTerm* body)
{
  {
    const SmtFun* fun = find_fun(name);
    if ( fun != NULL ) {
      if ( check_fun(fun, input_list, sort, body) ) {
	return fun;
      }
      // 同名で形の異なる関数が登録されている．
      return NULL;
    }
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
  }

  ymuint n = input_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtFun2) + sizeof(SmtSortedVar) * (n - 1));
  SmtFun2* fun = new (p) SmtFun2(name, sort, n, body);

  fun->mId = mNum;
  fun->mLevel = mLevel;
  ++ mNum;

  for (ymuint i = 0; i < n; ++ i) {
    fun->mInputList[i] = input_list[i];
  }

  ymuint h = name->id() % mTableSize;
  fun->mLink = mHashTable[h];
  mHashTable[h] = fun;

  return fun;
}

// @brief 関数を返す．
// @param[in] name 名前
const SmtFun*
SmtFunMgr::find_fun(const SmtId* name) const
{
  if ( mParent != NULL ) {
    const SmtFun* fun = mParent->find_fun(name);
    if ( fun != NULL ) {
      return fun;
    }
  }

  ymuint h = name->id();
  ymuint idx = h % mTableSize;
  for (SmtFunImpl* fun = mHashTable[idx];
       fun != NULL; fun = fun->mLink) {
    if ( fun->name() == name ) {
      return fun;
    }
  }
  return NULL;
}

// @brief ハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SmtFunMgr::expand_table(ymuint req_size)
{
  ymuint old_size = mTableSize;
  SmtFunImpl** old_table = mHashTable;

  mTableSize = req_size;
  mHashTable = new SmtFunImpl*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (SmtFunImpl* id = old_table[i]; id != NULL; ) {
	SmtFunImpl* tmp_id = id;
	id = id->mLink;

	ymuint h = tmp_id->hash() % mTableSize;
	tmp_id->mLink = mHashTable[h];
	mHashTable[h] = tmp_id;
      }
    }
    delete [] old_table;
  }
}

END_NAMESPACE_YM_SMTLIBV2
