
/// @file libym_networks/cmn/CmnMgrImpl.cc
/// @brief CmnMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CmnMgrImpl.h"

#include "ym_networks/CmnPort.h"
#include "ym_networks/CmnDff.h"
#include "ym_networks/CmnLatch.h"
#include "ym_networks/CmnNode.h"
#include "ym_networks/CmnEdge.h"
#include "ym_networks/CmnDffCell.h"
#include "ym_networks/CmnLatchCell.h"

#include "CmnNodePI.h"
#include "CmnNodeDffOut.h"
#include "CmnNodeLatchOut.h"
#include "CmnNodePO.h"
#include "CmnNodeDffIn.h"
#include "CmnNodeLatchIn.h"
#include "CmnNodeLogic.h"


BEGIN_NAMESPACE_YM_CMN

///////////////////////////////////////////////////////////////////////
// クラス CmnMgrImpl
///////////////////////////////////////////////////////////////////////

// コンストラクタ
CmnMgrImpl::CmnMgrImpl() :
  mAlloc(4096)
{
}

// デストラクタ
CmnMgrImpl::~CmnMgrImpl()
{
  clear();
}

// 複製する．
void
CmnMgrImpl::copy(const CmnMgrImpl& src)
{
  clear();

  // 名前の設定
  set_name(src.name());

  ymuint max_n = src.max_node_id();
  vector<CmnNode*> nodemap(max_n);

  // ポートの生成
  ymuint np = src.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const CmnPort* src_port = src.port(i);
    ymuint nb = src_port->bit_width();
    vector<ymuint> iovect(nb);
    for (ymuint b = 0; b < nb; ++ b) {
      ymuint pat = 0U;
      const CmnNode* src_input = src_port->input(b);
      if ( src_input ) {
	pat |= 1U;
      }
      const CmnNode* src_output = src_port->output(b);
      if ( src_output ) {
	pat |= 2U;
      }
      iovect[b] = pat;
    }
    CmnPort* port = new_port(src_port->name(), iovect);
    assert_cond( port->id() == i, __FILE__, __LINE__);

    for (ymuint i = 0; i < nb; ++ i) {
      const CmnNode* src_input = src_port->input(i);
      if ( src_input ) {
	nodemap[src_input->id()] = port->mInputArray[i];
      }
      const CmnNode* src_output = src_port->output(i);
      if ( src_output ) {
	nodemap[src_output->id()] = port->mOutputArray[i];
      }
    }
  }

  // D-FF の生成
  ymuint max_d = src.max_dff_id();
  vector<CmnDff*> dffmap(max_d);
  const CmnDffList& dff_list = src.dff_list();
  for (CmnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const CmnDff* src_dff = *p;
    CmnDff* dst_dff = new_dff(src_dff->cell(), src_dff->name());
    dffmap[src_dff->id()] = dst_dff;

    const CmnNode* src_output1 = src_dff->output1();
    nodemap[src_output1->id()] = dst_dff->mOutput1;

    const CmnNode* src_output2 = src_dff->output2();
    nodemap[src_output2->id()] = dst_dff->mOutput2;

    const CmnNode* src_input = src_dff->input();
    nodemap[src_input->id()] = dst_dff->mInput;

    const CmnNode* src_clock = src_dff->clock();
    nodemap[src_clock->id()] = dst_dff->mClock;

    const CmnNode* src_clear = src_dff->clear();
    if ( src_clear ) {
      nodemap[src_clear->id()] = dst_dff->mClear;
    }

    const CmnNode* src_preset = src_dff->preset();
    if ( src_preset ) {
      nodemap[src_preset->id()] = dst_dff->mPreset;
    }
  }

  // ラッチの生成
  ymuint max_l = src.max_latch_id();
  vector<CmnLatch*> latchmap(max_l);
  const CmnLatchList& latch_list = src.latch_list();
  for (CmnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const CmnLatch* src_latch = *p;
    CmnLatch* dst_latch = new_latch(src_latch->cell(), src_latch->name());
    latchmap[src_latch->id()] = dst_latch;

    const CmnNode* src_output1 = src_latch->output1();
    nodemap[src_output1->id()] = dst_latch->mOutput1;

    const CmnNode* src_output2 = src_latch->output2();
    nodemap[src_output2->id()] = dst_latch->mOutput2;

    const CmnNode* src_input = src_latch->input();
    nodemap[src_input->id()] = dst_latch->mInput;

    const CmnNode* src_enable = src_latch->enable();
    nodemap[src_enable->id()] = dst_latch->mEnable;

    const CmnNode* src_clear = src_latch->clear();
    if ( src_clear ) {
      nodemap[src_clear->id()] = dst_latch->mClear;
    }

    const CmnNode* src_preset = src_latch->preset();
    if ( src_preset ) {
      nodemap[src_preset->id()] = dst_latch->mPreset;
    }
  }

  // 論理ノードの生成
  vector<const CmnNode*> node_list;
  src.sort(node_list);
  ymuint nl = node_list.size();
  for (ymuint i = 0; i < nl; ++ i) {
    const CmnNode* src_node = node_list[i];
    ymuint ni = src_node->ni();
    vector<CmnNode*> dst_inputs(ni);
    for (ymuint j = 0; j < ni; ++ j) {
      const CmnNode* src_inode = src_node->fanin(j);
      CmnNode* input = nodemap[src_inode->id()];
      assert_cond(input, __FILE__, __LINE__);
      dst_inputs[j] = input;
    }
    const Cell* cell = src_node->cell();
    CmnNode* dst_node = new_logic(dst_inputs, cell);
    nodemap[src_node->id()] = dst_node;
  }

  // 出力ノードの接続
  const CmnNodeList& output_list = src.output_list();
  for (CmnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const CmnNode* src_node = *p;
    CmnNode* dst_node = nodemap[src_node->id()];
    const CmnNode* src_inode = src_node->fanin(0);
    CmnNode* dst_inode = nodemap[src_inode->id()];
    connect(dst_inode, dst_node, 0);
  }
}


