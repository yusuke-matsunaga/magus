
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
    h = h * 127 + sort->hash();
  }
  return h;
}

// ハッシュ関数
ymuint
hash_func(const SmtSort* sort)
{
  ymuint h = sort->name()->id();
  ymuint n = sort->elem_num();
  for (ymuint i = 0; i < n; ++ i) {
    const SmtSort* sort1 = sort->elem(i);
    h = h * 127 + sort1->hash();
  }
  return h;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス SmtSortMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
// @param[in] level スタックレベル
// @param[in] parent_mgr 上位のマネージャ
SmtSortMgr::SmtSortMgr(Alloc& alloc,
		       ymuint level,
		       SmtSortMgr* parent_mgr) :
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
SmtSortMgr::~SmtSortMgr()
{
  delete [] mHashTable;
}

// @brief 型名を登録する．
// @param[in] name_id 型名
// @param[in] param_num 引数の数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SmtSortMgr::reg_sort(const SmtId* name_id,
		     ymuint param_num)
{
  return reg_sub(name_id, NULL, param_num);
}


BEGIN_NONAMESPACE

// @brief sort に含まれるパラメータ番号を得る．
// @param[in] sort 対象の型
// @param[out] param_array パラメータ番号を収める配列
// @note param_array[i] = true なら i 番めのパラメータが使われている．
void
get_param(const SmtSort* sort,
	  vector<bool>& param_array)
{
  if ( sort->is_param() ) {
    ymuint pid = sort->param_id();
    while ( param_array.size() <= pid ) {
      param_array.push_back(false);
    }
    param_array[pid] = true;
  }
  else {
    ymuint n = sort->elem_num();
    for (ymuint i = 0; i < n; ++ i) {
      const SmtSort* sort1 = sort->elem(i);
      get_param(sort1, param_array);
    }
  }
}

END_NONAMESPACE

// @brief alias を登録する．
// @param[in] name_id 型名
// @param[in] sort 登録する型
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SmtSortMgr::reg_alias(const SmtId* name_id,
		      const SmtSort* sort)
{
  vector<bool> param_array;
  get_param(sort, param_array);
  ymuint param_num = param_array.size();
  for (ymuint i = 0; i < param_num; ++ i) {
    if ( !param_array[i] ) {
      // 歯抜け状態だった．
      return false;
    }
  }

  return reg_sub(name_id, sort, param_num);
}

// @brief reg_sort/reg_alias の下請け関数
// @param[in] name_id 型名
// @param[in] sort 登録する型
// @param[in] param_num 引数の数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．すでに同名の型が登録されている
bool
SmtSortMgr::reg_sub(const SmtId* name_id,
		    const SmtSort* sort,
		    ymuint param_num)
{
  hash_map<ymuint32, pair<const SmtSort*, ymuint32> >::iterator p = mHash.find(name_id->id());
  if ( p != mHash.end() ) {
    // 同名の宣言が登録されている．
    return false;
  }

  // 名前を登録する．
  mHash.insert(make_pair(name_id->id(), make_pair(sort, param_num)));

  return true;
}

// @brief SmtSort に変換する．
// @param[in] name_id 型名
// @param[in] param_list 部品の型のリスト
// @return 引数に合致する型を返す．
const SmtSort*
SmtSortMgr::make_sort(const SmtId* name_id,
		      const vector<const SmtSort*>& param_list)
{
  // 型のインスタンスを探す．
  {
    const SmtSort* sort = find_sort(name_id, param_list);
    if ( sort != NULL ) {
      // 見つかった．
      return sort;
    }
  }

  // 型宣言を探す．
  const SmtSort* sort_tmpl = NULL;
  ymuint param_num = 0;
  if ( !find_sort_decl(name_id, sort_tmpl, param_num) ) {
    // name_id という型は登録されていなかった．
    return NULL;
  }

  if ( param_num != param_list.size() ) {
    // 引数の数が合わない．
    return NULL;
  }

  SmtSortImpl* sort = NULL;
  if ( sort_tmpl != NULL ) {
    // テンプレートから実際の型を作る．
    ymuint n = sort_tmpl->elem_num();
    ASSERT_COND( n > 0 );

    vector<const SmtSort*> elem_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      elem_list[i] = replace_param(sort_tmpl->elem(i), param_list);
    }
    sort = new_complex_sort(sort_tmpl->name(), elem_list);
  }
  else if ( param_list.empty() ) {
    // 単純な型
    sort = new_simple_sort(name_id);
  }
  else {
    // 複合型
    sort = new_complex_sort(name_id, param_list);
  }

  if ( mNum >= mNextLimit ) {
    expand_table(mTableSize * 2);
  }
  ymuint h = hash_func(sort);
  ymuint idx = h % mTableSize;

  sort->mId = mNum;
  sort->mLevel = mLevel;
  ++ mNum;

  sort->mLink = mHashTable[idx];
  mHashTable[idx] = sort;

  return sort;
}

