
/// @file SortMgr.cc
/// @brief SortMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SortMgr.h"
#include "SortElem.h"
#include "SmtId.h"
#include "logic/SmtSolver.h"
#include "logic/SmtSortInfo.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

BEGIN_NONAMESPACE

// elem_list が等しいかチェックする．
// 等しいときに true を返す．
bool
check_elem(const SmtSortInfo* sort,
	   const vector<tSmtSortId>& elem_list)
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
	  const vector<tSmtSortId>& elem_list)
{
  ymuint h = name->id();
  for (vector<tSmtSortId>::const_iterator p = elem_list.begin();
       p != elem_list.end(); ++ p) {
    tSmtSortId sort1 = *p;
    h = h * 127 + sort1;
  }
  return h;
}

// ハッシュ関数
ymuint
hash_func(const SmtId* name,
	  const SmtSortInfo* sort)
{
  ymuint h = name->id();
  ymuint n = sort->elem_num();
  for (ymuint i = 0; i < n; ++ i) {
    tSmtSortId sort1 = sort->elem(i);
    h = h * 127 + sort1;
  }
  return h;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス SortMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] solver ソルバ
// @param[in] level スタックレベル
// @param[in] parent_mgr 上位のマネージャ
SortMgr::SortMgr(Alloc& alloc,
		 SmtSolver& solver,
		 ymuint level,
		 SortMgr* parent_mgr) :
  mAlloc(alloc),
  mSolver(solver),
  mLevel(level),
  mParent(parent_mgr)
{
  mNum1 = 0;
  mTableSize1 = 0;
  mHashTable1 = NULL;
  expand_table1(1024);

  mNum2 = 0;
  mTableSize2 = 0;
  mHashTable2 = NULL;
  expand_table2(1024);
}

// @brief デストラクタ
SortMgr::~SortMgr()
{
  delete [] mHashTable1;
  delete [] mHashTable2;
}

// @brief 型名を宣言する．
// @param[in] name_id 型名
// @param[in] param_num 引数の数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SortMgr::declare_sort(const SmtId* name_id,
		      ymuint param_num)
{
  ymuint dummy;
  if ( find_templ(name_id, dummy) != NULL ) {
    // 同じ名前の型が登録されている．
    return false;
  }

  vector<const SortElem*> elem_list(param_num);
  for (ymuint i = 0; i < param_num; ++ i) {
    elem_list[i] = make_param_sort_templ(i);
  }
  const SortElem* sort = make_sort_templ(name_id, elem_list);

  reg_templ(name_id, sort, param_num);

  return true;
}

// @breif パラメータ型のテンプレートを生成する．
// @param[in] param_id パラメータ番号
const SortElem*
SortMgr::make_param_sort_templ(ymuint param_id)
{
  while ( mParamArray.size() <= param_id ) {
    ymuint pid = mParamArray.size();
    void* p = mAlloc.get_memory(sizeof(ParamSort));
    ParamSort* sort = new (p) ParamSort(pid);
    mParamArray.push_back(sort);
  }
  return mParamArray[param_id];
}

// @brief 型テンプレートを生成する．
// @param[in] name_id 名前を表す識別子
// @param[in] elem_list 要素のリスト
const SortElem*
SortMgr::make_sort_templ(const SmtId* name_id,
			 const vector<const SortElem*>& elem_list)
{
  ymuint n = elem_list.size();
  if ( n == 0 ) {
    void* p = mAlloc.get_memory(sizeof(SimpleSort));
    SimpleSort* sort = new (p) SimpleSort(name_id);
    return sort;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(ComplexSort) + sizeof(const SortElem*) * (n - 1));
    ComplexSort* sort = new (p) ComplexSort(name_id, elem_list);
    return sort;
  }
}

BEGIN_NONAMESPACE

void
get_param_num(const SortElem* sort,
	      vector<bool>& param_map)
{
  if ( sort->is_param() ) {
    ymuint pid = sort->param_id();
    while ( param_map.size() <= pid ) {
      param_map.push_back(false);
    }
    param_map[pid] = true;
  }
  else {
    ymuint n = sort->elem_num();
    for (ymuint i = 0; i < n; ++ i) {
      get_param_num(sort->elem(i), param_map);
    }
  }
}

