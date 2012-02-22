
/// @file SnXor.cc
/// @brief SnXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "SnXor.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス SnXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] node0 ファンイン0のノード
// @param[in] inv0 ファンイン0の極性
// @param[in] node1 ファンイン1のノード
// @param[in] inv1 ファンイン1の極性
SnXor::SnXor(ymuint id,
	     StrNode* node0,
	     bool inv0,
	     StrNode* node1,
	     bool inv1) :
  StrNode(id, node0, inv0, node1, inv1)
{
  clear();
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

// @brief 出力値を返す．
Bool3
SnXor::val() const
{
  switch ( mState ) {
  case kStXX_X:
  case kStX0_X:
  case kStX1_X:
  case kSt0X_X:
  case kSt1X_X:
    return kB3X;

  case kStXX_0:
  case kSt00_0:
  case kSt11_0:
    return kB3False;

  case kStXX_1:
  case kSt01_1:
  case kSt10_1:
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
SnXor::calc_bitval()
{
  ymuint64 val0 = fanin0().src_node()->bitval();
  if ( fanin0().src_inv() ) {
    val0 = ~val0;
  }
  ymuint64 val1 = fanin1().src_node()->bitval();
  if ( fanin1().src_inv() ) {
    val1 = ~val1;
  }
  set_bitval(val0 ^ val1);
}

// @brief 状態を初期化する．
void
SnXor::clear()
{
  mState = kStXX_X;
}

// @brief 状態を返す．
ymuint32
SnXor::cur_state() const
{
  return static_cast<ymuint32>(mState);
}

// @brief 状態を元にもどす．
void
SnXor::restore(ymuint32 val)
{
  mState = static_cast<tState>(val);
}

// @brief ファンイン0を0にする．
// @param[in] mgr ImpMgr
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnXor::fwd0_imp0(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 0X:X
    change_value(mgr, kSt0X_X);
    break;

  case kStXX_0: // XX:0 -> 00:0
    change_value(mgr, kSt00_0);
    // ファンイン1に0を伝搬する．
    return mgr.fanin1_prop0(this, imp_list);

  case kStXX_1: // XX:1 -> 01:1
    change_value(mgr, kSt01_1);
    // ファンイン1に1を伝搬する．
    return mgr.fanin1_prop1(this, imp_list);

  case kStX0_X: // X0:X -> 00:0
    change_value(mgr, kSt00_0);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kStX1_X: // X1:X -> 01:1
    change_value(mgr, kSt01_1);
    // ファンアウト先に1を伝搬する．
    return mgr.fwd_prop1(this, imp_list);

  case kSt0X_X: // no change
  case kSt00_0: // no change
  case kSt01_1: // no change
    break;

  case kSt1X_X: // illegal
  case kSt10_1: // illegal
  case kSt11_0: // illegal
    return false;

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
SnXor::fwd0_imp1(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> 1X:X
    change_value(mgr, kSt1X_X);
    break;

  case kStXX_0: // XX:0 -> 11:0
    change_value(mgr, kSt11_0);
    // ファンイン1に1を伝搬する．
    return mgr.fanin1_prop1(this, imp_list);

  case kStXX_1: // XX:1 -> 10:1
    change_value(mgr, kSt10_1);
    // ファンイン1に0を伝搬する．
    return mgr.fanin1_prop0(this, imp_list);

  case kStX0_X: // X0:X -> 10:1
    change_value(mgr, kSt10_1);
    // ファンアウト先に1を伝搬する．
    return mgr.fwd_prop1(this, imp_list);

  case kStX1_X: // X1:X -> 11:0
    change_value(mgr, kSt11_0);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kSt0X_X: // illegal
  case kSt00_0: // illegal
  case kSt01_1: // illegal
    return false;

  case kSt1X_X: // no change
  case kSt10_1: // no change
  case kSt11_0: // no change
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
SnXor::fwd1_imp0(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X0:X
    change_value(mgr, kStX0_X);
    break;

  case kStXX_0: // XX:0 -> 00:0
    change_value(mgr, kSt00_0);
    // ファンイン0に0を伝搬する．
    return mgr.fanin0_prop0(this, imp_list);

  case kStXX_1: // XX:1 -> 10:1
    change_value(mgr, kSt10_1);
    // ファンイン0に1を伝搬する．
    return mgr.fanin0_prop1(this, imp_list);

  case kSt0X_X: // 0X:X -> 00:0
    change_value(mgr, kSt00_0);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kSt1X_X: // 1X:X -> 10:1
    change_value(mgr, kSt10_1);
    // ファンアウト先に1を伝搬する．
    return mgr.fwd_prop1(this, imp_list);

  case kStX0_X: // no change
  case kSt00_0: // no change
  case kSt10_1: // no change
    break;

  case kStX1_X: // illegal
  case kSt01_1: // illegal
  case kSt11_0: // illegal
    return false;

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
SnXor::fwd1_imp1(ImpMgr& mgr,
		 vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> X1:X
    change_value(mgr, kStX1_X);
    break;

  case kStXX_0: // XX:0 -> 11:0
    change_value(mgr, kSt11_0);
    // ファンイン0に1を伝搬する．
    return mgr.fanin0_prop1(this, imp_list);

  case kStXX_1: // XX:1 -> 01:1
    change_value(mgr, kSt01_1);
    // ファンイン0に0を伝搬する．
    return mgr.fanin0_prop0(this, imp_list);

  case kSt0X_X: // 0X:X -> 01:1
    change_value(mgr, kSt01_1);
    // ファンアウト先に1を伝搬する．
    return mgr.fwd_prop1(this, imp_list);

  case kSt1X_X: // 1X:X -> 11:0
    change_value(mgr, kSt11_0);
    // ファンアウト先に0を伝搬する．
    return mgr.fwd_prop0(this, imp_list);

  case kStX0_X: // illegal
  case kSt00_0: // illegal
  case kSt10_1: // illegal
    return false;

  case kStX1_X: // no change
  case kSt01_1: // no change
  case kSt11_0: // no change
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return true;
}

// @brief 出力を0にする．
// @param[in] mgr ImMgr
// @param[out] imp_list 含意の結果を格納するリスト
bool
SnXor::bwd_imp0(ImpMgr& mgr,
		vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX:0
    change_value(mgr, kStXX_0);
    break;

  case kStX0_X: // X0:X -> 00:0
    change_value(mgr, kSt00_0);
    // ファンイン0に0を伝搬する．
    return mgr.fanin0_prop0(this, imp_list);

  case kStX1_X: // X1:X -> 11:0
    change_value(mgr, kSt11_0);
    // ファンイン0に1を伝搬する．
    return mgr.fanin0_prop1(this, imp_list);

  case kSt0X_X: // 0X:X -> 00:0
    change_value(mgr, kSt00_0);
    // ファンイン1に0を伝搬する．
    return mgr.fanin1_prop0(this, imp_list);

  case kSt1X_X: // 1X:X -> 11:0
    change_value(mgr, kSt11_0);
    // ファンイン1に1を伝搬する．
    return mgr.fanin1_prop1(this, imp_list);

  case kStXX_0: // no change
  case kSt00_0: // no change
  case kSt11_0: // no change
    break;

  case kStXX_1: // illegal
  case kSt01_1: // illegal
  case kSt10_1: // illegal
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
SnXor::bwd_imp1(ImpMgr& mgr,
		vector<ImpCell>& imp_list)
{
  switch ( mState ) {
  case kStXX_X: // XX:X -> XX10
    change_value(mgr, kStXX_1);
    break;

  case kStX0_X: // X0:X -> 10:1
    change_value(mgr, kSt10_1);
    // ファンイン0に1を伝搬する．
    return mgr.fanin0_prop1(this, imp_list);

  case kStX1_X: // X1:X -> 01:1
    change_value(mgr, kSt01_1);
    // ファンイン0に0を伝搬する．
    return mgr.fanin0_prop0(this, imp_list);

  case kSt0X_X: // 0X:X -> 01:1
    change_value(mgr, kSt01_1);
    // ファンイン1に1を伝搬する．
    return mgr.fanin1_prop1(this, imp_list);

  case kSt1X_X: // 1X:X -> 10:1
    change_value(mgr, kSt10_1);
    // ファンイン1に0を伝搬する．
    return mgr.fanin1_prop0(this, imp_list);

  case kStXX_0: // illegal
  case kSt00_0: // illegal
  case kSt11_0: // illegal
    break;

  case kStXX_1: // no change
  case kSt01_1: // no change
  case kSt10_1: // no change
    return false;

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
SnXor::change_value(ImpMgr& mgr,
		    tState val)
{
  mgr.save_value(this, static_cast<ymuint32>(mState));
  mState = val;
}

END_NAMESPACE_YM_NETWORKS
