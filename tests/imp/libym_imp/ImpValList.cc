
/// @file ImpValList.cc
/// @brief ImpValList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpValList.h"


BEGIN_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// クラス ImpValList
//////////////////////////////////////////////////////////////////////

UnitAlloc ImpValList::mAlloc(sizeof(Cell), 1024);

/// @brief 空のコンストラクタ
ImpValList::ImpValList() :
  mNum(0)
{
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

// @brief 要素を追加する．
void
ImpValList::insert(ImpVal val)
{
  sanity_check();
  Cell* prev = &mDummyTop;
  Cell* cell = NULL;
  while ( (cell = prev->mLink) != NULL ) {
    if ( cell->mVal == val ) {
      // 同じ値があった．
      // なにもしないで終わる．
      return;
    }
    else if ( cell->mVal > val ) {
      break;
    }
    else { // cell->mVal < val
      prev = cell;
    }
  }
  Cell* new_cell = get_cell();
  new_cell->mVal = val;
  prev->mLink = new_cell;
  new_cell->mLink = cell;
  ++ mNum;
  sanity_check();
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
      prev = cell;
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
	prev = cell;
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
}

END_NAMESPACE_YM_NETWORKS