END_NONAMESPACE

// @brief 型名を定義する．
// @param[in] name_id 型名を表す識別子
// @param[in] sort 登録する型テンプレート
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SortMgr::define_sort(const SmtId* name_id,
		     const SortElem* sort)
{
  ymuint dummy;
  if ( find_templ(name_id, dummy) != NULL ) {
    // 同じ名前の型が登録されている．
    return false;
  }

  vector<bool> param_map;
  get_param_num(sort, param_map);
  ymuint param_num = param_map.size();
  for (ymuint i = 0; i < param_num; ++ i) {
    if ( !param_map[i] ) {
      // パラメータが歯抜けだった．
      return false;
    }
  }

  reg_templ(name_id, sort, param_num);

  return true;
}

// @brief 型を登録する．
// @param[in] name_id 型名
// @param[in] sort 登録する型テンプレート
// @param[in] param_num パラメータ数
void
SortMgr::reg_templ(const SmtId* name_id,
		   const SortElem* sort,
		   ymuint param_num)
{
  if ( mNum1 >= mNextLimit1 ) {
    expand_table1(mTableSize1 * 2);
  }

  ymuint idx = name_id->id() % mTableSize1;

  void* p = mAlloc.get_memory(sizeof(Cell1));
  Cell1* cell = new (p) Cell1;
  cell->mId = name_id;
  cell->mSort = sort;
  cell->mParamNum = param_num;

  cell->mLink = mHashTable1[idx];
  mHashTable1[idx] = cell;

  ++ mNum1;
}

// @brief 型テンプレートを探す．
// @param[in] name_id 型名を表す識別子
// @param[out] param_num パラメータ数
// @return 対象の型テンプレートを返す．
// @note 見つからなかったら NULL を返す．
const SortElem*
SortMgr::find_templ(const SmtId* name_id,
		    ymuint& param_num)
{
  if ( mParent != NULL ) {
    // 親のレベルで探す
    const SortElem* sort = mParent->find_templ(name_id, param_num);
    if ( sort != NULL ) {
      return sort;
    }
  }

  ymuint h = name_id->id();
  ymuint idx = h % mTableSize1;
  for (Cell1* cell = mHashTable1[idx];
       cell != NULL; cell = cell->mLink) {
    if ( cell->mId == name_id ) {
      param_num = cell->mParamNum;
      return cell->mSort;
    }
  }

  return NULL;
}

// @brief 型を作る．
// @param[in] name_id 型名
// @param[in] param_list 部品の型のリスト
// @return 引数に合致する型を返す．
tSmtSortId
SortMgr::make_sort(const SmtId* name_id,
		   const vector<tSmtSortId>& param_list)
{
  // 型宣言を探す．
  ymuint param_num = 0;
  const SortElem* sort_templ = find_templ(name_id, param_num);
  if ( sort_templ == NULL ) {
    // name_id という型は登録されていなかった．
    return kSmtSort_None;
  }
  if ( param_num != param_list.size() ) {
    // 引数の数が合わない．
    return kSmtSort_None;
  }

  // パラメータを置き換えて実際の型を作る．
  tSmtSortId sort = replace_param(sort_templ, param_list);

  return sort;
}

// @brief 型を作る．
// @param[in] name_id 型名を表す識別子
// @param[in] param_list 要素の型のリスト
// @return 生成した型を返す．
tSmtSortId
SortMgr::_make_sort(const SmtId* name_id,
		    const vector<tSmtSortId>& param_list)
{
  // 型のインスタンスを探す．
  const SmtSortInfo* sort = find_sort(name_id, param_list);
  if ( sort != NULL ) {
    // 見つかった．
    return sort->id();
  }

  // 型を作る．
  tSmtSortId sort_id = mSolver.make_sort(param_list);
  if ( sort_id == kSmtSort_None ) {
    return kSmtSort_None;
  }

  sort = mSolver.get_sort(sort_id);

  // 登録する．
  reg_sort(name_id, sort);

  return sort_id;
}

