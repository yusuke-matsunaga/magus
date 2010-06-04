
/// @file magus/cellmap/Conv2BNetCmd.cc
/// @brief Conv2BNetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Conv2BNetCmd.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CellmapCmd.h"
#include "CellNetwork.h"
#include "NetHandle.h"
#include <ym_bnet/BNetwork.h>
#include <ym_bnet/BNetManip.h>
#include <ym_tclpp/TclPopt.h>


BEGIN_NAMESPACE_MAGUS_CELLMAP

// CellNetwork を BNetwork に変換する．
void
lut2bnet(const CnGraph& src_network,
	 BNetwork& dst_network)
{
  size_t n = src_network.max_node_id();
  vector<BNode*> node_assoc(n);

  dst_network.clear();

  BNetManip manip(&dst_network);
  
  // 外部入力を作る．
  const CnNodeList& input_list = src_network.input_list();
  for (CnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    CnNode* src_node = *p;
    BNode* dst_node = manip.new_input(src_node->name());
    node_assoc[src_node->id()] = dst_node;
  }

  // 内部ノードを作る．
  vector<CnNode*> node_list;
  src_network.sort(node_list);
  for (vector<CnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    CnNode* src_node = *p;
    size_t ni = src_node->ni();
    vector<int> tv;
    src_node->tv(tv);
    BNode* dst_node = NULL;
    if ( ni == 0 ) {
      dst_node = manip.make_const(tv[0]);
    }
    else {
      vector<BNode*> dst_inodes(ni);
      for (size_t i = 0; i < ni; ++ i) {
	CnNode* src_inode = src_node->fanin(i);
	BNode* dst_inode = node_assoc[src_inode->id()];
	assert_cond(dst_inode, __FILE__, __LINE__);
	dst_inodes[i] = dst_inode;
      }
      LogExpr expr = LogExpr::make_zero();
      size_t np = 1U << ni;
      for (size_t p = 0; p < np; ++ p) {
	if ( tv[p] ) {
	  LogExpr prd = LogExpr::make_one();
	  for (size_t i = 0; i < ni; ++ i) {
	    if ( p & (1U << i) ) {
	      prd &= LogExpr::make_posiliteral(i);
	    }
	    else {
	      prd &= LogExpr::make_negaliteral(i);
	    }
	  }
	  expr |= prd;
	}
      }
      string name = src_node->name();
      if ( dst_network.find_node(name) ) {
	name = "";
      }
      dst_node = manip.new_logic(name);
      bool stat = manip.change_logic(dst_node, expr, dst_inodes, false);
      assert_cond(stat, __FILE__, __LINE__);
    }
    node_assoc[src_node->id()] = dst_node;
  }

  // 外部出力ノードを作る．
  const CnNodeList& output_list = src_network.output_list();
  for (CnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    CnNode* onode = *p;
    CnNode* inode = onode->fanin(0);
    BNode* dst_inode = node_assoc[inode->id()];
    assert_cond(dst_inode, __FILE__, __LINE__);
    BNode* dst_onode = manip.new_output(onode->name());
    bool stat = manip.change_output(dst_onode, dst_inode);
    assert_cond(stat, __FILE__, __LINE__);
  }
}


//////////////////////////////////////////////////////////////////////
// マッピングを行うコマンド
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Conv2BNetCmd::Conv2BNetCmd(NetMgr* mgr,
			   CellmapData* data) :
  CellmapCmd(mgr, data)
{
  mPoptDstNetwork = new TclPoptStr(this, "dst_network",
				   "specify destination network",
				   "<network-name>");
}

// @brief デストラクタ
Conv2BNetCmd::~Conv2BNetCmd()
{
}
  
// @brief コマンドを実行する仮想関数
int
Conv2BNetCmd::cmd_proc(TclObjVector& objv)
{
  NetHandle* neth = NULL;

  if ( mPoptDstNetwork->is_specified() ) {
    neth = find_or_new_nethandle(mPoptDstNetwork->val());
    if ( neth == NULL ) {
      // 見付からなかった
      // エラーメッセージは find_network() の中でセットされている．
      return TCL_ERROR;
    }
  }

  if ( objv.size() != 1 ) {
    print_usage();
    return TCL_ERROR;
  }

  try {
    if ( neth == NULL ) {
      neth = cur_nethandle();
    }
    BNetwork& dst_network = *neth->bnetwork();
    lut2bnet(lutnetwork(), dst_network);
    return TCL_OK;
  }
  catch ( AssertError x ) {
    cerr << x << endl;
    TclObj emsg;
    emsg << "Assertion Error";
    set_result(emsg);
    return TCL_ERROR;
  }
  
  return TCL_OK;
}

END_NAMESPACE_MAGUS_CELLMAP
