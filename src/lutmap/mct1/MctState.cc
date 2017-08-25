
/// @file MctState.cc
/// @brief MctState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MctState.h"
#include "MapRecord.h"

#include "SbjGraph.h"
#include "SbjNode.h"

#include "SbjDumper.h"


BEGIN_NAMESPACE_YM_LUTMAP_MCT1

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE


// @brief コンストラクタ
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_size カットサイズ
MctState::MctState(const SbjGraph& sbjgraph,
		   ymuint cut_size) :
  mSbjGraph(sbjgraph),
  mCutSize(cut_size),
  mMaxNodeId(sbjgraph.node_num())
{
  if ( debug ) { // デバッグ用
    SbjDumper dumper;
    dumper.dump(cout, sbjgraph);
  }

  mNodeInfo = new NodeInfo[mMaxNodeId];

  mQueue.reserve(mMaxNodeId);
  mMarkedList.reserve(mMaxNodeId);

  // 異なる出力から相異なる極性が必要とされているノードを数える．
  mInvNum = 0;
  vector<ymuint> phase_map(mMaxNodeId, 0);
  for (ymuint i = 0; i < sbjgraph.output_num(); ++ i) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    ymuint mask = 0;
    if ( onode->output_fanin_inv() ) {
      mask = 2;
    }
    else {
      mask = 1;
    }
    phase_map[node->id()] |= mask;
    if ( phase_map[node->id()] == 3 ) {
      ++ mInvNum;
    }
  }

  init();
}

// @brief デストラクタ
MctState::~MctState()
{
  delete [] mNodeInfo;
}

// @brief 初期状態に戻す．
void
MctState::init()
{
  for (ymuint i = 0; i < mMaxNodeId; ++ i) {
    NodeInfo& node_info = mNodeInfo[i];
    node_info.mInputs.clear();
    node_info.mCutInputs.clear();
    node_info.mFlags = 0;
    node_info.mDepth = 0;
  }

  mSelectedList.clear();

  // 入力ノードをフロンティアリストに入れる．
  ymuint ni = mSbjGraph.input_num();
  mQueue.clear();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = mSbjGraph.input(i);
    set_selected(node);
    check_blocked(node);

    mNodeInfo[node->id()].mInputs.clear();
    mNodeInfo[node->id()].mInputs.push_back(node);
    put_queue(node);
  }

  mDepth = 0;

  get_cut_candidates();
}

void
MctState::get_cut_candidates()
{
  // キューに積まれたノードのファンアウトのノードを根とするカットを探す．
  // カットが見つかったノードをキューに積む．
  for (ymuint rpos = 0; rpos < mQueue.size(); ++ rpos) {
    const SbjNode* node = mQueue[rpos];
    const vector<const SbjNode*>& inputs1 = mNodeInfo[node->id()].mInputs;
    ymuint nfo = node->fanout_num();
    for (ymuint j = 0; j < nfo; ++ j) {
      const SbjEdge* e = node->fanout_edge(j);
      const SbjNode* to = e->to();
      if ( to->is_output() ) {
	// 出力ノードは除外する．
	continue;
      }
      if ( is_blocked(to) ) {
	// すでにカバーされているノードも除外する．
	continue;
      }
      if ( is_selected(to) ) {
	// すでに選ばれているノードも除外する．
	continue;
      }
      if ( qmark(to) ) {
	// カットを持つ(=キューに入っている)ノードも除外する．
	continue;
      }

      ymuint to_id = to->id();

      // 反対側のファンインを求める．
      const SbjNode* inode0 = to->fanin(e->pos() ^ 1);
      const vector<const SbjNode*>& inputs0 = mNodeInfo[inode0->id()].mInputs;
      if ( inputs0.empty() ) {
	// 対象ではなかった．
	continue;
      }

      // マージできるか調べる．
      vector<const SbjNode*> tmp_inputs;
      merge_inputs(inputs1, inputs0, tmp_inputs);
      if ( tmp_inputs.size() <= mCutSize ) {
	if ( debug ) { // デバッグ用
	  cout << " " << node->id() << ", " << inode0->id()
	       << " --> " << to_id << ": ";
	  for (ymuint i = 0; i < tmp_inputs.size(); ++ i) {
	    cout << " " << tmp_inputs[i]->id();
	  }
	  cout << endl;
	}
	mNodeInfo[to_id].mInputs = tmp_inputs;
	put_queue(to);
	if ( !has_cut(to) ) {
	  set_cut(to);
	  if ( to->pomark() ) {
	    mPoCandidateList.push_back(to);
	  }
	  else {
	    mCandidateList.push_back(to);
	    mCandNiList.push_back(tmp_inputs.size() - 1);
	  }
	}
      }
    }
  }

  // キューをクリアする．
  clear_queue();
}

