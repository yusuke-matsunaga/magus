
/// @file lutmap/src/mvn2sbj.cc
/// @brief MvNetwork を SbjGraph に変換する関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/SbjGraph.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvNode.h"
#include "ym_mvn/MvPin.h"
#include "ym_mvn/MvNet.h"


BEGIN_NAMESPACE_YM_LUTMAP

using namespace nsYm::nsMvn;

class MvNodeMap
{
public:

  /// @brief コンストラクタ
  MvNodeMap(ymuint n);

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvNode
  /// @param[in] sbjnode SbjNode
  /// @param[in] inv 反転属性
  void
  put(const MvNode* mvnode,
      SbjNode* sbjnode,
      bool inv);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvNode
  /// @param[in] index ビット位置
  /// @param[in] sbjnode SbjNode
  /// @param[in] inv 反転属性
  void
  put(const MvNode* mvnode,
      ymuint index,
      SbjNode* sbjnode,
      bool inv);

  /// @brief 探す．(1ビット版)
  bool
  get(const MvNode* mvnode,
      SbjNode*& sbjnode,
      bool& inv) const;

  /// @brief 探す．(ベクタ版)
  bool
  get(const MvNode* mvnode,
      ymuint index,
      SbjNode*& sbjnode,
      bool& inv) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<ympuint> > mArray;

};

// @brief コンストラクタ
MvNodeMap::MvNodeMap(ymuint n) :
  mArray(n)
{
}

// @brief 登録する．(1ビット版)
// @param[in] mvnode MvNode
// @param[in] sbjnode SbjNode
// @param[in] inv 反転属性
void
MvNodeMap::put(const MvNode* mvnode,
	       SbjNode* sbjnode,
	       bool inv)
{
  put(mvnode, 0, sbjnode, inv);
}

// @brief 登録する．(ベクタ版)
// @param[in] mvnode MvNode
// @param[in] index ビット位置
// @param[in] sbjnode SbjNode
// @param[in] inv 反転属性
void
MvNodeMap::put(const MvNode* mvnode,
	       ymuint index,
	       SbjNode* sbjnode,
	       bool inv)
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  vector<ympuint>& array = mArray[mvnode->id()];
  if ( array.size() != mvnode->output(0)->bit_width() ) {
    array.resize(mvnode->output(0)->bit_width());
  }
  array[index] = reinterpret_cast<ympuint>(sbjnode) | (inv & 1U);
}

// @brief 探す．(1ビット版)
bool
MvNodeMap::get(const MvNode* mvnode,
	       SbjNode*& sbjnode,
	       bool& inv) const
{
  return get(mvnode, 0, sbjnode, inv);
}

// @brief 探す．(ベクタ版)
bool
MvNodeMap::get(const MvNode* mvnode,
	       ymuint index,
	       SbjNode*& sbjnode,
	       bool& inv) const
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  const vector<ympuint>& array = mArray[mvnode->id()];
  if ( array.empty() ) {
    return false;
  }
  assert_cond( array.size() == mvnode->output(0)->bit_width(), __FILE__, __LINE__);
  ympuint tmp = array[index];
  sbjnode = reinterpret_cast<SbjNode*>(tmp & ~1UL);
  inv = static_cast<bool>(tmp & 1U);
  return sbjnode != NULL;
}


