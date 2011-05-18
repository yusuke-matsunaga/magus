
/// @file libym_networks/BdnMgr.cc
/// @brief BdnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BdnMgrImpl.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnDff.h"
#include "ym_networks/BdnLatch.h"
#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnNodeHandle.h"
#include "BdnAuxData.h"


BEGIN_NAMESPACE_YM_BDN

///////////////////////////////////////////////////////////////////////
// クラス BdnMgrImpl
///////////////////////////////////////////////////////////////////////

// コンストラクタ
BdnMgrImpl::BdnMgrImpl() :
  mAlloc(4096),
  mHashTable(NULL),
  mHashSize(0),
  mLevel(0U)
{
  alloc_table(1024);
}

// デストラクタ
BdnMgrImpl::~BdnMgrImpl()
{
  clear();
  delete [] mHashTable;
}

// 複製する．
void
BdnMgrImpl::copy(const BdnMgr& src)
{
  ymuint n = src.max_node_id();
  vector<BdnNode*> nodemap(n);

  // ポートの生成
  ymuint np = src.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BdnPort* src_port = src.port(i);
    ymuint bw = src_port->bit_width();
    BdnPort* dst_port = new_port(src_port->name(), bw);
    for (ymuint j = 0; j < bw; ++ j) {
      const BdnNode* src_input = src_port->input(j);
      if ( src_input ) {
	BdnNode* dst_input = new_port_input(dst_port, j);
	nodemap[src_input->id()] = dst_input;
      }
      const BdnNode* src_output = src_port->output(j);
      if ( src_output ) {
	BdnNode* dst_output = new_port_output(dst_port, j);
	nodemap[src_output->id()] = dst_output;
      }
    }
  }

  // D-FFノードの生成
  const BdnDffList& src_dff_list = src.dff_list();
  for (BdnDffList::const_iterator p = src_dff_list.begin();
       p != src_dff_list.end(); ++ p) {
    const BdnDff* src_dff = *p;
    BdnDff* dst_dff = new_dff(src_dff->name());

    const BdnNode* src_output = src_dff->output();
    BdnNode* dst_output = dst_dff->output();
    nodemap[src_output->id()] = dst_output;

    const BdnNode* src_input = src_dff->input();
    BdnNode* dst_input = dst_dff->input();
    nodemap[src_input->id()] = dst_input;

    const BdnNode* src_clock = src_dff->clock();
    BdnNode* dst_clock = dst_dff->clock();
    nodemap[src_clock->id()] = dst_clock;

    const BdnNode* src_set = src_dff->set();
    BdnNode* dst_set = dst_dff->set();
    nodemap[src_set->id()] = dst_set;

    const BdnNode* src_reset = src_dff->reset();
    BdnNode* dst_reset = dst_dff->reset();
    nodemap[src_reset->id()] = dst_reset;
  }

  // ラッチノードの生成
  const BdnLatchList& src_latch_list = src.latch_list();
  for (BdnLatchList::const_iterator p = src_latch_list.begin();
       p != src_latch_list.end(); ++ p) {
    const BdnLatch* src_latch = *p;
    BdnLatch* dst_latch = new_latch(src_latch->name());

    const BdnNode* src_output = src_latch->output();
    BdnNode* dst_output = dst_latch->output();
    nodemap[src_output->id()] = dst_output;

    const BdnNode* src_input = src_latch->input();
    BdnNode* dst_input = dst_latch->input();
    nodemap[src_input->id()] = dst_input;

    const BdnNode* src_enable = src_latch->enable();
    BdnNode* dst_enable = dst_latch->enable();
    nodemap[src_enable->id()] = dst_enable;
  }

  // 論理ノードの生成
  vector<BdnNode*> node_list;
  src.sort(node_list);
  ymuint nl = node_list.size();
  for (ymuint i = 0; i < nl; ++ i) {
    BdnNode* src_node = node_list[i];

    BdnNode* src_inode0 = src_node->fanin0();
    BdnNode* input0 = nodemap[src_inode0->id()];
    assert_cond(input0, __FILE__, __LINE__);

    BdnNode* src_inode1 = src_node->fanin1();
    BdnNode* input1 = nodemap[src_inode1->id()];
    assert_cond(input1, __FILE__, __LINE__);

    BdnNodeHandle tmp_h = set_logic(NULL, src_node->_fcode(),
				    BdnNodeHandle(input0, false),
				    BdnNodeHandle(input1, false));
    assert_cond(tmp_h.inv() == false, __FILE__, __LINE__);
    BdnNode* dst_node = tmp_h.node();
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力ノードのファンインの設定
  const BdnNodeList& src_output_list = src.output_list();
  for (BdnNodeList::const_iterator p = src_output_list.begin();
       p != src_output_list.end(); ++ p) {
    const BdnNode* src_onode = *p;
    const BdnNode* src_inode = src_onode->output_fanin();
    BdnNode* dst_inode = NULL;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    BdnNodeHandle dst_handle(dst_inode, src_onode->output_fanin_inv());
    BdnNode* dst_onode = nodemap[src_onode->id()];
    change_output_fanin(dst_onode, dst_handle);
  }
}