// @brief キューに入れる．
void
MctState::put_queue(const SbjNode* node)
{
  mQueue.push_back(node);
  mNodeInfo[node->id()].mFlags |= (1U << 4);
}

// @brief キューを空にする．
void
MctState::clear_queue()
{
 for (ymuint i = 0; i < mQueue.size(); ++ i) {
   const SbjNode* node = mQueue[i];
   mNodeInfo[node->id()].mFlags &= ~(1U << 4);
  }
  mQueue.clear();
}

// @brief 2つのカットの入力をマージする．
void
MctState::merge_inputs(const vector<const SbjNode*>& inputs1,
		       const vector<const SbjNode*>& inputs2,
		       vector<const SbjNode*>& new_inputs)
{
  ymuint ni1 = inputs1.size();
  ymuint ni2 = inputs2.size();
  new_inputs.reserve(ni1 + ni2);
  for (ymuint i = 0; i < ni1; ++ i) {
    const SbjNode* inode = inputs1[i];
    new_inputs.push_back(inode);
    set_mark(inode);
  }
  for (ymuint i = 0; i < ni2; ++ i) {
    const SbjNode* inode = inputs2[i];
    if ( !is_marked(inode) ) {
      new_inputs.push_back(inode);
    }
  }
  clear_mark();
}

// @brief 次に選択可能な POカットのリストを得る．
// @param[in] cut_list カットのリスト
const vector<const SbjNode*>&
MctState::pocandidates() const
{
  return mPoCandidateList;
}

// @brief 次に選択可能なカットのリストを作る．
// @param[in] cut_list カットのリスト
const vector<const SbjNode*>&
MctState::candidates() const
{
  if ( debug ) {
    cout << "CANDIDATES: ";
    for (ymuint i = 0; i < mCandidateList.size(); ++ i) {
      const SbjNode* node = mCandidateList[i];
      cout << " " << node->id_str();
    }
    cout << endl;
  }
  return mCandidateList;
}

// @brief カット候補の重みリストを得る．
const vector<ymuint>&
MctState::weight_list() const
{
  return mCandNiList;
}

// @brief LUT 数を得る．
ymuint
MctState::lut_num() const
{
  return mSelectedList.size() + mInvNum;
}

