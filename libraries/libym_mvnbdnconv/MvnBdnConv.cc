
/// @file libym_mvnbdnconv/MvnBdnConv.cc
/// @brief MvnBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvnbdnconv/MvnBdnConv.h"
#include "ym_mvnbdnconv/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"

#include "ThroughConv.h"
#include "NotConv.h"
#include "AndConv.h"
#include "OrConv.h"
#include "XorConv.h"
#include "RandConv.h"
#include "RorConv.h"
#include "RxorConv.h"
#include "CmplConv.h"
#include "AddConv.h"
#include "SubConv.h"
#include "MultConv.h"
#include "DivConv.h"
#include "ModConv.h"
#include "PowConv.h"
#include "SllConv.h"
#include "SrlConv.h"
#include "SlaConv.h"
#include "SraConv.h"
#include "EqConv.h"
#include "LtConv.h"
#include "IteConv.h"
#include "ConcatConv.h"
#include "ConstBitSelectConv.h"
#include "ConstPartSelectConv.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

BEGIN_NONAMESPACE

void
enqueue(const MvNode* node0,
	list<const MvNode*>& queue,
	vector<bool>& mark)
{
  const MvInputPinList& folist = node0->output(0)->dst_pin_list();
  for (MvInputPinList::const_iterator p = folist.begin();
       p != folist.end(); ++ p) {
    const MvInputPin* dst_pin = *p;
    const MvNode* node = dst_pin->node();
    if ( node->type() == MvNode::kDff ||
	 node->type() == MvNode::kLatch ||
	 node->type() == MvNode::kConst ||
	 node->type() == MvNode::kOutput ) {
      continue;
    }
    if ( mark[node->id()] ) {
      continue;
    }
    ymuint ni = node->input_num();
    bool marked = true;
    const MvNode* unmark = NULL;
    for (ymuint i = 0; i < ni; ++ i) {
      const MvInputPin* ipin = node->input(i);
      const MvOutputPin* opin = ipin->src_pin();
      if ( opin == NULL ) {
	cerr << "node" << node->id() << "->input(" << i
	     << ") has no source" << endl;
	abort();
      }
      const MvNode* inode = opin->node();
      if ( !mark[inode->id()] ) {
	marked = false;
	unmark = inode;
	break;
      }
    }
    if ( marked ) {
      mark[node->id()] = true;
      queue.push_back(node);
    }
  }
}

END_NONAMESPACE


// @brief コンストラクタ
MvnBdnConv::MvnBdnConv()
{
  mConvList.push_back(new ThroughConv);
  mConvList.push_back(new NotConv);
  mConvList.push_back(new AndConv);
  mConvList.push_back(new OrConv);
  mConvList.push_back(new XorConv);
  mConvList.push_back(new RandConv);
  mConvList.push_back(new RorConv);
  mConvList.push_back(new RxorConv);
  mConvList.push_back(new CmplConv);
  mConvList.push_back(new AddConv);
  mConvList.push_back(new SubConv);
  mConvList.push_back(new MultConv);
  mConvList.push_back(new DivConv);
  mConvList.push_back(new ModConv);
  mConvList.push_back(new PowConv);
  mConvList.push_back(new SllConv);
  mConvList.push_back(new SrlConv);
  mConvList.push_back(new SlaConv);
  mConvList.push_back(new SraConv);
  mConvList.push_back(new EqConv);
  mConvList.push_back(new LtConv);
  mConvList.push_back(new IteConv);
  mConvList.push_back(new ConcatConv);
  mConvList.push_back(new ConstBitSelectConv);
  mConvList.push_back(new ConstPartSelectConv);
}

// @brief デストラクタ
MvnBdnConv::~MvnBdnConv()
{
  for (list<MvnConv*>::iterator p = mConvList.begin();
       p != mConvList.end(); ++ p) {
    delete *p;
  }
}

