
/// @file libym_networks/MvnBdnConv.cc
/// @brief MvnBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnBdnConv.h"
#include "ym_networks/MvnBdnMap.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnDff.h"
#include "ym_networks/BdnLatch.h"
#include "ym_networks/BdnNodeHandle.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnModule.h"
#include "ym_networks/MvnPort.h"
#include "ym_networks/MvnNode.h"
#include "ym_networks/MvnInputPin.h"
#include "ym_networks/MvnOutputPin.h"

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


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

#if 0
void
dump_nodehandle(ostream& s,
		BdnNodeHandle h)
{
  if ( h.is_zero() ) {
    s <<"ZERO";
  }
  else if ( h.is_one() ) {
    s << "ONE";
  }
  else {
    if ( h.inv() ) {
      s << "~";
    }
    s << "Node#" << h.node()->id();
  }
}
#endif

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

BEGIN_NONAMESPACE

// 入出力ノードを作る．
void
make_io(BdnMgr& bdnetwork,
	MvnBdnMap& mvnode_map,
	const MvnNode* node,
	ymuint src_bitpos,
	BdnPort* bdnport,
	ymuint dst_bitpos)
{
  MvnNode::tType type = node->type();
  if ( type == MvnNode::kInput ) {
    BdnNode* input = bdnport->_input(dst_bitpos);
    mvnode_map.put(node, src_bitpos, BdnNodeHandle(input, false));
  }
  else if ( type == MvnNode::kOutput ) {
    BdnNode* output = bdnport->_output(dst_bitpos);
    mvnode_map.put(node, src_bitpos, BdnNodeHandle(output, false));
  }
  else if ( type == MvnNode::kInout ) {
    BdnNode* input = bdnport->_input(dst_bitpos);
    mvnode_map.put(node, src_bitpos, BdnNodeHandle(input, false));
    BdnNode* output = bdnport->_output(dst_bitpos);
    mvnode_map.put(node, src_bitpos, BdnNodeHandle(output, false));
  }
}

