
/// @file ImpValList.cc
/// @brief ImpValList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpValList.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// クラス ImpValList
//////////////////////////////////////////////////////////////////////

UnitAlloc ImpValList::mAlloc(sizeof(Cell), 1024);

/// @brief 空のコンストラクタ
ImpValList::ImpValList() :
  mNum(0),
  mChanged(0)
{
  mDummyTop.mLink = NULL;
}

// @brief デストラクタ
ImpValList::~ImpValList()
{
  Cell* next = NULL;
  for (Cell* cell = mDummyTop.mLink;
       cell != NULL; cell = next) {
    next = cell->mLink;
    put_cell(cell);
  }
}

// @brief 要素数を得る．
ymuint
ImpValList::num() const
{
  return mNum;
}

// @brief 要素のリストをセットする．
void
ImpValList::set(ImpMgr& mgr,
		const vector<ImpVal>& val_list)
{
  assert_cond( mDummyTop.mLink == NULL, __FILE__, __LINE__);
  Cell* last = &mDummyTop;
  for (vector<ImpVal>::const_iterator p = val_list.begin();
       p != val_list.end(); ++ p) {
    const ImpVal& val = *p;
    if ( mgr.is_const(val.id()) ) {
      continue;
    }
    Cell* new_cell = get_cell();
    new_cell->mVal = val;
    last->mLink = new_cell;
    last = new_cell;
  }
  mNum = val_list.size();
}

// @brief リストの内容をマージする．
void
ImpValList::merge(const ImpValList& src)
{
  sanity_check();
  Cell* prev = &mDummyTop;
  Cell* cell = NULL;
  Cell* src_cell = src.mDummyTop.mLink;
  while ( (cell = prev->mLink) != NULL && src_cell != NULL ) {
    if ( cell->mVal < src_cell->mVal ) {
      prev = cell;
    }
    else if ( cell->mVal == src_cell->mVal ) {
      prev = cell;
      src_cell = src_cell->mLink;
    }
    else { // cell->mVal > src_cell->mVal
      Cell* new_cell = get_cell();
      new_cell->mVal = src_cell->mVal;
      prev->mLink = new_cell;
      new_cell->mLink = cell;
      prev = new_cell;
      src_cell = src_cell->mLink;
      ++ mNum;
    }
  }
  for ( ; src_cell != NULL; src_cell = src_cell->mLink ) {
    Cell* new_cell = get_cell();
    new_cell->mVal = src_cell->mVal;
    prev->mLink = new_cell;
    new_cell->mLink = NULL;
    prev = new_cell;
    ++ mNum;
  }
  sanity_check();
}

// @brief リストの内容をマージする．
void
ImpValList::merge(const vector<ImpVal>& src)
{
  sanity_check();
  Cell* prev = &mDummyTop;
  Cell* cell = NULL;
  vector<ImpVal>::const_iterator src_p = src.begin();
  vector<ImpVal>::const_iterator src_e = src.end();
  while ( (cell = prev->mLink) != NULL && src_p != src_e ) {
    const ImpVal& val = *src_p;
    if ( cell->mVal < val ) {
      prev = cell;
    }
    else if ( cell->mVal == val ) {
      prev = cell;
      ++ src_p;
    }
    else { // cell->mVal > val
      Cell* new_cell = get_cell();
      new_cell->mVal = *src_p;
      prev->mLink = new_cell;
      new_cell->mLink = cell;
      prev = new_cell;
      ++ src_p;
      ++ mNum;
    }
  }
  for ( ; src_p != src_e; ++ src_p) {
    Cell* new_cell = get_cell();
    new_cell->mVal = *src_p;
    prev->mLink = new_cell;
    new_cell->mLink = NULL;
    prev = new_cell;
    ++ mNum;
  }
  sanity_check();
}

// @brief 2つのリストの共通部分をマージする．
void
ImpValList::cap_merge(const ImpValList& src1,
		      const ImpValList& src2)
{
  sanity_check();
  Cell* prev = &mDummyTop;
  Cell* cell = NULL;
  Cell* src1_cell = src1.mDummyTop.mLink;
  Cell* src2_cell = src2.mDummyTop.mLink;
  while ( (cell = prev->mLink) != NULL && src1_cell != NULL && src2_cell != NULL ) {
    if ( src1_cell->mVal < src2_cell->mVal ) {
      src1_cell = src1_cell->mLink;
    }
    else if ( src1_cell->mVal > src2_cell->mVal ) {
      src2_cell = src2_cell->mLink;
    }
    else { // src1_cell->mVal == src2_cell->mVal
      if ( cell->mVal < src1_cell->mVal ) {
	prev = cell;
      }
      else if ( cell->mVal == src1_cell->mVal ) {
	prev = cell;
	src1_cell = src1_cell->mLink;
	src2_cell = src2_cell->mLink;
      }
      else { // cell->mVal > src1_cell->mVal
	Cell* new_cell = get_cell();
	new_cell->mVal = src1_cell->mVal;
	prev->mLink = new_cell;
	new_cell->mLink = cell;
	prev = new_cell;
	src1_cell = src1_cell->mLink;
	src2_cell = src2_cell->mLink;
	++ mNum;
      }
    }
  }
  while ( src1_cell != NULL && src2_cell != NULL ) {
    if ( src1_cell->mVal < src2_cell->mVal ) {
      src1_cell = src1_cell->mLink;
    }
    else if ( src1_cell->mVal > src2_cell->mVal ) {
      src2_cell = src2_cell->mLink;
    }
    else { // src1_cell->mVal == src2_cell->mVal;
      Cell* new_cell = get_cell();
      new_cell->mVal = src1_cell->mVal;
      prev->mLink = new_cell;
      new_cell->mLink = NULL;
      prev = new_cell;
      src1_cell = src1_cell->mLink;
      src2_cell = src2_cell->mLink;
      ++ mNum;
    }
  }
  sanity_check();
}

// @brief 先頭を表す反復子を返す．
ImpValListIter
ImpValList::begin() const
{
  return ImpValListIter(mDummyTop.mLink);
}

// @brief 末尾を表す反復子を返す．
ImpValListIter
ImpValList::end() const
{
  return ImpValListIter(NULL);
}

// @brief 内容を出力する
void
ImpValList::print(ostream& s) const
{
  for (Cell* cell = mDummyTop.mLink; cell; cell = cell->mLink) {
    const ImpVal& val = cell->mVal;
    cout << " ";
    val.print(s);
  }
  s << endl;
}

// @brief Cell を確保する関数
ImpValList::Cell*
ImpValList::get_cell()
{
  void* p = mAlloc.get_memory(sizeof(Cell));
  return new (p) Cell;
}

// @brief Cell を開放する関数
void
ImpValList::put_cell(Cell* cell)
{
  mAlloc.put_memory(sizeof(Cell), cell);
}

// @brief 結果が整列されているかと mNum が正しいかのテスト
void
ImpValList::sanity_check() const
{
#if 0
  ymuint n = 0;
  Cell* cell = mDummyTop.mLink;
  if ( cell != NULL ) {
    n = 1;
    for (Cell* next = cell->mLink; next; ) {
      if ( cell->mVal >= next->mVal ) {
	cout << "Error" << endl;
	print(cout);
	abort();
      }
      cell = next;
      next = cell->mLink;
      ++ n;
    }
  }
  if ( n != mNum ) {
    cout << "Error mNum mismatch" << endl
	 << "n = " << n << ", mNum = " << mNum << endl;
    print(cout);
    abort();
  }
#endif
}

END_NAMESPACE_YM_NETWORKS
