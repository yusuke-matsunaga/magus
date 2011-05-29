
/// @file core/NetCmd.cc
/// @brief NetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagBNetCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetCmd.h"
#include "ym_tclpp/TclPopt.h"
#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// Magus のコマンドオブジェクトの基底クラス NetCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr Magus の管理オブジェクト
// @param[in] new_bnet_enable -new_bnet オプションを使用するとき true
// @param[in] new_bdn_enable -new_bdn オプションを使用するとき true
// @param[in] new_mvn_enable -new_mvn オプションを使用するとき true
NetCmd::NetCmd(MagMgr* mgr,
	       bool new_bnet_enable,
	       bool new_bdn_enable,
	       bool new_mvn_enable) :
  MagCmd(mgr),
  mPoptNtwk(NULL),
  mPoptNewBNet(NULL),
  mPoptNewBdn(NULL),
  mPoptNewMvn(NULL),
  mNetworkSpecified(false)
{
  mPoptNtwk = new TclPoptObj(this, "network",
			     "specify target network",
			     "<network-name>");
  TclPoptGroup* popt_group = new_popt_group(mPoptNtwk);

  if ( new_bnet_enable ) {
    mPoptNewBNet = new TclPoptObj(this, "new_bnet",
				  "specify target with creating new bnetwork",
				  "<network-name>");
    add_popt(popt_group, mPoptNewBNet);
  }
  if ( new_bdn_enable ) {
    mPoptNewBdn = new TclPoptObj(this, "new_bdn",
				 "specify target with creating new bdnetwork",
				 "<network-name>");
    add_popt(popt_group, mPoptNewBdn);
  }
  if ( new_mvn_enable ) {
    mPoptNewMvn = new TclPoptObj(this, "new_mvn",
				 "specify target with creating new mvnetwork",
				 "<network-name>");
    add_popt(popt_group, mPoptNewMvn);
  }
}

// @brief デストラクタ
NetCmd::~NetCmd()
{
}

// コマンド行の引数を解析しネットワークとライブラリをセットする．
// エラーが起きた場合にはインタプリタにメッセージをセットし，
// false を返す．
// ネットワークおよびライブラリが指定されていた場合には，
// それぞれ mNetworkSpecified, mLibrarySpecified に true を入れる．
int
NetCmd::before_cmd_proc(TclObjVector& objv)
{
  mNetworkSpecified = false;

  if ( MagCmd::before_cmd_proc(objv) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  // ネットワークを指定したときに true とするフラグ
  bool ntwk_flag = false;

  // BNetwork を新規作成する時 true とするフラグ
  bool bnet_flag = false;

  // BdnMgr を新規作成する時 true とするフラグ
  bool bdn_flag = false;

  // MvNetwork を新規作成する時 true とするフラグ
  bool mvn_flag = false;

  // ネットワーク名
  TclObj name;
  if ( mPoptNtwk->is_specified() ) {
    name = mPoptNtwk->val();
    ntwk_flag = true;
  }
  else if ( mPoptNewBNet != NULL && mPoptNewBNet->is_specified() ) {
    bnet_flag = true;
    name = mPoptNewBNet->val();
    ntwk_flag = true;
  }
  else if ( mPoptNewBdn != NULL && mPoptNewBdn->is_specified() ) {
    bdn_flag = true;
    name = mPoptNewBdn->val();
    ntwk_flag = true;
  }
  else if ( mPoptNewMvn != NULL && mPoptNewMvn->is_specified() ) {
    mvn_flag = true;
    name = mPoptNewMvn->val();
    ntwk_flag = true;
  }


  if ( ntwk_flag ) {
    // name をネットワーク名と見なしてスタックに入れる．
    // 上書きを考慮して関数を直接呼ばずに Tcl スクリプト
    // を評価させる．
    TclObj script;
    if ( script.append_element("::magus::push_current_network") != TCL_OK ) {
      return TCL_ERROR;
    }
    if ( bnet_flag ) {
      if ( script.append_element("-new_bnet") != TCL_OK ) {
	return TCL_ERROR;
      }
    }
    else if ( bdn_flag ) {
      if ( script.append_element("-new_bdn") != TCL_OK ) {
	return TCL_ERROR;
      }
    }
    else if ( mvn_flag ) {
      if ( script.append_element("-new_mvn") != TCL_OK ) {
	return TCL_ERROR;
      }
    }
    // name は {-new_bnet <name>}の場合があるので単一の要素とは限らない．
    if ( script.append_list(name) != TCL_OK ) {
      return TCL_ERROR;
    }

    TclObj result_obj = result();
    if ( eval(script) != TCL_OK ) {
      // (たぶん)ネットワークが存在しなかった．
      return TCL_ERROR;
    }
    // push_current_network の実行結果は捨てる．
    set_result(result_obj);

    mNetworkSpecified = true;
  }

  return TCL_OK;
}

void
NetCmd::after_cmd_proc()
{
  if ( mNetworkSpecified ) {
    TclObj result_obj = result();

    // カレントネットワークをもとに戻す．
    // 上書きを考慮して関数を直接呼ばずに Tcl スクリプト
    // を評価させる．
    int stat = eval("::magus::pop_current_network");
    assert_cond( stat == TCL_OK, __FILE__, __LINE__);
    mNetworkSpecified = false;

    set_result(result_obj);
  }
}

END_NAMESPACE_MAGUS
