
/// @file libym_bdn/BlifBdnConv.cc
/// @brief BlifBdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBdnConv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BlifBdnConv.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"
#include "ym_bdn/BdnDff.h"
#include "ym_blif/BlifNetwork.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
BlifBdnConv::BlifBdnConv()
{
}

// @brief デストラクタ
BlifBdnConv::~BlifBdnConv()
{
}

// @brief 変換する
// @param[in] blif_network 変換元のネットワーク
// @param[in] network 変換先のネットワーク
bool
BlifBdnConv::operator()(const BlifNetwork& blif_network,
			BdNetwork& network)
{
  mNetwork = &network;
  ymuint n = blif_network.max_node_id();
  mNodeMap.resize(n);
  mNodeFlag.resize(n, false);

  // モデル名の設定
  mNetwork->set_name(blif_network.name());

  // 外部入力ノードの生成
  ymuint npi = blif_network.npi();
  for (ymuint i = 0; i < npi; ++ i) {
    const BlifNode* blif_node = blif_network.pi(i);
    BdnNode* node = mNetwork->new_input();
    put_node(blif_node, BdnNodeHandle(node, false));
  }

  // D-FFの出力(擬似入力)ノードの生成
  ymuint nff = blif_network.nff();
  vector<BdnDff*> dff_array(nff);
  for (ymuint i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    int reset_val = 2;
    if ( blif_node->opat() == '0' ) {
      reset_val = 0;
    }
    else if ( blif_node->opat() == '1' ) {
      reset_val = 1;
    }
    // 今はリセット値は無視
    BdnDff* dff = mNetwork->new_dff();
    put_node(blif_node, BdnNodeHandle(dff->output(), false));
    dff_array[i] = dff;
  }

  // 外部出力に用いられているノードを再帰的に生成
  ymuint npo = blif_network.npo();
  for (ymuint i = 0; i < npo; ++ i) {
    const BlifNode* blif_node = blif_network.po(i);
    BdnNodeHandle node_h = make_node(blif_node);
    (void) mNetwork->new_output(node_h);
  }

  // D-FFに用いられているノードを再帰的に生成
  for (ymuint i = 0; i < nff; ++ i) {
    const BlifNode* blif_node = blif_network.ff(i);
    BdnNodeHandle inode_h = make_node(blif_node->fanin(0));
    BdnDff* dff = dff_array[i];
    BdnNode* dff_input = dff->input();
    mNetwork->set_output_fanin(dff_input, inode_h);
  }

  return true;
}

// blif_node に対応するノードを作る．
BdnNodeHandle
BlifBdnConv::make_node(const BlifNode* blif_node)
{
  BdnNodeHandle node_handle;
  if ( !get_node(blif_node, node_handle) ) {
    assert_cond( blif_node->type() == BlifNode::kLogic, __FILE__, __LINE__);
    ymuint32 ni = blif_node->ni();
    vector<BdnNodeHandle> fanins(ni);
    for (ymuint32 i = 0; i < ni; ++ i) {
      fanins[i] = make_node(blif_node->fanin(i));
    }

    ymuint32 nc = blif_node->nc();
    if ( blif_node->opat() == '1' ) {
      vector<BdnNodeHandle> or_leaves;
      or_leaves.reserve(nc);
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<BdnNodeHandle> and_leaves;
	and_leaves.reserve(ni);
	for (ymuint32 i = 0; i < ni; ++ i) {
	  char v = blif_node->cube_pat(c, i);
	  if ( v == '0' ) {
	    and_leaves.push_back(~fanins[i]);
	  }
	  else if ( v == '1' ) {
	    and_leaves.push_back(fanins[i]);
	  }
	}
	ymuint32 n = and_leaves.size();
	assert_cond( n > 0, __FILE__, __LINE__);
	or_leaves.push_back(bidecomp(0x8, 0, n, and_leaves));
      }
      ymuint32 n = or_leaves.size();
      if ( n == 0 ) {
	node_handle = BdnNodeHandle::make_one();
      }
      else {
	node_handle = bidecomp(0xE, 0, n, or_leaves);
      }
    }
    else {
      vector<BdnNodeHandle> and_leaves;
      and_leaves.reserve(nc);
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<BdnNodeHandle> or_leaves;
	or_leaves.reserve(ni);
	for (ymuint32 i = 0; i < ni; ++ i) {
	  char v = blif_node->cube_pat(c, i);
	  if ( v == '0' ) {
	    or_leaves.push_back(fanins[i]);
	  }
	  else if ( v == '1' ) {
	    or_leaves.push_back(~fanins[i]);
	  }
	}
	ymuint32 n = or_leaves.size();
	assert_cond( n > 0, __FILE__, __LINE__);
	and_leaves.push_back(bidecomp(0xE, 0, n, or_leaves));
      }
      ymuint32 n = and_leaves.size();
      if ( n == 0 ) {
	node_handle = BdnNodeHandle::make_zero();
      }
      else {
	node_handle = bidecomp(0x8, 0, n, and_leaves);
      }
    }
    put_node(blif_node, node_handle);
  }
  return node_handle;
}

// @brief 2分木を生成する．
BdnNodeHandle
BlifBdnConv::bidecomp(ymuint32 fcode,
		      ymuint32 start,
		      ymuint32 size,
		      const vector<BdnNodeHandle>& child_array)
{
  if ( size == 1 ) {
    return child_array[start];
  }
  ymuint32 nl = size / 2;
  BdnNodeHandle l = bidecomp(fcode, start, nl, child_array);
  ymuint32 nr = size - nl;
  BdnNodeHandle r = bidecomp(fcode, start + nl, nr, child_array);
  return mNetwork->new_logic(fcode, l, r);
}

// @brief blif_node に対応した BdnNode を取り出す．
bool
BlifBdnConv::get_node(const BlifNode* blif_node,
		      BdnNodeHandle& node_handle)
{
  ymuint id = blif_node->id();
  if ( mNodeFlag[id] ) {
    node_handle = mNodeMap[id];
    return true;
  }
  return false;
}

// @brief blif_node に対応した BdnNode を登録する．
void
BlifBdnConv::put_node(const BlifNode* blif_node,
		      BdnNodeHandle node_handle)
{
  ymuint id = blif_node->id();
  mNodeMap[id] = node_handle;
  mNodeFlag[id] = true;
}

END_NAMESPACE_YM_BDN