// 空にする．
void
BdnMgrImpl::clear()
{
  mName = string();
  mPortArray.clear();
  mDffItvlMgr.clear();
  mDffList.clear();
  mLatchItvlMgr.clear();
  mLatchList.clear();
  mNodeItvlMgr.clear();
  mInputList.clear();
  mOutputList.clear();
  mLnodeList.clear();
  mLevel = 0U;

  // mNodeArray, mDffArray, mLatchArray はクリアしない．
}

BEGIN_NONAMESPACE

// sort() の下請け関数
// node のファンアウトのうち，すべてのファンインがマークされている
// ノードを node_list に追加する．
void
sort_sub(BdnNode* node,
	 vector<bool>& marks,
	 vector<BdnNode*>& node_list)
{
  const BdnFanoutList& fo_list = node->fanout_list();
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* e = *p;
    BdnNode* onode = e->to();
    if ( !marks[onode->id()] && onode->is_logic() &&
	 marks[onode->fanin0()->id()] &&
	 marks[onode->fanin1()->id()] ) {
      marks[onode->id()] = true;
      node_list.push_back(onode);
    }
  }
}

END_NONAMESPACE

// @brief ソートされたノードのリストを得る．
void
BdnMgrImpl::sort(vector<BdnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  // 処理済みの印を表す配列
  ymuint n = max_node_id();
  vector<bool> marks(n, false);

  // 外部入力のみをファンインにするノードを node_list に追加する．
  for (BdnNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    BdnNode* node = *p;
    marks[node->id()] = true;
    sort_sub(node, marks, node_list);
  }
  // node_list のノードを一つずつとりだし，ファンアウトが node_list
  // に積めるかチェックする．
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    BdnNode* node = node_list[rpos];
    sort_sub(node, marks, node_list);
  }
  // うまくいっていれば全ての論理ノードが node_list に入っているはず．
  assert_cond(node_list.size() == lnode_num(), __FILE__, __LINE__);
}


BEGIN_NONAMESPACE

// rsort() の下請け関数
// node のすべてのファンアウトがマークされていたら node_list に積む．
void
rsort_sub(BdnNode* node,
	  vector<bool>& marks,
	  vector<BdnNode*>& node_list)
{
  if ( node == NULL ||
       !node->is_logic() ||
       marks[node->id()] ) return;

  const BdnFanoutList& fo_list = node->fanout_list();
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* e = *p;
    BdnNode* onode = e->to();
    if ( !marks[onode->id()] ) {
      return;
    }
  }
  marks[node->id()] = true;
  node_list.push_back(node);
}

END_NONAMESPACE

