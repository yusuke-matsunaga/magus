
/// @file SimpleAlloc.cc
/// @brief SimpleAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス SimpleAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SimpleAlloc::SimpleAlloc(ymuint64 max_size)
{
  assert_cond(max_size > 0, __FILE__, __LINE__);
  mMaxSize = align(max_size);
  mCurBlock = NULL;
  mNextPos = mMaxSize;
}

// デストラクタ
SimpleAlloc::~SimpleAlloc()
{
  destroy();
}

// @brief n バイトの領域を確保する．
void*
SimpleAlloc::_get_memory(ymuint64 n)
{
  if ( n > mMaxSize ) {
    // mMaxSize を越えるものは普通にアロケートする．
    return alloc(n);
  }

  // ワード境界に乗るようにサイズに整える．
  ymuint64 alloc_size = align(n);
  if ( mNextPos + alloc_size > mMaxSize ) {
    void* p = alloc(mMaxSize);
    mCurBlock = static_cast<char*>(p);
    mNextPos = 0;
    mAllocList.push_back(mCurBlock);
  }
  char* p = &mCurBlock[mNextPos];
  mNextPos += alloc_size;

  return static_cast<void*>(p);
}

// @brief n バイトの領域を開放する．
void
SimpleAlloc::_put_memory(ymuint64 n,
			 void* block)
{
  if ( n > mMaxSize ) {
    // mMaxSize を越えるものは普通に開放する．
    free(n, block);
  }

  // このクラスでは領域の再利用はしない．
  return;
}

// @brief 今までに確保した全ての領域を破棄する．
void
SimpleAlloc::_destroy()
{
  for (list<char*>::iterator p = mAllocList.begin();
       p != mAllocList.end(); ++ p) {
    char* s = *p;
    free(mMaxSize, static_cast<void*>(s));
  }
  mAllocList.clear();
  mCurBlock = NULL;
  mNextPos = mMaxSize;
}

// @brief アラインメントを考慮してサイズを調節する．
inline
ymuint64
SimpleAlloc::align(ymuint64 req_size)
{
  return ((req_size + ALIGNOF_DOUBLE - 1) / ALIGNOF_DOUBLE) * ALIGNOF_DOUBLE;
}

END_NAMESPACE_YM
