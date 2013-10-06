
/// @file SmtSortMgr.cc
/// @brief SmtSortMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtSortMgr.h"
#include "ym_smtlibv2/SmtId.h"
#include "SmtSortImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtSortMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
SmtSortMgr::SmtSortMgr(Alloc& alloc) :
  mAlloc(alloc)
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
  hash_map<ymuint32, ymuint32>::iterator p = mHash.find(name->id());
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
  if ( sort->elem_num() != n ) {
    return false;
  }
  for (ymuint i = 0; i < n; ++ i) {
    if ( sort->elem(i) != elem_list[i] ) {
      return false;
    }
  }
  return true;
}

// ハッシュ関数
ymuint
hash_func(const SmtId* name,
	  const vector<const SmtSort*>& elem_list)
{
  ymuint h = name->id();
  for (vector<const SmtSort*>::const_iterator p = elem_list.begin();
       p != elem_list.end(); ++ p) {
    const SmtSort* sort = *p;
    h = h * 127 + sort->id();
  }
  return h;
}

END_NONAMESPACE

// @brief alias を登録する．
// @param[in] name 型名
// @param[in] sort 登録する型
// @retval true 登録が成功した．
// @retval false 登録が失敗した．同名で異なる alias が登録されている．
bool
SmtSortMgr::reg_alias(const SmtId* name,
		      ymuint arg_num,
		      const SmtSort* sort)
{
  hash_map<ymuint32, ymuint32>::iterator p = mHash.find(name->id());
  if ( p == mHash.end() ) {
    mHash.insert(make_pair(name->id(), arg_num));
  }
  else {
    if ( p->second != arg_num ) {
      // 同名で異なる宣言が登録されている．
      return false;
    }
  }

  ymuint h = hash_func(name, vector<const SmtSort*>(0));
  ymuint idx = h % mTableSize;
  for (SmtSortImpl* sort1 = mHashTable[idx]; sort1 != NULL; sort1 = sort1->mLink) {
    if ( sort1->name() == name ) {
      // 同名の定義がすでに存在している．
      return false;
    }
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
    idx = h % mTableSize;
  }

  void* q = mAlloc.get_memory(sizeof(SmtAliasSort));
  SmtSortImpl* sort1 = new (q) SmtAliasSort(name, sort);

  sort1->mId = mNum;
  ++ mNum;

  sort1->mLink = mHashTable[idx];
  mHashTable[idx] = sort1;

  return sort1;
}


// @brief SmtSort に変換する．
// @param[in] name 型名
// @param[in] elem_list 部品の型のリスト
// @return 登録されていなければ NULL を返す．
const SmtSort*
SmtSortMgr::new_sort(const SmtId* name,
		     const vector<const SmtSort*>& elem_list)
{
  hash_map<ymuint32, ymuint32>::iterator p = mHash.find(name->id());
  if ( p == mHash.end() ) {
    // name という型名は登録されていなかった．
    return NULL;
  }

  ymuint arg_num = p->second;
  if ( arg_num != elem_list.size() ) {
    // 引数の数が合わない．
    return NULL;
  }

  // name がパラメータ付きの型 alias の可能性を調べる．
  // elem_list が空の場合には本物と区別はない．
  ymuint h = hash_func(name, vector<const SmtSort*>(0));
  ymuint idx = h % mTableSize;
  SmtSortImpl* sort = NULL;
  for (SmtSortImpl* sort = mHashTable[idx]; sort != NULL; sort = sort->mLink) {
    if ( sort->name() == name && sort->elem_num() == 0) {
      if ( elem_list.empty() ) {
	return sort;
      }
      // テンプレートから実際の型を作る．
      sort = replace_param(sort->sort_template(), elem_list);
      break;
    }
  }

  if ( sort == NULL ) {
    // 同じ型が存在するか調べる．
    h = hash_func(name, elem_list);
    ymuint idx = h % mTableSize;
    for (SmtSortImpl* sort = mHashTable[idx]; sort != NULL; sort = sort->mLink) {
      if ( sort->name() == name && check_elem(sort, elem_list) ) {
	return sort;
      }
    }
  }

  if ( sort == NULL ) {
    if ( elem_list.empty() ) {
      sort = new_simple_sort(name);
    }
    else {
      sort = new_complex_sort(name, elem_list);
    }
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
    idx = h % mTableSize;
  }

  sort->mId = mNum;
  ++ mNum;

  sort->mLink = mHashTable[idx];
  mHashTable[idx] = sort;

  return sort;
}

