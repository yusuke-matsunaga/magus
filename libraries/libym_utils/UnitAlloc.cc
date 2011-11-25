
/// @file UnitAlloc.cc
/// @brief UnitAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス UnitAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UnitAlloc::UnitAlloc(ymuint unit_size,
		     ymuint block_size) :
  mUnitSize(unit_size),
  mBlockSize(block_size),
  mAvailTop(NULL),
  mUsedSize(0),
  mMaxUsedSize(0),
  mAllocSize(0),
  mAllocCount(0)
{
  if ( mUnitSize < sizeof(Block) ) {
    mUnitSize = sizeof(Block);
  }
  assert_cond(block_size > 0, __FILE__, __LINE__);
}

// デストラクタ
UnitAlloc::~UnitAlloc()
{
  destroy();
}

// @brief n バイトの領域を確保する．
void*
UnitAlloc::get_memory(ymuint n)
{
  mUsedSize += n;
  if ( mMaxUsedSize < mUsedSize ) {
    mMaxUsedSize = mUsedSize;
  }

  if ( n != mUnitSize ) {
    mAllocSize += n;
    return ::operator new(n);
  }

  if ( mAvailTop == NULL ) {
    char* chunk = new char[mUnitSize * mBlockSize];
    mAllocSize += mUnitSize * mBlockSize;
    ++ mAllocCount;
    mAllocList.push_back(chunk);

    for (ymuint i = 0; i < mBlockSize; ++ i, chunk += mUnitSize) {
      Block* b = reinterpret_cast<Block*>(chunk);
      b->mLink = mAvailTop;
      mAvailTop = b;
    }
    assert_cond(mAvailTop, __FILE__, __LINE__);
  }
  Block* b = mAvailTop;
  mAvailTop = b->mLink;
  return static_cast<void*>(b);
}

// @brief n バイトの領域を開放する．
void
UnitAlloc::put_memory(ymuint n,
		      void* block)
{
  mUsedSize -= n;

  if ( n != mUnitSize ) {
    mAllocSize -= n;
    ::operator delete(block);
  }
  else {
    Block* b = reinterpret_cast<Block*>(block);
    b->mLink = mAvailTop;
    mAvailTop = b;
  }
}

// @brief 今までに確保した全ての領域を破棄する．
void
UnitAlloc::destroy()
{
  for (list<char*>::iterator p = mAllocList.begin();
       p != mAllocList.end(); ++ p) {
    delete [] *p;
  }
  mAllocList.clear();
  mAvailTop = NULL;
}

// @brief 使用されているメモリ量を返す．
ymuint
UnitAlloc::used_size() const
{
  return mUsedSize;
}

// @brief used_size() の最大値を返す．
ymuint
UnitAlloc::max_used_size() const
{
  return mMaxUsedSize;
}

// @brief 実際に確保したメモリ量を返す．
ymuint
UnitAlloc::allocated_size() const
{
  return mAllocSize;
}

// @brief 実際に確保した回数を返す．
ymuint
UnitAlloc::allocated_count() const
{
  return mAllocCount;
}

// @brief 内部状態を出力する．
void
UnitAlloc::print_stats(ostream& s) const
{
  s << "maximum used size: " << max_used_size() << endl
    << "current used size: " << used_size() << endl
    << "allocated size:    " << allocated_size() << endl
    << "allocated count:   " << allocated_count() << endl
    << endl;
}

END_NAMESPACE_YM
