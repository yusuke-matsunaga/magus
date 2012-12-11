
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
  mAlloc(4096),
  mTgNetwork(tgnetwork)
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
    ymuint id = 0;

    for (ymuint i = 0; i < mInputNum; ++ i, ++ id) {
      const TgNode* tgnode = tgnetwork.input(i);
      DtpgNode* node = &mNodeArray[id];
      mInputArray[i] = node;
      set_node(tgnode, node, id);
    }

    ymuint nl = tgnetwork.logic_num();
    for (ymuint i = 0; i < nl; ++ i, ++ id) {
      const TgNode* tgnode = tgnetwork.sorted_logic(i);
      DtpgNode* node = &mNodeArray[id];
      set_node(tgnode, node, id);
    }

    for (ymuint i = 0; i < mOutputNum; ++ i, ++ id) {
      const TgNode* tgnode = tgnetwork.output(i);
      DtpgNode* node = &mNodeArray[id];
      mOutputArray[i] = node;
      set_node(tgnode, node, id);
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
  mFaultChunk = new DtpgFault[nf];
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
      df->mPosVal = (ipos << 2) | (fval << 1) | 1U;
      node->mInputFault[ipos * 2 + fval] = df;
    }
    else {
      df->mSrcNode = NULL;
      df->mPosVal = (fval << 1);
      node->mOutputFault[fval] = df;
    }
  }

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    DtpgNode* node = &mNodeArray[i];
    DtpgFault* f0 = node->output_fault(0);
    DtpgFault* f1 = node->output_fault(1);
    if ( f0 != NULL ) {
      f0->mAltFault = f1;
    }
    if ( f1 != NULL ) {
      f1->mAltFault = f0;
    }
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      DtpgFault* f0 = node->input_fault(0, j);
      DtpgFault* f1 = node->input_fault(1, j);
      if ( f0 != NULL ) {
	f0->mAltFault = f1;
      }
      if ( f1 != NULL ) {
	f1->mAltFault = f0;
      }
    }
  }

  activate_all();
}

// @brief デストラクタ
DtpgNetwork::~DtpgNetwork()
{
  delete [] mFaultChunk;

  // 残りは mAlloc のデストラクタがすべて開放してくれる
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

  dfs(output(pos), mark);

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
}

// @brief 全てのノードをアクティブにする．
void
DtpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  vector<bool> mark(mNodeNum, false);

  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    dfs(output(i), mark);
  }

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
}

// @brief ノードを得る．
DtpgNode*
DtpgNetwork::node(ymuint pos)
{
  // アドレス計算のために DtpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}


BEGIN_NONAMESPACE

DtpgNode*
merge(DtpgNode* node1,
      DtpgNode* node2,
      const vector<DtpgNode*>& mffc_root)
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
    if ( id1 > id2 ) {
      node1 = mffc_root[node1->id()];
    }
    else if ( id1 < id2 ) {
      node2 = mffc_root[node2->id()];
    }
  }
}

END_NONAMESPACE


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
  vector<DtpgNode*> mffc_root(mNodeNum, NULL);
  vector<DtpgFFR*> mffc_array(mNodeNum, NULL);
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint id = node->id();
    ymuint nfo = node->active_fanout_num();
    if ( node->is_output() || nfo > 1 ) {
      DtpgNode* tmp_root = NULL;
      if ( nfo > 0 ) {
	tmp_root = mffc_root[node->active_fanout(0)->id()];
	for (ymuint i = 1; i < nfo; ++ i) {
	  DtpgNode* tmp_root1 = mffc_root[node->active_fanout(i)->id()];
	  tmp_root = merge(tmp_root, tmp_root1, mffc_root);
	  if ( tmp_root == NULL ) {
	    break;
	  }
	}
      }
      mffc_root[id] = tmp_root;
      if ( tmp_root == NULL ) {
	DtpgFFR* mffc = new DtpgFFR;
	mffc_array[id] = mffc;
	mffc_list.push_back(mffc);
	mffc->mRoot = node;
      }
    }
    else {
      DtpgNode* fonode = node->active_fanout(0);
      DtpgNode* root = mffc_root[fonode->id()];
      if ( root == NULL ) {
	root = fonode;
      }
      mffc_root[id] = root;
    }
  }

  // 入力からのトポロジカル順にMFFC内のノードリストを作る．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    DtpgNode* node = mActNodeArray[i];
    DtpgFFR* mffc = mffc_array[node->id()];
    if ( mffc == NULL ) {
      mffc = mffc_array[mffc_root[node->id()]->id()];
    }
    assert_cond( mffc != NULL, __FILE__, __LINE__);
    mffc->mNodeList.push_back(node);
  }

  cout << "#MFFC = " << mffc_list.size() << endl;
}

// @brief ノードのマークをクリアする．
void
DtpgNetwork::clear_node_mark()
{
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeArray[i].set_mark(DtpgNode::kNone);
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
  clear_node_mark();

  // まず fnode の TFO にマークをつける．
  // 同時にマークの付いたノードは tfo_list につまれる．
  tfo_list.clear();
  tfo_list.reserve(mNodeNum);
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
  tfi_list.reserve(mNodeNum);
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

// @brief DtpgNode の内容を設定する．
// @param[in] tgnode もととなる TgNode
// @param[in] node 対象のノード
// @param[in] id ID番号
void
DtpgNetwork::set_node(const TgNode* tgnode,
		      DtpgNode* node,
		      ymuint id)
{
  mNodeMap[tgnode->gid()] = node;
  node->mTgNode = tgnode;
  node->mId = id;

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
}

END_NAMESPACE_YM_SATPG_DTPG