void
mvn2sbj(const MvMgr& mvmgr,
	SbjGraph& sbjgraph)
{
  sbjgraph.clear();

  list<const MvModule*> module_list;
  ymuint n = mvmgr.topmodule_list(module_list);
  if ( n != 1 ) {
    cout << "# of topmodules is not 1" << endl;
    return;
  }

  ymuint nmax = mvmgr.max_node_id();
  MvNodeMap mvmap(nmax);
  
  const MvModule* module = module_list.front();

  vector<bool> mark(nmax, false);
  list<const MvNode*> queue;
  
  // 外部入力を作る．
  ymuint ni = module->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvNode* node = module->input(i);
    ymuint bw = node->output(0)->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      SbjNode* sbjnode = sbjgraph.new_input(string());
      mvmap.put(node, j, sbjnode, false);
    }
    mark[node->id()] = true;
    queue.push_back(node);
  }

  // DFF を作る．
  for (ymuint i = 0; i < nmax; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    if ( node->type() != MvNode::kDff1 && node->type() != MvNode::kDff2 ) {
      continue;
    }
    ymuint bw = node->output(0)->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      SbjNode* sbjnode = sbjgraph.new_dff(string());
      mvmap.put(node, j, sbjnode, false);
    }
    mark[node->id()] = true;
    queue.push_back(node);
  }

  // 論理ノードを作る．
  while ( !queue.empty() ) {
    const MvNode* node0 = queue.front();
    queue.pop_front();
    const MvNetList& folist = node0->output(0)->net_list();
    for (MvNetList::const_iterator p = folist.begin();
	 p != folist.end(); ++ p) {
      const MvNet* net = *p;
      const MvNode* node = net->dst_pin()->node();
      if ( node->type() == MvNode::kDff1 || node->type() == MvNode::kDff2 ) {
	continue;
      }
      ymuint ni = node->input_num();
      bool marked = true;
      for (ymuint i = 0; i < ni; ++ i) {
	const MvInputPin* ipin = node->input(i);
	const MvNet* net = ipin->net();
	if ( net == NULL ) {
	  cout << "node" << node->id() << "->input(" << i
	       << ") has no net" << endl;
	}
	assert_cond( net != NULL, __FILE__, __LINE__);
	const MvOutputPin* opin = net->src_pin();
	const MvNode* inode = opin->node();
	if ( !mark[inode->id()] ) {
	  marked = false;
	  break;
	}
      }
      if ( !marked ) continue;

      // node に対応する SbjNode を作る．
      switch ( node->type() ) {
      case MvNode::kOutput:
	break;
	
      case MvNode::kThrough:
	{
	  const MvInputPin* ipin = node->input(0);
	  const MvNet* net = ipin->net();
	  const MvOutputPin* opin = net->src_pin();
	  const MvNode* src_node = opin->node();
	  ymuint bw = opin->bit_width();
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode;
	    bool inv;
	    bool stat = mvmap.get(src_node, i, sbjnode, inv);
	    assert_cond( stat , __FILE__, __LINE__);
	    mvmap.put(node, i, sbjnode, inv);
	  }
	}
	break;
	
      case MvNode::kNot:
	{
	  const MvInputPin* ipin = node->input(0);
	  const MvNet* net = ipin->net();
	  const MvOutputPin* opin = net->src_pin();
	  const MvNode* src_node = opin->node();
	  ymuint bw = opin->bit_width();
	  assert_cond( node->output(0)->bit_width(), __FILE__, __LINE__);
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode;
	    bool inv;
	    bool stat = mvmap.get(src_node, i, sbjnode, inv);
	    assert_cond( stat , __FILE__, __LINE__);
	    mvmap.put(node, i, sbjnode, !inv);
	  }
	}
	break;

      case MvNode::kAnd:
	{
	  const MvInputPin* ipin0 = node->input(0);
	  const MvNet* net0 = ipin0->net();
	  const MvOutputPin* src_pin0 = net0->src_pin();
	  const MvNode* src_node0 = src_pin0->node();

	  const MvInputPin* ipin1 = node->input(1);
	  const MvNet* net1 = ipin1->net();
	  const MvOutputPin* src_pin1 = net1->src_pin();
	  const MvNode* src_node1 = src_pin1->node();

	  ymuint bw = node->output(0)->bit_width();
	  assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
	  assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode0;
	    bool inv0;
	    bool stat0 = mvmap.get(src_node0, i, sbjnode0, inv0);
	    SbjNode* sbjnode1;
	    bool inv1;
	    bool stat1 = mvmap.get(src_node1, i, sbjnode1, inv1);
	    assert_cond( stat0 && stat1 , __FILE__, __LINE__);
	    ymuint fcode = 0U;
	    if ( inv0 ) {
	      if ( inv1 ) {
		fcode = 0x1; // 0001
	      }
	      else {
		fcode = 0x4; // 0100
	      }
	    }
	    else {
	      if ( inv1 ) {
		fcode = 0x2; // 0010
	      }
	      else {
		fcode = 0x8; // 1000
	      }
	    }
	    SbjNode* sbjnode = sbjgraph.new_logic(string(),
						  fcode, sbjnode0, sbjnode1);
	    mvmap.put(node, i, sbjnode, false);
	  }
	}
	break;

      case MvNode::kOr:
	{
	  const MvInputPin* ipin0 = node->input(0);
	  const MvNet* net0 = ipin0->net();
	  const MvOutputPin* src_pin0 = net0->src_pin();
	  const MvNode* src_node0 = src_pin0->node();

	  const MvInputPin* ipin1 = node->input(1);
	  const MvNet* net1 = ipin1->net();
	  const MvOutputPin* src_pin1 = net1->src_pin();
	  const MvNode* src_node1 = src_pin1->node();

	  ymuint bw = node->output(0)->bit_width();
	  assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
	  assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode0;
	    bool inv0;
	    bool stat0 = mvmap.get(src_node0, i, sbjnode0, inv0);
	    SbjNode* sbjnode1;
	    bool inv1;
	    bool stat1 = mvmap.get(src_node1, i, sbjnode1, inv1);
	    assert_cond( stat0 && stat1 , __FILE__, __LINE__);
	    ymuint fcode = 0U;
	    if ( inv0 ) {
	      if ( inv1 ) {
		fcode = 0x7; // 0111
	      }
	      else {
		fcode = 0xd; // 1101
	      }
	    }
	    else {
	      if ( inv1 ) {
		fcode = 0xb; // 1011
	      }
	      else {
		fcode = 0xe; // 1110
	      }
	    }
	    SbjNode* sbjnode = sbjgraph.new_logic(string(),
						  fcode, sbjnode0, sbjnode1);
	    mvmap.put(node, i, sbjnode, false);
	  }
	}
	break;

      case MvNode::kXor:
	{
	  const MvInputPin* ipin0 = node->input(0);
	  const MvNet* net0 = ipin0->net();
	  const MvOutputPin* src_pin0 = net0->src_pin();
	  const MvNode* src_node0 = src_pin0->node();

	  const MvInputPin* ipin1 = node->input(1);
	  const MvNet* net1 = ipin1->net();
	  const MvOutputPin* src_pin1 = net1->src_pin();
	  const MvNode* src_node1 = src_pin1->node();

	  ymuint bw = node->output(0)->bit_width();
	  assert_cond( src_pin0->bit_width() == bw, __FILE__, __LINE__);
	  assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode0;
	    bool inv0;
	    bool stat0 = mvmap.get(src_node0, i, sbjnode0, inv0);
	    SbjNode* sbjnode1;
	    bool inv1;
	    bool stat1 = mvmap.get(src_node1, i, sbjnode1, inv1);
	    assert_cond( stat0 && stat1 , __FILE__, __LINE__);
	    ymuint fcode = 0U;
	    if ( inv0 ^ inv1 ) {
	      fcode = 0x9; // 1001
	    }
	    else {
	      fcode = 0x6; // 0110
	    }
	    SbjNode* sbjnode = sbjgraph.new_logic(string(),
						  fcode, sbjnode0, sbjnode1);
	    mvmap.put(node, i, sbjnode, false);
	  }
	}
	break;

      case MvNode::kRand:
      case MvNode::kRor:
      case MvNode::kRxor:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      case MvNode::kEq:
      case MvNode::kLt:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      case MvNode::kAdd:
      case MvNode::kSub:
      case MvNode::kMult:
      case MvNode::kDiv:
      case MvNode::kMod:
      case MvNode::kPow:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      case MvNode::kIte:
	{
	  const MvInputPin* ipin0 = node->input(0);
	  const MvNet* net0 = ipin0->net();
	  const MvOutputPin* src_pin0 = net0->src_pin();
	  const MvNode* src_node0 = src_pin0->node();
	  
	  const MvInputPin* ipin1 = node->input(1);
	  const MvNet* net1 = ipin1->net();
	  const MvOutputPin* src_pin1 = net1->src_pin();
	  const MvNode* src_node1 = src_pin1->node();
	  
	  const MvInputPin* ipin2 = node->input(2);
	  const MvNet* net2 = ipin2->net();
	  const MvOutputPin* src_pin2 = net2->src_pin();
	  const MvNode* src_node2 = src_pin2->node();

	  ymuint bw = node->output(0)->bit_width();
	  assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
	  assert_cond( src_pin2->bit_width() == bw, __FILE__, __LINE__);
	  SbjNode* sbjnode0;
	  bool inv0;
	  bool stat0 = mvmap.get(src_node0, sbjnode0, inv0);
	  assert_cond( stat0, __FILE__, __LINE__);
	  
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode1;
	    bool inv1;
	    bool stat1 = mvmap.get(src_node1, i, sbjnode1, inv1);
	    SbjNode* sbjnode2;
	    bool inv2;
	    bool stat2 = mvmap.get(src_node2, i, sbjnode2, inv2);
	    assert_cond( stat1 && stat2 , __FILE__, __LINE__);
	    ymuint fcode1 = 0U;
	    if ( inv0 ) {
	      if ( inv1 ) {
		fcode1 = 0x1;
	      }
	      else {
		fcode1 = 0x4;
	      }
	    }
	    else {
	      if ( inv1 ) {
		fcode1 = 0x2;
	      }
	      else {
		fcode1 = 0x8;
	      }
	    }
	    SbjNode* and1 = sbjgraph.new_logic(string(),
					       fcode1, sbjnode0, sbjnode1);
	    ymuint fcode2 = 0U;
	    if ( inv0 ) {
	      if ( inv2 ) {
		fcode2 = 0x2;
	      }
	      else {
		fcode2 = 0x8;
	      }
	    }
	    else {
	      if ( inv2 ) {
		fcode2 = 0x1;
	      }
	      else {
		fcode2 = 0x4;
	      }
	    }
	    SbjNode* and2 = sbjgraph.new_logic(string(),
					       fcode2, sbjnode0, sbjnode2);
	    SbjNode* or1 = sbjgraph.new_logic(string(),
					      0xe, and1, and2);
	    mvmap.put(node, i, or1, false);
	  }
	}
	break;

      case MvNode::kConcat:
	{
	  ymuint bw = node->output(0)->bit_width();
	  ymuint ni = node->input_num();
	  ymuint offset = 0U;
	  for (ymuint i = 0; i < ni; ++ i) {
	    const MvInputPin* ipin = node->input(i);
	    const MvNet* net = ipin->net();
	    const MvOutputPin* opin = net->src_pin();
	    const MvNode* src_node = opin->node();
	    ymuint bw1 = opin->bit_width();
	    for (ymuint j = 0; j < bw1; ++ j) {
	      ymuint index = offset + j;

	      SbjNode* sbjnode0;
	      bool inv0;
	      bool stat0 = mvmap.get(src_node, j, sbjnode0, inv0);
	      assert_cond( stat0, __FILE__, __LINE__);
	      mvmap.put(node, index, sbjnode0, inv0);
	    }
	    offset += bw1;
	  }
	  assert_cond( bw == offset, __FILE__, __LINE__);
	}
	break;

      case MvNode::kConstBitSelect:
	{
	  const MvInputPin* ipin = node->input(0);
	  const MvNet* net = ipin->net();
	  const MvOutputPin* src_pin = net->src_pin();
	  const MvNode* src_node = src_pin->node();

	  SbjNode* sbjnode0;
	  bool inv0;
	  bool stat0 = mvmap.get(src_node, node->bitpos(), sbjnode0, inv0);
	  assert_cond( stat0 , __FILE__, __LINE__);
	  mvmap.put(node, sbjnode0, inv0);
	}
	break;

      case MvNode::kConstPartSelect:
	{
	  const MvInputPin* ipin = node->input(0);
	  const MvNet* net = ipin->net();
	  const MvOutputPin* src_pin = net->src_pin();
	  const MvNode* src_node = src_pin->node();

	  ymuint bw = node->output(0)->bit_width();
	  ymuint msb = node->msb();
	  ymuint lsb = node->lsb();
	  assert_cond( bw == msb - lsb + 1,
		       __FILE__, __LINE__);
	  for (ymuint i = 0; i < bw; ++ i) {
	    SbjNode* sbjnode0;
	    bool inv0;
	    bool stat0 = mvmap.get(src_node, i + lsb, sbjnode0, inv0);
	    assert_cond( stat0 , __FILE__, __LINE__);
	    mvmap.put(node, i, sbjnode0, inv0);
	  }
	}
	break;

      case MvNode::kBitSelect:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      case MvNode::kPartSelect:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      case MvNode::kCombUdp:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      case MvNode::kSeqUdp:
	{
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
      // node に処理済みの印をつける．
      mark[node->id()] = true;
      queue.push_back(node);
    }
  }

  // DFFノードの入力を接続する．
  for (ymuint i = 0; i < nmax; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvNode::kDff1 || node->type() == MvNode::kDff2 ) {
      const MvInputPin* ipin = node->input(0);
      const MvNet* net = ipin->net();
      assert_cond( net != NULL, __FILE__, __LINE__);
      const MvOutputPin* opin = net->src_pin();
      const MvNode* src_node = opin->node();
      ymuint bw = ipin->bit_width();
      for (ymuint j = 0; j < bw; ++ j) {
	SbjNode* sbjnode;
	bool inv;
	bool stat = mvmap.get(node, j, sbjnode, inv);
	SbjNode* isbjnode;
	bool iinv;
	bool stat1 = mvmap.get(src_node, j, isbjnode, iinv);
	assert_cond( stat && stat1 , __FILE__, __LINE__);
	assert_cond( inv == false, __FILE__, __LINE__);
	sbjgraph.change_dff(sbjnode, isbjnode, iinv);
      }
    }
  }

  // 外部出力ノードを作る．
  ymuint no = module->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvNode* node = module->output(i);
    const MvInputPin* ipin = node->input(0);
    const MvNet* net = ipin->net();
    assert_cond( net != NULL, __FILE__, __LINE__);
    const MvOutputPin* opin = net->src_pin();
    const MvNode* src_node = opin->node();

    ymuint bw = ipin->bit_width();
    for (ymuint j = 0; j < bw; ++ j) {
      SbjNode* sbjnode;
      bool inv;
      bool stat = mvmap.get(src_node, j, sbjnode, inv);
      assert_cond( stat, __FILE__, __LINE__);
      SbjNode* osbjnode = sbjgraph.new_output(string(), sbjnode, inv);
      mvmap.put(node, j, osbjnode, false);
    }
  }
}

END_NAMESPACE_YM_LUTMAP
