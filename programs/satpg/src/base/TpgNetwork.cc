
/// @file TpgNetwork.cc
/// @brief TpgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "networks/TgNetwork.h"
#include "networks/TgNode.h"
#include "networks/TgBlifReader.h"
#include "networks/TgIscas89Reader.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// 指定された型の配列を確保するテンプレート関数
template <typename T>
T*
alloc_array(Alloc& alloc,
	    ymuint n)
{
  void* p = alloc.get_memory(sizeof(T) * n);
  return new (p) T[n];
}


// immediate dominator リストをマージする．
TpgNode*
merge(TpgNode* node1,
      TpgNode* node2)
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

// pair の最初の要素で比較を行なうファンクター
// こういうのは lambda 表記があると簡潔にすむ．
struct Lt
{
  bool
  operator()(const pair<ymuint, ymuint>& left,
	     const pair<ymuint, ymuint>& right)
  {
    return left.first < right.first;
  }

};

// 追加で必要なノード数を数える．
ymuint
extra_node_count(const Expr& expr,
		 ymuint ni)
{
  ymuint n = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint p_num = expr.litnum(VarId(i), false);
    ymuint n_num = expr.litnum(VarId(i), true);
    assert_cond( p_num > 0 || n_num > 0, __FILE__, __LINE__);
    if ( n_num == 0 ) {
      if ( p_num > 1 ) {
	n = 1;
      }
    }
    else {
      n = 1;
      if ( p_num > 0 ) {
	++ n;
      }
    }
  }
  return n;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TpgNetwork
//////////////////////////////////////////////////////////////////////

// @brief blif ファイルを読み込んでインスタンスを作る．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @note エラーが起こったら NULL を返す．
TpgNetwork*
TpgNetwork::read_blif(const string& filename,
		      const CellLibrary* cell_library)
{
  TgNetwork tgnetwork;
  TgBlifReader read;

  bool stat = read(filename, tgnetwork, cell_library);
  if ( !stat ) {
    return NULL;
  }

  TpgNetwork* network = new TpgNetwork(tgnetwork);

  return network;
}

// @brief iscas89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @note エラーが起こったら NULL を返す．
TpgNetwork*
TpgNetwork::read_iscas89(const string& filename)
{
  TgNetwork tgnetwork;
  TgIscas89Reader read;

  bool stat = read(filename, tgnetwork);
  if ( !stat ) {
    return NULL;
  }

  TpgNetwork* network = new TpgNetwork(tgnetwork);

  return network;
}