// @brief 逆順でソートされたノードのリストを得る．
void
BdnMgrImpl::rsort(vector<BdnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(lnode_num());

  // 処理済みの印をあらわす配列
  ymuint n = max_node_id();
  vector<bool> marks(n, false);

  // 外部出力のみをファンアウトにするノードを node_list に追加する．
  for (BdnNodeList::const_iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    BdnNode* node = *p;
    marks[node->id()] = true;
    BdnNode* inode = node->output_fanin();
    rsort_sub(inode, marks, node_list);
  }

  // node_list からノードを取り出し，同様の処理を行う．
  for (ymuint rpos = 0; rpos < node_list.size(); ++ rpos) {
    BdnNode* node = node_list[rpos];
    rsort_sub(node->fanin0(), marks, node_list);
    rsort_sub(node->fanin1(), marks, node_list);
  }
  // うまくいっていればすべての論理ノードが node_list に入っているはず．
  assert_cond(node_list.size() == lnode_num(), __FILE__, __LINE__);
}

// @brief ポートを作る．
BdnPort*
BdnMgrImpl::new_port(const string& name,
		 ymuint bit_width)
{
  void* p = mAlloc.get_memory(sizeof(BdnPort));
  BdnPort* port = new (p) BdnPort();

  port->mId = mPortArray.size();
  mPortArray.push_back(port);

  port->mName = name;

  port->mBitWidth = bit_width;

  void* q = mAlloc.get_memory(sizeof(BdnNode*) * bit_width);
  port->mInputArray = new (q) BdnNode*[bit_width];

  void* r = mAlloc.get_memory(sizeof(BdnNode*) * bit_width);
  port->mOutputArray = new (r) BdnNode*[bit_width];

  void* s = mAlloc.get_memory(sizeof(BdnAuxData*) * bit_width);
  port->mAuxDataArray = new (s) BdnAuxData*[bit_width];

  for (ymuint i = 0; i < bit_width; ++ i) {
    port->mInputArray[i] = NULL;
    port->mOutputArray[i] = NULL;
    void* t = mAlloc.get_memory(sizeof(BdnPortData));
    port->mAuxDataArray[i] = new (t) BdnPortData(port, i);
  }

  return port;
}

// @brief D-FF を作る．
// @param[in] name 名前
// @return 生成されたD-FFを返す．
BdnDff*
BdnMgrImpl::new_dff(const string& name)
{
  // 空いているIDを探して配列へ登録
  int id = mDffItvlMgr.avail_num();
  assert_cond( id >= 0, __FILE__, __LINE__);
  mDffItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  if ( mDffArray.size() == uid ) {
    void* p = mAlloc.get_memory(sizeof(BdnDff));
    BdnDff* dff = new (p) BdnDff;
    dff->mId = uid;
    void* q = mAlloc.get_memory(sizeof(BdnDffData));
    BdnDffData* data = new (q) BdnDffData(dff);
    dff->mAuxData = data;
    mDffArray.push_back(dff);
  }
  else {
    assert_cond( mDffArray.size() > uid, __FILE__, __LINE__);
  }

  BdnDff* dff = mDffArray[uid];

  dff->mName = name;

  BdnNode* output = alloc_node();
  dff->mOutput = output;
  output->set_input_type(BdnNode::kDFF_OUTPUT);
  output->mAuxData = dff->mAuxData;
  mInputList.push_back(output);

  BdnNode* input = alloc_node();
  dff->mInput = input;
  input->set_output_type(BdnNode::kDFF_DATA);
  input->mAuxData = dff->mAuxData;
  mOutputList.push_back(input);

  BdnNode* clock = alloc_node();
  dff->mClock = clock;
  clock->set_output_type(BdnNode::kDFF_CLOCK);
  clock->mAuxData = dff->mAuxData;
  mOutputList.push_back(clock);

  BdnNode* set = alloc_node();
  dff->mSet = set;
  set->set_output_type(BdnNode::kDFF_SET);
  set->mAuxData = dff->mAuxData;
  mOutputList.push_back(set);

  BdnNode* reset = alloc_node();
  dff->mReset = reset;
  reset->set_output_type(BdnNode::kDFF_RESET);
  reset->mAuxData = dff->mAuxData;
  mOutputList.push_back(reset);

  mDffList.push_back(dff);

  return dff;
}

