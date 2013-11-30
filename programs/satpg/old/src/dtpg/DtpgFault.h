#ifndef DTPGFAULT_H
#define DTPGFAULT_H

/// @file DtpgFault.h
/// @brief DtpgFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "SaFault.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

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

  /// @brief 一時的な番号を返す．
  ymuint
  tmp_id() const;

  /// @brief 一時的な番号をつける．
  void
  set_tmp_id(ymuint id);

  /// @brief 入力の故障の時 true を返す．
  bool
  is_input_fault() const;

  /// @brief 出力の故障の時 true を返す．
  bool
  is_output_fault() const;

  /// @brief 故障の出力側のノードを返す．
  DtpgNode*
  node() const;

  /// @brief 故障位置を返す．
  /// @note 入力側のノードの時のみ意味を持つ．
  ymuint
  pos() const;

  /// @brief 故障値を返す．
  int
  val() const;

  /// @brief スキップフラグを返す．
  bool
  is_skip() const;

  /// @brief スキップフラグをセットする．
  void
  set_skip();

  /// @brief スキップフラグをクリアする．
  void
  clear_skip();

  /// @brief POモードで検出不能と判定された回数を得る．
  ymuint
  untest_num() const;

  /// @brief POモードで検出不能と判定された回数を1増やす．
  void
  inc_untest_num();

  /// @brief POモードで検出不能と判定された回数をクリアする．
  void
  clear_untest_num();

  /// @brief 必要割り当てのリストを得る．
  vector<ymuint32>&
  ma_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの故障
  SaFault* mSaFault;

  // ID番号
  ymuint32 mId;

  // 一時的な ID 番号
  ymuint32 mTmpId;

  // 故障の出力側のノード
  DtpgNode* mNode;

  // 故障位置 + 故障値 + スキップフラグ
  ymuint32 mPosVal;

  // POモードで検出不能と判定された回数
  ymuint32 mUntestNum;

  // 必要割り当てのリスト
  // 値は　ノードのID番号 × 2 + 値
  vector<ymuint32> mMaList;

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

// @brief 一時的な番号を返す．
inline
ymuint
DtpgFault::tmp_id() const
{
  return mTmpId;
}

// @brief 一時的な番号をつける．
inline
void
DtpgFault::set_tmp_id(ymuint id)
{
  mTmpId = id;
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

// @brief 故障位置を返す．
// @note 入力側のノードの時のみ意味を持つ．
inline
ymuint
DtpgFault::pos() const
{
  return (mPosVal >> 4);
}

// @brief 故障値を返す．
inline
int
DtpgFault::val() const
{
  return static_cast<int>((mPosVal >> 1) & 1);
}

// @brief スキップフラグを返す．
inline
bool
DtpgFault::is_skip() const
{
  if ( safault()->status() == kFsDetected ) {
    return true;
  }
  return static_cast<bool>((mPosVal >> 2) & 1U);
}

// @brief スキップフラグをセットする．
inline
void
DtpgFault::set_skip()
{
  mPosVal |= 4U;
}

// @brief スキップフラグをクリアする．
inline
void
DtpgFault::clear_skip()
{
  mPosVal &= ~4U;
}

// @brief POモードで検出不能と判定された回数を得る．
inline
ymuint
DtpgFault::untest_num() const
{
  return mUntestNum;
}

// @brief POモードで検出不能と判定された回数を1増やす．
inline
void
DtpgFault::inc_untest_num()
{
  ++ mUntestNum;
}

// @brief POモードで検出不能と判定された回数をクリアする．
inline
void
DtpgFault::clear_untest_num()
{
  mUntestNum = 0;
}

// @brief 必要割り当てのリストを得る．
inline
vector<ymuint32>&
DtpgFault::ma_list()
{
  return mMaList;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGFAULT_H