// @brief 全体の段数を得る．
int
MctState::depth() const
{
  return mDepth;
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief カットを追加して状態を更新する．
// @param[in] root カットの根のノード
void
MctState::update(const SbjNode* root)
{
  if ( debug ) {
    cout << endl
	 << "update(" << root->id_str() << ")" << endl;
  }

  set_selected(root);
  mSelectedList.push_back(root);

  // cut によってカバーされたノードに印をつける．
  // カバーとは cut の根を経由せずに外部出力へ到達不能となること．

  // cut の根から入力の間にあるノードを node_list に入れる．
  // cut の入力のノードを node_info.mCutInputs に入れる．
  NodeInfo& node_info = mNodeInfo[root->id()];
  node_info.mCutInputs.clear();
  vector<bool> mark(mMaxNodeId, false);
  vector<const SbjNode*> node_list;
  mark[root->id()] = true;
  node_list.push_back(root);
  ymuint depth = 0;
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    const SbjNode* node = node_list[rpos];
    if ( node != root && is_selected(node) ) {
      // カットの入力よりファンイン側にはたどらない．
      node_info.mCutInputs.push_back(node);
      int depth1 = mNodeInfo[node->id()].mDepth;
      if ( depth < depth1 ) {
	depth = depth1;
      }
      continue;
    }

    const SbjNode* inode0 = node->fanin(0);
    if ( !mark[inode0->id()] ) {
      mark[inode0->id()] = true;
      node_list.push_back(inode0);
    }
    const SbjNode* inode1 = node->fanin(1);
    if ( !mark[inode1->id()] ) {
      mark[inode1->id()] = true;
      node_list.push_back(inode1);
    }
  }
  node_info.mDepth = depth + 1;

  if ( root->pomark() ) {
    if ( mDepth < node_info.mDepth ) {
      mDepth = node_info.mDepth;
    }
  }

  // node_list を出力側からのトポロジカル順に並べる．
  // 具体的には mark[] を参考にしながらもう一度おなじことをする．
  // なんかもっとスマートな方法があるような気がする．
  ymuint n0 = node_list.size();
  node_list.clear();
  node_list.push_back(root);
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    const SbjNode* node = node_list[rpos];
    mark[node->id()] = false;
    if ( node != root && is_selected(node) ) {
      // カットの入力よりファンイン側にはたどらない．
      continue;
    }
    if ( !node->is_logic() ) {
      // 入力ノードなので終わり．
      continue;
    }

    const SbjNode* inode0 = node->fanin(0);
    ymuint nfo0 = inode0->fanout_num();
    for (ymuint i = 0; i < nfo0; ++ i) {
      const SbjEdge* e = inode0->fanout_edge(i);
      const SbjNode* to = e->to();
      if ( !is_selected(to) && mark[to->id()] ) {
	// まだ未処理のファンアウトがあった．
	goto next0;
      }
    }
    node_list.push_back(inode0);

  next0:
    const SbjNode* inode1 = node->fanin(1);
    ymuint nfo1 = inode1->fanout_num();
    for (ymuint i = 0; i < nfo1; ++ i) {
      const SbjEdge* e = inode1->fanout_edge(i);
      const SbjNode* to = e->to();
      if ( !is_selected(to) && mark[to->id()] ) {
	// まだ未処理のファンアウトがあった．
	goto next1;
      }
    }
    node_list.push_back(inode1);

  next1:
    ;
  }
  ASSERT_COND( node_list.size() == n0 );

  // node_list に含まれるノードで root を経由せずに外部出力へ到達可能かどうか調べる．
  for (ymuint i = 0; i < node_list.size(); ++ i) {
    const SbjNode* node = node_list[i];
    check_blocked(node);
  }

  // mCandidateList を作り直す．
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < mCandidateList.size(); ++ rpos) {
    const SbjNode* node = mCandidateList[rpos];
    if ( node == root ) {
      mNodeInfo[node->id()].mInputs = vector<const SbjNode*>(1, node);
      clear_cut(node);
      if ( debug ) {
	cout << "  " << node->id_str() << " is removed" << endl;
      }
    }
    else if ( is_blocked(node) ) {
      clear_cut(node);
      if ( debug ) {
	cout << "  " << node->id_str() << " is removed" << endl;
      }
    }
    else {
      if ( wpos != rpos ) {
	mCandidateList[wpos] = node;
	mCandNiList[wpos] = mCandNiList[rpos];
      }
      ++ wpos;
    }
  }
  mCandidateList.erase(mCandidateList.begin() + wpos, mCandidateList.end());
  mCandNiList.erase(mCandNiList.begin() + wpos, mCandNiList.end());

  // mPoCandidateList を作り直す．
  wpos = 0;
  for (ymuint rpos = 0; rpos < mPoCandidateList.size(); ++ rpos) {
    const SbjNode* node = mPoCandidateList[rpos];
    if ( node == root ) {
      mNodeInfo[node->id()].mInputs = vector<const SbjNode*>(1, node);
      clear_cut(node);
      if ( debug ) {
	cout << "  " << node->id_str() << " is removed" << endl;
      }
    }
    else {
      if ( wpos != rpos ) {
	mPoCandidateList[wpos] = node;
      }
      ++ wpos;
    }
  }
  mPoCandidateList.erase(mPoCandidateList.begin() + wpos, mPoCandidateList.end());

  if ( !is_blocked(root) ) {
    put_queue(root);
    get_cut_candidates();
  }
}

