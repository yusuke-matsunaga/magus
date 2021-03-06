﻿
/// @file magus/equiv/EquivCmd.cc
/// @brief EquivCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "EquivCmd.h"
#include "EquivMgr.h"
#include "ym/BnNode.h"
#include "ym/BnPort.h"
#include "ym/TclPopt.h"
#include "ym/MvnMgr.h"
#include "ym/Range.h"


#define DEBUG_OUTPUTS 1


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// 2つのネットワークの入力，出力間の対応関係を取るための方法
enum Assoc {
  None,   // 指定されていない
  Order,  // 回路記述順
  Name    // 名前
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス EquivCmdBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EquivCmdBase::EquivCmdBase(MagMgr* mgr) :
  MagCmd(mgr)
{
  mPoptLoglevel = new TclPoptInt(this, "loglevel",
				 "set log level");

  mPoptOrder = new TclPopt(this, "assoc_by_order",
			   "associate PI/PO pairs by their orders");
  mPoptName = new TclPopt(this, "assoc_by_name",
			  "associate PI/PO pairs by their names");
  new_popt_group(mPoptOrder, mPoptName);

  mPoptTimeLimit = new TclPoptObj(this, "time_limit",
				  "specify time limit",
				  "xxx\'s\'|xxx\'m\'|xxx\'h\'");

  mPoptSat = new TclPoptStr(this, "sat",
			    "specify SAT-Solver",
			    "ymsat|minisat|satlog");

  mPoptSatlog = new TclPoptStr(this, "satlog",
			       "output logfile for SAT solver",
			       "<filename>");
}

// @brief デストラクタ
EquivCmdBase::~EquivCmdBase()
{
}


// コマンド処理関数
int
EquivCmdBase::prologue(TclObjVector& objv)
{
  Assoc assoc_mode = Assoc::None;

  if ( mPoptLoglevel->is_specified() ) {
    mLogLevel = mPoptLoglevel->val();
  }
  else {
    TclObj obj = var("magus::default", "equiv_verbose_level",
		     TCL_GLOBAL_ONLY | TCL_LEAVE_ERR_MSG);
    if ( obj.ptr() == nullptr ) {
      return TCL_ERROR;
    }
    int stat = int_conv(obj, mLogLevel);
    if ( stat != TCL_OK ) {
      TclObj emsg;
      emsg << arg0() << " : value of default(equiv_verbose_level) "
	   << "should be an integer.";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  if ( mPoptOrder->is_specified() ) {
    assoc_mode = Assoc::Order;
  }
  else if ( mPoptName->is_specified() ) {
    assoc_mode = Assoc::Name;
  }
  else {
    TclObj obj = var("magus::default", "equiv_assoc_mode",
		     TCL_GLOBAL_ONLY | TCL_LEAVE_ERR_MSG);
    if ( obj.ptr() == nullptr ) {
      return TCL_ERROR;
    }
    string val = obj;
    if ( val == "name" ) {
      assoc_mode = Assoc::Name;
    }
    else if ( val == "order" ) {
      assoc_mode = Assoc::Order;
    }
    else {
      TclObj emsg;
      emsg << arg0()
	   << " : value of default(equiv_assoc_mode) should be "
	   << "\"name\" or \"order\".";
      set_result(emsg);
      return TCL_ERROR;
    }
  }

  if ( mPoptTimeLimit->is_specified() ) {
    int stat = get_time(mPoptTimeLimit->val(), mTimeLimit);
    if ( stat != TCL_OK ) {
      return stat;
    }
  }
  else {
    TclObj obj = var("magus::default", "equiv_time_limit",
		     TCL_GLOBAL_ONLY | TCL_LEAVE_ERR_MSG);
    if ( obj.ptr() == nullptr ) {
      return TCL_ERROR;
    }
    string val = obj;
    int stat = get_time(val, mTimeLimit);
    if ( stat != TCL_OK ) {
      return TCL_ERROR;
    }
  }

  if ( mPoptSat->is_specified() ) {
    mSatType = SatSolverType(mPoptSat->val());
  }
  else {
    ; // デフォルト
  }

#if 0
  mSatOut = nullptr;
  if ( mPoptSatlog->is_specified() ) {
    mSatType = "satlog";
    string satlog_file = mPoptSatlog->val();
    if ( satlog_file == "-" ) {
      mSatOut = &cout;
    }
    else {
      if ( !open_ofile(mSatLogFile, satlog_file) ) {
	TclObj emsg;
	emsg << satlog_file << " : No such file";
	set_result(emsg);
	return TCL_ERROR;
      }
      mSatOut = &mSatLogFile;
    }
  }
#endif

  int objc = objv.size();
  if ( objc != 2 && objc != 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  NetHandle* neth1 = find_nethandle(objv[1]);
  NetHandle* neth2 = nullptr;
  if ( neth1 == nullptr ) {
    // 見付からなかった．
    // エラーメッセージはセットされている．
    return TCL_ERROR;
  }

  if ( objc == 2 ) {
    neth2 = neth1;
    neth1 = cur_nethandle();
  }
  else {
    neth2 = find_nethandle(objv[2]);
    if ( neth2 == nullptr ) {
      // 見付からなかった．
      // エラーメッセージはセットされている．
      return TCL_ERROR;
    }
  }

  conv_to_bnet(neth1, mNetwork1);
  conv_to_bnet(neth2, mNetwork2);

  // 最も簡単な比較．入力，出力数を調べる．
  int ni1 = mNetwork1.input_num();
  int no1 = mNetwork1.output_num();
  int ni2 = mNetwork2.input_num();
  int no2 = mNetwork2.output_num();

  if ( ni1 != ni2 ) {
    TclObj result_obj = "The numbers of inputs differ.";
    set_result(result_obj);
    // 比較結果は偽だがコマンド自体は正しく実行されたので
    // TCL_OK を返す．
    return TCL_OK;
  }
  if ( no1 != no2 ) {
    TclObj result_obj = "The numbers of outputs differ.";
    set_result(result_obj);
    // 比較結果は偽だがコマンド自体は正しく実行されたので
    // TCL_OK を返す．
    return TCL_OK;
  }

  // 外部入力/外部出力の対応をとる．
  switch ( assoc_mode ) {
  case Assoc::Order:
    // これは必ず成功する．
    assoc_by_order();
    break;

  case Assoc::Name:
    if ( !assoc_by_name() ) {
      // 名前の対応がとれなかった．
      // エラーメッセージは assoc_by_name() 中で出力されている．
      return TCL_OK;
    }
    break;

  default:
    break;
  }

  return TCL_OK;
}

// 時間を表す文字列から int に直す．
// - ただの数字ならばそのまま単位は秒だと思う．
// - s が末尾に付いていたら単位は秒だと思う．
// - m が末尾に付いていたら単位は分だと思う．
// - h が末尾に付いていたら単位は時間だと思う．
// それ以外の場合には TCL_ERROR を返す．
int
EquivCmdBase::get_time(const string& str,
		       int& time)
{
  const char* p = str.c_str();
  char c;
  int factor = 1;
  int val = 0;
  while ( (c = *p ++) != '\0' ) {
    if ( isdigit(c) == 0 ) {
      switch (c) {
      case 's':	factor = 1; break;    // 秒
      case 'm': factor = 60; break;   // 分
      case 'h': factor = 3600; break; // 時間
      default:
	set_result("Syntax error in time specification.");
	return TCL_ERROR;
      }
      if (*p != '\0') {
	set_result("Syntax error in time specification.");
	return TCL_ERROR;
      }
      break;
    }
    else {
      val *= 10;
      val += c - '0';
    }
  }
  time = val * factor;

  return TCL_OK;
}

// ネットワーク中に現われる入力，出力の順番で対応を取る．
void
EquivCmdBase::assoc_by_order()
{
  // まずは外部入力ノードの対応関係を取る．
  int ni = mNetwork1.input_num();
  ASSERT_COND( mNetwork2.input_num() == ni );
  mInput2List.clear();
  mInput2List.resize(ni);
  for ( int i: Range(ni) ) {
    mInput2List[i] = i;
  }

  // 次は外部出力ノードの対応関係を取る．
  int no = mNetwork1.output_num();
  ASSERT_COND( mNetwork2.output_num() == no );
  mOutput2List.clear();
  mOutput2List.resize(no);
  for ( int i: Range(no) ) {
    mOutput2List[i] = i;
  }
}

// 名前で対応を取る．
bool
EquivCmdBase::assoc_by_name()
{
  int np1 = mNetwork1.port_num();
  int np2 = mNetwork2.port_num();
  if ( np1 != np2 ) {
    TclObj emsg;
    emsg << "Number of ports is different";
    set_result(emsg);
    return false;
  }

  int ni = mNetwork1.input_num();
  ASSERT_COND( mNetwork2.input_num() == ni );
  mInput2List.clear();
  mInput2List.reserve(ni);

  int no = mNetwork1.output_num();
  ASSERT_COND( mNetwork2.output_num() == no );
  mOutput2List.clear();
  mOutput2List.reserve(no);

  for ( int i: Range(np1) ) {
    auto& port1 = mNetwork1.port(i);
    int bw1 = port1.bit_width();

    // とりあえず単純な線形探索
    bool found = false;
    for ( int j: Range(np2) ) {
      auto& port2 = mNetwork2.port(j);
      if ( port1.name() == port2.name() ) {
	found = true;
	int bw2 = port2.bit_width();
	if ( bw1 != bw2 ) {
	  TclObj emsg;
	  emsg << "bitwidth of port '" << port1.name()
	       << "' is different";
	  set_result(emsg);
	  return false;
	}

	for ( int k: Range(bw1) ) {
	  auto& node1 = mNetwork1.node(port1.bit(k));
	  auto& node2 = mNetwork2.node(port2.bit(k));
	  if ( node1.type() != node2.type() ) {
	    TclObj emsg;
	    emsg
	      << "direction of port '"
	      << port1.name()
	      << "] is different";
	    set_result(emsg);
	    return false;
	  }
	  if ( node1.is_input() ) {
	    mInput2List[node2.input_pos()] = node1.input_pos();
	  }
	  if ( node1.is_output() ) {
	    mOutput2List[node2.output_pos()] = node1.output_pos();
	  }
	}
	break;
      }
    }
    if ( found == false ) {
      TclObj emsg;
      emsg << "port '" << port1.name() << "' does not found"
	   << " in network2";
      set_result(emsg);
      return false;
    }
  }

  return true;
}

// @brief 対象のネットワークを BnNetwork に変換する．
// @param[in] net_handle ネットワークハンドル
// @param[out] dst_network 変換したネットワークを格納する変数
void
EquivCmdBase::conv_to_bnet(const NetHandle* net_handle,
			   BnNetwork& dst_network)
{
  switch ( net_handle->type() ) {
  case NetHandle::kMagBn:
    {
      // ただのコピー
      // 本当はコピーする必要はないけど他のケースと同じ扱いにするため．
      dst_network.copy( *(net_handle->bnetwork()) );
    }
    break;

  case NetHandle::kMagMvn:
    {
#if 0
      const MvnMgr& mvn = *(net_handle->mvn());
      MvnBnConv conv;
      MvnBnMap mvnode_map(mvn.max_node_id());
      conv(mvn, dst_network, mvnode_map);
#endif
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EquivCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EquivCmd::EquivCmd(MagMgr* mgr) :
  EquivCmdBase(mgr)
{

  mPoptSigSize = new TclPoptInt(this, "sigsize",
				"specify the size of signature");

  set_usage_string("<network1> ?<network2>?");
}

// @brief デストラクタ
EquivCmd::~EquivCmd()
{
}


// コマンド処理関数
int
EquivCmd::cmd_proc(TclObjVector& objv)
{
  int stat1 = prologue(objv);
  if ( stat1 != TCL_OK ) {
    return stat1;
  }

#if 0
  try {
#endif
    int sigsize = 1;
    if ( mPoptSigSize->is_specified() ) {
      sigsize = mPoptSigSize->val();
    }

    MAGUS_NAMESPACE::EquivMgr equiv_mgr(sigsize, sat_type());

    EquivResult result = equiv_mgr.check(network1(),
					 network2(),
					 input2_list(),
					 output2_list());

    bool has_neq = false;
    bool has_abt = false;
    // statistics を Tcl 変数にセットする．
    // 本当はエラーチェックをする必要があるがここでは無視する．
    int no = network1().output_num();
    for ( int i = 0; i < no; ++ i ) {
      string str;
      switch ( result.output_results()[i] ) {
      case SatBool3::True:  str = "Equivalent"; break;
      case SatBool3::False: str = "Not Equivalent"; has_neq = true; break;
      case SatBool3::X:     str = "Aborted"; has_abt = true; break;
      }
      TclObj buf;
      buf << "o" << TclObj(i) << "result";
      set_var("::magus::equiv_stats", buf, str,
	      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    }
#if defined(DEBUG_OUTPUTS)
    {
      int no = network1().output_num();
      for ( int i = 0; i < no; ++ i ) {
	auto oid1 = network1().output_id(i);
	auto oid2 = output2_list()[i];
	auto& node1 = network1().node(oid1);
	auto& node2 = network2().node(oid2);
	if ( result.output_results()[i] == SatBool3::False ) {
	  cout << "Node#" << node1.fanin_id(0) << "@network1 and "
	       << "Node#" << node2.fanin_id(0) << "@network2 are not equivalent" << endl;
	}
	else if ( result.output_results()[i] == SatBool3::X ) {
	  cout << "Node#" << node1.fanin_id(0) << "@network1 and "
	       << "Node#" << node2.fanin_id(0) << "@network2 are unknown" << endl;
	}
      }
    }
#endif

    // 検証結果をセットする．
    TclObj result_obj;
    if ( has_abt ) {
      result_obj = "Aborted";
    }
    else if ( has_neq ) {
      result_obj = "Not Equivalent";
    }
    else {
      result_obj = "Equivalent";
    }
    set_result(result_obj);
#if 0
  }
  catch (AssertError x) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "AssertError";
    set_result(emsg);
    return TCL_ERROR;
  }
#endif
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// クラス EquivCmd2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EquivCmd2::EquivCmd2(MagMgr* mgr) :
  EquivCmdBase(mgr)
{

  mPoptSigSize = new TclPoptInt(this, "sigsize",
				"specify the size of signature");

  set_usage_string("<network1> ?<network2>?");
}

// @brief デストラクタ
EquivCmd2::~EquivCmd2()
{
}


// コマンド処理関数
int
EquivCmd2::cmd_proc(TclObjVector& objv)
{
  int stat1 = prologue(objv);
  if ( stat1 != TCL_OK ) {
    return stat1;
  }

  try {
    int sigsize = 1;
    if ( mPoptSigSize->is_specified() ) {
      sigsize = mPoptSigSize->val();
    }

    vector<SatBool3> comp_stats;
#if 0
    nsYm::nsCec::check_ceq2(network1(), network2(),
			    input_match(), output_match(),
			    log_level(), &cout,
			    sat_type(), sat_option(), sat_out(),
			    sigsize,
			    comp_stats);
#endif
    bool has_neq = false;
    bool has_abt = false;
    // statistics を Tcl 変数にセットする．
    // 本当はエラーチェックをする必要があるがここでは無視する．
    int no = network1().output_num();
    for ( int i = 0; i < no; ++ i ) {
      string str;
      switch ( comp_stats[i] ) {
      case SatBool3::True:  str = "Equivalent"; break;
      case SatBool3::False: str = "Not Equivalent"; has_neq = true; break;
      case SatBool3::X:     str = "Aborted"; has_abt = true; break;
      }
      TclObj buf;
      buf << "o" << TclObj(i) << "result";
      set_var("::magus::equiv_stats", buf, str,
	      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    }
#if defined(DEBUG_OUTPUTS)
    {
      int no = network1().output_num();
      for ( int i = 0; i < no; ++ i ) {
	int id1 = network1().output_id(i);
	auto& node1 = network1().node(id1);
	int id2 = network2().output_id(i);
	auto& node2 = network2().node(id2);
	if ( comp_stats[i] == SatBool3::False ) {
	  cout << "Node#" << node1.fanin_id(0) << "@network1 and "
	       << "Node#" << node2.fanin_id(0) << "@network2 are not equivalent" << endl;
	}
	else if ( comp_stats[i] == SatBool3::X ) {
	  cout << "Node#" << node1.fanin_id(0) << "@network1 and "
	       << "Node#" << node2.fanin_id(0) << "@network2 are unknown" << endl;
	}
      }
    }
#endif

    // 検証結果をセットする．
    TclObj result_obj;
    if ( has_abt ) {
      result_obj = "Aborted";
    }
    else if ( has_neq ) {
      result_obj = "Not Equivalent";
    }
    else {
      result_obj = "Equivalent";
    }
    set_result(result_obj);
  }
  catch (AssertError x) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "AssertError";
    set_result(emsg);
    return TCL_ERROR;
  }

  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// Equiv の初期化関数
//////////////////////////////////////////////////////////////////////

int
equiv_init(Tcl_Interp* interp,
	   MagMgr* mgr)
{
  // 等価検証を行うコマンド
  TclCmdBinder1<EquivCmd, MagMgr*>::reg(interp, mgr, "magus::equiv");
  TclCmdBinder1<EquivCmd2, MagMgr*>::reg(interp, mgr, "magus::equiv2");

  //////////////////////////////////////////////////////////////////////
  // デフォルト値を入れる変数の初期化
  //////////////////////////////////////////////////////////////////////
  // 直接 Tcl のスクリプトを実行させる．
  const char* const script =
    "set ::magus::default(equiv_assoc_mode) order\n"
    "set ::magus::default(equiv_verbose_level) 0\n"
    "set ::magus::default(equiv_time_limit) 0\n";
  if ( Tcl_Eval(interp, (char*) script) != TCL_OK ) {
    return TCL_ERROR;
  }


  //////////////////////////////////////////////////////////////////////
  // tclreadline 用の初期化コード
  //////////////////////////////////////////////////////////////////////
  const char* init =
    "namespace eval tclreadline {\n"
    "namespace eval magus {\n"
    "proc complete(equiv) { text start end line pos mod } { return \"\" }\n"
    "proc complete(equiv2) { text start end line pos mod } { return \"\" }\n"
    "}\n"
    "}\n";
  if ( Tcl_Eval(interp, (char*) init) == TCL_ERROR ) {
    return TCL_ERROR;
  }

  return TCL_OK;
}

END_NAMESPACE_MAGUS
