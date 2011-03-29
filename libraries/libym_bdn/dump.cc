
/// @file libym_bdn/BdNetwork.cc
/// @brief BdNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnPort.h"
#include "ym_bdn/BdnDff.h"
#include "ym_bdn/BdnLatch.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

BEGIN_NONAMESPACE

// 出力ノードの内容を出力する．
void
dump_output(ostream& s,
	    const BdnNode* node)
{
  const BdnNode* inode = node->output_fanin();
  bool oinv = node->output_fanin_inv();
  if ( inode ) {
    // 普通のノードの場合
    if ( oinv ) {
      s << "~";
    }
    s << inode->id_str();
  }
  else {
    // 定数ノードの場合
    if ( oinv ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

END_NONAMESPACE

// @brief BdNetwork の内容をダンプする関数
void
dump(ostream& s,
     const BdNetwork& network)
{
  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << node->id_str() << " : = INPUT" << endl;
  }

  const BdnNodeList& lnode_list = network.lnode_list();
  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << node->id_str();
    s << " :  = LOGIC[";
    ymuint fcode = node->fcode();
    s << ((fcode >> 3) & 1) << ((fcode >> 2) & 1)
      << ((fcode >> 1) & 1) << (fcode & 1) << "] ("
      << " " << node->fanin0()->id_str()
      << ", " << node->fanin1()->id_str()
      << ")" << endl;
  }

  const BdnNodeList& output_list = network.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << node->id_str() << " : = OUTPUT(";
    dump_output(s, node);
    s << ")" << endl;
  }

  const BdnDffList& dff_list = network.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* output = dff->output();
    const BdnNode* input = dff->input();
    const BdnNode* clock = dff->clock();
    const BdnNode* set = dff->set();
    const BdnNode* reset = dff->reset();
    s << "DFF#" << dff->id() << "("
      << "OUTPUT: " << output->id_str()
      << ", INPUT: ";
    dump_output(s, input);
    s << ", CLOCK: ";
    dump_output(s, clock);
    if ( set ) {
      s << ", SET: ";
      dump_output(s, set);
    }
    if ( reset ) {
      s << ", RESET: ";
      dump_output(s, reset);
    }
    s << ")" << endl;
  }
}

END_NAMESPACE_YM_BDN
