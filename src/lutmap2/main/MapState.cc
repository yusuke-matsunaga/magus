
/// @file MapState.cc
/// @brief MapState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MapState.h"
#include "ym/BnBuilder.h"
#include "ym/TvFunc.h"
#include "SbjGraph.h"
#include "SbjNode.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjPort.h"

#include "SbjDumper.h"


BEGIN_NAMESPACE_YM_LUTMAP2

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE


// @brief コンストラクタ
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_size カットサイズ
MapState::MapState(const SbjGraph& sbjgraph,
		   ymuint cut_size) :
  mSbjGraph(sbjgraph),
  mCutSize(cut_size),
  mMaxNodeId(sbjgraph.max_node_id()),
  mQmark(mMaxNodeId, false)
{
  if ( debug ) { // デバッグ用
    SbjDumper dumper;
    dumper.dump(cout, sbjgraph);
  }

  mNodeInfo = new NodeInfo[mMaxNodeId];

  mQueue.reserve(mMaxNodeId);
  mMarkedList.reserve(mMaxNodeId);

  // 入力ノードをフロンティアリストに入れる．
  ymuint ni = sbjgraph.input_num();
  mQueue.clear();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = sbjgraph.input(i);
    set_selected(node);
    check_blocked(node);

    mNodeInfo[node->id()].mInputs.clear();
    mNodeInfo[node->id()].mInputs.push_back(node);
    mQueue.push_back(node);
    mQmark[node->id()] = true;
  }

  get_cut_candidates();

  mLutNum = 0;

  mNextId = 0;
}

// @brief デストラクタ
MapState::~MapState()
{
  delete [] mNodeInfo;
}

void
MapState::get_cut_candidates()
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
      ymuint to_id = to->id();
      if ( mQmark[to_id] ) {
	// カットを持つ(=キューに入っている)ノードも除外する．
	continue;
      }

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
	mQueue.push_back(to);
	mQmark[to_id] = true;
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
}

