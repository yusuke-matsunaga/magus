
/// @file SnAnd.cc
/// @brief SnAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SnAnd.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス SnAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node0 ファンイン0のノード
// @param[in] inv0 ファンイン0の極性
// @param[in] node1 ファンイン1のノード
// @param[in] inv1 ファンイン1の極性
SnAnd::SnAnd(ymuint id,
	     StrNode* node0,
	     bool inv0,
	     StrNode* node1,
	     bool inv1) :
  StrNode(id, node0, inv0, node1, inv1)
{
}

// @brief デストラクタ
SnAnd::~SnAnd()
{
}

// @brief AND タイプのときに true を返す．
bool
SnAnd::is_and() const
{
  return true;
}

// @brief 値を返す．
ymuint
SnAnd::val() const
{
  switch ( mState ) {
  case kStXX_X:
  case kSt1X_X:
  case kStX1_X:
    return 0;

  case kStXX_0:
  case kStX0_0:
  case kSt0X_0:
  case kSt00_0:
  case kSt10_0:
  case kSt01_0:
    return 1;

  case kSt11_1:
    return 2;
  }
  return 0;
}

// @brief 状態を初期化する．
void
SnAnd::clear()
{
  mState = kStXX_X;
}

// @brief ファンイン0を0にする．
bool
SnAnd::fwd0_imp0()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 0X:0
    mChangedList.push_back(this);
    mState = kSt0X_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kStX1_X: // X1:X -> 01:0
    mState = kSt01_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kStXX_0: // XX:0 -> 0X:0
    mState = kSt0X_0;
    break;

  case kStX0_0: // X0:0 -> 00:0
    mState = kSt00_0;
    break;

  case kSt1X_X: // illegal
  case kSt10_0: // illegal
  case kSt11_1: // illegal
    return false;

  case kSt0X_0: // no change
  case kSt01_0: // no change
  case kSt00_0: // no change
    break;
  }
  return true;
}

// @brief ファンイン0を1にする．
bool
SnAnd::fwd0_imp1()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 1X:X
    mChangedList.push_back(this);
    mState = kSt1X_X;
    break;

  case kStX1_X: // X1:X -> 11:1
    mState = kSt11_1;
    // ファンアウト先に1を伝搬する．
    return fwd_prop1();

  case kStXX_0: // XX:0 -> 10:0
    mState = kSt10_0;
    // ファンイン1に0を伝搬する．
    return fanin1_prop0();

  case kStX0_0: // X0:0 -> 10:0
    mState = kSt10_0;
    break;

  case kSt0X_0: // illegal
  case kSt00_0: // illegal
  case kSt01_0: // illegal
    return false;

  case kSt1X_X: // no change
  case kSt10_0: // no change
  case kSt11_1: // no change
    break;
  }
  return true;
}

// @brief ファンイン1を0にする．
bool
SnAnd::fwd1_imp0()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X0:0
    mChangedList.push_back(this);
    mState = kStX0_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kSt1X_X: // 1X:X -> 10:0
    mState = kSt10_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kSt0X_0: // 0X:0 -> 00:0
    mState = kSt00_0;
    break;

  case kStXX_0: // XX:0 -> X0:0
    mState = kStX0_0;
    break;

  case kStX1_X: // illegal
  case kSt01_0: // illegal
  case kSt11_1: // illegal
    return false;

  case kStX0_0: // no change
  case kSt00_0: // no change
  case kSt10_0: // no change
    break;
  }
  return true;
}

// @brief ファンイン1を1にする．
bool
SnAnd::fwd1_imp1()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X1:X
    mChangedList.push_back(this);
    mState = kStX1_X;
    break;

  case kSt1X_X: // 1X:X -> 11:1
    mState = kSt11_1;
    // ファンアウト先に1を伝搬する．
    return fwd_prop1();

  case kStXX_0: // XX:0 -> 01:0
    mState = kSt01_0;
    // ファンイン0に0を伝搬する．
    return fanin0_prop0();

  case kSt0X_0: // 0X:0 -> 01:0
    mState = kSt01_0;
    break;

  case kStX0_0: // illegal
  case kSt00_0: // illegal
  case kSt10_0: // illegal
    return false;

  case kStX1_X: // no change
  case kSt01_0: // no change
  case kSt11_1: // no change
    break;
  }
  return true;
}

// @brief 出力を0にする．
bool
SnAnd::bwd_imp0()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX:0
    mChangedList.push_back(this);
    mState = kStXX_0;
    break;

  case kSt1X_X: // 1X:X -> 10:0
    mState = kSt10_0;
    // ファンイン1に0を伝搬する．
    return fanin1_prop0();

  case kStX1_X: // X1:X -> 01:0
    mState = kSt01_0;
    // ファンイン0に0を伝搬する．
    return fanin0_prop0();

  case kStXX_0: // no change
  case kStX0_0: // no change
  case kSt0X_0: // no change
  case kSt00_0: // no change
  case kSt10_0: // no change
  case kSt01_0: // no change
    break;

  case kSt11_1: // illegal
    return false;
  }
  return true;
}

// @brief 出力を1にする．
bool
SnAnd::bwd_imp1()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 11:1
    mChangedList.push_back(this);
    mState = kSt11_1;
    // ファンイン0に1を伝搬する．
    // ファンイン1に1を伝搬する．
    return fanin0_prop1() && fanin1_prop1();

  case kSt1X_X: // 1X:X -> 11:1
    mState = kSt11_1;
    // ファンイン1に1を伝搬する．
    return fanin1_prop1();

  case kStX1_X: // X1:X -> 11:1
    mState = kSt11_1;
    // ファンイン0に1を伝搬する．
    return fanin0_prop0();

  case kStXX_0: // illegal
  case kStX0_0: // illegal
  case kSt0X_0: // illegal
  case kSt00_0: // illegal
  case kSt10_0: // illegal
  case kSt01_0: // illegal
    return false;

  case kSt11_1: // no change
    break;
  }
  return true;
}

END_NAMESPACE_YM_NETWORKS
