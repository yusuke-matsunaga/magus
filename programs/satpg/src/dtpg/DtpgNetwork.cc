
/// @file DtpgNetwork.cc
/// @brief DtpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgNetwork.h"
#include "DtpgNode.h"
#include "DtpgFFR.h"
#include "DtpgFault.h"
#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

inline
DtpgNode**
alloc_nodearray(Alloc& alloc,
		ymuint n)
{
  void* p = alloc.get_memory(sizeof(DtpgNode*) * n);
  return new (p) DtpgNode*[n];
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス DtpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tgnetwork もとのネットワーク
// @param[in] fault_list 故障リスト
DtpgNetwork::DtpgNetwork(const TgNetwork& tgnetwork,
			 const vector<SaFault*>& fault_list) :
  mAlloc(4096)
{
  mNodeNum = tgnetwork.node_num();
  mInputNum = tgnetwork.input_num1();
  mOutputNum = tgnetwork.output_num1();
  mFFNum = tgnetwork.ff_num();

  {
    void* p = mAlloc.get_memory(sizeof(DtpgNode) * mNodeNum);
    mNodeArray = new (p) DtpgNode[mNodeNum];
  }

  mNodeMap = alloc_nodearray(mAlloc, mNodeNum);
  mInputArray = alloc_nodearray(mAlloc, input_num2());
  mOutputArray = alloc_nodearray(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_nodearray(mAlloc, mNodeNum);

  { // TgNode を DtpgNode にコピーする．
    // ただし mNodeArray は入力からのトポロジカル順になる．
    ymuint id = 0;

    for (ymuint i = 0; i < mInputNum; ++ i, ++ id) {
      const TgNode* tgnode = tgnetwork.input(i);
      DtpgNode* node = &mNodeArray[id];
      mInputArray[i] = node;
      set_node(tgnode, tgnetwork, node, id);
    }

    ymuint nl = tgnetwork.logic_num();
    for (ymuint i = 0; i < nl; ++ i, ++ id) {
      const TgNode* tgnode = tgnetwork.sorted_logic(i);
      DtpgNode* node = &mNodeArray[id];
      set_node(tgnode, tgnetwork, node, id);
    }

    for (ymuint i = 0; i < mOutputNum; ++ i, ++ id) {
      const TgNode* tgnode = tgnetwork.output(i);
      DtpgNode* node = &mNodeArray[id];
      mOutputArray[i] = node;
      set_node(tgnode, tgnetwork, node, id);
    }

    assert_cond( id == mNodeNum, __FILE__, __LINE__);
  }

  // ファンアウトの情報をコピーする．
  ymuint n = tgnetwork.node_num();
  for (ymuint i = 0; i < n; ++ i) {
    const TgNode* tgnode = tgnetwork.node(i);
    ymuint nfo = tgnode->fanout_num();
    DtpgNode* node = mNodeMap[tgnode->gid()];
    for (ymuint i = 0; i < nfo; ++ i) {
      const TgNode* otgnode = tgnode->fanout(i);
      DtpgNode* onode = mNodeMap[otgnode->gid()];
      node->mFanouts[i] = onode;
    }
  }

  // 故障リストを作る．
  ymuint nf = fault_list.size();
  void* p = mAlloc.get_memory(sizeof(DtpgFault) * nf);
  mFaultChunk = new (p) DtpgFault[nf];
  mFaultList.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    SaFault* f = fault_list[i];
    DtpgFault* df = &mFaultChunk[i];
    mFaultList.push_back(df);
    df->mSaFault = f;
    df->mId = i;
    const TgNode* tgnode = f->node();
    DtpgNode* node = mNodeMap[tgnode->gid()];
    df->mNode = node;
    int fval = f->val();
    assert_cond( fval == 0 || fval == 1, __FILE__, __LINE__);
    if ( f->is_input_fault() ) {
      ymuint ipos = f->pos();
      const TgNode* tgsrc = f->source_node();
      df->mSrcNode = mNodeMap[tgsrc->gid()];
      df->mPosVal = (ipos << 3) | (fval << 1) | 1U;
      node->mInputFault[ipos * 2 + fval] = df;
    }
    else {
      df->mSrcNode = NULL;
      df->mPosVal = (fval << 1);
      node->mOutputFault[fval] = df;
    }
  }
  activate_all();
}

// @brief デストラクタ
DtpgNetwork::~DtpgNetwork()
{
}

BEGIN_NONAMESPACE

void
dfs(DtpgNode* node,
    vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return;
  }

  mark[node->id()] = true;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    dfs(node->fanin(i), mark);
  }
}

