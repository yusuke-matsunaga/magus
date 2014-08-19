
/// @file src/base/TvMgr.cc
/// @brief TvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TvMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2009, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "TvMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
TvMgr::TvMgr() :
  mAlloc(NULL),
  mNi(0)
{
}

// @brief デストラクタ
TvMgr::~TvMgr()
{
  clear();
}

/// @brief 内容を空にする．
void
TvMgr::clear()
{
  if ( mAlloc != NULL ) {
    delete mAlloc;
    mAlloc = NULL;
    mNi = 0;
  }
}

// @brief 内容を初期化する．
// @param[in] ni 入力数
void
TvMgr::init(ymuint ni)
{
  clear();

  mNi = ni;
  if ( mNi == 0 ) {
    // 0 だとヤバい
    mNi = 1;
  }

  ymuint nb = TestVector::block_num(mNi);
  ymuint size = sizeof(TestVector) + kPvBitLen * (nb - 1);
  mAlloc = new UnitAlloc(size, 1024);
}

// @brief 新しいパタンを生成する．
// @return 生成されたパタンを返す．
// @note パタンは0で初期化される．
TestVector*
TvMgr::new_vector()
{
  ymuint nb = TestVector::block_num(mNi);
  ymuint size = sizeof(TestVector) + kPvBitLen * (nb - 1);
  void* p = mAlloc->get_memory(size);
  TestVector* tv = new (p) TestVector(mNi);

  // X に初期化しておく．
  for (ymuint i = 0; i < nb; ++ i) {
    tv->mPat[i] = kPvAll0;
  }

  return tv;
}

// @brief パタンを削除する．
void
TvMgr::delete_vector(TestVector* tv)
{
  ymuint nb = TestVector::block_num(mNi);
  ymuint size = sizeof(TestVector) + kPvBitLen * (nb - 1);
  mAlloc->put_memory(size, (void*)tv);
}

END_NAMESPACE_YM_SATPG