// @brief マッピング情報をコピーする．
void
MctState::copy_to(MapRecord& maprecord)
{
  maprecord.init(mSbjGraph);
  for (ymuint i = 0; i < mSelectedList.size(); ++ i) {
    const SbjNode* node = mSelectedList[i];
    //maprecord.set_cut(node, mNodeInfo[node->id()].mCutInputs);
  }
}

// @brief block されているか調べる．
// @return block されていたら true を返す．
bool
MctState::check_blocked(const SbjNode* node)
{
  if ( debug ) {
    cout << " checking for " << node->id_str() << endl;
  }
  ymuint nfo = node->fanout_num();
  bool blocked = true;
  for (ymuint i = 0; i < nfo; ++ i) {
    const SbjEdge* e = node->fanout_edge(i);
    const SbjNode* to = e->to();
    if ( to->is_output() ) {
      if ( is_selected(node) ) {
	// 出力は無視
	continue;
      }
      // node が選ばれていない状態で出力にファンアウトしていたら
      // ブロックされていないということ．
      blocked = false;
      break;
    }
    if ( !is_selected(to) && !is_blocked(to) ) {
      // ブロックされていないファンアウトがあった．
      blocked = false;
      break;
    }
  }
  if ( blocked ) {
    set_blocked(node);
    if ( debug ) {
      cout << "   " << node->id_str() << " is blocked" << endl;
    }
  }
  return blocked;
}

// @brief 'blocked' 状態をチェックする．
void
MctState::check_cover()
{
  ymuint nl = mSbjGraph.logic_num();
  for (ymuint i = 0; i < nl; ++ i) {
    const SbjNode* node = mSbjGraph.logic(nl - i - 1);
    check_cover_node(node);
  }
  ymuint ni = mSbjGraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = mSbjGraph.input(i);
    check_cover_node(node);
  }
}

void
MctState::check_cover_node(const SbjNode* node)
{
  ymuint nfo = node->fanout_num();
  if ( is_blocked(node) ) {
    for (ymuint i = 0; i < nfo; ++ i) {
      const SbjEdge* e = node->fanout_edge(i);
      const SbjNode* to = e->to();
      if ( to->is_output() ) {
	if ( is_selected(node) ) {
	  continue;
	}
      }
      if ( !is_selected(to) && !is_blocked(to) ) {
	cout << "Error: " << node->id_str() << endl;
	if ( is_selected(node) ) {
	  cout << "  SELECTED" << endl;
	}
	if ( is_blocked(node) ) {
	  cout << "  COVERED" << endl;
	}
	cout << "    but has fanout " << to->id_str()
	     << ", which is not blocked" << endl;
	break;
      }
    }
  }
  else {
    bool blocked = true;
    for (ymuint i = 0; i < nfo; ++ i) {
      const SbjEdge* e = node->fanout_edge(i);
      const SbjNode* to = e->to();
      if ( to->is_output() ) {
	// 外部出力はスキップ
	if ( is_selected(node) ) {
	  continue;
	}
	blocked = false;
	cout << "    " << node->id_str() << " is not 'blocked' since "
	     << to->id_str() << " is the output" << endl;
      }
      if ( !is_selected(to) && !is_blocked(to) ) {
	// たしかにカバーしていないファンアウトがあった．
	blocked = false;
	cout << "    " << node->id_str() << " is not 'blocked' since "
	     << to->id_str() << " is not 'blocked'" << endl;
      }
    }
    if ( blocked ) {
      cout << "Error: " << node->id_str() << endl;
      if ( is_selected(node) ) {
	cout << "  SELECTED" << endl;
      }
      if ( is_blocked(node) ) {
	cout << "  COVERED" << endl;
      }
      cout << "has no fanout which is not blocked" << endl;
    }
  }
}

END_NAMESPACE_YM_LUTMAP_MCT1