// @brief コンストラクタ
// @param[in] tgnetwork もとのネットワーク
TpgNetwork::TpgNetwork(const TgNetwork& tgnetwork) :
  mAlloc(4096)
{
  //////////////////////////////////////////////////////////////////////
  // 要素数を数え，必要なメモリ領域を確保する．
  //////////////////////////////////////////////////////////////////////

  ymuint nn_orig = tgnetwork.node_num();
  ymuint nl = tgnetwork.logic_num();
  ymuint nn = nn_orig;
  unordered_map<ymuint, ymuint> en_hash;
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* tgnode = tgnetwork.logic(i);
    if ( tgnode->is_cplx_logic() ) {
      ymuint fid = tgnode->func_id();
      if ( en_hash.count(fid) > 0 ) {
	nn += en_hash[fid];
      }
      else {
	ymuint n = extra_node_count(tgnetwork.get_lexp(fid), tgnode->fanin_num());
	en_hash.insert(make_pair(fid, n));
	nn += n;
      }
    }
  }

  mInputNum = tgnetwork.input_num1();
  mOutputNum = tgnetwork.output_num1();
  mFFNum = tgnetwork.ff_num();

  mNodeNum = 0;
  mNodeArray = alloc_array<TpgNode>(mAlloc, nn);

  mNodeMap = alloc_array<TpgNode*>(mAlloc, nn_orig);
  mInputArray = alloc_array<TpgNode*>(mAlloc, input_num2());
  mOutputArray = alloc_array<TpgNode*>(mAlloc, output_num2());
  mOutputArray2 = alloc_array<TpgNode*>(mAlloc, output_num2());

  mActNodeNum = 0;
  mActNodeArray = alloc_array<TpgNode*>(mAlloc, nn);

  mTmpNodeNum = 0;
  mTmpNodeList = alloc_array<TpgNode*>(mAlloc, nn);

  mTmpMark = alloc_array<bool>(mAlloc, nn);
  for (ymuint i = 0; i < nn; ++ i) {
    mTmpMark[i] = false;
  }

  //////////////////////////////////////////////////////////////////////
  // 外部入力を作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npi = input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    const TgNode* tgnode = tgnetwork.input(i);
    TpgNode* node = make_input_node(tgnode);
    mInputArray[i] = node;
  }


  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  // ただし mNodeArray は入力からのトポロジカル順になる．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* tgnode = tgnetwork.sorted_logic(i);
    TpgNode* node = make_logic_node(tgnode, tgnetwork);
  }


  //////////////////////////////////////////////////////////////////////
  // 外部出力ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  ymuint npo = output_num2();
  for (ymuint i = 0; i < npo; ++ i) {
    const TgNode* tgnode = tgnetwork.output(i);
    TpgNode* node = make_output_node(tgnode);
    mOutputArray[i] = node;
  }

  assert_cond( nn == mNodeNum, __FILE__, __LINE__);


  // 全部アクティブにしておく．
  activate_all();

  // TFI のサイズの昇順に並べ方出力順を
  // mOutputArray2 に記録する．
  vector<pair<ymuint, ymuint> > tmp_list(output_num2());
  for (ymuint i = 0; i < output_num2(); ++ i) {
    TpgNode* onode = output(i);
    tfimark(onode);
    ymuint n = mTmpNodeNum;
    clear_tfimark();
    tmp_list[i] = make_pair(n, i);
  }

  sort(tmp_list.begin(), tmp_list.end(), Lt());

  for (ymuint i = 0; i < output_num2(); ++ i) {
    mOutputArray2[i] = mOutputArray[tmp_list[i].second];
  }
}

// @brief デストラクタ
TpgNetwork::~TpgNetwork()
{
  // このオブジェクトが確保したメモリは mAlloc のデストラクタが
  // 開放してくれる．
}

// @brief 一つの外部出力に関係するノードのみをアクティブにする．
// @param[in] po_pos 出力番号
void
TpgNetwork::activate_po(ymuint po_pos)
{
  TpgNode* onode = output2(po_pos);

  // pos 番めの出力から到達可能なノードにマークをつける．
  tfimark(onode);

  activate_sub();
}

// @brief 全てのノードをアクティブにする．
void
TpgNetwork::activate_all()
{
  // といっても PO に到達できないノードは除外する．

  // すべての PO から到達可能なノードにマークをつける．
  for (ymuint i = 0; i < mOutputNum + mFFNum; ++ i) {
    tfimark(output(i));
  }

  activate_sub();
}

// @brief ノードの TFI にマークをつける．
// @note 結果は mTmpMark[node->id()] に格納される．
void
TpgNetwork::tfimark(TpgNode* node)
{
  if ( mTmpMark[node->id()] ) {
    return;
  }

  mTmpMark[node->id()] = true;
  mTmpNodeList[mTmpNodeNum] = node;
  ++ mTmpNodeNum;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    tfimark(node->fanin(i));
  }
}

// @brief activate_po(), activate_all() の下請け関数
void
TpgNetwork::activate_sub()
{
  // マークにしたがってファンアウトなどの情報をセットする．
  mActNodeNum = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    TpgNode* node = &mNodeArray[i];
    if ( !mTmpMark[i] ) {
      node->clear_active();
      continue;
    }
    node->set_active();
    mActNodeArray[mActNodeNum] = node;
    ++ mActNodeNum;

    ymuint nfo = node->fanout_num();
    ymuint act_nfo = 0;
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* onode = node->fanout(i);
      if ( mTmpMark[onode->id()] ) {
	node->mActFanouts[act_nfo] = onode;
	++ act_nfo;
      }
    }
    node->mActFanoutNum = act_nfo;
  }

  // immediate dominator を求める．
  for (ymuint i = 0; i < mActNodeNum; ++ i) {
    TpgNode* node = mActNodeArray[mActNodeNum - i - 1];
    ymuint nfo = node->active_fanout_num();
    if ( nfo == 0 ) {
      assert_cond( node->is_output(), __FILE__, __LINE__);
      node->mImmDom = NULL;
    }
    else {
      TpgNode* node0 = node->active_fanout(0);
      for (ymuint i = 1; i < nfo; ++ i) {
	node0 = merge(node0, node->active_fanout(i));
	if ( node0 == NULL ) {
	  break;
	}
      }
      node->mImmDom = node0;
    }
  }

  // マークを消す．
  clear_tfimark();
}

