
/// @file ImpAnd.cc
/// @brief ImpAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpAnd.h"
#include "ImpMgr.h"

#define DEBUG_CHANGE_VALUE 0


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] handle0 ファンイン0のハンドル
// @param[in] handle1 ファンイン1のハンドル
ImpAnd::ImpAnd(ImpNodeHandle handle0,
	       ImpNodeHandle handle1) :
  ImpNode(handle0, handle1)
{
  mState = kStXX_X;
}

// @brief デストラクタ
ImpAnd::~ImpAnd()
{
}

// @brief AND タイプのときに true を返す．
bool
ImpAnd::is_and() const
{
  return true;
}

// @brief 出力値を返す．
Bool3
ImpAnd::val() const
{
  switch ( mState ) {
  case kStXX_X:
  case kSt1X_X:
  case kStX1_X:
    return kB3X;

  case kStXX_0:
  case kStX0_0:
  case kSt0X_0:
  case kSt00_0:
  case kSt10_0:
  case kSt01_0:
    return kB3False;

  case kSt11_1:
    return kB3True;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return kB3X;
}

// @brief ビットベクタ値の計算を行なう．
void
ImpAnd::calc_bitval()
{
  ymuint64 val0 = fanin0().src_node()->bitval();
  if ( fanin0().src_inv() ) {
    val0 = ~val0;
  }
  ymuint64 val1 = fanin1().src_node()->bitval();
  if ( fanin1().src_inv() ) {
    val1 = ~val1;
  }
  set_bitval(val0 & val1);
}

// @brief 状態を初期化する．
void
ImpAnd::clear()
{
  cout << "node#" << id() << " clear" << endl;
  mState = kStXX_X;
}

// @brief 状態を返す．
ymuint32
ImpAnd::cur_state() const
{
  return static_cast<ymuint32>(mState);
}

// @brief 状態を表す文字列を返す．
string
ImpAnd::cur_state_str() const
{
  switch ( mState ) {
  case kStXX_X: return "XX:X";
  case kSt1X_X: return "1X:X";
  case kStX1_X: return "X1:X";
  case kStXX_0: return "XX:0";
  case kStX0_0: return "X0:0";
  case kSt0X_0: return "0X:0";
  case kSt00_0: return "00:0";
  case kSt10_0: return "10:0";
  case kSt01_0: return "01:0";
  case kSt11_1: return "11:1";
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return "";
}

// @brief 状態を元にもどす．
void
ImpAnd::restore(ImpMgr& mgr,
		ymuint32 val)
{
  change_value(mgr, static_cast<tState>(val), false);
}

// @brief unjustified ノードの時 true を返す．
bool
ImpAnd::is_unjustified() const
{
  switch ( mState ) {
  case kSt1X_X:
  case kStX1_X:
  case kStXX_0:
    return true;

  default:
    break;
  }
  return false;
}

// @brief justification パタン数を得る．
ymuint
ImpAnd::justification_num()
{
  switch ( mState ) {
  case kSt1X_X:
    // 10:0 と 11:1
    return 2;

  case kStX1_X:
    // 01:0 と 11:1
    return 2;

  case kStXX_0:
    // 0X:0 と X0:0
    return 2;

  default:
    break;
  }
  return 0;
}

BEGIN_NONAMESPACE

inline
ImpDst
imp(const ImpEdge& e,
    ymuint val)
{
  if ( e.src_inv() ) {
    val ^= 1;
  }
  return ImpDst(e.src_node(), val);
}

END_NONAMESPACE

// @brief justification パタン を得る．
// @param[in] pos 位置番号 ( 0 <= pos < justification_num() )
// @return 値割り当て
ImpDst
ImpAnd::get_justification(ymuint pos)
{
  switch ( mState ) {
  case kSt1X_X:
    // 10:0 と 11:1
    if ( pos == 0 ) {
      // 10:0
      return imp(fanin1(), 0);
    }
    else if ( pos == 1 ) {
      // 11:1
      return imp(fanin1(), 1);
    }
    break;

  case kStX1_X:
    // 01:0 と 11:1
    if ( pos == 0 ) {
      return imp(fanin0(), 0);
    }
    else if ( pos == 1 ) {
      return imp(fanin0(), 1);
    }
    break;

  case kStXX_0:
    // 0X:0 と X0:0
    if ( pos == 0 ) {
      return imp(fanin0(), 0);
    }
    else if ( pos == 1 ) {
      return imp(fanin1(), 0);
    }
    break;

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return ImpDst(NULL, 0);
}

// @brief ファンイン0を0にする．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpAnd::fwd0_imp0(ImpMgr& mgr,
		  ImpRec& rec)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 0X:0
    change_value(mgr, kSt0X_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL, rec);

  case kStX1_X: // X1:X -> 01:0
    change_value(mgr, kSt01_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL, rec);

  case kStXX_0: // XX:0 -> 0X:0
    change_value(mgr, kSt0X_0);
    break;

  case kStX0_0: // X0:0 -> 00:0
    change_value(mgr, kSt00_0);
    break;

  case kSt1X_X: // illegal
  case kSt10_0: // illegal
  case kSt11_1: // illegal
    return false;

  case kSt0X_0: // no change
  case kSt01_0: // no change
  case kSt00_0: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン0を1にする．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpAnd::fwd0_imp1(ImpMgr& mgr,
		  ImpRec& rec)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 1X:X
    change_value(mgr, kSt1X_X);
    break;

  case kStX1_X: // X1:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンアウト先に1を伝搬する．
    return fanout_prop1(mgr, NULL, rec);

  case kStXX_0: // XX:0 -> 10:0
    change_value(mgr, kSt10_0);
    // ファンイン1に0を伝搬する．
    return fanin1_prop0(mgr, rec);

  case kStX0_0: // X0:0 -> 10:0
    change_value(mgr, kSt10_0);
    break;

  case kSt0X_0: // illegal
  case kSt00_0: // illegal
  case kSt01_0: // illegal
    return false;

  case kSt1X_X: // no change
  case kSt10_0: // no change
  case kSt11_1: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン1を0にする．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpAnd::fwd1_imp0(ImpMgr& mgr,
		  ImpRec& rec)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X0:0
    change_value(mgr, kStX0_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL, rec);

  case kSt1X_X: // 1X:X -> 10:0
    change_value(mgr, kSt10_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL, rec);

  case kSt0X_0: // 0X:0 -> 00:0
    change_value(mgr, kSt00_0);
    break;

  case kStXX_0: // XX:0 -> X0:0
    change_value(mgr, kStX0_0);
    break;

  case kStX1_X: // illegal
  case kSt01_0: // illegal
  case kSt11_1: // illegal
    return false;

  case kStX0_0: // no change
  case kSt00_0: // no change
  case kSt10_0: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン1を1にする．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpAnd::fwd1_imp1(ImpMgr& mgr,
		  ImpRec& rec)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X1:X
    change_value(mgr, kStX1_X);
    break;

  case kSt1X_X: // 1X:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンアウト先に1を伝搬する．
    return fanout_prop1(mgr, NULL, rec);

  case kStXX_0: // XX:0 -> 01:0
    change_value(mgr, kSt01_0);
    // ファンイン0に0を伝搬する．
    return fanin0_prop0(mgr, rec);

  case kSt0X_0: // 0X:0 -> 01:0
    change_value(mgr, kSt01_0);
    break;

  case kStX0_0: // illegal
  case kSt00_0: // illegal
  case kSt10_0: // illegal
    return false;

  case kStX1_X: // no change
  case kSt01_0: // no change
  case kSt11_1: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief 出力を0にする．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpAnd::bwd_imp0(ImpMgr& mgr,
		 ImpRec& rec)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX:0
    change_value(mgr, kStXX_0);
    break;

  case kSt1X_X: // 1X:X -> 10:0
    change_value(mgr, kSt10_0);
    // ファンイン1に0を伝搬する．
    return fanin1_prop0(mgr, rec);

  case kStX1_X: // X1:X -> 01:0
    change_value(mgr, kSt01_0);
    // ファンイン0に0を伝搬する．
    return fanin0_prop0(mgr, rec);

  case kStXX_0: // no change
  case kStX0_0: // no change
  case kSt0X_0: // no change
  case kSt00_0: // no change
  case kSt10_0: // no change
  case kSt01_0: // no change
    break;

  case kSt11_1: // illegal
    return false;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief 出力を1にする．
// @param[in] mgr ImpMgr
// @param[in] rec 含意を記録するオブジェクト
bool
ImpAnd::bwd_imp1(ImpMgr& mgr,
		 ImpRec& rec)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン0に1を伝搬する．
    // ファンイン1に1を伝搬する．
    return fanin0_prop1(mgr, rec) && fanin1_prop1(mgr, rec);

  case kSt1X_X: // 1X:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン1に1を伝搬する．
    return fanin1_prop1(mgr, rec);

  case kStX1_X: // X1:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン0に1を伝搬する．
    return fanin0_prop1(mgr, rec);

  case kStXX_0: // illegal
  case kStX0_0: // illegal
  case kSt0X_0: // illegal
  case kSt00_0: // illegal
  case kSt10_0: // illegal
  case kSt01_0: // illegal
    return false;

  case kSt11_1: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン0を0にする．
// @param[in] mgr ImMgr
bool
ImpAnd::fwd0_imp0(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 0X:0
    change_value(mgr, kSt0X_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL);

  case kStX1_X: // X1:X -> 01:0
    change_value(mgr, kSt01_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL);

  case kStXX_0: // XX:0 -> 0X:0
    change_value(mgr, kSt0X_0);
    break;

  case kStX0_0: // X0:0 -> 00:0
    change_value(mgr, kSt00_0);
    break;

  case kSt1X_X: // illegal
  case kSt10_0: // illegal
  case kSt11_1: // illegal
    return false;

  case kSt0X_0: // no change
  case kSt01_0: // no change
  case kSt00_0: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン0を1にする．
// @param[in] mgr ImMgr
bool
ImpAnd::fwd0_imp1(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 1X:X
    change_value(mgr, kSt1X_X);
    break;

  case kStX1_X: // X1:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンアウト先に1を伝搬する．
    return fanout_prop1(mgr, NULL);

  case kStXX_0: // XX:0 -> 10:0
    change_value(mgr, kSt10_0);
    // ファンイン1に0を伝搬する．
    return fanin1_prop0(mgr);

  case kStX0_0: // X0:0 -> 10:0
    change_value(mgr, kSt10_0);
    break;

  case kSt0X_0: // illegal
  case kSt00_0: // illegal
  case kSt01_0: // illegal
    return false;

  case kSt1X_X: // no change
  case kSt10_0: // no change
  case kSt11_1: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン1を0にする．
// @param[in] mgr ImMgr
bool
ImpAnd::fwd1_imp0(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X0:0
    change_value(mgr, kStX0_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL);

  case kSt1X_X: // 1X:X -> 10:0
    change_value(mgr, kSt10_0);
    // ファンアウト先に0を伝搬する．
    return fanout_prop0(mgr, NULL);

  case kSt0X_0: // 0X:0 -> 00:0
    change_value(mgr, kSt00_0);
    break;

  case kStXX_0: // XX:0 -> X0:0
    change_value(mgr, kStX0_0);
    break;

  case kStX1_X: // illegal
  case kSt01_0: // illegal
  case kSt11_1: // illegal
    return false;

  case kStX0_0: // no change
  case kSt00_0: // no change
  case kSt10_0: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief ファンイン1を1にする．
// @param[in] mgr ImMgr
bool
ImpAnd::fwd1_imp1(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X1:X
    change_value(mgr, kStX1_X);
    break;

  case kSt1X_X: // 1X:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンアウト先に1を伝搬する．
    return fanout_prop1(mgr, NULL);

  case kStXX_0: // XX:0 -> 01:0
    change_value(mgr, kSt01_0);
    // ファンイン0に0を伝搬する．
    return fanin0_prop0(mgr);

  case kSt0X_0: // 0X:0 -> 01:0
    change_value(mgr, kSt01_0);
    break;

  case kStX0_0: // illegal
  case kSt00_0: // illegal
  case kSt10_0: // illegal
    return false;

  case kStX1_X: // no change
  case kSt01_0: // no change
  case kSt11_1: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief 出力を0にする．
// @param[in] mgr ImMgr
bool
ImpAnd::bwd_imp0(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX:0
    change_value(mgr, kStXX_0);
    break;

  case kSt1X_X: // 1X:X -> 10:0
    change_value(mgr, kSt10_0);
    // ファンイン1に0を伝搬する．
    return fanin1_prop0(mgr);

  case kStX1_X: // X1:X -> 01:0
    change_value(mgr, kSt01_0);
    // ファンイン0に0を伝搬する．
    return fanin0_prop0(mgr);

  case kStXX_0: // no change
  case kStX0_0: // no change
  case kSt0X_0: // no change
  case kSt00_0: // no change
  case kSt10_0: // no change
  case kSt01_0: // no change
    break;

  case kSt11_1: // illegal
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
ImpAnd::bwd_imp1(ImpMgr& mgr)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン0に1を伝搬する．
    // ファンイン1に1を伝搬する．
    return fanin0_prop1(mgr) && fanin1_prop1(mgr);

  case kSt1X_X: // 1X:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン1に1を伝搬する．
    return fanin1_prop1(mgr);

  case kStX1_X: // X1:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン0に1を伝搬する．
    return fanin0_prop1(mgr);

  case kStXX_0: // illegal
  case kStX0_0: // illegal
  case kSt0X_0: // illegal
  case kSt00_0: // illegal
  case kSt10_0: // illegal
  case kSt01_0: // illegal
    return false;

  case kSt11_1: // no change
    break;

  default:
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
ImpAnd::prop_const(ImpMgr& mgr,
		   ymuint val,
		   ymuint ipos)
{
  switch ( mState ) {
  case kStXX_X:
    if ( ipos == 0 ) {
      if ( val == 0 ) {
	change_value(mgr, kSt0X_0, false);
	cout << "Node#" << id() << " is const0" << endl;
	set_const(mgr, 0);
      }
      else {
	change_value(mgr, kSt1X_X, false);
      }
    }
    else {
      if ( val == 0 ) {
	change_value(mgr, kStX0_0, false);
	cout << "Node#" << id() << " is const0" << endl;
	set_const(mgr, 0);
      }
      else {
	change_value(mgr, kStX1_X, false);
      }
    }
    break;

  case kStX1_X:
    if ( ipos == 0 ) {
      if ( val == 0 ) {
	change_value(mgr, kSt01_0, false);
	cout << "Node#" << id() << " is const0" << endl;
	set_const(mgr, 0);
      }
      else {
	change_value(mgr, kSt11_1, false);
	cout << "Node#" << id() << " is const1" << endl;
	set_const(mgr, 1);
      }
    }
    else {
      assert_cond( val == 1, __FILE__, __LINE__);
    }
    break;

  case kSt1X_X:
    if ( ipos == 0 ) {
      assert_cond( val == 1, __FILE__, __LINE__);
    }
    else {
      if ( val == 0 ) {
	change_value(mgr, kSt10_0, false);
	cout << "Node#" << id() << " is const0" << endl;
	set_const(mgr, 0);
      }
      else {
	change_value(mgr, kSt11_1, false);
	cout << "Node#" << id() << " is const1" << endl;
	set_const(mgr, 1);
      }
    }
    break;

  case kStXX_0:
    if ( ipos == 0 ) {
      if ( val == 0 ) {
	change_value(mgr, kSt0X_0, false);
      }
      else {
	change_value(mgr, kSt10_0, false);
      }
    }
    else {
      if ( val == 0 ) {
	change_value(mgr, kStX0_0, false);
      }
      else {
	change_value(mgr, kSt01_0, false);
      }
    }
    break;

  case kStX0_0:
    if ( ipos == 0 ) {
      // どうでもいい
    }
    else {
      assert_cond( val == 0, __FILE__, __LINE__);
    }
    break;

  case kSt0X_0:
    if ( ipos == 0 ) {
      assert_cond( val == 0, __FILE__, __LINE__);
    }
    else {
      // どうでもいい
    }
    break;

  case kSt00_0:
    assert_cond( val == 0, __FILE__, __LINE__);
    break;

  case kSt01_0:
    if ( ipos == 0 ) {
      assert_cond( val == 0, __FILE__, __LINE__);
    }
    else {
      assert_cond( val == 1, __FILE__, __LINE__);
    }
    break;

  case kSt10_0:
    if ( ipos == 0 ) {
      assert_cond( val == 1, __FILE__, __LINE__);
    }
    else {
      assert_cond( val == 0, __FILE__, __LINE__);
    }
    break;

  case kSt11_1:
    assert_cond( val == 1, __FILE__, __LINE__);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

END_NAMESPACE_YM_NETWORKS