// @brief テンプレートから実際の型を作る．
// @param[in] templ テンプレート
// @param[in] param_list パラメータリスト
const SmtSort*
SmtSortMgr::replace_param(const SmtSort* templ,
			  const vector<const SmtSort*>& param_list)
{
  ymuint n = templ->elem_num();
  if ( n == 0 ) {
    if ( templ->is_param() ) {
      // パラメータ型
      ymuint pid = templ->param_id();
      ASSERT_COND( pid < param_list.size() );
      return param_list[pid];
    }
    else {
      // 通常の型
      return templ;
    }
  }
  else {
    // 複合型
    vector<const SmtSort*> elem_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      elem_list[i] = replace_param(templ->elem(i), param_list);
    }
    return new_complex_sort(templ->name(), elem_list);
  }
}

// @brief 型パラメータを作る．
// @param[in] pid パラメータ番号
const SmtSort*
SmtSortMgr::make_param_sort(ymuint pid)
{
  void* p = mAlloc.get_memory(sizeof(SmtParamSort));
  SmtSort* sort = new (p) SmtParamSort(pid);
  return sort;
}

// @brief 型宣言を探す．
// @param[in] name_id 型名
// @param[out] sort_tmpl 型テンプレート
// @param[out] param_num パラメータ数
// @retval true name_id とういう名の型テンプレートが見つかった．
// @retval false name_id という名の型テンプレートは登録されていなかった．
bool
SmtSortMgr::find_sort_decl(const SmtId* name_id,
			   const SmtSort*& sort_tmpl,
			   ymuint& param_num)
{
  if ( mParent != NULL ) {
    // 親のレベルで探す．
    bool stat = mParent->find_sort_decl(name_id, sort_tmpl, param_num);
    if ( stat ) {
      return true;
    }
  }

  // 型名を探す．
  hash_map<ymuint32, pair<const SmtSort*, ymuint32> >::iterator p = mHash.find(name_id->id());
  if ( p == mHash.end() ) {
    // name という型名は登録されていなかった．
    return false;
  }

  // 見つかった．
  sort_tmpl = p->second.first;
  param_num = p->second.second;
  return true;
}

// @brief 型を探す．
// @param[in] name_id 型名
// @param[in] elem_list 部品の型のリスト
// @return 登録されていなければ NULL を返す．
const SmtSort*
SmtSortMgr::find_sort(const SmtId* name_id,
		      const vector<const SmtSort*>& elem_list)
{
  if ( mParent != NULL ) {
    // 親のレベルで探す．
    const SmtSort* sort = mParent->find_sort(name_id, elem_list);
    if ( sort != NULL ) {
      return sort;
    }
  }

  // 同じ型が存在するか調べる．
  ymuint h = hash_func(name_id, elem_list);
  ymuint idx = h % mTableSize;
  for (SmtSortImpl* sort = mHashTable[idx]; sort != NULL; sort = sort->mLink) {
    if ( sort->name() == name_id && check_elem(sort, elem_list) ) {
      // 見つかった．
      return sort;
    }
  }

  // 見つからなかった．
  return NULL;
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