// @brief TFI マークを消す．
// @note この関数が終了すると mTmpNodeNum は 0 になる．
void
TpgNetwork::clear_tfimark()
{
  for (ymuint i = 0; i < mTmpNodeNum; ++ i) {
    mTmpMark[mTmpNodeList[i]->id()] = false;
  }
  mTmpNodeNum = 0;
}

// @brief ノードを得る．
TpgNode*
TpgNetwork::node(ymuint pos)
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief ノードを得る．
const TpgNode*
TpgNetwork::node(ymuint pos) const
{
  // アドレス計算のために TpgNode の定義が必要なのでここに置く．
  assert_cond( pos < mNodeNum, __FILE__, __LINE__);
  return &mNodeArray[pos];
}

// @brief 入力ノードを生成する．
// @param[in] tgnode もととなる TgNode
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_input_node(const TgNode* tgnode)
{
  TpgNode* node = make_node(0, tgnode->fanout_num());
  node->mTypeId = 1U | (tgnode->lid() << 3);

  bind(node, tgnode);

  return node;
}

// @brief 出力ノードを生成する．
// @param[in] tgnode もととなる TgNode
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_output_node(const TgNode* tgnode)
{
  TpgNode* node = make_node(1, 0);
  node->mTypeId = 2U | (tgnode->lid() << 3);

  const TgNode* itgnode = tgnode->fanin(0);
  TpgNode* inode = mNodeMap[itgnode->gid()];
  connect(inode, node, 0);

  bind(node, tgnode);

  return node;
}

// @brief 論理ノードを生成する．
// @param[in] tgnode もととなる TgNode
// @param[in] tgnetwork もととなるネットワーク
// @return 生成したノードを返す．
//
/// 場合によっては複数の TpgNode を生成する．
TpgNode*
TpgNetwork::make_logic_node(const TgNode* tgnode,
			    const TgNetwork& tgnetwork)
{
  ymuint ni = tgnode->fanin_num();
  ymuint nfo = tgnode->fanout_num();

  TpgNode* node = NULL;
  if ( tgnode->is_cplx_logic() ) {
    // 論理式をとってくる．
    ymuint fid = tgnode->func_id();
    Expr expr = tgnetwork.get_lexp(fid);

    // expr の内容を表す TpgNode の木を作る．
    vector<TpgNode*> leaf_nodes(ni * 2, NULL);
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint p_num = expr.litnum(VarId(i), false);
      ymuint n_num = expr.litnum(VarId(i), true);
      const TgNode* itgnode = tgnode->fanin(i);
      TpgNode* inode = mNodeMap[itgnode->gid()];
      if ( n_num == 0 ) {
	if ( p_num == 1 ) {
	  leaf_nodes[i * 2 + 0] = inode;
	}
	else {
	  TpgNode* dummy_buff = make_prim_node(kTgGateBuff, 1, p_num);
	  connect(inode, dummy_buff, 0);
	  leaf_nodes[i * 2 + 0] = dummy_buff;
	}
      }
      else {
	if ( p_num > 0 ) {
	  TpgNode* dummy_buff = make_prim_node(kTgGateBuff, 1, p_num + 1);
	  connect(inode, dummy_buff, 0);
	  inode = dummy_buff;
	}

	TpgNode* not_gate = make_prim_node(kTgGateNot, 1, n_num);
	connect(inode, not_gate, 0);
	leaf_nodes[i * 2 + 1] = not_gate;
      }
    }
    node = make_cplx_node(expr, nfo, leaf_nodes);
  }
  else {
    // 組み込み型のゲート
    node = make_prim_node(tgnode->gate_type(), ni, nfo);
    for (ymuint i = 0; i < ni; ++ i) {
      const TgNode* itgnode = tgnode->fanin(i);
      TpgNode* inode = mNodeMap[itgnode->gid()];
      connect(inode, node, i);
    }
  }
  bind(node, tgnode);

  return node;
}