// @brief 2つのカットの入力をマージする．
void
MapState::merge_inputs(const vector<const SbjNode*>& inputs1,
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
MapState::next_pocandidates() const
{
  return mPoCandidateList;
}

// @brief 次に選択可能なカットのリストを作る．
// @param[in] cut_list カットのリスト
const vector<const SbjNode*>&
MapState::next_candidates() const
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
MapState::weight_list() const
{
  return mCandNiList;
}

// @brief LUT 数を得る．
ymuint
MapState::lut_num() const
{
  return mLutNum;
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief カットを追加して状態を更新する．
// @param[in] root カットの根のノード
void
MapState::update(const SbjNode* root)
{
  if ( debug ) {
    cout << endl
	 << "update(" << root->id_str() << ")" << endl;
  }

  set_selected(root);

  ++ mLutNum;

  mNextId = root->id();

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
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    const SbjNode* node = node_list[rpos];
    if ( node != root && is_selected(node) ) {
      // カットの入力よりファンイン側にはたどらない．
      node_info.mCutInputs.push_back(node);
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
    mQueue.clear();
    mQueue.push_back(root);
    mQmark.clear();
    mQmark.resize(mMaxNodeId, false);
    mQmark[root->id()] = true;
    get_cut_candidates();
  }
}

void
MapState::dfs(const SbjNode* node,
	      vector<const SbjNode*>& node_list)
{
  if ( is_marked(node) ) {
    return;
  }
  set_mark(node);

  if ( is_selected(node) ) {
    node_list.push_back(node);
  }
  else {
    dfs(node->fanin(0), node_list);
    dfs(node->fanin(1), node_list);
  }
}

// @brief block されているか調べる．
// @return block されていたら true を返す．
bool
MapState::check_blocked(const SbjNode* node)
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

// @brief マッピング結果を BnNetwork にセットする．
void
MapState::gen_mapgraph(BnBuilder& mapgraph,
		       ymuint& lut_num,
		       ymuint& depth)
{
  mapgraph.clear();

  mapgraph.set_model_name(mSbjGraph.name());

  // 外部入力の生成
  ymuint ni = mSbjGraph.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* node = mSbjGraph.input(i);
    ymuint map_id = node->id() * 2;
    ymuint node_id = mapgraph.add_input(string());
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.mMapNode = node_id;
    node_info.mDepth = 0;
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  ymuint no = mSbjGraph.output_num();
  int max_depth = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const SbjNode* onode = mSbjGraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    ymuint node_id;
    if ( node ) {
      ymuint node1_id = back_trace(node, mapgraph);
      int depth1 = mNodeInfo[node->id()].mDepth;
      if ( inv ) {
	// NOT ゲート用のテーブルを挿入する．
	TvFunc tv = TvFunc::nega_literal(1, VarId(0));
	node_id = mapgraph.add_tv(string(), tv);
	mapgraph.connect(node1_id, node_id, 0);
	++ depth1;
      }
      else {
	node_id = node1_id;
      }
      if ( max_depth < depth1 ) {
	max_depth = depth1;
      }
    }
    else {
      TvFunc tv;
      if ( inv ) {
	tv = TvFunc::const_one(0);
      }
      else {
	tv = TvFunc::const_zero(0);
      }
      node_id = mapgraph.add_tv(string(), tv);
    }
    ymuint onode_id = mapgraph.add_output(string(), node_id);
    mNodeInfo[onode->id()].mMapNode = onode_id;
  }

  // DFFの生成
  ymuint nf = mSbjGraph.dff_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const SbjDff* dff = mSbjGraph.dff(i);
    BnBuilder::DffInfo& dff_info = mapgraph.add_dff(string());

    const SbjNode* input = dff->data_input();
    dff_info.mInput = mNodeInfo[input->id()].mMapNode;

    const SbjNode* output = dff->data_output();
    dff_info.mOutput = mNodeInfo[output->id()].mMapNode;

    const SbjNode* clock = dff->clock();
    dff_info.mClock = mNodeInfo[clock->id()].mMapNode;

    const SbjNode* clear = dff->clear();
    if ( clear != nullptr ) {
      dff_info.mClear = mNodeInfo[clear->id()].mMapNode;
    }

    const SbjNode* preset = dff->preset();
    if ( preset != nullptr ) {
      dff_info.mPreset = mNodeInfo[preset->id()].mMapNode;
    }
  }

  // ラッチの生成
  ymuint nlatch = mSbjGraph.latch_num();
  for (ymuint i = 0; i < nlatch; ++ i) {
    const SbjLatch* latch = mSbjGraph.latch(i);
    BnBuilder::LatchInfo& latch_info = mapgraph.add_latch(string());

    const SbjNode* input = latch->data_input();
    latch_info.mInput = mNodeInfo[input->id()].mMapNode;

    const SbjNode* output = latch->data_output();
    latch_info.mOutput = mNodeInfo[output->id()].mMapNode;

    const SbjNode* enable = latch->enable();
    latch_info.mEnable = mNodeInfo[enable->id()].mMapNode;

    const SbjNode* clear = latch->clear();
    if ( clear != nullptr ) {
      latch_info.mClear = mNodeInfo[clear->id()].mMapNode;
    }

    const SbjNode* preset = latch->preset();
    if ( preset != nullptr ) {
      latch_info.mPreset = mNodeInfo[preset->id()].mMapNode;
    }
  }

  // ポートの生成
  ymuint np = mSbjGraph.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const SbjPort* sbjport = mSbjGraph.port(i);
    ymuint nb = sbjport->bit_width();
    vector<ymuint> tmp(nb);
    for (ymuint j = 0; j < nb; ++ j) {
      const SbjNode* sbjnode = sbjport->bit(j);
      tmp[j] = mNodeInfo[sbjnode->id()].mMapNode;
    }
    mapgraph.add_port(sbjport->name(), tmp);
  }

  bool stat = mapgraph.wrap_up();
  ASSERT_COND( stat );

  lut_num = mapgraph.logic_num();
  depth = max_depth;
}

BEGIN_NONAMESPACE

