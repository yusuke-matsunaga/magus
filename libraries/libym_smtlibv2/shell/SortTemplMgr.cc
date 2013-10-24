
/// @file SortTemplMgr.cc
/// @brief SortTemplMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SortTemplMgr.h"
#include "SortElem.h"
#include "ym_smtlibv2/SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SortTemplMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] level スタックレベル
// @param[in] parent_mgr 上位のマネージャ
SortTemplMgr::SortTemplMgr(Alloc& alloc,
			   ymuint level,
			   SortTemplMgr* parent_mgr) :
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
SortTemplMgr::~SortTemplMgr()
{
  delete [] mHashTable;
}

// @brief 型名を登録する．
// @param[in] name_id 型名
// @param[in] param_num 引数の数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SortTemplMgr::reg_sort(const SmtId* name_id,
		       ymuint param_num)
{
  ymuint dummy;
  if ( find_sort(name_id, dummy) != NULL ) {
    // 同じ名前の型が登録されている．
    return false;
  }

  vector<const SortElem*> elem_list(param_num);
  for (ymuint i = 0; i < param_num; ++ i) {
    elem_list[i] = new_param_sort(i);
  }
  SortElem* sort = new_complex_sort(name_id, elem_list);

  reg_common(sort, param_num);

  return true;
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

// @brief alias を登録する．
// @param[in] name_id 型名
// @param[in] sort 登録する型テンプレート
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SortTemplMgr::reg_alias(const SmtId* name_id,
			SortElem* sort)
{
  ymuint dummy;
  if ( find_sort(name_id, dummy) != NULL ) {
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

  reg_common(sort, param_num);

  return true;
}

// @brief 型を登録する．
// @param[in] sort 登録する型テンプレート
// @param[in] param_num パラメータ数
void
SortTemplMgr::reg_common(SortElem* sort,
			 ymuint param_num)
{
  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
  }

  ymuint idx = sort->name()->id() % mTableSize;

  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mSort = sort;
  cell->mParamNum = param_num;

  cell->mLink = mHashTable[idx];
  mHashTable[idx] = cell;

  ++ mNum;
}

// @brief 型テンプレートを探す．
// @param[in] name_id 型名を表す識別子
// @param[out] param_num パラメータ数
// @return 対象の型テンプレートを返す．
// @note 見つからなかったら NULL を返す．
SortElem*
SortTemplMgr::find_sort(const SmtId* name_id,
			ymuint& param_num)
{
  if ( mParent != NULL ) {
    // 親のレベルで探す
    SortElem* sort = mParent->find_sort(name_id, param_num);
    if ( sort != NULL ) {
      return sort;
    }
  }

  ymuint h = name_id->id();
  ymuint idx = h % mTableSize;
  for (Cell* cell = mHashTable[idx];
       cell != NULL; cell = cell->mLink) {
    SortElem* sort = cell->mSort;
    if ( sort->name() == name_id ) {
      param_num = cell->mParamNum;
      return sort;
    }
  }

  return NULL;
}

// @brief 単純な型を作る．
// @param[in] name_id 型名を表す識別子
// @return 生成した型を返す．
// @note エラーが起こったら NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id という名の型が定義されていなかった．
//  - name_id という名の型のパラメータ数が0ではなかった．
const SortElem*
SortTemplMgr::make_sort(const SmtId* name_id)
{
  return make_sort(name_id, vector<const SortElem*>(0));
}

// @brief 複合型を作る．
// @param[in] name_id 型名を表す識別子
// @param[in] param_list 要素の型のリスト
// @return 生成した型を返す．
// @note エラーが起こったら NULL を返す．
//
// エラーの原因は以下のとおり
//  - name_id という名の型が定義されていなかった．
//  - name_id という名の型のパラメータ数が0ではなかった．
const SortElem*
SortTemplMgr::make_sort(const SmtId* name_id,
			const vector<const SortElem*>& param_list)
{
  ymuint param_id;
  const SortElem* sort_templ = find_sort(name_id, param_id);
  if ( sort_templ == NULL ) {
    // name_id という型名は登録されていなかった．
    return NULL;
  }
  if ( param_id != param_list.size() ) {
    // パラメータ数が異なっていた．
    return NULL;
  }

  // sort_templ のパラメータを param_list に置き換える．
  const SortElem* sort = replace_param(sort_templ, param_list);

  return sort;
}

// @brief パラメータ型を作る．
const SortElem*
SortTemplMgr::make_param_sort(ymuint param_id)
{
  while ( mParamArray.size() <= param_id ) {
    ymuint pid = mParamArray.size();
    mParamArray.push_back(new_param_sort(pid));
  }
  return mParamArray[param_id];
}

// @breif パラメータ型を生成する．
// @param[in] param_id パラメータ番号
const SortElem*
SortTemplMgr::new_param_sort(ymuint param_id)
{
  void* p = mAlloc.get_memory(sizeof(ParamSort));
  SortElem* sort = new (p) ParamSort(param_id);
  return sort;
}

// @brief 単純な型を生成する．
// @param[in] name_id 名前を表す識別子
const SortElem*
SortTemplMgr::new_simple_sort(const SmtId* name_id)
{
  void* p = mAlloc.get_memory(sizeof(SimpleSort));
  SortElem* sort = new (p) SimpleSort(name_id);
  return sort;
}

// @brief 複合型を生成する．
// @param[in] name_id 名前を表す識別子
// @param[in] elem_list 要素のリスト
const SortElem*
SortTemplMgr::new_complex_sort(const SmtId* name_id,
			       const vector<const SortElem*>& elem_list)
{
  ymuint n = elem_list.size();
  void* p = mAlloc.get_memory(sizeof(ComplexSort) + sizeof(const SortElem*) * (n - 1));
  SortElem* sort = new (p) ComplexSort(name_id, elem_list);
  return sort;
}

// @brief ハッシュ表を拡大する．
// @param[in] req_size 新しいサイズ
void
SortTemplMgr::expand_table(ymuint req_size)
{
  ymuint old_size = mTableSize;
  Cell** old_table = mHashTable;

  mTableSize = req_size;
  mHashTable = new Cell*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell != NULL; ) {
	Cell* tmp_cell = cell;
	cell = cell->mLink;

	ymuint idx = tmp_cell->mSort->name()->id() % mTableSize;
	tmp_cell->mLink = mHashTable[idx];
	mHashTable[idx] = tmp_cell;
      }
    }
    delete [] old_table;
  }
}

END_NAMESPACE_YM_SMTLIBV2