// @brief 論理式から TpgNode の木を生成する．
// @param[in] expr 式
// @param[in] nfo 根のノードのファンアウト数
// @param[in] leaf_nodes 式のリテラルに対応するノードの配列
// @return 生成したノードを返す．
//
// leaf_nodes は 変数番号 * 2 + (0/1) に
// 該当する変数の肯定/否定のリテラルが入っている．
TpgNode*
TpgNetwork::make_cplx_node(const Expr& expr,
			   ymuint nfo,
			   const vector<TpgNode*>& leaf_nodes)
{
  if ( expr.is_posiliteral() ) {
    return leaf_nodes[expr.varid().val() * 2 + 0];
  }
  if ( expr.is_negaliteral() ) {
    return leaf_nodes[expr.varid().val() * 2 + 1];
  }

  tTgGateType gate_type;
  if ( expr.is_and() ) {
    gate_type = kTgGateAnd;
  }
  else if ( expr.is_or() ) {
    gate_type = kTgGateOr;
  }
  else if ( expr.is_xor() ) {
    gate_type = kTgGateXor;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  ymuint nc = expr.child_num();
  TpgNode* node = make_prim_node(gate_type, nc, nfo);

  for (ymuint i = 0; i < nc; ++ i) {
    const Expr& expr1 = expr.child(i);
    TpgNode* inode = make_cplx_node(expr1, 1, leaf_nodes);
    connect(inode, node, i);
  }

  return node;
}

// @brief 組み込み型の論理ゲートを生成する．
// @param[in] type ゲートの型
// @param[in] ni ファンイン数
// @param[in] nfo ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_prim_node(tTgGateType type,
			   ymuint ni,
			   ymuint nfo)
{
  TpgNode* node = make_node(ni, nfo);
  node->mTypeId = 4U | (static_cast<ymuint>(type) << 3);

  switch ( type ) {
  case kTgGateAnd:
  case kTgGateNand:
    node->mCval = kB3False;
    node->mNval = kB3True;
    break;

  case kTgGateOr:
  case kTgGateNor:
    node->mCval = kB3True;
    node->mNval = kB3False;
    break;

  default:
    break;
  }

  return node;
}

// @brief TpgNode を生成する．
// @param[in] ni ファンイン数
// @param[in] nfo ファンアウト数
// @return 生成したノードを返す．
TpgNode*
TpgNetwork::make_node(ymuint ni,
		      ymuint nfo)

{
  TpgNode* node = &mNodeArray[mNodeNum];
  node->mId = mNodeNum;
  ++ mNodeNum;

  node->mCval = kB3X;
  node->mNval = kB3X;
  node->mMaVal = kB3X;

  node->mFaninNum = ni;
  if ( ni > 0 ) {
    node->mFanins = alloc_array<TpgNode*>(mAlloc, ni);
    node->mInputFault = alloc_array<TpgFault*>(mAlloc, ni * 2);
  }
  else {
    node->mFanins = NULL;
    node->mInputFault = NULL;
  }

  node->mFanoutNum = 0;
  node->mFanouts = alloc_array<TpgNode*>(mAlloc, nfo);
  node->mActFanoutNum = 0;
  node->mActFanouts = alloc_array<TpgNode*>(mAlloc, nfo);

  for (ymuint i = 0; i < ni; ++ i) {
    node->mInputFault[i * 2 + 0] = NULL;
    node->mInputFault[i * 2 + 1] = NULL;
  }
  node->mOutputFault[0] = NULL;
  node->mOutputFault[1] = NULL;

  node->mMarks = 0U;

  return node;
}

// @brief ノード間の接続を行う．
// @param[in] src ソースノード
// @param[in] dst ディスティネーションノード
// @param[in] ipos ファンイン番号
void
TpgNetwork::connect(TpgNode* src,
		    TpgNode* dst,
		    ymuint ipos)
{
  dst->mFanins[ipos] = src;
  src->mFanouts[src->mFanoutNum] = dst;
  ++ src->mFanoutNum;
}