BEGIN_NONAMESPACE

// node のファンアウトのうちファンインがすべてマークされているノード
// をリストに加える．
void
sort_sub(const CmnNode* node,
	 vector<bool>& mark,
	 vector<const CmnNode*>& node_list)
{
  const CmnFanoutList& fo_list = node->fanout_list();
  for (CmnFanoutList::const_iterator p = fo_list.begin();
       p != fo_list.end(); ++ p) {
    const CmnEdge* e = *p;
    const CmnNode* onode = e->to();
    if ( mark[onode->id()] || !onode->is_logic() ) continue;
    bool ready = true;
    ymuint ni = onode->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      const CmnNode* inode0 = onode->fanin(i);
      if ( !mark[inode0->id()] ) {
	ready = false;
	break;
      }
    }
    if ( ready ) {
      mark[onode->id()] = true;
      node_list.push_back(onode);
    }
  }
}

END_NONAMESPACE

// @brief ソートされたノードのリストを得る．
void
CmnMgrImpl::sort(vector<const CmnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(logic_num());

  ymuint n = max_node_id();
  vector<bool> mark(n, false);

  // 入力のみをファンインにするノードを node_list に追加する．
  for (CmnNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    const CmnNode* node = *p;
    mark[node->id()] = true;
    sort_sub(node, mark, node_list);
  }

  // 入力を持たないノード(定数ノード)を node_list に追加する．
  for (CmnNodeList::const_iterator p = mLogicList.begin();
       p != mLogicList.end(); ++ p) {
    const CmnNode* node = *p;
    if ( node->ni() == 0 && !mark[node->id()] ) {
      mark[node->id()] = true;
      node_list.push_back(node);
    }
  }

  ymuint rpos = 0;
  while ( rpos < node_list.size() ) {
    const CmnNode* node = node_list[rpos];
    ++ rpos;
    sort_sub(node, mark, node_list);
  }
  assert_cond(node_list.size() == logic_num(), __FILE__, __LINE__);
}

