#ifndef DTPGFAULT_H
#define DTPGFAULT_H

/// @file DtpgFault.h
/// @brief DtpgFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

class SaFault;

END_NAMESPACE_YM_SATPG

BEGIN_NAMESPACE_YM_SATPG_DTPG

class DtpgNode;

//////////////////////////////////////////////////////////////////////
/// @class DtpgFault DtpgFault.h "DtpgFault.h"
/// @brief DtpgSat 用の故障を表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgFault
{
  friend class DtpgNetwork;

public:

  /// @brief コンストラクタ
  DtpgFault();

  /// @brief デストラクタ
  ~DtpgFault();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief もとの故障を返す．
  SaFault*
  safault() const;

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 入力の故障の時 true を返す．
  bool
  is_input_fault() const;

  /// @brief 出力の故障の時 true を返す．
  bool
  is_output_fault() const;

  /// @brief 故障の出力側のノードを返す．
  DtpgNode*
  node() const;

  /// @brief 故障の入力側のノードを返す．
  DtpgNode*
  source_node() const;

  /// @brief 故障位置を返す．
  /// @note 入力側のノードの時のみ意味を持つ．
  ymuint
  pos() const;

  /// @brief 故障値を返す．
  int
  val() const;

  /// @brief 反対の値の故障を返す．
  /// @note なければ NULL を返す．
  DtpgFault*
  alt_fault() const;

  /// @brief テストパタンを設定する．
  /// @note 同時に検出済みになる．
  void
  set_tv(TestVector* tv);

  /// @brief テストパタンを返す．
  TestVector*
  tv() const;

  /// @brief 検出不能の印をつける．
  void
  set_untest();

  /// @brief 状態を返す．
  tStat
  stat() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの故障
  SaFault* mSaFault;

  // ID番号
  ymuint mId;

  // 故障の出力側のノード
  DtpgNode* mNode;

  // 故障の入力側のノード
  DtpgNode* mSrcNode;

  // 故障位置 + 故障値
  ymuint32 mPosVal;

  // 反対の値を持つ故障
  DtpgFault* mAltFault;

  // テストパタン
  TestVector* mTv;

  // 状態
  tStat mStat;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgFault::DtpgFault()
{
}

// @brief デストラクタ
inline
DtpgFault::~DtpgFault()
{
}

// @brief もとの故障を返す．
inline
SaFault*
DtpgFault::safault() const
{
  return mSaFault;
}

// @brief ID番号を返す．
inline
ymuint
DtpgFault::id() const
{
  return mId;
}

// @brief 入力の故障の時 true を返す．
inline
bool
DtpgFault::is_input_fault() const
{
  return static_cast<bool>(mPosVal & 1U);
}

// @brief 出力の故障の時 true を返す．
inline
bool
DtpgFault::is_output_fault() const
{
  return !is_input_fault();
}

// @brief 故障の出力側のノードを返す．
inline
DtpgNode*
DtpgFault::node() const
{
  return mNode;
}

// @brief 故障の入力側のノードを返す．
inline
DtpgNode*
DtpgFault::source_node() const
{
  return mSrcNode;
}

// @brief 故障位置を返す．
// @note 入力側のノードの時のみ意味を持つ．
inline
ymuint
DtpgFault::pos() const
{
  return (mPosVal >> 2);
}

// @brief 故障値を返す．
inline
int
DtpgFault::val() const
{
  return static_cast<int>((mPosVal >> 1) & 1);
}

// @brief 反対の値の故障を返す．
// @note なければ NULL を返す．
inline
DtpgFault*
DtpgFault::alt_fault() const
{
  return mAltFault;
}

// @brief テストパタンを設定する．
// @note 同時に検出済みになる．
inline
void
DtpgFault::set_tv(TestVector* tv)
{
  mTv = tv;
  mStat = kDetect;
}

// @brief テストパタンを返す．
inline
TestVector*
DtpgFault::tv() const
{
  return mTv;
}

// @brief 検出不能の印をつける．
inline
void
DtpgFault::set_untest()
{
  mStat = kUntest;
}

// @brief 状態を返す．
inline
tStat
DtpgFault::stat() const
{
  return mStat;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGFAULT_H