END_NONAMESPACE

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] pos 外部出力の位置番号
void
DtpgNetwork::activate_po(ymuint pos)
{
  vector<bool> mark(mNodeNum, false);

  // pos 番めの出力から到達可能なノードにマークをつける．
  dfs(output(pos), mark);

  activate_sub(mark);
}

// @brief 全てのノードをアクティブにする．
void
DtpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  vector<bool> mark(mNodeNum, false);

  // すべての PO から到達可能なノードにマークをつける．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    dfs(output(i), mark);
  }

  activate_sub(mark);
}


BEGIN_NONAMESPACE

DtpgNode*
merge(DtpgNode* node1,
      DtpgNode* node2)
{
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( node1 == NULL || node2 == NULL ) {
      return NULL;
    }
    ymuint id1 = node1->id();
    ymuint id2 = node2->id();
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
    }
  }
}

END_NONAMESPACE

// @brief activate_po(), activate_all() の下請け関数
void
DtpgNetwork::activate_sub(const vector<bool>& mark)
{
  // マークにしたがってファンアウトなどの情報をセットする．
  mActNodeNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    DtpgNode* node = &mNodeArray[i];
    if ( !mark[i] ) {
      node->clear_active();
      continue;
    }
    node->set_active();
    mActNodeArray[mActNodeNum] = node;
    ++ mActNodeNum;

    ymuint nfo = node->fanout_num();
    ymuint act_nfo = 0;
    for (ymuint i = 0; i < nfo; ++ i) {
      DtpgNode* onode = node->fanout(i);
      if ( mark[onode->id()] ) {
	node->mActFanouts[act_nfo] = onode;
	++ act_nfo;
      }
    }
    node->mActFanoutNum = act_nfo;
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    switch ( nfo ) {
    case 0:
      assert_cond( node->is_output(), __FILE__, __LINE__);
      node->mImmDom = NULL;
      break;

    case 1:
      node->mImmDom = node->active_fanout(0);
      break;

    default:
      {
	DtpgNode* node0 = node->active_fanout(0);
	for (ymuint i = 1; i < nfo; ++ i) {
	  node0 = merge(node0, node->active_fanout(i));
	  if ( node0 == NULL ) {
	    break;
	  }
	}
	node->mImmDom = node0;
      }
      break;
    }
  }
}

// @brief ノードを得る．
DtpgNode*
DtpgNetwork::node(ymuint pos)
{
  // アドレス計算のために DtpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief アクティブな部分に対して FFR を求める．
void
DtpgNetwork::get_ffr_list(vector<DtpgFFR*>& ffr_list)
{
  // FFR を求める．
  vector<DtpgFFR*> ffr_array(mNodeNum);
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint id = node->id();
    if ( node->is_output() || node->active_fanout_num() > 1 ) {
      DtpgFFR* ffr = new DtpgFFR;
      ffr_array[id] = ffr;
      ffr_list.push_back(ffr);
      ffr->mRoot = node;
    }
    else if ( node->active_fanout_num() == 1 ) {
      DtpgNode* fonode = node->active_fanout(0);
      DtpgFFR* ffr = ffr_array[fonode->id()];
      assert_cond( ffr != NULL, __FILE__, __LINE__);
      ffr_array[id] = ffr;
    }
  }

  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[i];
    DtpgFFR* ffr = ffr_array[node->id()];
    assert_cond( ffr != NULL, __FILE__, __LINE__);
    ffr->mNodeList.push_back(node);
  }
}

// @brief アクティブな部分に対して MFFC を求める．
void
DtpgNetwork::get_mffc_list(vector<DtpgFFR*>& mffc_list)
{
  vector<DtpgFFR*> mffc_array(mNodeNum, NULL);
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint id = node->id();
    DtpgNode* root = node->imm_dom();
    if ( root == NULL ) {
      DtpgFFR* mffc = new DtpgFFR;
      mffc_array[id] = mffc;
      mffc_list.push_back(mffc);
      mffc->mRoot = node;
    }
    else {
      mffc_array[id] = mffc_array[root->id()];
    }
  }

  // 入力からのトポロジカル順にMFFC内のノードリストを作る．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[i];
    ymuint id = node->id();
    DtpgFFR* mffc = mffc_array[id];
    assert_cond( mffc != NULL, __FILE__, __LINE__);
    mffc->mNodeList.push_back(node);
  }
}

