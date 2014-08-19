
/// @file magus/equiv/EquivCmd.cc
/// @brief EquivCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EquivCmd.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EquivCmd.h"
#include "cec_nsdef.h"
#include "YmTclpp/TclPopt.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/MvnMgr.h"
#include "YmNetworks/BNetBdnConv.h"
#include "YmNetworks/MvnBdnConv.h"
#include "YmNetworks/MvnBdnMap.h"


#define DEBUG_OUTPUTS 1


BEGIN_NAMESPACE_MAGUS

BEGIN_NONAMESPACE

// 2つのネットワークの入力，出力間の対応関係を取るための方法
enum tAssoc {
  kAssocNone,   // 指定されていない
  kAssocOrder,  // 回路記述順
  kAssocName    // 名前
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
  tAssoc assoc_mode = kAssocNone;

  if ( mPoptLoglevel->is_specified() ) {
    mLogLevel = mPoptLoglevel->val();
  }
  else {
    TclObj obj = var("magus::default", "equiv_verbose_level",
		     TCL_GLOBAL_ONLY | TCL_LEAVE_ERR_MSG);
    if ( obj.ptr() == NULL ) {
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
    assoc_mode = kAssocOrder;
  }
  else if ( mPoptName->is_specified() ) {
    assoc_mode = kAssocName;
  }
  else {
    TclObj obj = var("magus::default", "equiv_assoc_mode",
		     TCL_GLOBAL_ONLY | TCL_LEAVE_ERR_MSG);
    if ( obj.ptr() == NULL ) {
      return TCL_ERROR;
    }
    string val = obj;
    if ( val == "name" ) {
      assoc_mode = kAssocName;
    }
    else if ( val == "order" ) {
      assoc_mode = kAssocOrder;
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
    if ( obj.ptr() == NULL ) {
      return TCL_ERROR;
    }
    string val = obj;
    int stat = get_time(val, mTimeLimit);
    if ( stat != TCL_OK ) {
      return TCL_ERROR;
    }
  }

  if ( mPoptSat->is_specified() ) {
    mSatType = mPoptSat->val();
  }
  else {
    mSatType = "ymsat";
  }

  mSatOut = NULL;
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

  ymuint objc = objv.size();
  if ( objc != 2 && objc != 3 ) {
    print_usage();
    return TCL_ERROR;
  }

  NetHandle* neth1 = find_nethandle(objv[1]);
  NetHandle* neth2 = NULL;
  if ( neth1 == NULL ) {
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
    if ( neth2 == NULL ) {
      // 見付からなかった．
      // エラーメッセージはセットされている．
      return TCL_ERROR;
    }
  }

  conv_to_bdn(neth1, mNetwork1);
  conv_to_bdn(neth2, mNetwork2);

  // 最も簡単な比較．入力，出力数を調べる．
  ymuint ni1 = mNetwork1.input_num();
  ymuint no1 = mNetwork1.output_num();
  ymuint ni2 = mNetwork2.input_num();
  ymuint no2 = mNetwork2.output_num();

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
  case kAssocOrder:
    // これは必ず成功する．
    assoc_by_order(mNetwork1, mNetwork2,
		   mInputMatch, mOutputMatch);
    break;

  case kAssocName:
    if ( !assoc_by_name(mNetwork1, mNetwork2,
			mInputMatch, mOutputMatch) ) {
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
EquivCmdBase::assoc_by_order(const BdnMgr& network1,
			     const BdnMgr& network2,
			     vector<pair<ymuint32, ymuint32> >& iassoc,
			     vector<pair<ymuint32, ymuint32> >& oassoc)
{
  iassoc.clear();
  oassoc.clear();

  // まずは外部入力ノードの対応関係を取る．
  const BdnNodeList& inputs1 = network1.input_list();
  const BdnNodeList& inputs2 = network2.input_list();
  BdnNodeList::const_iterator p1 = inputs1.begin();
  BdnNodeList::const_iterator p2 = inputs2.begin();
  while ( p1 != inputs1.end() &&
	  p2 != inputs2.end() ) {
    BdnNode* node1 = *p1;
    BdnNode* node2 = *p2;
    iassoc.push_back(make_pair(node1->id(), node2->id()));
    ++ p1;
    ++ p2;
  }
  assert_cond(p1 == inputs1.end(), __FILE__, __LINE__);
  assert_cond(p2 == inputs2.end(), __FILE__, __LINE__);

  // 次は外部出力ノードの対応関係を取る．
  const BdnNodeList& outputs1 = network1.output_list();
  const BdnNodeList& outputs2 = network2.output_list();
  p1 = outputs1.begin();
  p2 = outputs2.begin();
  while ( p1 != outputs1.end() &&
	  p2 != outputs2.end() ) {
    BdnNode* node1 = *p1;
    BdnNode* node2 = *p2;
    oassoc.push_back(make_pair(node1->id(), node2->id()));
    ++ p1;
    ++ p2;
  }
  assert_cond(p1 == outputs1.end(), __FILE__, __LINE__);
  assert_cond(p2 == outputs2.end(), __FILE__, __LINE__);
}

// 名前で対応を取る．
bool
EquivCmdBase::assoc_by_name(const BdnMgr& network1,
			    const BdnMgr& network2,
			    vector<pair<ymuint32, ymuint32> >& iassoc,
			    vector<pair<ymuint32, ymuint32> >& oassoc)
{
  iassoc.clear();
  oassoc.clear();

  ymuint np1 = network1.port_num();
  ymuint np2 = network2.port_num();
  if ( np1 != np2 ) {
    TclObj emsg;
    emsg << "Number of ports is different";
    set_result(emsg);
    return false;
  }

  for (ymuint i = 0; i < np1; ++ i) {
    const BdnPort* port1 = network1.port(i);
    ymuint bw1 = port1->bit_width();
    vector<ymuint> iov1;
    port1->get_iovect(iov1);
    // とりあえず単純な線形探索
    bool found = false;
    for (ymuint j = 0; j < np2; ++ j) {
      const BdnPort* port2 = network2.port(j);
      if ( port1->name() == port2->name() ) {
	found = true;
	ymuint bw2 = port2->bit_width();
	if ( bw1 != bw2 ) {
	  TclObj emsg;
	  emsg << "bitwidth of port '" << port1->name()
	       << "' is different";
	  set_result(emsg);
	  return false;
	}
	vector<ymuint> iov2;
	port2->get_iovect(iov2);
	for (ymuint k = 0; k < bw1; ++ k) {
	  if ( iov1[k] != iov2[k] ) {
	    TclObj emsg;
	    emsg
	      << "direction of port '"
	      << port1->name()
	      << "] is different";
	    set_result(emsg);
	    return false;
	  }
	  if ( iov1[k] & 1 ) {
	    const BdnNode* node1 = port1->input(k);
	    const BdnNode* node2 = port2->input(k);
	    assert_cond( node1 != NULL, __FILE__, __LINE__);
	    assert_cond( node2 != NULL, __FILE__, __LINE__);
	    iassoc.push_back(make_pair(node1->id(), node2->id()));
	  }
	  if ( iov1[k] & 2 ) {
	    const BdnNode* node1 = port1->output(k);
	    const BdnNode* node2 = port2->output(k);
	    assert_cond( node1 != NULL, __FILE__, __LINE__);
	    assert_cond( node2 != NULL, __FILE__, __LINE__);
	    oassoc.push_back(make_pair(node1->id(), node2->id()));
	  }
	}
	break;
      }
    }
    if ( found == false ) {
      TclObj emsg;
      emsg << "port '" << port1->name() << "' does not found"
	   << " in network2";
      set_result(emsg);
      return false;
    }
  }

  return true;
}

// @brief 対象のネットワークを BDN に変換する．
// @param[in] net_handle ネットワークハンドル
// @param[out] dst_network 変換したネットワークを格納する変数
void
EquivCmdBase::conv_to_bdn(const NetHandle* net_handle,
			  BdnMgr& dst_network)
{
  switch ( net_handle->type() ) {
  case NetHandle::kMagBNet:
    {
      const BNetwork& bnet = *(net_handle->bnetwork());
      BNetBdnConv conv;
      conv(bnet, dst_network);
    }
    break;

  case NetHandle::kMagBdn:
    // ただのコピー
    // 本当はコピーする必要はないけど他のケースと同じ扱いにするため．
    dst_network = *(net_handle->bdn());
    break;

  case NetHandle::kMagMvn:
    {
      const MvnMgr& mvn = *(net_handle->mvn());
      MvnBdnConv conv;
      MvnBdnMap mvnode_map(mvn.max_node_id());
      conv(mvn, dst_network, mvnode_map);
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

  try {
    ymuint sigsize = 1;
    if ( mPoptSigSize->is_specified() ) {
      sigsize = mPoptSigSize->val();
    }

    vector<Bool3> comp_stats;
    nsYm::nsCec::check_ceq(network1(), network2(),
			   input_match(), output_match(),
			   log_level(), &cout,
			   sat_type(), sat_option(), sat_out(),
			   sigsize,
			   comp_stats);

    bool has_neq = false;
    bool has_abt = false;
    // statistics を Tcl 変数にセットする．
    // 本当はエラーチェックをする必要があるがここでは無視する．
    ymuint no = network1().output_num();
    for (ymuint i = 0; i < no; ++ i) {
      string str;
      switch ( comp_stats[i] ) {
      case kB3True:  str = "Equivalent"; break;
      case kB3False: str = "Not Equivalent"; has_neq = true; break;
      case kB3X:     str = "Aborted"; has_abt = true; break;
      }
      TclObj buf;
      buf << "o" << TclObj(i) << "result";
      set_var("::magus::equiv_stats", buf, str,
	      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    }
#if defined(DEBUG_OUTPUTS)
    {
      const BdnNodeList& outputs1 = network1().output_list();
      const BdnNodeList& outputs2 = network2().output_list();
      BdnNodeList::const_iterator o1 = outputs1.begin();
      BdnNodeList::const_iterator o2 = outputs2.begin();
      for (ymuint i =0; o1 != outputs1.end();
	   ++ o1, ++ o2, ++ i) {
	if ( comp_stats[i] == kB3False ) {
	  const BdnNode* node1 = *o1;
	  const BdnNode* node2 = *o2;
	  cout << "Node#" << node1->id() << "@network1 and "
	       << "Node#" << node2->id() << "@network2 are not equivalent" << endl;
	}
	else if ( comp_stats[i] == kB3X ) {
	  const BdnNode* node1 = *o1;
	  const BdnNode* node2 = *o2;
	  cout << "Node#" << node1->id() << "@network1 and "
	       << "Node#" << node2->id() << "@network2 are unknown" << endl;
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
    ymuint sigsize = 1;
    if ( mPoptSigSize->is_specified() ) {
      sigsize = mPoptSigSize->val();
    }

    vector<Bool3> comp_stats;
    nsYm::nsCec::check_ceq2(network1(), network2(),
			    input_match(), output_match(),
			    log_level(), &cout,
			    sat_type(), sat_option(), sat_out(),
			    sigsize,
			    comp_stats);

    bool has_neq = false;
    bool has_abt = false;
    // statistics を Tcl 変数にセットする．
    // 本当はエラーチェックをする必要があるがここでは無視する．
    ymuint no = network1().output_num();
    for (ymuint i = 0; i < no; ++ i) {
      string str;
      switch ( comp_stats[i] ) {
      case kB3True:  str = "Equivalent"; break;
      case kB3False: str = "Not Equivalent"; has_neq = true; break;
      case kB3X:     str = "Aborted"; has_abt = true; break;
      }
      TclObj buf;
      buf << "o" << TclObj(i) << "result";
      set_var("::magus::equiv_stats", buf, str,
	      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG);
    }
#if defined(DEBUG_OUTPUTS)
    {
      const BdnNodeList& outputs1 = network1().output_list();
      const BdnNodeList& outputs2 = network2().output_list();
      BdnNodeList::const_iterator o1 = outputs1.begin();
      BdnNodeList::const_iterator o2 = outputs2.begin();
      for (ymuint i =0; o1 != outputs1.end();
	   ++ o1, ++ o2, ++ i) {
	if ( comp_stats[i] == kB3False ) {
	  const BdnNode* node1 = *o1;
	  const BdnNode* node2 = *o2;
	  cout << "Node#" << node1->id() << "@network1 and "
	       << "Node#" << node2->id() << "@network2 are not equivalent" << endl;
	}
	else if ( comp_stats[i] == kB3X ) {
	  const BdnNode* node1 = *o1;
	  const BdnNode* node2 = *o2;
	  cout << "Node#" << node1->id() << "@network1 and "
	       << "Node#" << node2->id() << "@network2 are unknown" << endl;
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
