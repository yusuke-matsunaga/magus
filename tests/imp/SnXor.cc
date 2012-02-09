
/// @file SnXor.cc
/// @brief SnXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SnXor.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス SnXor
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
/// @param[in] id ID番号
/// @param[in] node0 ファンイン0のノード
/// @param[in] inv0 ファンイン0の極性
/// @param[in] node1 ファンイン1のノード
/// @param[in] inv1 ファンイン1の極性
SnXor::SnXor(ymuint id,
	     StrNode* node0,
	     bool inv0,
	     StrNode* node1,
	     bool inv1) :
  StrNode(id, node0, inv0, node1, inv1)
{
}

// @brief デストラクタ
SnXor::~SnXor()
{
}

// @brief XOR タイプのときに true を返す．
bool
SnXor::is_xor() const
{
  return true;
}

// @brief 値を返す．
ymuint
SnXor::val() const
{
  switch ( mState ) {
  case kStXX_X:
  case kStX0_X:
  case kStX1_X:
  case kSt0X_X:
  case kSt1X_X:
    return 0;

  case kStXX_0:
  case kSt00_0:
  case kSt11_0:
    return 1;

  case kStXX_1:
  case kSt01_1:
  case kSt10_1:
    return 2;
  }
  return 0;
}

// @brief 状態を初期化する．
void
SnXor::clear()
{
  mState = kStXX_X;
}

// @brief ファンイン0を0にする．
bool
SnXor::fwd0_imp0()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 0X:X
    mState = kSt0X_X;
    mChangedList.push_back(this);
    break;

  case kStXX_0: // XX:0 -> 00:0
    mState = kSt00_0;
    // ファンイン1に0を伝搬する．
    return fanin1_prop0();

  case kStXX_1: // XX:1 -> 01:1
    mState = kSt01_1;
    // ファンイン1に1を伝搬する．
    return fanin1_prop1();

  case kStX0_X: // X0:X -> 00:0
    mState = kSt00_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kStX1_X: // X1:X -> 01:1
    mState = kSt01_1;
    // ファンアウト先に1を伝搬する．
    return fwd_prop1();

  case kSt0X_X: // no change
  case kSt00_0: // no change
  case kSt01_1: // no change
    break;

  case kSt1X_X: // illegal
  case kSt10_1: // illegal
  case kSt11_0: // illegal
    return false;
  }
  return true;
}

// @brief ファンイン0を1にする．
bool
SnXor::fwd0_imp1()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 1X:X
    mChangedList.push_back(this);
    mState = kSt1X_X;
    break;

  case kStXX_0: // XX:0 -> 11:0
    mState = kSt11_0;
    // ファンイン1に1を伝搬する．
    return fanin1_prop1();

  case kStXX_1: // XX:1 -> 10:1
    mState = kSt10_1;
    // ファンイン1に0を伝搬する．
    return fanin1_prop0();

  case kStX0_X: // X0:X -> 10:1
    mState = kSt10_1;
    // ファンアウト先に1を伝搬する．
    return fwd_prop1();

  case kStX1_X: // X1:X -> 11:0
    mState = kSt11_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kSt0X_X: // illegal
  case kSt00_0: // illegal
  case kSt01_1: // illegal
    return false;

  case kSt1X_X: // no change
  case kSt10_1: // no change
  case kSt11_0: // no change
    break;
  }
  return true;
}

// @brief ファンイン1を0にする．
bool
SnXor::fwd1_imp0()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X0:X
    mChangedList.push_back(this);
    mState = kStX0_X;
    break;

  case kStXX_0: // XX:0 -> 00:0
    mState = kSt00_0;
    // ファンイン0に0を伝搬する．
    return fanin0_prop0();

  case kStXX_1: // XX:1 -> 10:1
    mState = kSt10_1;
    // ファンイン0に1を伝搬する．
    return fanin0_prop1();

  case kSt0X_X: // 0X:X -> 00:0
    mState = kSt00_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kSt1X_X: // 1X:X -> 10:1
    mState = kSt10_1;
    // ファンアウト先に1を伝搬する．
    return fwd_prop1();

  case kStX0_X: // no change
  case kSt00_0: // no change
  case kSt10_1: // no change
    break;

  case kStX1_X: // illegal
  case kSt01_1: // illegal
  case kSt11_0: // illegal
    return false;
  }
  return true;
}

// @brief ファンイン1を1にする．
bool
SnXor::fwd1_imp1()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X1:X
    mChangedList.push_back(this);
    mState = kStX1_X;
    break;

  case kStXX_0: // XX:0 -> 11:0
    mState = kSt11_0;
    // ファンイン0に1を伝搬する．
    return fanin0_prop1();

  case kStXX_1: // XX:1 -> 01:1
    mState = kSt01_1;
    // ファンイン0に0を伝搬する．
    return fanin0_prop0();

  case kSt0X_X: // 0X:X -> 01:1
    mState = kSt01_1;
    // ファンアウト先に1を伝搬する．
    return fwd_prop1();

  case kSt1X_X: // 1X:X -> 11:0
    mState = kSt11_0;
    // ファンアウト先に0を伝搬する．
    return fwd_prop0();

  case kStX0_X: // illegal
  case kSt00_0: // illegal
  case kSt10_1: // illegal
    return false;

  case kStX1_X: // no change
  case kSt01_1: // no change
  case kSt11_0: // no change
    break;
  }
  return true;
}

// @brief 出力を0にする．
bool
SnXor::bwd_imp0()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX:0
    mChangedList.push_back(this);
    mState = kStXX_0;
    break;

  case kStX0_X: // X0:X -> 00:0
    mState = kSt00_0;
    // ファンイン0に0を伝搬する．
    return fanin0_prop0();

  case kStX1_X: // X1:X -> 11:0
    mState = kSt11_0;
    // ファンイン0に1を伝搬する．
    return fanin0_prop1();

  case kSt0X_X: // 0X:X -> 00:0
    mState = kSt00_0;
    // ファンイン1に0を伝搬する．
    return fanin1_prop0();

  case kSt1X_X: // 1X:X -> 11:0
    mState = kSt11_0;
    // ファンイン1に1を伝搬する．
    return fanin1_prop1();

  case kStXX_0: // no change
  case kSt00_0: // no change
  case kSt11_0: // no change
    break;

  case kStXX_1: // illegal
  case kSt01_1: // illegal
  case kSt10_1: // illegal
    return false;

  }
  return true;
}

// @brief 出力を1にする．
bool
SnXor::bwd_imp1()
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX10
    mChangedList.push_back(this);
    mState = kStXX_1;
    break;

  case kStX0_X: // X0:X -> 10:1
    mState = kSt10_1;
    // ファンイン0に1を伝搬する．
    return fanin0_prop1();

  case kStX1_X: // X1:X -> 01:1
    mState = kSt01_1;
    // ファンイン0に0を伝搬する．
    return fanin0_prop0();

  case kSt0X_X: // 0X:X -> 01:1
    mState = kSt01_1;
    // ファンイン1に1を伝搬する．
    return fanin1_prop1();

  case kSt1X_X: // 1X:X -> 10:1
    mState = kSt10_1;
    // ファンイン1に0を伝搬する．
    return fanin1_prop0();

  case kStXX_0: // illegal
  case kSt00_0: // illegal
  case kSt11_0: // illegal
    break;

  case kStXX_1: // no change
  case kSt01_1: // no change
  case kSt10_1: // no change
    return false;
  }
  return true;
}

END_NAMESPACE_YM_NETWORKS