// @brief 型を登録する．
// @param[in] name_id 型名
// @param[in] sort 登録する型
void
SortMgr::reg_sort(const SmtId* name_id,
		  const SmtSortInfo* sort)
{
  // ハッシュ表に登録する．
  if ( mNum2 >= mNextLimit2 ) {
    expand_table2(mTableSize2 * 2);
  }

  void* p = mAlloc.get_memory(sizeof(Cell2));
  Cell2* cell = new (p) Cell2;
  cell->mId = name_id;
  cell->mSort = sort;

  ymuint h = hash_func(name_id, sort);
  ymuint idx = h % mTableSize2;

  cell->mLink = mHashTable2[idx];
  mHashTable2[idx] = cell;

  ++ mNum2;
}

// @brief 型を探す．
// @param[in] name_id 型名
// @param[in] elem_list 部品の型のリスト
// @return 登録されていなければ NULL を返す．
const SmtSortInfo*
SortMgr::find_sort(const SmtId* name_id,
		   const vector<tSmtSortId>& elem_list)
{
  if ( mParent != NULL ) {
    // 親のレベルで探す．
    const SmtSortInfo* sort = mParent->find_sort(name_id, elem_list);
    if ( sort != NULL ) {
      return sort;
    }
  }

  // 同じ型が存在するか調べる．
  ymuint h = hash_func(name_id, elem_list);
  ymuint idx = h % mTableSize2;
  for (Cell2* cell = mHashTable2[idx]; cell != NULL; cell = cell->mLink) {
    const SmtSortInfo* sort = cell->mSort;
    if ( cell->mId == name_id && check_elem(sort, elem_list) ) {
      // 見つかった．
      return sort;
    }
  }

  // 見つからなかった．
  return NULL;
}

// @brief テンプレートから実際の型を作る．
// @param[in] templ テンプレート
// @param[in] param_list パラメータリスト
tSmtSortId
SortMgr::replace_param(const SortElem* templ,
		       const vector<tSmtSortId>& param_list)
{
  if ( templ->is_param() ) {
    // パラメータ型
    ymuint pid = templ->param_id();
    assert_cond( pid < param_list.size(), __FILE__, __LINE__);
    return param_list[pid];
  }

  ymuint n = templ->elem_num();
  vector<tSmtSortId> elem_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    elem_list[i] = replace_param(templ->elem(i), param_list);
  }
  return _make_sort(templ->name(), elem_list);
}

// @brief 型テンプレート用のハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SortMgr::expand_table1(ymuint req_size)
{
  ymuint old_size = mTableSize1;
  Cell1** old_table = mHashTable1;

  mTableSize1 = req_size;
  mHashTable1 = new Cell1*[mTableSize1];
  for (ymuint i = 0; i < mTableSize1; ++ i) {
    mHashTable1[i] = NULL;
  }
  mNextLimit1 = static_cast<ymuint32>(mTableSize1 * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell1* cell = old_table[i]; cell != NULL; ) {
	Cell1* tmp_cell = cell;
	cell = cell->mLink;

	ymuint idx = tmp_cell->mSort->name()->id() % mTableSize1;
	tmp_cell->mLink = mHashTable1[idx];
	mHashTable1[idx] = tmp_cell;
      }
    }
    delete [] old_table;
  }
}

// @brief ハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SortMgr::expand_table2(ymuint req_size)
{
  ymuint old_size = mTableSize2;
  Cell2** old_table = mHashTable2;

  mTableSize2 = req_size;
  mHashTable2 = new Cell2*[mTableSize2];
  for (ymuint i = 0; i < mTableSize2; ++ i) {
    mHashTable2[i] = NULL;
  }
  mNextLimit2 = static_cast<ymuint32>(mTableSize2 * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell2* cell = old_table[i]; cell != NULL; ) {
	Cell2* tmp_cell = cell;
	cell = cell->mLink;

	const SmtId* id = cell->mId;
	const SmtSortInfo* sort = cell->mSort;
	ymuint h = hash_func(id, sort) % mTableSize2;
	tmp_cell->mLink = mHashTable2[h];
	mHashTable2[h] = tmp_cell;
      }
    }
    delete [] old_table;
  }
}

END_NAMESPACE_YM_SMTLIBV2