// @brief テンプレートから実際の型を作る．
// @param[in] templ テンプレート
// @param[in] param_list パラメータリスト
SmtSortImpl*
SmtSortMgr::replace_param(const SmtSort* templ,
			  const vector<const SmtSort*>& param_list)
{
  // replace_param_sub() の複合型の処理だけ抜き出したもの
  // なぜかというと， SmtSortImpl を返せるのは新しいクラスを作ったときだけだから
  ymuint n = templ->elem_num();
  assert_cond( n > 0, __FILE__, __LINE__);

  vector<const SmtSort*> elem_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    elem_list[i] = replace_param_sub(templ->elem(i), param_list);
  }
  return new_complex_sort(templ->name(), elem_list);
}

// @brief replace_param の下請け関数
// @param[in] templ テンプレート
// @param[in] param_list パラメータリスト
const SmtSort*
SmtSortMgr::replace_param_sub(const SmtSort* templ,
			      const vector<const SmtSort*>& param_list)
{
  ymuint n = templ->elem_num();
  if ( n == 0 ) {
    ymint pid = templ->param_id();
    if ( pid >= 0 ) {
      if ( static_cast<ymuint>(pid) >= param_list.size() ) {
	// エラー
	return NULL;
      }
      return param_list[pid];
    }
    return templ;
  }
  else {
    vector<const SmtSort*> elem_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      elem_list[i] = replace_param_sub(templ->elem(i), param_list);
    }
    return new_complex_sort(templ->name(), elem_list);
  }
}

// @brief 型パラメータを作る．
// @param[in] pid パラメータ番号
const SmtSort*
SmtSortMgr::new_sort_param(ymuint pid)
{
  void* p = mAlloc.get_memory(sizeof(SmtParamSort));
  SmtSort* sort = new (p) SmtParamSort(pid);
  return sort;
}

// @brief 単純な型を作る．
SmtSortImpl*
SmtSortMgr::new_simple_sort(const SmtId* name)
{
  void* p = mAlloc.get_memory(sizeof(SmtSortImpl));
  SmtSortImpl* sort = new (p) SmtSortImpl(name);
  return sort;
}

// @brief 複合型を作る．
SmtSortImpl*
SmtSortMgr::new_complex_sort(const SmtId* name,
			     const vector<const SmtSort*>& elem_list)
{
  ymuint n = elem_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtCplxSort) + sizeof(const SmtSort*) * (n - 1));
  SmtCplxSort* sort = new (p) SmtCplxSort(name, elem_list);
  return sort;
}


BEGIN_NONAMESPACE

// ハッシュ関数
ymuint
hash_func(SmtSortImpl* sort)
{
  ymuint h = sort->name()->id();
  ymuint n = sort->elem_num();
  for (ymuint i = 0; i < n; ++ i) {
    h = h * 12 + sort->elem(i)->id();
  }
  return h;
}

END_NONAMESPACE

// @brief ハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SmtSortMgr::expand_table(ymuint req_size)
{
  ymuint old_size = mTableSize;
  SmtSortImpl** old_table = mHashTable;

  mTableSize = req_size;
  mHashTable = new SmtSortImpl*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (SmtSortImpl* sort = old_table[i]; sort != NULL; ) {
	SmtSortImpl* tmp_sort = sort;
	sort = sort->mLink;

	ymuint h = hash_func(tmp_sort) % mTableSize;
	tmp_sort->mLink = mHashTable[h];
	mHashTable[h] = tmp_sort;
      }
    }
    delete [] old_table;
  }
}

END_NAMESPACE_YM_SMTLIBV2
