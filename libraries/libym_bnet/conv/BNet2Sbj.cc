
/// @file libym_bnet/conv/BNet2Sbj.cc
/// @brief BNet2Sbj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bnet2sbj.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bnet/BNet2Sbj.h"
#include "ym_bnet/BNetwork.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
// BNode へのポインタをキーにして SbjNode と極性を保持する連想配列．
//////////////////////////////////////////////////////////////////////
class BNodeMap
{
public:

  // コンストラクタ
  BNodeMap(ymuint n);

  // 登録する．
  void
  put(BNode* bnode,
      SbjHandle sbjhandle);

  // 探す．
  SbjHandle
  get(BNode* bnode) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ただの配列
  vector<SbjHandle> mArray;

};

// コンストラクタ
BNodeMap::BNodeMap(ymuint n) :
  mArray(n, SbjHandle::make_zero())
{
}

// 登録する．
void
BNodeMap::put(BNode* bnode,
	      SbjHandle sbjhandle)
{
  mArray[bnode->id()] = sbjhandle;
}

// 探す．
SbjHandle
BNodeMap::get(BNode* bnode) const
{
  return mArray[bnode->id()];
}


//////////////////////////////////////////////////////////////////////
// クラス BNet2Sbj
//////////////////////////////////////////////////////////////////////

// @brief BNetwork を SbjGraph へ変換する
// @param[in] network 変換元のネットワーク
// @param[out] sbjgraph 変換先のネットワーク
// @param[in] err_out エラーメッセージの出力先
// @note network はすでに 2-feasible ネットワークに分解されているとする．
// @return 2-feasible でない場合には false を返す．
bool
BNet2Sbj::operator()(const BNetwork& network,
		     SbjGraph& sbjgraph,
		     ostream& err_out)
{

  ymuint n = network.max_node_id();
  // BNetwork 中のノードと BinGraph 中のノードの対応を持つ連想配列
  BNodeMap assoc(n);

  sbjgraph.clear();

  sbjgraph.set_name(network.model_name());

  // 外部入力を作る．
  // 同時にポートも作る．
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++p) {
    BNode* bnode = *p;
    SbjNode* node = sbjgraph.new_input();
    assoc.put(bnode, SbjHandle(node, false));
    sbjgraph.add_port(bnode->name(), vector<SbjNode*>(1, node));
  }

  // DFFを作る．
  for (BNodeList::const_iterator p = network.latch_nodes_begin();
       p != network.latch_nodes_end(); ++ p) {
    BNode* bnode = *p;
    SbjNode* node = sbjgraph.new_dff();
    assoc.put(bnode, SbjHandle(node, false));
  }

  // 定数0に縮退している外部出力/DFFを求めておく
  vector<bool> zero_outs(n, false);
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    // 外部出力
    BNode* obnode = *p;
    // 外部出力から参照されているノード
    BNode* ibnode = obnode->fanin(0);
    if ( ibnode->ni() == 0 ) {
      zero_outs[ibnode->id()] = true;
    }
  }
  for (BNodeList::const_iterator p = network.latch_nodes_begin();
       p != network.latch_nodes_end(); ++ p) {
    BNode* obnode = *p;
    // ラッチから参照されているノード
    BNode* ibnode = obnode->fanin(0);
    if ( ibnode->ni() == 0 ) {
      zero_outs[ibnode->id()] = true;
    }
  }

  // 内部ノードを作る．
  // まず入力からのトポロジカル順にソートし bnode_list に入れる．
  ymuint nv = network.logic_node_num();
  BNodeVector node_list;
  network.tsort(node_list);
  vector<SbjNode*> inode;
  for (ymuint i = 0; i < nv; ++ i) {
    BNode* bnode = node_list[i];
    ymuint ni = bnode->ni();
    if ( ni == 0 ) {
      if ( zero_outs[bnode->id()] == false ) {
	err_out << "ERROR: number of fanins is 0" << endl;
	err_out << bnode->name() << endl;
	return false;
      }
      bool inv = bnode->is_one();
      assoc.put(bnode, SbjHandle(NULL, inv));
    }
    else if ( ni == 1 ) {
      SbjHandle ihandle = assoc.get(bnode->fanin(0));
      if ( bnode->is_buffer() ) {
	assoc.put(bnode, ihandle);
      }
      else if ( bnode->is_inverter() ) {
	assoc.put(bnode, ~ihandle);
      }
      else {
	err_out << "ERROR: " << bnode->name()
	     << " should be a buffer or an inverter" << endl;
	return false;
      }
    }
    else if ( ni == 2 ) {
      LogExpr expr = bnode->func();
      assert_cond(expr.child(0).is_literal(), __FILE__, __LINE__);
      assert_cond(expr.child(1).is_literal(), __FILE__, __LINE__);

      SbjHandle ihandle0 = assoc.get(bnode->fanin(0));
      SbjHandle ihandle1 = assoc.get(bnode->fanin(1));

      if ( expr.child(0).is_negaliteral() ) {
	if ( expr.child(0).varid() == 0 ) {
	  ihandle0.invert();
	}
	else {
	  ihandle1.invert();
	}
      }

      if ( expr.child(1).is_negaliteral() ) {
	if ( expr.child(1).varid() == 0 ) {
	  ihandle0.invert();
	}
	else {
	  ihandle1.invert();
	}
      }

      SbjHandle handle;
      if ( expr.is_and() ) {
	handle = sbjgraph.new_and(ihandle0, ihandle1);
      }
      else if ( expr.is_or() ) {
	handle = sbjgraph.new_or(ihandle0, ihandle1);
      }
      else if ( expr.is_xor() ) {
	handle = sbjgraph.new_xor(ihandle0, ihandle1);
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      assoc.put(bnode, handle);
    }
    else { // ni > 2
      err_out << "ERROR: number of fanins exceeds 2" << endl;
      err_out << bnode->name() << " : " << bnode->ni() << endl;
      return false;
    }
  }

  // DFFノードの入力を接続する．
  for (BNodeList::const_iterator p = network.latch_nodes_begin();
       p != network.latch_nodes_end(); ++ p) {
    BNode* obnode = *p;
    SbjHandle ohandle = assoc.get(obnode);
    assert_cond( ohandle.inv() == false, __FILE__, __LINE__);
    SbjNode* onode = ohandle.node();
    BNode* ibnode = obnode->fanin(0);
    SbjHandle ihandle = assoc.get(ibnode);
    sbjgraph.set_dff_data(onode, ihandle);
  }

  // 外部出力ノードを作る．
  // 同時にポートも作る．
  for (BNodeList::const_iterator p = network.outputs_begin();
       p != network.outputs_end(); ++ p) {
    BNode* obnode = *p;
    BNode* ibnode = obnode->fanin(0);
    SbjHandle ihandle = assoc.get(ibnode);
    SbjNode* node = sbjgraph.new_output(ihandle);
    sbjgraph.add_port(obnode->name(), vector<SbjNode*>(1, node));
  }

  return true;
}

END_NAMESPACE_YM_BNET