// @brief D-FF を削除する．
// @param[in] dff 削除対象の D-FF
void
BdnMgrImpl::delete_dff(BdnDff* dff)
{
  // new_dff() と逆の処理を行う．
  mDffItvlMgr.add(static_cast<int>(dff->id()));

  mDffList.erase(dff);

  delete_node(dff->output());
  delete_node(dff->input());
  delete_node(dff->clock());
  if ( dff->set() ) {
    delete_node(dff->set());
  }
  if ( dff->reset() ) {
    delete_node(dff->reset());
  }
}

// @brief ラッチを作る．
// @param[in] name 名前
// @return 生成されたラッチを返す．
BdnLatch*
BdnMgrImpl::new_latch(const string& name)
{
  // 空いているIDを探して配列へ登録する．
  int id = mLatchItvlMgr.avail_num();
  assert_cond( id >= 0, __FILE__, __LINE__);
  mLatchItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  if ( mLatchArray.size() == uid ) {
    void* p = mAlloc.get_memory(sizeof(BdnLatch));
    BdnLatch* latch = new (p) BdnLatch;
    latch->mId = uid;
    void* q = mAlloc.get_memory(sizeof(BdnLatchData));
    BdnAuxData* data = new (q) BdnLatchData(latch);
    latch->mAuxData = data;
    mLatchArray.push_back(latch);
  }
  else {
    assert_cond( mLatchArray.size() > uid, __FILE__, __LINE__);
  }
  BdnLatch* latch = mLatchArray[uid];

  latch->mName = name;

  BdnNode* output = alloc_node();
  latch->mOutput = output;
  output->set_input_type(BdnNode::kLATCH_OUTPUT);
  output->mAuxData = latch->mAuxData;
  mInputList.push_back(output);

  BdnNode* input = alloc_node();
  latch->mInput = input;
  input->set_output_type(BdnNode::kLATCH_DATA);
  input->mAuxData = latch->mAuxData;
  mOutputList.push_back(input);

  BdnNode* enable = alloc_node();
  latch->mEnable = enable;
  enable->set_output_type(BdnNode::kLATCH_ENABLE);
  enable->mAuxData = latch->mAuxData;
  mOutputList.push_back(enable);

  mLatchList.push_back(latch);

  return latch;
}

// @brief ラッチを削除する．
// @param[in] latch 削除対象のラッチ
void
BdnMgrImpl::delete_latch(BdnLatch* latch)
{
  // new_latch() と逆の処理を行う．
  mLatchItvlMgr.add(static_cast<int>(latch->id()));

  mLatchList.erase(latch);

  delete_node(latch->output());
  delete_node(latch->input());
  delete_node(latch->enable());
}

// @brief 外部入力を作る．
// @param[in] port ポート
// @param[in] bitpos ビット位置
// @return 作成したノードを返す．
// @note エラー条件は以下の通り
//  - bitpos が port のビット幅を越えている．
//  - port の bitpos にすでにノードがある．
BdnNode*
BdnMgrImpl::new_port_input(BdnPort* port,
			   ymuint bitpos)
{
  assert_cond( bitpos < port->bit_width(), __FILE__, __LINE__);
  assert_cond( port->mInputArray[bitpos] == NULL, __FILE__, __LINE__);

  BdnNode* node = alloc_node();
  node->set_input_type(BdnNode::kPRIMARY_INPUT);
  node->mAuxData = port->mAuxDataArray[bitpos];
  port->mInputArray[bitpos] = node;

  mInputList.push_back(node);

  return node;
}

