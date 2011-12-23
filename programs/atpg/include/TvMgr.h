#ifndef TVMGR_H
#define TVMGR_H

/// @file TvMgr.h
/// @brief TvMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"
#include "TestVector.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @class TvMgr TvMgr.h "TvMgr.h"
/// @brief テストベクタを管理するクラス
//////////////////////////////////////////////////////////////////////
class TvMgr
{
public:

  /// @brief コンストラクタ
  TvMgr();

  /// @brief デストラクタ
  ~TvMgr();


public:

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 初期化する．
  /// @param[in] ni 入力数
  /// @note 副作用で clear() が呼ばれる．
  void
  init(size_t ni);

  /// @brief 新しいパタンを生成する．
  /// @return 生成されたパタンを返す．
  /// @note パタンは0で初期化される．
  TestVector*
  new_vector();

  /// @brief パタンを削除する．
  void
  delete_vector(TestVector* tv);


private:

  /// @brief コピーコンストラクタ
  /// @note 使用禁止なので実装しない．
  TvMgr(const TvMgr& src);

  /// @brief 代入演算子
  /// @note 使用禁止なので実装しない．
  const TvMgr&
  operator=(const TvMgr& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テストベクタのメモリ確保用のアロケータ
  SimpleAlloc mAlloc;

  // テストベクタの入力数
  size_t mNi;

  // 未使用のテストベクタをつないでおくリスト
  list<TestVector*> mAvail;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

#if 0
// @brief テストベクタの要素数を返す．
inline
size_t
TvMgr::num() const
{
  return mNum;
}

// @brief 内部ポインタを先頭のテストベクタにセットする．
// @return 内部ポインタの指しているテストベクタを返す．
// @note 内部ポインタが範囲外を指していたら NULL を返す．
inline
TestVector*
TvMgr::seek_top()
{
  mCurVector = mDummyTop->mNextLink;
  return cur_vector();
}

// @brief 内部ポインタを末尾のテストベクタにセットする．
// @return 内部ポインタの指しているテストベクタを返す．
// @note 内部ポインタが範囲外を指していたら NULL を返す．
inline
TestVector*
TvMgr::seek_tail()
{
  mCurVector = mDummyTop->mPrevLink;
  return cur_vector();
}

// @brief 内部ポインタを一つ進める．
// @return 内部ポインタの指しているテストベクタを返す．
// @note 内部ポインタが範囲外を指していたら NULL を返す．
inline
TestVector*
TvMgr::seek_next()
{
  if ( mCurVector == mDummyTop ) {
    return NULL;
  }
  mCurVector = mCurVector->mNextLink;
  return cur_vector();
}

// @brief 内部ポインタを一つ戻す．
// @return 内部ポインタの指しているテストベクタを返す．
// @note 内部ポインタが範囲外を指していたら NULL を返す．
inline
TestVector*
TvMgr::seek_prev()
{
  if ( mCurVector == mDummyTop ) {
    return NULL;
  }
  mCurVector = mCurVector->mPrevLink;
  return cur_vector();
}

// @brief 内部ポインタの指しているテストベクタを返す．
// @note 内部ポインタが範囲外を指していたら NULL を返す．
inline
TestVector*
TvMgr::cur_vector()
{
  if ( mCurVector == mDummyTop ) {
    return NULL;
  }
  else {
    return mCurVector;
  }
}
#endif

END_NAMESPACE_YM_ATPG

#endif // TVMGR_H
