
/// @file SnAnd.cc
/// @brief SnAnd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "SnAnd.h"
#include "ImpMgr.h"


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
  clear();
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

// @brief 出力値を返す．
Bool3
SnAnd::val() const
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
    cout << "mState = " << mState << endl;
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return kB3X;
}

// @brief ビットベクタ値の計算を行なう．
void
SnAnd::calc_bitval()
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
SnAnd::clear()
{
  mState = kStXX_X;
}

// @brief 状態を返す．
ymuint32
SnAnd::cur_state() const
{
  return static_cast<ymuint32>(mState);
}

// @brief 状態を元にもどす．
void
SnAnd::restore(ymuint32 val)
{
  mState = static_cast<tState>(val);
}

// @brief ファンイン0を0にする．
// @param[in] mgr ImpMgr
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnAnd::fwd0_imp0(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 0X:0
    change_value(mgr, kSt0X_0);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kStX1_X: // X1:X -> 01:0
    change_value(mgr, kSt01_0);
    mgr.reset_unjustified(this);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kStXX_0: // XX:0 -> 0X:0
    change_value(mgr, kSt0X_0);
    mgr.reset_unjustified(this);
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
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnAnd::fwd0_imp1(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 1X:X
    change_value(mgr, kSt1X_X);
    mgr.set_unjustified(this);
    break;

  case kStX1_X: // X1:X -> 11:1
    change_value(mgr, kSt11_1);
    mgr.reset_unjustified(this);
    // ファンアウト先に1を伝搬する．
    return mgr.fwd_prop1(this, imp_list);

  case kStXX_0: // XX:0 -> 10:0
    change_value(mgr, kSt10_0);
    mgr.reset_unjustified(this);
    // ファンイン1に0を伝搬する．
    return mgr.fanin1_prop0(this, imp_list);

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
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnAnd::fwd1_imp0(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X0:0
    change_value(mgr, kStX0_0);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kSt1X_X: // 1X:X -> 10:0
    change_value(mgr, kSt10_0);
    mgr.reset_unjustified(this);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kSt0X_0: // 0X:0 -> 00:0
    change_value(mgr, kSt00_0);
    break;

  case kStXX_0: // XX:0 -> X0:0
    change_value(mgr, kStX0_0);
    mgr.reset_unjustified(this);
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
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnAnd::fwd1_imp1(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X1:X
    change_value(mgr, kStX1_X);
    mgr.set_unjustified(this);
    break;

  case kSt1X_X: // 1X:X -> 11:1
    change_value(mgr, kSt11_1);
    mgr.reset_unjustified(this);
    // ファンアウト先に1を伝搬する．
    return mgr.fwd_prop1(this, imp_list);

  case kStXX_0: // XX:0 -> 01:0
    change_value(mgr, kSt01_0);
    mgr.reset_unjustified(this);
    // ファンイン0に0を伝搬する．
    return mgr.fanin0_prop0(this, imp_list);

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
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnAnd::bwd_imp0(ImpMgr& mgr,
		vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX:0
    change_value(mgr, kStXX_0);
    mgr.set_unjustified(this);
    break;

  case kSt1X_X: // 1X:X -> 10:0
    change_value(mgr, kSt10_0);
    mgr.reset_unjustified(this);
    // ファンイン1に0を伝搬する．
    return mgr.fanin1_prop0(this, imp_list);

  case kStX1_X: // X1:X -> 01:0
    change_value(mgr, kSt01_0);
    mgr.reset_unjustified(this);
    // ファンイン0に0を伝搬する．
    return mgr.fanin0_prop0(this, imp_list);

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
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnAnd::bwd_imp1(ImpMgr& mgr,
		vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 11:1
    change_value(mgr, kSt11_1);
    // ファンイン0に1を伝搬する．
    // ファンイン1に1を伝搬する．
    return mgr.fanin0_prop1(this, imp_list) && mgr.fanin1_prop1(this, imp_list);

  case kSt1X_X: // 1X:X -> 11:1
    change_value(mgr, kSt11_1);
    mgr.reset_unjustified(this);
    // ファンイン1に1を伝搬する．
    return mgr.fanin1_prop1(this, imp_list);

  case kStX1_X: // X1:X -> 11:1
    change_value(mgr, kSt11_1);
    mgr.reset_unjustified(this);
    // ファンイン0に1を伝搬する．
    return mgr.fanin0_prop1(this, imp_list);

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

// @brief 値を変える．
// @param[in] mgr ImpMgr
// @param[in] val 値
void
SnAnd::change_value(ImpMgr& mgr,
		    tState val)
{
  mgr.save_value(this, static_cast<ymuint32>(mState));
  mState = val;
}

END_NAMESPACE_YM_NETWORKS
