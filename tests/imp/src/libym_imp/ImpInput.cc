
/// @file ImpInput.cc
/// @brief ImpInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpInput.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ImpInput::ImpInput() :
  ImpNode(ImpNodeHandle::make_zero(), ImpNodeHandle::make_zero())
{
  clear();
}

// @brief デストラクタ
ImpInput::~ImpInput()
{
}

// @brief 外部入力のときに true を返す．
bool
ImpInput::is_input() const
{
  return true;
}

// @brief 出力値を返す．
Bool3
ImpInput::val() const
{
  switch ( mState ) {
  case kStX:
    return kB3X;

  case kSt0:
    return kB3False;

  case kSt1:
    return kB3True;

  default:
    cout << "mState = " << mState << endl;
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return kB3X;
}

// @brief ビットベクタ値の計算を行なう．
void
ImpInput::calc_bitval()
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 状態を初期化する．
void
ImpInput::clear()
{
  mState = kStX;
}

// @brief 状態を返す．
ymuint32
ImpInput::cur_state() const
{
  return static_cast<ymuint32>(mState);
}

// @brief 状態を元にもどす．
void
ImpInput::restore(ymuint32 val)
{
  mState = static_cast<tState>(val);
}

// @brief unjustified ノードの時 true を返す．
bool
ImpInput::is_unjustified() const
{
  return false;
}

// @brief justification パタン数を得る．
ymuint
ImpInput::justification_num()
{
  return 0;
}

// @brief justification パタン を得る．
// @param[in] pos 位置番号 ( 0 <= pos < justification_num() )
// @return 値割り当て
ImpDst
ImpInput::get_justification(ymuint pos)
{
  assert_not_reached(__FILE__, __LINE__);
  return ImpDst(NULL, 0);
}

// @brief ファンイン0を0にする．
// @param[in] mgr ImMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpInput::fwd0_imp0(ImpMgr& mgr,
		    ImpRec& rec)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン0を1にする．
// @param[in] mgr ImMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpInput::fwd0_imp1(ImpMgr& mgr,
		    ImpRec& rec)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン1を0にする．
// @param[in] mgr ImMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpInput::fwd1_imp0(ImpMgr& mgr,
		    ImpRec& rec)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン1を1にする．
// @param[in] mgr ImMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpInput::fwd1_imp1(ImpMgr& mgr,
		    ImpRec& rec)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 出力を0にする．
// @param[in] mgr ImMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpInput::bwd_imp0(ImpMgr& mgr,
		   ImpRec& rec)
{
  switch ( mState ) {
  case kStX: // X -> 0
    mgr.save_value(this, static_cast<ymuint32>(mState));
    mState = kSt0;
    break;

  case kSt0: // no change
    break;

  case kSt1: // illegal
    return false;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return true;
}

// @brief 出力を1にする．
// @param[in] mgr ImMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpInput::bwd_imp1(ImpMgr& mgr,
		   ImpRec& rec)
{
  switch ( mState ) {
  case kStX: // X -> 1
    mgr.save_value(this, static_cast<ymuint32>(mState));
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

// @brief ファンイン0を0にする．
// @param[in] mgr ImMgr
bool
ImpInput::fwd0_imp0(ImpMgr& mgr)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン0を1にする．
// @param[in] mgr ImMgr
bool
ImpInput::fwd0_imp1(ImpMgr& mgr)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン1を0にする．
// @param[in] mgr ImMgr
bool
ImpInput::fwd1_imp0(ImpMgr& mgr)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ファンイン1を1にする．
// @param[in] mgr ImMgr
bool
ImpInput::fwd1_imp1(ImpMgr& mgr)
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 出力を0にする．
// @param[in] mgr ImMgr
bool
ImpInput::bwd_imp0(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStX: // X -> 0
    mgr.save_value(this, static_cast<ymuint32>(mState));
    mState = kSt0;
    break;

  case kSt0: // no change
    break;

  case kSt1: // illegal
    return false;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return true;
}

// @brief 出力を1にする．
// @param[in] mgr ImMgr
bool
ImpInput::bwd_imp1(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStX: // X -> 1
    mgr.save_value(this, static_cast<ymuint32>(mState));
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

// @brief 定数伝搬を行なう．
// @param[in] mgr ImMgr
// @param[in] val 値
// @param[in] ipos 入力位置
void
ImpInput::prop_const(ImpMgr& mgr,
		     ymuint val,
		     ymuint ipos)
{
  assert_not_reached( __FILE__, __LINE__);
}

END_NAMESPACE_YM_NETWORKS