// valmap に終端のノード番号をキーとしてビットベクタ値を登録する．
// その時の node の値を計算する．
ymuint64
eval_node(const SbjNode* node,
	  HashMap<ymuint, ymuint64>& valmap)
{
  if ( node == nullptr ) {
    return 0ULL;
  }

  // まずすでに評価済みかどうか調べる．
  // 葉のノードの場合もここに登録されている．
  ymuint64 ans;
  if ( !valmap.find(node->id(), ans) ) {
    // 未登録の場合は必ず論理ノード
    ASSERT_COND( node->is_logic() );

    // 子供の値を評価する．
    ymuint64 val0 = eval_node(node->fanin(0), valmap);
    if ( node->fanin_inv(0) ) {
      val0 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    ymuint64 val1 = eval_node(node->fanin(1), valmap);
    if ( node->fanin_inv(0) ) {
      val1 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    // 自分の値を計算する．
    if ( node->is_xor() ) {
      ans = val0 ^ val1;
    }
    else {
      ans = val0 & val1;
    }

    // 登録しておく．
    valmap.add(node->id(), ans);
  }

  return ans;
}

// カットの表している論理関数を評価する．
ymuint64
eval_cut(const SbjNode* root,
	 const vector<const SbjNode*>& cut_inputs,
	 const vector<ymuint64>& vals)
{
  ymuint ni = cut_inputs.size();
  ASSERT_COND( ni == vals.size() );
  HashMap<ymuint, ymuint64> valmap;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut_inputs[i];
    valmap.add(inode->id(), vals[i]);
  }
  return eval_node(root, valmap);
}

// @brief 論理関数を表す真理値表を得る．
TvFunc
make_tv(const SbjNode* root,
	const vector<const SbjNode*>& cut_inputs)
{
  ymuint ni = cut_inputs.size();
  ymuint np = 1 << ni;

  vector<int> tv(np);

  // 1 の値と 0 の値
  int v1 = 1;
  int v0 = 0;

  // 真理値表の各変数の値を表すビットベクタ
  // 6入力以上の場合には1語に収まらないので複数回にわけて処理する．
  vector<ymuint64> vals(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vals[i] = 0ULL;
  }

  ymuint64 s = 1ULL;
  ymuint p0 = 0;
  for (ymuint p = 0; p < np; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( p & (1U << i) ) {
	vals[i] |= s;
      }
    }
    s <<= 1;
    if ( s == 0ULL ) {
      // 64 パタン目
      ymuint64 tmp = eval_cut(root, cut_inputs, vals);
      for (ymuint p1 = p0; p1 < p; ++ p1) {
	if ( tmp & (1ULL << (p1 - p0)) ) {
	  tv[p1] = v1;
	}
	else {
	  tv[p1] = v0;
	}
      }
      s = 1ULL;
      p0 = p + 1;
      for (ymuint i = 0; i < ni; ++ i) {
	vals[i] = 0ULL;
      }
    }
  }
  if ( s != 1ULL ) {
    // 処理されていない残りがあった．
    ymuint64 tmp = eval_cut(root, cut_inputs, vals);
    for (ymuint p1 = p0; p1 < np; ++ p1) {
      if ( tmp & (1ULL << (p1 - p0)) ) {
	tv[p1] = v1;
      }
      else {
	tv[p1] = v0;
      }
    }
  }

  return TvFunc(ni, tv);
}

END_NONAMESPACE

// サブジェクトグラフの node に対応するマップされたノードを
// 生成し，それを返す．
ymuint
MapState::back_trace(const SbjNode* node,
		     BnBuilder& mapnetwork)
{
  NodeInfo& node_info = mNodeInfo[node->id()];
  ymuint node_id = node_info.mMapNode;
  if ( node_id ) {
    // すでに生成済みならそのノードを返す．
    return node_id;
  }

  ASSERT_COND( !node->is_input() );

  // node を根とするカットを取り出す．
  const vector<const SbjNode*>& cut_inputs = node_info.mCutInputs;

  // その入力に対応するノードを再帰的に生成する．
  ymuint ni = cut_inputs.size();
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut_inputs[i];
    back_trace(inode, mapnetwork);
  }

  // カットの実現している関数の真理値表を得る．
  TvFunc tv = make_tv(node, cut_inputs);

  // 新しいノードを作り mNodeMap に登録する．
  node_id = mapnetwork.add_tv(string(), tv);
  int idepth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    const SbjNode* inode = cut_inputs[i];
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    ymuint src_id = inode_info.mMapNode;
    mapnetwork.connect(src_id, node_id, i);
    int idepth1 = inode_info.mDepth;
    if ( idepth < idepth1 ) {
      idepth = idepth1;
    }
  }
  node_info.mMapNode = node_id;
  node_info.mDepth = idepth + 1;

  return node_id;
}

// @brief 'blocked' 状態をチェックする．
void
MapState::check_cover()
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
MapState::check_cover_node(const SbjNode* node)
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

END_NAMESPACE_YM_LUTMAP2