// @brief 外部出力ノードを作る．
// @param[in] port ポート
// @param[in] bitpos ビット位置
// @return 作成したノードを返す．
// @note エラー条件は以下の通り
//  - bitpos が port のビット幅を越えている．
//  - port の bitpos にすでにノードがある．
BdnNode*
BdnMgrImpl::new_port_output(BdnPort* port,
			    ymuint bitpos)
{
  assert_cond( bitpos < port->bit_width(), __FILE__, __LINE__);
  assert_cond( port->mOutputArray[bitpos] == NULL, __FILE__, __LINE__);

  BdnNode* node = alloc_node();
  node->set_output_type(BdnNode::kPRIMARY_OUTPUT);
  node->mAuxData = port->mAuxDataArray[bitpos];
  port->mOutputArray[bitpos] = node;

  mOutputList.push_back(node);

  return node;
}

// @brief 論理ノードの内容を変更する．
// @param[in] node 変更対象の論理ノード
// @param[in] new_handle 設定する新しいハンドル
// @note node のファンアウト先の情報を書き換える．
void
BdnMgrImpl::change_logic(BdnNode* node ,
			 BdnNodeHandle new_handle)
{
  if ( new_handle == BdnNodeHandle(node, false) ) {
    // 変化なし
    return;
  }

  const BdnFanoutList& fo_list = node->fanout_list();
  // ループ中で接続が変わる可能性があるので BdnEdge のリストをコピーする．
  vector<BdnEdge*> tmp_list;
  tmp_list.reserve(fo_list.size());
  for (BdnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    BdnEdge* edge = *p;
    tmp_list.push_back(edge);
  }

  // node のファンアウト先の内容を変える．
  for (vector<BdnEdge*>::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    BdnEdge* edge = *p;
    BdnNode* onode = edge->to();
    if ( onode->is_logic() ) {
      BdnNodeHandle inode1_handle(onode->fanin0());
      BdnNodeHandle inode2_handle(onode->fanin1());
      if ( edge->pos() == 0 ) {
	inode1_handle = new_handle;
      }
      else {
	inode2_handle = new_handle;
      }
      BdnNodeHandle new_oh = set_logic(onode, onode->_fcode(),
				       inode1_handle, inode2_handle);
      change_logic(onode, new_oh);
    }
    else if ( onode->is_output() ) {
      change_output_fanin(onode, new_handle);
    }
  }
}

// @brief 出力ノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] inode 入力のノード
void
BdnMgrImpl::change_output_fanin(BdnNode* node,
				BdnNodeHandle inode_handle)
{
  if ( node->is_input() ) {
    // BdnNode::alt_node() と同じコードだが const がつかない．
    const BdnPort* port = node->port();
    assert_cond( port != NULL, __FILE__, __LINE__);
    node = port->mOutputArray[node->port_bitpos()];
    assert_cond( node != NULL, __FILE__, __LINE__);
  }
  assert_cond( node->is_output(), __FILE__, __LINE__);

  bool inv = inode_handle.inv();
  node->set_output_fanin_inv(inv);

  BdnNode* inode = inode_handle.node();
  connect(inode, node, 0);
}


BEGIN_NONAMESPACE

// BdnNode のハッシュ関数
inline
ymuint
hash_func(ymuint fcode,
	  const BdnNode* node1,
	  const BdnNode* node2)
{
  return (node1->id() * 3 + node2->id() << 3) | fcode;
}

END_NONAMESPACE