// 次に処理可能なノードをキューに積む．
// 具体的には node0 のファンアウト先のノードで
// - D-FF， ラッチ，定数，出力以外
// - すでにキューに積まれていない(mark[node->id()] が false)
// - 全てのファンインが処理済み
// の条件を満たすものをキューに積む．
void
enqueue(const MvnNode* node0,
	list<const MvnNode*>& queue,
	vector<bool>& mark)
{
  const MvnInputPinList& folist = node0->output(0)->dst_pin_list();
  for (MvnInputPinList::const_iterator p = folist.begin();
       p != folist.end(); ++ p) {
    const MvnInputPin* dst_pin = *p;
    const MvnNode* node = dst_pin->node();
    if ( node->type() == MvnNode::kDff ||
	 node->type() == MvnNode::kLatch ||
	 node->type() == MvnNode::kConst ||
	 node->type() == MvnNode::kOutput ||
	 node->type() == MvnNode::kInout ) {
      continue;
    }
    if ( mark[node->id()] ) {
      continue;
    }
    ymuint ni = node->input_num();
    bool marked = true;
    const MvnNode* unmark = NULL;
    for (ymuint i = 0; i < ni; ++ i) {
      const MvnInputPin* ipin = node->input(i);
      const MvnOutputPin* opin = ipin->src_pin();
      if ( opin == NULL ) {
	cerr << "node" << node->id() << "->input(" << i
	     << ") has no source" << endl;
	abort();
      }
      const MvnNode* inode = opin->node();
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


// @brief MvnMgr の内容を BdnMgr に変換する．
// @param[in] mvmgr 対象の MvnNetwork
// @param[out] bdnetwork 変換先の BdnMgr
// @param[out] mvnode_map 対応関係を格納するオブジェクト
void
MvnBdnConv::operator()(const MvnMgr& mvmgr,
		       BdnMgr& bdnetwork,
		       MvnBdnMap& mvnode_map)
{
  bdnetwork.clear();

  list<const MvnModule*> module_list;
  ymuint n = mvmgr.topmodule_list(module_list);
  if ( n != 1 ) {
    cerr << "# of topmodules is not 1" << endl;
    return;
  }

  ymuint nmax = mvmgr.max_node_id();

  const MvnModule* module = module_list.front();

  bdnetwork.set_name(module->name());

  // 入力側から処理するためのキューと
  // キューに積まれたことを記録するためのマーク
  vector<bool> mark(nmax, false);
  list<const MvnNode*> queue;

  // ポートを生成する．
  ymuint np = module->port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const MvnPort* port = module->port(i);
    ymuint nb = port->bit_width();
    vector<ymuint> iovect(nb);
    ymuint bitpos = 0;
    ymuint n = port->port_ref_num();
    for (ymuint j = 0; j < n; ++ j) {
      const MvnPortRef* port_ref = port->port_ref(j);
      ymuint nb1 = port_ref->bit_width();
      const MvnNode* node = port_ref->node();
      ymuint val = 0U;
      switch ( node->type() ) {
      case MvnNode::kInput: val = 1U; break;
      case MvnNode::kOutput: val = 2U; break;
      case MvnNode::kInout: val = 3U; break;
      default: assert_not_reached(__FILE__, __LINE__); break;
      }
      if ( port_ref->is_simple() ) {
	for (ymuint k = 0; k < nb1; ++ k) {
	  iovect[bitpos] = val;
	  ++ bitpos;
	}
      }
      else if ( port_ref->has_bitselect() ) {
	iovect[bitpos] = val;
	++ bitpos;
      }
      else if ( port_ref->has_partselect() ) {
	ymuint msb = port_ref->msb();
	ymuint lsb = port_ref->lsb();
	for (ymuint k = lsb; k <= msb; ++ k) {
	  iovect[bitpos] = val;
	  ++ bitpos;
	}
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    BdnPort* bdnport = bdnetwork.new_port(port->name(), iovect);
    vector<BdnNode*> tmp;
    tmp.reserve(nb);
    bitpos = 0;
    for (ymuint j = 0; j < n; ++ j) {
      const MvnPortRef* port_ref = port->port_ref(j);
      ymuint nb1 = port_ref->bit_width();
      const MvnNode* node = port_ref->node();
      if ( port_ref->is_simple() ) {
	for (ymuint k = 0; k < nb1; ++ k) {
	  make_io(bdnetwork, mvnode_map, node, k, bdnport, bitpos);
	  ++ bitpos;
	}
      }
      else if ( port_ref->has_bitselect() ) {
	make_io(bdnetwork, mvnode_map, node, port_ref->bitpos(),
		bdnport, bitpos);
	++ bitpos;
      }
      else if ( port_ref->has_partselect() ) {
	ymuint msb = port_ref->msb();
	ymuint lsb = port_ref->lsb();
	for (ymuint k = lsb; k <= msb; ++ k) {
	  make_io(bdnetwork, mvnode_map, node, k, bdnport, bitpos);
	  ++ bitpos;
	}
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
  }

  // 外部入力ノードをキューに積む．
  ymuint ni = module->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvnNode* node = module->input(i);
    mark[node->id()] = true;
    enqueue(node, queue, mark);
  }

  // 外部入出力をキューに積む．
  ymuint nio = module->inout_num();
  for (ymuint i = 0; i < nio; ++ i) {
    const MvnNode* node = module->inout(i);
    mark[node->id()] = true;
    enqueue(node, queue, mark);
  }

  // DFF と定数ノードを作る．
  const list<MvnNode*>& node_list = module->node_list();
  vector<vector<BdnDff*> > dff_map(nmax);
  vector<vector<BdnLatch*> > latch_map(nmax);
  for (list<MvnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const MvnNode* node = *p;
    if ( node->type() == MvnNode::kDff ) {
      // DFF
      ymuint bw = node->output(0)->bit_width();
      vector<BdnDff*>& dff_array = dff_map[node->id()];
      dff_array.resize(bw, NULL);
      for (ymuint j = 0; j < bw; ++ j) {
	BdnDff* dff = bdnetwork.new_dff();
	dff_array[j] = dff;
	BdnNode* bdnnode = dff->_output();
	mvnode_map.put(node, j, BdnNodeHandle(bdnnode, false));
      }
      mark[node->id()] = true;
      enqueue(node, queue, mark);
    }
    else if ( node->type() == MvnNode::kLatch ) {
      // LATCH
      ymuint bw = node->output(0)->bit_width();
      vector<BdnLatch*>& latch_array = latch_map[node->id()];
      latch_array.resize(bw, NULL);
      for (ymuint j = 0; j < bw; ++ j) {
	BdnLatch* latch = bdnetwork.new_latch();
	latch_array[j] = latch;
	BdnNode* bdnnode = latch->_output();
	mvnode_map.put(node, j, BdnNodeHandle(bdnnode, false));
      }
      mark[node->id()] = true;
      enqueue(node, queue, mark);
    }
    else if ( node->type() == MvnNode::kConst ) {
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
    const MvnNode* node = queue.front();
    queue.pop_front();

    // node に対応する BdnNode を作る．
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

  // DFFノードとラッチノードの入力を接続する．
  for (ymuint i = 0; i < nmax; ++ i) {
    const MvnNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvnNode::kDff ) {
      // データ入力
      const MvnInputPin* data_ipin = node->input(0);
      const MvnOutputPin* data_opin = data_ipin->src_pin();
      assert_cond( data_opin != NULL, __FILE__, __LINE__);
      const MvnNode* data_src_node = data_opin->node();
      ymuint bw = data_ipin->bit_width();

      // クロック
      const MvnInputPin* clock_ipin = node->input(1);
      const MvnOutputPin* clock_opin = clock_ipin->src_pin();
      assert_cond( clock_opin != NULL, __FILE__, __LINE__);
      assert_cond( clock_opin->bit_width() == 1, __FILE__, __LINE__);
      const MvnNode* clock_src_node = clock_opin->node();
      BdnNodeHandle clock_ihandle = mvnode_map.get(clock_src_node);
      if ( node->clock_pol() == 0 ) {
	clock_ihandle = ~clock_ihandle;
      }

      // 非同期セット/非同期リセット
      vector<BdnNodeHandle> preset_array(bw, BdnNodeHandle::make_zero());
      vector<BdnNodeHandle> clear_array(bw, BdnNodeHandle::make_zero());
      ymuint nc = node->input_num() - 2;
      for (ymuint k = 0; k < nc; ++ k) {
	const MvnInputPin* ctrl_ipin = node->input(k + 2);
	const MvnOutputPin* ctrl_opin = ctrl_ipin->src_pin();
	const MvnNode* ctrl_src_node = ctrl_opin->node();
	BdnNodeHandle ctrl_dst_ihandle = mvnode_map.get(ctrl_src_node);
	if ( node->control_pol(k) == 0 ) {
	  ctrl_dst_ihandle = ~ctrl_dst_ihandle;
	}
	const MvnNode* val_src_node = node->control_val(k);
	BdnNodeHandle cond = ctrl_dst_ihandle;
	for (ymuint j = 0; j < bw; ++ j) {
	  BdnNodeHandle val_dst_ihandle = mvnode_map.get(val_src_node, j);
	  BdnNodeHandle new_preset = bdnetwork.new_and(cond, val_dst_ihandle);
	  BdnNodeHandle new_clear = bdnetwork.new_and(cond, ~val_dst_ihandle);
	  preset_array[j] = bdnetwork.new_or(preset_array[j], new_preset);
	  clear_array[j] = bdnetwork.new_or(clear_array[j], new_clear);
	}
      }

      const vector<BdnDff*>& dff_array = dff_map[node->id()];
      for (ymuint j = 0; j < bw; ++ j) {
	BdnDff* dff = dff_array[j];
	BdnNodeHandle data_ihandle = mvnode_map.get(data_src_node, j);
	bdnetwork.change_output_fanin(dff->_input(), data_ihandle);
	bdnetwork.change_output_fanin(dff->_clock(), clock_ihandle);
	BdnNodeHandle preset_ihandle = preset_array[j];
	if ( !preset_ihandle.is_zero() ) {
	  bdnetwork.change_output_fanin(dff->_preset(), preset_ihandle);
	}
	BdnNodeHandle clear_ihandle = clear_array[j];
	if ( !clear_ihandle.is_zero() ) {
	  bdnetwork.change_output_fanin(dff->_clear(), clear_ihandle);
	}
      }
    }
    else if ( node->type() == MvnNode::kLatch ) {
      // データ入力
      const MvnInputPin* data_ipin = node->input(0);
      const MvnOutputPin* data_opin = data_ipin->src_pin();
      assert_cond( data_opin != NULL, __FILE__, __LINE__);
      const MvnNode* data_src_node = data_opin->node();
      ymuint bw = data_ipin->bit_width();

      // イネーブル
      const MvnInputPin* enable_ipin = node->input(1);
      const MvnOutputPin* enable_opin = enable_ipin->src_pin();
      assert_cond( enable_opin != NULL, __FILE__, __LINE__);
      assert_cond( enable_opin->bit_width() == 1, __FILE__, __LINE__);
      const MvnNode* enable_src_node = enable_opin->node();
      BdnNodeHandle enable_ihandle = mvnode_map.get(enable_src_node);

      const vector<BdnLatch*>& latch_array = latch_map[node->id()];
      for (ymuint j = 0; j < bw; ++ j) {
	BdnLatch* latch = latch_array[j];
	BdnNodeHandle data_ihandle = mvnode_map.get(data_src_node, j);
	bdnetwork.change_output_fanin(latch->_input(), data_ihandle);
	bdnetwork.change_output_fanin(latch->_enable(), enable_ihandle);
      }
    }
  }

  // 外部出力ノードのファンインを接続する．
  ymuint no = module->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvnNode* node = module->output(i);
    const MvnInputPin* ipin = node->input(0);
    const MvnOutputPin* opin = ipin->src_pin();
    if ( opin == NULL ) continue;

    const MvnNode* src_node = opin->node();

    ymuint bw = ipin->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      BdnNodeHandle handle = mvnode_map.get(node, j);
      assert_cond( handle.inv() == false, __FILE__, __LINE__);
      BdnNode* onode = handle.node();
      assert_cond( onode != NULL, __FILE__, __LINE__);
      BdnNodeHandle ihandle = mvnode_map.get(src_node, j);
      bdnetwork.change_output_fanin(onode, ihandle);
    }
  }

  // 外部入出力ノードのファンインを接続する．
  for (ymuint i = 0; i < nio; ++ i) {
    const MvnNode* node = module->inout(i);
    const MvnInputPin* ipin = node->input(0);
    const MvnOutputPin* opin = ipin->src_pin();
    if ( opin == NULL ) continue;

    const MvnNode* src_node = opin->node();

    ymuint bw = ipin->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      BdnNodeHandle handle = mvnode_map.get(node, j);
      assert_cond( handle.inv() == false, __FILE__, __LINE__);
      BdnNode* input = handle.node();
      assert_cond( input != NULL, __FILE__, __LINE__);
      BdnNodeHandle ihandle = mvnode_map.get(src_node, j);
      bdnetwork.change_output_fanin(input, ihandle);
    }
  }
}

END_NAMESPACE_YM_NETWORKSBDNCONV
