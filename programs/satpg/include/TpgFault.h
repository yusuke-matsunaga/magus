#ifndef TPGFAULT_H
#define TPGFAULT_H

/// @file TpgFault.h
/// @brief TpgFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2007, 2012-2013 Yusuke Matsunaga
/// All rights reserved.

#include "satpg_nsdef.h"
#include "Val3.h"
#include "FaultStatus.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgFault TpgFault.h "TpgFault.h"
/// @brief 単一縮退故障を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgFault
{
  friend class FaultMgr;

private:

  /// @brief コンストラクタ
  TpgFault();

  /// @brief デストラクタ
  ~TpgFault();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 対象のゲートを返す．
  TpgNode*
  node() const;

  /// @brief 故障の入力側のゲートを返す．
  TpgNode*
  source_node() const;

  /// @brief 入力の故障の時 true を返す．
  bool
  is_input_fault() const;

  /// @brief 出力の故障の時 true を返す．
  bool
  is_output_fault() const;

  /// @brief 故障位置を返す．
  ymuint
  pos() const;

  /// @brief 故障値を返す．
  /// @note 返す値は 0 か 1
  int
  val() const;

  /// @brief 故障値を3値型で返す．
  Val3
  val3() const;

  /// @brief 故障の内容を表す文字列を返す．
  string
  str() const;

  /// @brief 状態を返す．
  FaultStatus
  status() const;

  /// @brief 代表故障の時 true を返す．
  bool
  is_rep() const;

  /// @brief 代表故障を返す．
  /// @note 代表故障の時は自分自身を返す．
  TpgFault*
  rep_fault() const;

  /// @brief 作業用に用いられる一時的なID番号を設定する．
  void
  set_tmp_id(ymuint id);

  /// @brief 作業用に用いられる一時的なID番号を得る．
  ymuint
  tmp_id() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // TpgNetwork が使用する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] id ID番号
  /// @param[in] node 対象のゲート
  /// @param[in] output 入力の故障のとき false, 出力の故障のとき true を与える．
  /// @param[in] pos 故障位置
  /// @param[in] val 故障値 0 か非0 で 0/1 を区別する．
  /// @param[in] rep_fault 代表故障
  void
  set(ymuint id,
      TpgNode* node,
      bool output,
      ymuint pos,
      int val,
      TpgFault* rep_fault);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 対象のノード
  TpgNode* mNode;

  // 故障位置と故障値をパックしたもの
  ymuint32 mPosVal;

  // 状態
  FaultStatus mStatus;

  // 代表故障
  TpgFault* mRepFault;

  // 一時的なID番号
  ymuint32 mTmpId;

  // POモードで検出不能と判定された回数
  ymuint32 mUntestNum;

};

/// @relates TpgFault
/// @brief ストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] f 故障
ostream&
operator<<(ostream& s,
	   const TpgFault* f);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TpgFault::TpgFault()
{
}

// @brief デストラクタ
inline
TpgFault::~TpgFault()
{
}

// @brief ID番号を返す．
inline
ymuint
TpgFault::id() const
{
  return mId;
}

// @brief 対象のゲートを返す．
inline
TpgNode*
TpgFault::node() const
{
  return mNode;
}

// @brief 入力の故障の時 true を返す．
inline
bool
TpgFault::is_input_fault() const
{
  return !is_output_fault();
}

// @brief 出力の故障の時 true を返す．
inline
bool
TpgFault::is_output_fault() const
{
  return static_cast<bool>((mPosVal >> 1) & 1UL);
}

// @brief 故障位置を返す．
inline
ymuint
TpgFault::pos() const
{
  return mPosVal >> 3;
}

// @brief 故障値を返す．
// @note 返す値は 0 か 1
inline
int
TpgFault::val() const
{
  return static_cast<int>(mPosVal & 1UL);
}

// @brief 故障値を3値型で返す．
inline
Val3
TpgFault::val3() const
{
  if ( val() ) {
    return kVal1;
  }
  else {
    return kVal0;
  }
}

// @brief 状態を返す．
inline
FaultStatus
TpgFault::status() const
{
  return mStatus;
}

// @brief 代表故障の時 true を返す．
inline
bool
TpgFault::is_rep() const
{
  return mRepFault == this;
}

// @brief 代表故障を返す．
// @note 代表故障の時は自分自身を返す．
inline
TpgFault*
TpgFault::rep_fault() const
{
  return mRepFault;
}

// @brief 作業用に用いられる一時的なID番号を設定する．
inline
void
TpgFault::set_tmp_id(ymuint id)
{
  mTmpId = id;
}

// @brief 作業用に用いられる一時的なID番号を得る．
inline
ymuint
TpgFault::tmp_id() const
{
  return mTmpId;
}

// @brief スキップフラグを返す．
inline
bool
TpgFault::is_skip() const
{
  if ( status() == kFsDetected ) {
    return true;
  }
  return static_cast<bool>((mPosVal >> 2) & 1U);
}

// @brief スキップフラグをセットする．
inline
void
TpgFault::set_skip()
{
  mPosVal |= 4U;
}

// @brief スキップフラグをクリアする．
inline
void
TpgFault::clear_skip()
{
  mPosVal &= ~4U;
}

// @brief POモードで検出不能と判定された回数を得る．
inline
ymuint
TpgFault::untest_num() const
{
  return mUntestNum;
}

// @brief POモードで検出不能と判定された回数を1増やす．
inline
void
TpgFault::inc_untest_num()
{
  ++ mUntestNum;
}

// @brief POモードで検出不能と判定された回数をクリアする．
inline
void
TpgFault::clear_untest_num()
{
  mUntestNum = 0;
}

END_NAMESPACE_YM_SATPG

#endif // TPGFAULT_H
