#ifndef DTPGFAULT_H
#define DTPGFAULT_H

/// @file DtpgFault.h
/// @brief DtpgFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "FaultStatus.h"


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

  /// @brief 状態を返す．
  FaultStatus
  status() const;

  /// @brief スキップフラグを返す．
  bool
  is_skip() const;

  /// @brief スキップフラグをセットする．
  void
  set_skip();

  /// @brief スキップフラグをクリアする．
  void
  clear_skip();

  /// @brief untestable フラグを返す．
  bool
  is_untestable() const;

  /// @brief untestable フラグをセットする．
  void
  set_untestable();

  /// @brief untestable フラグをクリアする．
  void
  clear_untestable();

  /// @brief 必要割り当てのリストを得る．
  vector<ymuint32>&
  ma_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 一時的な ID 番号
  ymuint32 mTmpId;

  // 故障の出力側のノード
  DtpgNode* mNode;

  // 故障の入力側のノード
  DtpgNode* mSrcNode;

  // 故障位置 + 故障値 + スキップフラグ + untestable フラグ
  ymuint32 mPosVal;

  // 必要割り当てのリスト
  // 値は　ノードのID番号 × 2 + 値
  vector<ymuint32> mMaList;

  // 状態
  FaultStatus mStatus;

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
  return (mPosVal >> 4);
}

// @brief 故障値を返す．
inline
int
DtpgFault::val() const
{
  return static_cast<int>((mPosVal >> 1) & 1);
}

// @brief 状態を返す．
inline
FaultStatus
DtpgFault::status() const
{
  return mStatus;
}

// @brief スキップフラグを返す．
inline
bool
DtpgFault::is_skip() const
{
  if ( status() == kFsDetected ) {
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

// @brief untestable フラグを返す．
inline
bool
DtpgFault::is_untestable() const
{
  return static_cast<bool>((mPosVal >> 3) & 1U);
}

// @brief untestable フラグをセットする．
inline
void
DtpgFault::set_untestable()
{
  mPosVal |= 8U;
}

// @brief untestable フラグをクリアする．
inline
void
DtpgFault::clear_untestable()
{
  mPosVal &= ~8U;
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
