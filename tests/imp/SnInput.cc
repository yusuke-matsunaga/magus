
/// @file SnInput.cc
/// @brief SnInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SnInput.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス SnInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
SnInput::SnInput(ymuint id) :
  StrNode(id, NULL, false, NULL, false)
{
  clear();
}

// @brief デストラクタ
SnInput::~SnInput()
{
}

// @brief 外部入力のときに true を返す．
bool
SnInput::is_input() const
{
  return true;
}

// @brief 値を返す．
ymuint
SnInput::val() const
{
  switch ( mState ) {
  case kStX:
    return 0;

  case kSt0:
    return 1;

  case kSt1:
    return 2;

  default:
    cout << "mState = " << mState << endl;
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return 0;
}

// @brief ビットベクタ値の計算を行なう．
void
SnInput::calc_bitval()
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 状態を初期化する．
void
SnInput::clear()
{
  mState = kStX;
}


// @brief ファンイン0を0にする．
bool
SnInput::fwd0_imp0()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン0を1にする．
bool
SnInput::fwd0_imp1()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン1を0にする．
bool
SnInput::fwd1_imp0()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン1を1にする．
bool
SnInput::fwd1_imp1()
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 出力を0にする．
bool
SnInput::bwd_imp0()
{
  switch ( mState ) {
  case kStX: // X -> 0
    mChangedList.push_back(this);
    mState = kSt0;
    break;

  case kSt0: // no change
    break;

  case kSt1: // illegal
    return false;

  default:
    cout << "mState = " << mState << endl;
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return true;
}

// @brief 出力を1にする．
bool
SnInput::bwd_imp1()
{
  switch ( mState ) {
  case kStX: // X -> 1
    mChangedList.push_back(this);
    mState = kSt1;
    break;

  case kSt0: // illegal
    return false;

  case kSt1: // no change
    break;

  default:
    cout << "mState = " << mState << endl;
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

END_NAMESPACE_YM_NETWORKS
