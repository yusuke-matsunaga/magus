#ifndef DTPGFAULT_H
#define DTPGFAULT_H

/// @file DtpgFault.h
/// @brief DtpgFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"


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

  /// @brief スキップフラグを返す．
  bool
  is_skip() const;

  /// @brief スキップフラグをセットする．
  void
  set_skip();

  /// @brief スキップフラグをクリアする．
  void
  clear_skip();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの故障
  SaFault* mSaFault;

  // ID番号
  ymuint32 mId;

  // 故障の出力側のノード
  DtpgNode* mNode;

  // 故障の入力側のノード
  DtpgNode* mSrcNode;

  // 故障位置 + 故障値 + スキップフラグ
  ymuint32 mPosVal;
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgFault::DtpgFault()
{
  mPosVal = 0;
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
  return (mPosVal >> 3);
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

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGFAULT_H
