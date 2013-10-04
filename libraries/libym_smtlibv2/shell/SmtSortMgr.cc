
/// @file SmtSortMgr.cc
/// @brief SmtSortMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSortMgr.h"
#include "ym_smtlibv2/SmtSort.h"
#include "SmtCplxSort.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtSortMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtSortMgr::SmtSortMgr()
{
  mNum = 0;
  mTableSize = 0;
  mHashTable = NULL;

  expand_table(1024);
}

// @brief デストラクタ
SmtSortMgr::~SmtSortMgr()
{
  delete [] mHashTable;
}

// @brief 型名を登録する．
// @param[in] name 型名
// @param[in] arg_num 引数の数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．同名で異なる宣言がすでに登録されている．
bool
SmtSortMgr::reg_sort(const SmtId* name,
		     ymuint arg_num)
{
  hash_map<ymuint32, ymint32>::iterator p = mHash.find(name->id());
  if ( p != mHash.end() ) {
    if ( p->second != arg_num ) {
      return false;
    }
    return true;
  }
  mHash.insert(make_pair(name->id(), arg_num));
  return true;
}

BEGIN_NONAMESPACE

// elem_list が等しいかチェックする．
// 等しいときに true を返す．
bool
check_elem(const SmtSort* sort,
	   const vector<const SmtSort*>& elem_list)
{
  ymuint n = elem_list.size();
  if ( sort->elem_num != n ) {
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( sort->elem(i) != elem_list[i] ) {
      return false;
    }
  }
  return true;
}

END_NONAMESPACE

// @brief SmtSort に変換する．
// @param[in] name 型名
// @param[in] elem_list 部品の型のリスト
// @return 登録されていなければ NULL を返す．
const SmtSort*
SmtSortMgr::new_sort(const SmtId* name,
		     const vector<const SmtSort*>& elem_list)
{
  hash_map<ymuint32, ymint32>::iterator p = mHash.find(name->id());
  if ( p == mHash.end() ) {
    // name という型名は登録されていなかった．
    return NULL;
  }

  ymint arg_num = p->second;
  if ( arg_num != elem_list.size() ) {
    // 引数の数が合わない．
    return NULL;
  }

  ymuint h = hash(name, elem_list);
  ymuint idx = h % mTableSize;
  for (const SmtSort* sort = mHashTable[idx]; sort != NULL; sort = sort->mLink) {
    if ( sort->name() == name && check_elem(sort, elem_list) ) {
      return sort;
    }
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
    idx = h % mTableSize;
  }

  SmtSort* sort = NULL;
  if ( elem_list.empty() ) {
    void* p = mAlloc.get_memory(sizeof(SmtSort));
    sort = new (p) SmtSort(name);
  }
  else {
    ymuint n = elem_list.size();
    void* p = mAlloc.get_memory(sizeof(SmtCplxSort) + sizeof(const SmtSort*) * (n - 1));
    sort = new (p) SmtCpxSort(name);
    sort->mElemNum = n;
    for (ymuint i = 0; i < n; ++ i) {
      sort->mElemList[i] = elem_list[i];
    }
  }

  sort->mId = mNum;
  ++ mNum;

  sort->mLink = mHashTable[idx];
  mHashTable[idx] = sort;

  return sort;
}

END_NAMESPACE_YM_SMTLIBV2