// 論理ノードの内容を設定する．
BdnNodeHandle
BdnMgrImpl::set_logic(BdnNode* node,
		      ymuint fcode,
		      BdnNodeHandle inode0_handle,
		      BdnNodeHandle inode1_handle)
{
  // 境界条件の検査
  if ( fcode & 4U ) {
    // XOR の場合
    if ( inode0_handle.is_zero() ) {
      // 入力0が定数0だった．
      return inode1_handle;
    }
    else if ( inode0_handle.is_one() ) {
      // 入力0が定数1だった．
      return ~inode1_handle;
    }
    else if ( inode1_handle.is_zero() ) {
      // 入力1が定数0だった．
      return inode0_handle;
    }
    else if ( inode1_handle.is_one() ) {
      // 入力1が定数1だった．
      return ~inode0_handle;
    }
    else if ( inode0_handle == inode1_handle ) {
      // 2つの入力が同一だった．
      return inode0_handle;
    }
    else if ( inode0_handle == ~inode1_handle ) {
      // 2つの入力が極性違いだった．
      return BdnNodeHandle::make_zero();
    }
  }
  else {
    // AND の場合
    if ( inode0_handle.is_zero() ) {
      // 入力0が定数1だった．
      return inode1_handle;
    }
    else if ( inode0_handle.is_one() ) {
      // 入力0が定数0だった．
      return BdnNodeHandle::make_zero();
    }
    else if ( inode1_handle.is_zero() ) {
      // 入力1が定数1だった．
      return inode0_handle;
    }
    else if ( inode1_handle.is_one() ) {
      // 入力1が定数0だった．
      return BdnNodeHandle::make_zero();
    }
    else if ( inode0_handle == inode1_handle ) {
      // 2つの入力が同じだった．
      return inode0_handle;
    }
    else if ( inode0_handle == ~inode1_handle ) {
      // 2つの入力が極性違いだった．
      return BdnNodeHandle::make_zero();
    }
  }

  // 入力の反転属性
  bool inv0 = inode0_handle.inv() ^ static_cast<bool>(fcode & 1U);
  bool inv1 = inode1_handle.inv() ^ static_cast<bool>((fcode >> 1) & 1U);

  // 入力のノード
  BdnNode* inode0 = inode0_handle.node();
  BdnNode* inode1 = inode1_handle.node();

  // ファンインの順番の正規化
  if ( inode0->id() > inode1->id() ) {
    BdnNode* tmp = inode0;
    inode0 = inode1;
    inode1 = tmp;
    bool tmp_inv = inv0;
    inv0 = inv1;
    inv1 = tmp_inv;
  }

  // 出力の反転属性
  bool oinv = static_cast<bool>((fcode >> 3) & 1U);

  // fcode の正規化
  if ( fcode & 4U ) {
    // XOR の場合には入力に反転属性はつかない．
    oinv = inv0 ^ inv1;
    fcode = 4U;
  }
  else {
    fcode = static_cast<ymuint>(inv0) | (static_cast<ymuint>(inv1) << 1);
  }

  // 同じ構造を持つノードが既にないか調べる．
  ymuint pos = hash_func(fcode, inode0, inode1);
  ymuint idx = pos % mHashSize;
  for (BdnNode* node1 = mHashTable[idx]; node1; node1 = node1->mLink) {
    if ( node1->_fcode() == fcode &&
	 node1->fanin0() == inode0 &&
	 node1->fanin1() == inode1 ) {
      // 同じノードがあった．
      return BdnNodeHandle(node1, oinv);
    }
  }

  if ( node == NULL ) {
    // 新しいノードを作る．
    node = alloc_node();

    // 論理ノードリストに登録
    mLnodeList.push_back(node);

    if ( lnode_num() >= mNextLimit ) {
      alloc_table(mHashSize * 2);
      // サイズが変わったのでインデックスを再計算する．
      idx = pos % mHashSize;
    }
  }
  else {
    // ハッシュ表から取り除く
    ymuint pos0 = hash_func(node->_fcode(), node->fanin0(), node->fanin1());
    ymuint idx0 = pos0 % mHashSize;
    BdnNode* prev = mHashTable[idx0];
    if ( prev == node ) {
      mHashTable[idx0] = node->mLink;
    }
    for (BdnNode* node0 = 0; (node0 = prev->mLink); prev = node0) {
      if ( node0 == node ) {
	prev->mLink = node->mLink;
	break;
      }
    }
    // エラーチェック(node0 == NULL) はしていない．
  }

  node->set_logic_type(fcode);
  connect(inode0, node, 0);
  connect(inode1, node, 1);

  // ハッシュ表に登録する．
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;

  return BdnNodeHandle(node, oinv);
}

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
BdnMgrImpl::connect(BdnNode* from,
		    BdnNode* to,
		    ymuint pos)
{
  // BdnNode::mFaoutList を変更するのはここだけ

  BdnEdge* edge = &(to->mFanins[pos]);
  BdnNode* old_from = edge->from();
  if ( old_from ) {
    old_from->mFanoutList.erase(edge);
    old_from->scan_po();
  }
  edge->set_from(from);
  if ( from ) {
    from->mFanoutList.push_back(edge);
    from->scan_po();
  }

  // 構造が変わったのでレベルの情報は無効化される．
  mLevel = 0U;
}