// 空にする．
void
CmnMgrImpl::clear()
{
  mName.clear();

  mPortArray.clear();
  mDffArray.clear();
  mDffItvlMgr.clear();
  mDffList.clear();
  mLatchArray.clear();
  mLatchItvlMgr.clear();
  mLatchList.clear();
  mNodeArray.clear();
  mNodeItvlMgr.clear();
  mInputList.clear();
  mOutputList.clear();
  mLogicList.clear();

  mAlloc.destroy();
}

// @brief D-FFセルの情報を得る．
// @param[in] cell 対象のセル
// @note cell が D-FF でない場合と登録されていない場合には NULL を返す．
const CmnDffCell*
CmnMgrImpl::dff_cell(const Cell* cell) const
{
  hash_map<const Cell*, const CmnDffCell*>::const_iterator p
    = mDffCellMap.find(cell);
  if ( p == mDffCellMap.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief ラッチセルの情報を得る．
// @param[in] cell 対象のセル
// @note cell がラッチでない場合と登録されていない場合には NULL を返す．
const CmnLatchCell*
CmnMgrImpl::latch_cell(const Cell* cell) const
{
  hash_map<const Cell*, const CmnLatchCell*>::const_iterator p
    = mLatchCellMap.find(cell);
  if ( p == mLatchCellMap.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief モジュール名を設定する．
void
CmnMgrImpl::set_name(const string& name)
{
  mName = name;
}

// @brief ポートを生成する．
// @param[in] name ポート名
// @param[in] iovect ビットごとの方向を指定する配列
// @note iovect の要素の値の意味は以下の通り
// - 0 : なし
// - 1 : 入力のみ
// - 2 : 出力のみ
// - 3 : 入力と出力
CmnPort*
CmnMgrImpl::new_port(const string& name,
		     const vector<ymuint>& iovect)
{
  ymuint bit_width = iovect.size();
  void* p = mAlloc.get_memory(sizeof(CmnPort));
  CmnPort* port = new (p) CmnPort(name, bit_width);
  port->mId = mPortArray.size();
  mPortArray.push_back(port);
  void* q = mAlloc.get_memory(sizeof(CmnNode*) * bit_width);
  port->mInputArray = new (q) CmnNode*[bit_width];
  void* r = mAlloc.get_memory(sizeof(CmnNode*) * bit_width);
  port->mOutputArray = new (r) CmnNode*[bit_width];
  for (ymuint i = 0; i < bit_width; ++ i) {
    CmnNodePI* input = NULL;
    CmnNodePO* output = NULL;
    if ( iovect[i] & 1U ) {
      void* p = mAlloc.get_memory(sizeof(CmnNodePI));
      input = new (p) CmnNodePI(port, i);
      reg_input(input);
    }
    if ( iovect[i] & 2U ) {
      void* p = mAlloc.get_memory(sizeof(CmnNodePO));
      output = new (p) CmnNodePO(port, i);
      reg_output(output);
    }
    port->mInputArray[i] = input;
    port->mOutputArray[i] = output;
  }
  return port;
}

// @brief DFFノードを作る．
// @param[in] cell セル
// @param[in] name 名前
// @return 作成したノードを返す．
CmnDff*
CmnMgrImpl::new_dff(const CmnDffCell* cell,
		    const string& name)
{
  // CmnDff の生成
  void* p = mAlloc.get_memory(sizeof(CmnDff));
  CmnDff* dff = new (p) CmnDff(name, cell);

  // CmnDff の登録
  int id = mDffItvlMgr.avail_num();
  mDffItvlMgr.erase(id);
  ymuint uid = static_cast<ymuint>(id);
  dff->mId = uid;
  if ( mDffArray.size() < uid ) {
    mDffArray.resize(uid + 1, NULL);
  }
  mDffArray[uid] = dff;

  // DFFリストに登録
  mDffList.push_back(dff);

  // 入出力ノードの生成
  p = mAlloc.get_memory(sizeof(CmnNodeDffOut1));
  CmnNode* out1 = new (p) CmnNodeDffOut1(dff);
  dff->mOutput1 = out1;
  reg_input(out1);

  p = mAlloc.get_memory(sizeof(CmnNodeDffOut2));
  CmnNode* out2 = new (p) CmnNodeDffOut2(dff);
  dff->mOutput2 = out2;
  reg_input(out2);

  p = mAlloc.get_memory(sizeof(CmnNodeDffData));
  CmnNode* input = new (p) CmnNodeDffData(dff);
  dff->mInput = input;
  reg_output(input);

  p = mAlloc.get_memory(sizeof(CmnNodeDffClock));
  CmnNode* clock = new (p) CmnNodeDffClock(dff);
  dff->mClock = clock;
  reg_output(clock);

  if ( cell->has_clear() ) {
    p = mAlloc.get_memory(sizeof(CmnNodeDffClear));
    CmnNode* clear = new (p) CmnNodeDffClear(dff);
    dff->mClear = clear;
    reg_output(clear);
  }

  if ( cell->has_preset() ) {
    p = mAlloc.get_memory(sizeof(CmnNodeDffPreset));
    CmnNode* preset = new (p) CmnNodeDffPreset(dff);
    dff->mPreset = preset;
    reg_output(preset);
  }

  return dff;
}

#if 0
// @brief D-FFを削除する．
// @param[in] dff 対象のD-FF
void
CmnMgrImpl::delete_dff(CmnDff* dff)
{
  mDffItvlMgr.add(dff->id());
  mDffArray[dff->id()] = NULL;

  mDffList.erase(dff);

  mAlloc.put_memory(sizeof(CmnDff), dff);
}
#endif

// @brief ラッチを作る．
// @param[in] cell セル
// @param[in] name 名前
// @return 作成したノードを返す．
CmnLatch*
CmnMgrImpl::new_latch(const CmnLatchCell* cell,
		      const string& name)
{
  // CmnLatch の生成
  void* p = mAlloc.get_memory(sizeof(CmnLatch));
  CmnLatch* latch = new (p) CmnLatch(name, cell);

  // CmnLatch の登録
  int id = mLatchItvlMgr.avail_num();
  mLatchItvlMgr.erase(id);
  ymuint uid = static_cast<ymuint>(id);
  latch->mId = uid;
  if ( mLatchArray.size() < uid ) {
    mLatchArray.resize(uid + 1, NULL);
  }
  mLatchArray[uid] = latch;

  // ラッチノードリストに登録
  mLatchList.push_back(latch);

  // 入出力ノードの生成
  p = mAlloc.get_memory(sizeof(CmnNodeLatchOut1));
  CmnNode* out1 = new (p) CmnNodeLatchOut1(latch);
  latch->mOutput1 = out1;
  reg_input(out1);

  p = mAlloc.get_memory(sizeof(CmnNodeLatchOut2));
  CmnNode* out2 = new (p) CmnNodeLatchOut2(latch);
  latch->mOutput2 = out2;
  reg_input(out2);

  p = mAlloc.get_memory(sizeof(CmnNodeLatchData));
  CmnNode* input = new (p) CmnNodeLatchData(latch);
  latch->mInput = input;
  reg_output(input);

  p = mAlloc.get_memory(sizeof(CmnNodeLatchEnable));
  CmnNode* enable = new (p) CmnNodeLatchEnable(latch);
  latch->mEnable = enable;
  reg_output(enable);

  if ( cell->has_clear() ) {
    p = mAlloc.get_memory(sizeof(CmnNodeLatchClear));
    CmnNode* clear = new (p) CmnNodeLatchClear(latch);
    latch->mClear = clear;
    reg_output(clear);
  }

  if ( cell->has_preset() ) {
    p = mAlloc.get_memory(sizeof(CmnNodeLatchPreset));
    CmnNode* preset = new (p) CmnNodeLatchPreset(latch);
    latch->mPreset = preset;
    reg_output(preset);
  }

  return latch;
}

#if 0
// @brief ラッチを削除する．
// @param[in] latch 対象のラッチ
void
CmnMgrImpl::delete_latch(CmnLatch* latch)
{
  mLatchItvlMgr.add(latch->id());
  mLatchArray[latch->id()] = NULL;

  mLatchList.erase(latch);

  mAlloc.put_memory(sizeof(CmnLatch), latch);
}
#endif

// @brief 論理ノードを作る．
// @param[in] inodes 入力ノードのベクタ
// @param[in] cell セル
// @return 作成したノードを返す．
CmnNode*
CmnMgrImpl::new_logic(const vector<CmnNode*>& inodes,
		      const Cell* cell)
{
  ymuint ni = inodes.size();
  void* p = mAlloc.get_memory(sizeof(CmnNodeLogic));
  void* q = mAlloc.get_memory(sizeof(CmnEdge) * ni);
  CmnEdge* fanins = new (q) CmnEdge[ni];
  CmnNode* node = new (p) CmnNodeLogic(ni, fanins, cell);
  reg_node(node);
  mLogicList.push_back(node);

  for (ymuint i = 0; i < ni; ++ i) {
    connect(inodes[i], node, i);
  }

  return node;
}

// @brief 出力ノードのファンインを設定する．
// @param[in] output 出力ノード
// @param[in] fanin ファンインのノード
void
CmnMgrImpl::set_output_fanin(CmnNode* output,
			     CmnNode* fanin)
{
  connect(fanin, output, 0);
}

// @brief D-FFセルを登録する．
// @param[in] cell 対象のセル．
// @param[in] pos_array ピン情報の配列
// @return D-FFセルの情報を表すオブジェクトを返す．
// @note pos_array の意味は以下の通り
//  - pos_array[0] : データ入力のピン番号     (3bit)
//  - pos_array[1] : クロック入力のピン番号   (3bit)
//  - pos_array[2] : クリア入力のピン番号     (3bit) | ピン情報 (2bit)
//  - pos_array[3] : プリセット入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[4] : 肯定出力のピン番号       (3bit)
//  - pos_array[5] : 否定出力のピン番号       (3bit)
const CmnDffCell*
CmnMgrImpl::reg_dff_cell(const Cell* cell,
			 ymuint pos_array[])
{
  const CmnDffCell* dff_cell = this->dff_cell(cell);
  if ( dff_cell ) {
    return dff_cell;
  }
  dff_cell = new CmnDffCell(cell, pos_array);
  mDffCellMap.insert(make_pair(cell, dff_cell));
  return dff_cell;
}

// @brief ラッチセルを登録する．
// @param[in] cell 対象のセル．
// @param[in] pos_array ピン情報の配列
// @return ラッチセルの情報を表すオブジェクトを返す．
// @note pos_array の意味は以下の通り
//  - pos_array[0] : データ入力のピン番号     (3bit)
//  - pos_array[1] : イネーブル入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[2] : クリア入力のピン番号     (3bit) | ピン情報 (2bit)
//  - pos_array[3] : プリセット入力のピン番号 (3bit) | ピン情報 (2bit)
//  - pos_array[4] : 肯定出力のピン番号       (3bit)
//  - pos_array[5] : 否定出力のピン番号       (3bit)
const CmnLatchCell*
CmnMgrImpl::reg_latch_cell(const Cell* cell,
			   ymuint pos_array[])
{
  const CmnLatchCell* latch_cell = this->latch_cell(cell);
  if ( latch_cell ) {
    return latch_cell;
  }
  latch_cell = new CmnLatchCell(cell, pos_array);
  mLatchCellMap.insert(make_pair(cell, latch_cell));
  return latch_cell;
}

#if 0
// 新しいノードを作成する．
// 作成されたノードを返す．
CmnNode*
CmnMgrImpl::new_node(ymuint ni)
{
  CmnNode* node = NULL;

  // 空いているIDを探してノード配列へ登録
  int id = mItvlMgr.avail_num();
  mItvlMgr.erase(id);

  ymuint uid = static_cast<ymuint>(id);
  while( mNodeArray.size() <= uid ) {
    // mNodeArray の大きさが小さいときのための埋め草
    void* p = mAlloc.get_memory(sizeof(CmnNode));
    CmnNode* node = new (p) CmnNode;
    mNodeArray.push_back(node);
  }
  node = mNodeArray[uid];
  node->mNi = ni;
  if ( ni ) {
    void* q = mAlloc2.get_memory(sizeof(CmnEdge) * ni);
    node->mFanins = new (q) CmnEdge[ni];
    for (ymuint i = 0; i < ni; ++ i) {
      node->mFanins[i].set_to(node, i);
    }
  }
  else {
    node->mFanins = NULL;
  }
  node->mId = uid;

  return node;
}
#endif

// @brief 入力ノードを登録する．
void
CmnMgrImpl::reg_input(CmnNode* node)
{
  reg_node(node);
  mInputList.push_back(node);
}

// @brief 出力ノードを登録する．
void
CmnMgrImpl::reg_output(CmnNode* node)
{
  reg_node(node);
  mOutputList.push_back(node);
}

// @breif ノードを登録して ID番号を割り振る．
void
CmnMgrImpl::reg_node(CmnNode* node)
{
  int id = mNodeItvlMgr.avail_num();
  mNodeItvlMgr.erase(id);
  node->mId = id;
  if ( mNodeArray.size() <= node->id() ) {
    mNodeArray.resize(node->id() + 1, NULL);
  }
  mNodeArray[node->id()] = node;
}

// node の登録を削除する．
void
CmnMgrImpl::unreg_node(CmnNode* node)
{
  // new_node の逆の処理を行なう．
  mNodeItvlMgr.add(static_cast<int>(node->id()));
  mNodeArray[node->id()] = NULL;
}

#if 0
// 入力ノードの削除
void
CmnMgrImpl::delete_input(CmnNode* node)
{
  assert_cond(node->is_input(), __FILE__, __LINE__);
  mInputList.erase(node);
  unreg_node(node);
}

// 出力ノードの削除
void
CmnMgrImpl::delete_output(CmnNode* node)
{
  assert_cond(node->is_output(), __FILE__, __LINE__);
  mOutputList.erase(node);
  unreg_node(node);
}

// 論理ノードの削除
void
CmnMgrImpl::delete_logic(CmnNode* node)
{
  assert_cond(node->is_logic(), __FILE__, __LINE__);
  assert_cond(node->fanout_num() == 0, __FILE__, __LINE__);
  ymuint ni = node->ni();
  for (ymuint i = 0; i < ni; ++ i) {
    connect(NULL, node, i);
  }

  mLogicList.erase(node);

  unreg_node(node);

  if ( ni > 0 ) {
    mAlloc.put_memory(sizeof(CmnEdge)* ni, node->mFanins);
  }
}
#endif

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
CmnMgrImpl::connect(CmnNode* from,
		    CmnNode* to,
		    ymuint pos)
{
  // CmnNode::mFaoutList を変更するのはここだけ

  CmnEdge* edge = to->_fanin_edge(pos);
  CmnNode* old_from = edge->from();
  if ( old_from ) {
    old_from->mFanoutList.erase(edge);
  }
  edge->set_from(from);
  if ( from ) {
    from->mFanoutList.push_back(edge);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CmnDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] cell セル
CmnDff::CmnDff(const string& name,
	       const CmnDffCell* cell) :
  mName(name),
  mCell(cell)
{
}

// @brief デストラクタ
CmnDff::~CmnDff()
{
}


//////////////////////////////////////////////////////////////////////
// クラス CmnLatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] cell セル
CmnLatch::CmnLatch(const string& name,
		   const CmnLatchCell* cell) :
  mName(name),
  mCell(cell)
{
}

// @brief デストラクタ
CmnLatch::~CmnLatch()
{
}

END_NAMESPACE_YM_CMN