// @brief TpgNode と TpgNode の対応付けを行う．
// @param[in] node TpgNode
// @param[in] tgnode もととなる TgNodep
void
TpgNetwork::bind(TpgNode* node,
		 const TgNode* tgnode)
{
  const char* src_name = tgnode->name();
  if ( tgnode->is_output() ) {
    src_name = tgnode->fanin(0)->name();
  }
  if ( src_name != NULL ) {
    ymuint len = strlen(src_name) + 1;
    char* dst_name = alloc_array<char>(mAlloc, len);
    for (ymuint i = 0; i < len; ++ i) {
      dst_name[i] = src_name[i];
    }
    node->mName = dst_name;
  }
  else {
    node->mName = NULL;
  }

  mNodeMap[tgnode->gid()] = node;
}


//////////////////////////////////////////////////////////////////////
// 必要割り当て関係のコード
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// node のファンアウトに mark1 をつける．
void
set_tfo_mark(TpgNode* node)
{
  if ( node->mark1() ) {
    return;
  }
  node->set_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    set_tfo_mark(onode);
  }
}

// node のファンアウトに mark1 をつける．
void
clear_tfo_mark(TpgNode* node)
{
  if ( !node->mark1() ) {
    return;
  }
  node->clear_mark1();

  ymuint n = node->active_fanout_num();
  for (ymuint i = 0; i < n; ++ i) {
    TpgNode* onode = node->active_fanout(i);
    clear_tfo_mark(onode);
  }
}

END_NONAMESPACE

// @brief f の検出に必要な割り当てを求める．
// @param[in] f 対象の故障
// @param[in] ma_list 割り当て結果を格納するリスト
// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
// @note TpgNetwork のメンバにはアクセスしないので static メンバになっている．
// @note ma_list の内容は TpgNode::id() * 2 + val (0 / 1)
bool
TpgNetwork::get_mandatory_assignment(TpgFault* f,
				     vector<ymuint32>& ma_list)
{
  TpgNode* fnode = f->node();
  TpgNode* fsrc = fnode;
  if ( f->is_input_fault() ) {
    fsrc = fnode->fanin(f->pos());
  }

  vector<TpgNode*> node_list;
  Bool3 gval;
  if ( f->val() == 1 ) {
    gval = kB3False;
  }
  else {
    gval = kB3True;
  }
  bool stat = fsrc->bwd_prop(NULL, gval, node_list);
  if ( !stat ) {
    goto untestable;
  }

  if ( f->is_input_fault() ) {
    Bool3 nval = fnode->nval();
    if ( nval != kB3X ) {
      ymuint ni = fnode->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	if ( i == f->pos() ) continue;
	TpgNode* inode = fnode->fanin(i);
	if ( !inode->bwd_prop(fnode, nval, node_list) ) {
	  goto untestable;
	}
      }
    }
  }
  if ( fnode->imm_dom() != NULL ) {
    set_tfo_mark(fnode);
    for (TpgNode* dom = fnode->imm_dom(); dom != NULL; dom = dom->imm_dom()) {
      Bool3 nval = dom->nval();
      if ( nval != kB3X ) {
	ymuint ni = dom->fanin_num();
	for (ymuint i = 0; i < ni; ++ i) {
	  TpgNode* inode = dom->fanin(i);
	  if ( inode->mark1() ) continue;
	  if ( !inode->bwd_prop(dom, nval, node_list) ) {
	    clear_tfo_mark(fnode);
	    goto untestable;
	  }
	}
      }
    }
    clear_tfo_mark(fnode);
  }

  for (vector<TpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    TpgNode* node = *p;
    assert_cond( node->ma_value() != kB3X, __FILE__, __LINE__);
    int val = node->ma_value() == kB3True ? 1 : 0;
    ma_list.push_back(node->id() * 2 + val);
    node->clear_ma_value();
  }
  sort(ma_list.begin(), ma_list.end());
  return true;

 untestable:
  for (vector<TpgNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    TpgNode* node = *p;
    node->clear_ma_value();
  }
  return false;
}

END_NAMESPACE_YM_SATPG