// @brief ノードを作成する．
// @return 作成されたノードを返す．
BdnNode*
BdnMgrImpl::alloc_node()
{
  // 空いているIDを探してノード配列へ登録
  int id = mNodeItvlMgr.avail_num();
  assert_cond( id >= 0, __FILE__, __LINE__);
  mNodeItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  if ( mNodeArray.size() == uid ) {
    void* p = mAlloc.get_memory(sizeof(BdnNode));
    BdnNode* node = new (p) BdnNode();
    node->mId = uid;
    mNodeArray.push_back(node);
  }
  else {
    assert_cond( mNodeArray.size() > uid, __FILE__, __LINE__);
  }
  BdnNode* node = mNodeArray[uid];

  node->mFlags = 0U;
  node->mAuxData = NULL;

  return node;
}

// node を削除する．
void
BdnMgrImpl::delete_node(BdnNode* node)
{
  // alloc_node の逆の処理を行なう．
  mNodeItvlMgr.add(static_cast<int>(node->id()));

  if ( node->is_logic() ) {
    mLnodeList.erase(node);
  }
  else if ( node->is_input() ) {
    mInputList.erase(node);
  }
  else if ( node->is_output() ) {
    mOutputList.erase(node);
  }

  // mNodeArray 内のエントリはクリアしない．
  // id の再利用と同様に BdnNode も再利用する．
}

// @brief 段数を求める．
ymuint
BdnMgrImpl::level() const
{
  if ( (mLevel & 1U) == 0U ) {
    for (BdnNodeList::const_iterator p = mInputList.begin();
	 p != mInputList.end(); ++ p) {
      BdnNode* node = *p;
      node->mLevel = 0U;
    }

    vector<BdnNode*> node_list;
    sort(node_list);
    for (vector<BdnNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      BdnNode* node = *p;
      BdnNode* inode0 = node->fanin0();
      ymuint l = inode0->mLevel;
      BdnNode* inode1 = node->fanin1();
      ymuint l1 = inode1->mLevel;
      if ( l < l1 ) {
	l = l1;
      }
      node->mLevel = l + 1;
    }

    ymuint max_l = 0;
    for (BdnNodeList::const_iterator p = mOutputList.begin();
	 p != mOutputList.end(); ++ p) {
      BdnNode* node = *p;
      BdnNode* inode = node->fanin0();
      if ( inode ) {
	ymuint l1 = inode->mLevel;
	if ( max_l < l1 ) {
	  max_l = l1;
	}
      }
    }
    mLevel = (max_l << 1) | 1U;
  }
  return (mLevel >> 1);
}

// @brief ハッシュ表を確保する．
void
BdnMgrImpl::alloc_table(ymuint req_size)
{
  BdnNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new BdnNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      BdnNode* next = NULL;
      for (BdnNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->_fcode(), node->fanin0(), node->fanin1());
	ymuint idx = pos % mHashSize;
	node->mLink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}

END_NAMESPACE_YM_BDN