// @brief MvMgr の内容を BdNetwork に変換する．
// @param[in] mvmgr 対象の MvNetwork
// @param[out] bdnetwork 変換先の BdNetwork
// @param[out] mvnode_map 対応関係を格納するオブジェクト
void
MvnBdnConv::operator()(const MvMgr& mvmgr,
		       BdNetwork& bdnetwork,
		       MvnBdnMap& mvnode_map)
{
  bdnetwork.clear();

  list<const MvModule*> module_list;
  ymuint n = mvmgr.topmodule_list(module_list);
  if ( n != 1 ) {
    cerr << "# of topmodules is not 1" << endl;
    return;
  }

  ymuint nmax = mvmgr.max_node_id();

  const MvModule* module = module_list.front();

  bdnetwork.set_name(module->name());

  vector<bool> mark(nmax, false);
  list<const MvNode*> queue;

  // 外部入力を作る．
  ymuint ni = module->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvNode* node = module->input(i);
    ymuint bw = node->output(0)->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      BdnNode* bdnnode = bdnetwork.new_input(string());
      mvnode_map.put(node, j, BdnNodeHandle(bdnnode, false));
    }
    mark[node->id()] = true;
    enqueue(node, queue, mark);
  }

  // 外部入出力を作る．
  ymuint nio = module->inout_num();
  for (ymuint i = 0; i < nio; ++ i) {
    const MvNode* node = module->inout(i);
    ymuint bw = node->output(0)->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      BdnNode* bdnnode = bdnetwork.new_input(string());
      mvnode_map.put(node, j, BdnNodeHandle(bdnnode, false));
    }
    mark[node->id()] = true;
    enqueue(node, queue, mark);
  }

  // DFF と定数ノードを作る．
  const list<MvNode*>& node_list = module->node_list();
  for (list<MvNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const MvNode* node = *p;
    if ( node->type() == MvNode::kDff ) {
      // DFF
      ymuint bw = node->output(0)->bit_width();
      for (ymuint j = 0; j < bw; ++ j) {
	BdnNode* bdnnode = bdnetwork.new_dff();
	mvnode_map.put(node, j, BdnNodeHandle(bdnnode, false));
      }
      mark[node->id()] = true;
      enqueue(node, queue, mark);
    }
    else if ( node->type() == MvNode::kLatch ) {
      // LATCH
    }
    else if ( node->type() == MvNode::kConst ) {
      // 定数
      ymuint bw = node->output(0)->bit_width();
      vector<ymuint32> value;
      node->const_value(value);
      ymuint bpos = 0;
      ymuint32 pat = value[bpos];
      for (ymuint j = 0; j < bw; ++ j) {
	if ( pat & 1U ) {
	  // 1
	  mvnode_map.put(node, j, BdnNodeHandle::make_one());
	}
	else {
	  // 0
	  mvnode_map.put(node, j, BdnNodeHandle::make_zero());
	}
	if ( j % 32 == 31 ) {
	  ++ bpos;
	  pat = value[bpos];
	}
	else {
	  pat >>= 1;
	}
      }
      mark[node->id()] = true;
      enqueue(node, queue, mark);
    }
  }

  // 論理ノードを作る．
  while ( !queue.empty() ) {
    const MvNode* node = queue.front();
    queue.pop_front();

    // node に対応する SbjNode を作る．
    bool done = false;
    for (list<MvnConv*>::iterator p = mConvList.begin();
	 p != mConvList.end(); ++ p) {
      MvnConv& conv = **p;
      if ( conv(node, bdnetwork, mvnode_map) ) {
	done = true;
	break;
      }
    }
    if ( !done ) {
      assert_not_reached(__FILE__, __LINE__);
    }
    enqueue(node, queue, mark);
  }

  // DFFノードの入力を接続する．
  for (ymuint i = 0; i < nmax; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvNode::kDff ) {
      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* opin = ipin->src_pin();
      assert_cond( opin != NULL, __FILE__, __LINE__);
      const MvNode* src_node = opin->node();
      ymuint bw = ipin->bit_width();
      for (ymuint j = 0; j < bw; ++ j) {
	BdnNodeHandle handle = mvnode_map.get(node, j);
	BdnNodeHandle ihandle = mvnode_map.get(src_node, j);
	assert_cond( handle.inv() == false, __FILE__, __LINE__);
	BdnNode* node = handle.node();
	bdnetwork.set_dff_data(node, ihandle);
      }

      const MvInputPin* ipin1 = node->input(1);
      const MvOutputPin* opin1 = ipin1->src_pin();
      assert_cond( opin1 != NULL, __FILE__, __LINE__);
      assert_cond( opin1->bit_width() == 1, __FILE__, __LINE__);
      const MvNode* src_node1 = opin1->node();
      BdnNodeHandle ihandle1 = mvnode_map.get(src_node1);
      for (ymuint j = 0; j < bw; ++ j) {
	BdnNodeHandle handle = mvnode_map.get(node, j);
	assert_cond( handle.inv() == false, __FILE__, __LINE__);
	BdnNode* node = handle.node();
	bdnetwork.set_dff_clock(node, ihandle1);
      }

      const MvInputPin* ipin2 = node->input(2);
      const MvOutputPin* opin2 = ipin2->src_pin();
      if ( opin2 ) {
	assert_cond( opin2->bit_width() == 1, __FILE__, __LINE__);
	const MvNode* src_node2 = opin2->node();
	BdnNodeHandle ihandle2 = mvnode_map.get(src_node2);
	for (ymuint j = 0; j < bw; ++ j) {
	  BdnNodeHandle handle = mvnode_map.get(node, j);
	  assert_cond( handle.inv() == false, __FILE__, __LINE__);
	  BdnNode* node = handle.node();
	  bdnetwork.set_dff_rst(node, ihandle2);
	}
      }

      const MvInputPin* ipin3 = node->input(3);
      const MvOutputPin* opin3 = ipin3->src_pin();
      if ( opin3 ) {
	assert_cond( opin3->bit_width() == 1, __FILE__, __LINE__);
	const MvNode* src_node3 = opin3->node();
	BdnNodeHandle ihandle3 = mvnode_map.get(src_node3);
	for (ymuint j = 0; j < bw; ++ j) {
	  BdnNodeHandle handle = mvnode_map.get(node, j);
	  assert_cond( handle.inv() == false, __FILE__, __LINE__);
	  BdnNode* node = handle.node();;
	  bdnetwork.set_dff_set(node, ihandle3);
	}
      }
    }
  }

  // 外部出力ノードを作る．
  ymuint no = module->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvNode* node = module->output(i);
    const MvInputPin* ipin = node->input(0);
    const MvOutputPin* opin = ipin->src_pin();
    assert_cond( opin != NULL, __FILE__, __LINE__);
    const MvNode* src_node = opin->node();

    ymuint bw = ipin->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      BdnNodeHandle handle = mvnode_map.get(src_node, j);
      BdnNode* onode = bdnetwork.new_output(string(), handle);
      mvnode_map.put(node, j, BdnNodeHandle(onode, false));
    }
  }

  // ポートを生成する．
  ymuint np = module->port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const MvPort* port = module->port(i);
    ymuint nb = port->bit_width();
    vector<BdnNode*> tmp;
    tmp.reserve(nb);
    ymuint n = port->port_ref_num();
    for (ymuint j = 0; j < n; ++ j) {
      const MvPortRef* port_ref = port->port_ref(j);
      ymuint nb1 = port_ref->bit_width();
      const MvNode* node = port_ref->node();
      if ( port_ref->is_simple() ) {
	for (ymuint k = 0; k < nb1; ++ k) {
	  BdnNodeHandle handle = mvnode_map.get(node, k);
	  assert_cond( handle.inv() == false, __FILE__, __LINE__);
	  BdnNode* bdnnode = handle.node();
	  tmp.push_back(bdnnode);
	}
      }
      else if ( port_ref->has_bitselect() ) {
	BdnNodeHandle handle = mvnode_map.get(node, port_ref->bitpos());
	assert_cond( handle.inv() == false, __FILE__, __LINE__);
	BdnNode* bdnnode = handle.node();
	tmp.push_back(bdnnode);
      }
      else if ( port_ref->has_partselect() ) {
	ymuint msb = port_ref->msb();
	ymuint lsb = port_ref->lsb();
	for (ymuint k = lsb; k <= msb; ++ k) {
	  BdnNodeHandle handle = mvnode_map.get(node, k);
	  assert_cond( handle.inv() == false, __FILE__, __LINE__);
	  BdnNode* bdnnode = handle.node();
	  tmp.push_back(bdnnode);
	}
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
#if 0
    bdnetwork.add_port(port->name(), tmp);
#endif
  }
}

END_NAMESPACE_YM_MVNBDNCONV
