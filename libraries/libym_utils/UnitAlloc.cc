
/// @file UnitAlloc.cc
/// @brief UnitAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス UnitAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UnitAlloc::UnitAlloc(ymuint64 unit_size,
		     ymuint64 block_size) :
  mUnitSize(unit_size),
  mBlockSize(block_size),
  mAvailTop(NULL)
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
UnitAlloc::_get_memory(ymuint64 n)
{
  if ( n != mUnitSize ) {
    // mUnitSize と異なっていたら普通のアロケータを使う．
    return alloc(n);
  }

  if ( mAvailTop == NULL ) {
    void* p = alloc(mUnitSize * mBlockSize);
    char* chunk = static_cast<char*>(p);
    mAllocList.push_back(chunk);

    for (ymuint64 i = 0; i < mBlockSize; ++ i, chunk += mUnitSize) {
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
UnitAlloc::_put_memory(ymuint64 n,
		       void* block)
{
  if ( n != mUnitSize ) {
    free(n, block);
  }
  else {
    Block* b = reinterpret_cast<Block*>(block);
    b->mLink = mAvailTop;
    mAvailTop = b;
  }
}

// @brief 今までに確保した全ての領域を破棄する．
void
UnitAlloc::_destroy()
{
  for (list<char*>::iterator p = mAllocList.begin();
       p != mAllocList.end(); ++ p) {
    char* chunk = *p;
    free(mUnitSize * mBlockSize, static_cast<void*>(chunk));
  }
  mAllocList.clear();
  mAvailTop = NULL;
}

END_NAMESPACE_YM