// @brief fnode の TFO の TFI にマークをつける．
// @param[in] fnode 起点となるノード
// @param[in] tfo_list TFO ノードを入れるリスト
// @param[in] tfi_list TFI ノードを入れるリスト
void
DtpgNetwork::mark_tfo_tfi(DtpgNode* fnode,
			  vector<DtpgNode*>& tfo_list,
			  vector<DtpgNode*>& tfi_list)
{
  // まず fnode の TFO にマークをつける．
  // 同時にマークの付いたノードは tfo_list につまれる．
  tfo_list.clear();
  tfo_list.reserve(mActNodeNum);
  tfo_list.push_back(fnode);
  fnode->set_mark(DtpgNode::kTFO);
  for (ymuint rpos = 0; rpos < tfo_list.size(); ++ rpos) {
    DtpgNode* node = tfo_list[rpos];
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      DtpgNode* onode = node->active_fanout(i);
      if ( onode->mark() == DtpgNode::kNone ) {
	tfo_list.push_back(onode);
	onode->set_mark(DtpgNode::kTFO);
      }
    }
  }

  // TFO マークの付いたノードの TFI に別のマークをつける．
  tfi_list.clear();
  tfi_list.reserve(mActNodeNum);
  for (ymuint rpos = 0; rpos < tfo_list.size(); ++ rpos) {
    DtpgNode* node = tfo_list[rpos];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      if ( inode->mark() == DtpgNode::kNone ) {
	// マークの付いていないファンインがあった．
	tfi_list.push_back(inode);
	inode->set_mark(DtpgNode::kTFI);
      }
    }
  }
  for (ymuint rpos2 = 0; rpos2 < tfi_list.size(); ++ rpos2) {
    DtpgNode* node = tfi_list[rpos2];
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      DtpgNode* inode = node->fanin(i);
      if ( inode->mark() == DtpgNode::kNone ) {
	// マークの付いていないファンインがあった．
	tfi_list.push_back(inode);
	inode->set_mark(DtpgNode::kTFI);
      }
    }
  }
}

// @brief ノードのマークをクリアする．
void
DtpgNetwork::clear_node_mark(const vector<DtpgNode*>& tfo_list,
			     const vector<DtpgNode*>& tfi_list)
{
  for (vector<DtpgNode*>::const_iterator p = tfo_list.begin();
       p != tfo_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->set_mark(DtpgNode::kNone);
  }
  for (vector<DtpgNode*>::const_iterator p = tfi_list.begin();
       p != tfi_list.end(); ++ p) {
    DtpgNode* node = *p;
    node->set_mark(DtpgNode::kNone);
  }
}

// @brief DtpgNode の内容を設定する．
// @param[in] tgnode もととなる TgNode
// @param[in] node 対象のノード
// @param[in] id ID番号
void
DtpgNetwork::set_node(const TgNode* tgnode,
		      const TgNetwork& tgnetwork,
		      DtpgNode* node,
		      ymuint id)
{
  mNodeMap[tgnode->gid()] = node;
  node->mGateType = tgnode->type();
  node->mId = id;
  node->mLid = tgnode->lid();

  if ( tgnode->is_cplx_logic() ) {
    node->mExpr = tgnetwork.get_lexp(tgnode);
  }
  ymuint ni = tgnode->ni();
  node->mFaninNum = ni;
  node->mFanins = alloc_nodearray(mAlloc, ni);
  {
    void* p = mAlloc.get_memory(sizeof(DtpgFault*) * ni * 2);
    node->mInputFault = new (p) DtpgFault*[ni * 2];
  }

  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* itgnode = tgnode->fanin(i);
    DtpgNode* inode = mNodeMap[itgnode->gid()];
    node->mFanins[i] = inode;
  }

  ymuint no = tgnode->fanout_num();
  node->mFanoutNum = no;
  node->mFanouts = alloc_nodearray(mAlloc, no);
  node->mActFanoutNum = 0;
  node->mActFanouts = alloc_nodearray(mAlloc, no);

  for (ymuint i = 0; i < ni; ++ i) {
    node->mInputFault[i * 2 + 0] = NULL;
    node->mInputFault[i * 2 + 1] = NULL;
  }
  node->mOutputFault[0] = NULL;
  node->mOutputFault[1] = NULL;

  node->mMarks = (static_cast<ymuint32>(DtpgNode::kNone) << 1);
}

END_NAMESPACE_YM_SATPG_DTPG
