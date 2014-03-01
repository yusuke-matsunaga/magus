
/// @file BNetDecomp.cc
/// @brief BNetDecomp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BNetDecomp.h"
#include "networks/BNetManip.h"
#include "utils/HeapTree.h"
#include "utils/RandPermGen.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

BEGIN_NONAMESPACE

// mindepth_XXXX で用いるヒープ木用のノード定義
struct Node
{
  BNode* mNode;
  bool mInv;
  int mDepth;

  Node() { }

  Node(BNode* node,
       bool inv,
       int depth) :
    mNode(node),
    mInv(inv),
    mDepth(depth)
  {
  }
};

// mindepth_XXXX で用いるヒープ木用の比較関数定義
struct CompFunc
{
  int
  operator()(Node a,
	     Node b)
  {
    return a.mDepth - b.mDepth;
  }
};

END_NONAMESPACE


// @brief コンストラクタ．
BNetDecomp::BNetDecomp()
{
}

// @brief デストラクタ
BNetDecomp::~BNetDecomp()
{
}

// @brief 単純分解を行う．(バランス型: type1)
// @param[in] network 操作対象のネットワーク
// @param[in] max_fanin ファンインの最大数(0, 1で制限なし)
// @param[in] no_xor true の時は XOR/XNOR もAND/OR に分解する．
// @note 単純分解とは(AND/OR/XOR)のみからなるノードの木に分解すること
// @note ただし，入力に否定属性がつく場合がある．
// @note 副作用として sweep() が行われる．
void
BNetDecomp::operator()(BNetwork& network,
		       ymuint max_fanin,
		       bool no_xor)
{
  network.sweep();

  mDepthMap.clear();
  mDepthMap.resize(network.max_node_id());

  // 入力ノードとFFノードの深さを0とする．
  for (BNodeList::const_iterator p = network.inputs_begin();
       p != network.inputs_end(); ++ p) {
    BNode* node = *p;
    mDepthMap[node->id()] = 0;
  }
  for (BNodeList::const_iterator p = network.latch_nodes_begin();
       p != network.latch_nodes_end(); ++ p) {
    BNode* node = *p;
    mDepthMap[node->id()] = 0;
  }

  mManip = new BNetManip(&network);

  // 内部ノードを入力側からのトポロジカル順で処理する．
  BNodeVector node_list;
  network.tsort(node_list);
  ymuint n = network.logic_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    BNode* node = node_list[i];

    ymuint max_fanin1 = ( max_fanin < 2 ) ? node->fanin_num() : max_fanin;
    const Expr& expr = node->func();
    if ( !expr.is_simple() || expr.litnum() > max_fanin || no_xor && expr.is_xor() ) {
      // ここに来ているということは expr の根のタイプは二項演算子
      decomp_type1_sub(node, max_fanin1, expr, node, no_xor);
    }
    else {
      // ここに来ているということは node は変更の必要が無い．
      // ただし深さだけは計算しておく必要がある．
      calc_depth(node);
    }
  }

  delete mManip;
  mManip = NULL;
}

// @brief 単純分解を行う．(ランダム型: type2)
// @param[in] network 操作対象のネットワーク
// @param[in] max_fanin ファンインの最大数(0, 1で制限なし)
// @param[in] randgen 分解で用いる乱数発生器
// @param[in] no_xor true の時は XOR/XNOR もAND/OR に分解する．
// @note 単純分解とは(AND/OR/XOR)のみからなるノードの木に分解すること
// @note ただし，入力に否定属性がつく場合がある．
// @note 副作用として sweep() が行われる．
void
BNetDecomp::operator()(BNetwork& network,
		       ymuint max_fanin,
		       RandGen& randgen,
		       bool no_xor)
{
  network.sweep();

  mRandGen = &randgen;

  mManip = new BNetManip(&network);

  BNodeVector node_list;
  network.tsort(node_list);
  ymuint n = network.logic_node_num();
  for (ymuint i = 0; i < n; ++ i) {
    BNode* node = node_list[i];

    ymuint max_fanin1 = ( max_fanin < 2 ) ? node->fanin_num() : max_fanin;
    const Expr& expr = node->func();
    if ( !expr.is_simple() || expr.litnum() > max_fanin1 || no_xor && expr.is_xor() ) {
      // ここに来ているということは expr の根のタイプは二項演算子
      decomp_type2_sub(node, max_fanin1, expr, node, no_xor);
    }
  }

  delete mManip;
  mManip = NULL;
}

// decomp_type1 で用いられるサブルーティン
// expr を根とする論理式を分解して root_node を根のノード
// とする木を作る．
// この関数を呼び出す時には根のタイプは二項演算子であるこ
// とを保証すること．
void
BNetDecomp::decomp_type1_sub(BNode* orig_node,
			     ymuint max_fanin,
			     const Expr& expr,
			     BNode* root_node,
			     bool no_xor)
{
  assert_cond(!expr.is_zero() && !expr.is_one() && !expr.is_literal(),
	      __FILE__, __LINE__);

  HeapTree<Node, CompFunc> work;

  ymuint ni = expr.child_num();
  for (ymuint i = 0; i < ni; i ++) {
    Expr opr1 = expr.child(i);
    assert_cond(!opr1.is_zero() && !opr1.is_one(), __FILE__, __LINE__);

    BNode* node1;
    bool inv1;
    if ( opr1.is_literal() ) {
      VarId var = opr1.varid();
      ymuint pos = var.val();
      node1 = orig_node->fanin(pos);
      inv1 = opr1.is_negaliteral();
    }
    else {
      node1 = mManip->new_logic();
      decomp_type1_sub(orig_node, max_fanin, opr1, node1, no_xor);
      inv1 = false;
    }
    work.put(Node(node1, inv1, mDepthMap[node1->id()]));
  }

  // 高さ最小の論理木を作る．
  if ( no_xor && expr.is_xor() ) {
    max_fanin = 2;
  }
  else if ( max_fanin < 2 ) {
    // ファンイン数制限なし
    max_fanin = ni;
  }

  vector<BNode*> fanins;
  vector<Expr> literals;
  fanins.reserve(max_fanin);
  literals.reserve(max_fanin);
  for ( ; ; ) {
    fanins.clear();
    literals.clear();
    ymuint new_ni;
    for (new_ni = 0; !work.empty() && new_ni < max_fanin; ++ new_ni) {
      Node tmp1 = work.getmin();
      work.popmin();
      fanins.push_back(tmp1.mNode);
      literals.push_back(Expr::make_literal(VarId(new_ni), tmp1.mInv));
    }

    Expr tmp_expr;
    if ( expr.is_and() ) {
      tmp_expr = Expr::make_and(literals);
    }
    else if ( expr.is_or() ) {
      tmp_expr = Expr::make_or(literals);
    }
    else if ( expr.is_xor() ) {
      if ( no_xor ) {
	assert_cond(max_fanin == 2, __FILE__, __LINE__);
	Expr lit1 = literals[0];
	Expr lit2 = literals[1];
	BNode* and1 = mManip->new_logic();
	bool stat1 = mManip->change_logic(and1, lit1 & ~lit2, fanins);
	assert_cond(stat1, __FILE__, __LINE__);
	BNode* and2 = mManip->new_logic();
	bool stat2 = mManip->change_logic(and2, ~lit1 & lit2, fanins);
	assert_cond(stat2, __FILE__, __LINE__);
	fanins[0] = and1;
	fanins[1] = and2;
	tmp_expr = lit1 | lit2;
      }
      else {
	tmp_expr = Expr::make_xor(literals);
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }

    BNode* node = NULL;

    if ( work.empty() ) {
      node = root_node;
    }
    else {
      node = mManip->new_logic();
    }
    bool stat = mManip->change_logic(node, tmp_expr, fanins);
    assert_cond(stat, __FILE__, __LINE__);
    int d = calc_depth(node);
    if ( work.empty() ) {
      return;
    }
    work.put(Node(node, false, d));
  }
}

// decomp_type2_sub で用いられるサブルーティン
// expr を根とする論理式を分解して root_node を根のノード
// とする木を作る．
// この関数を呼び出す時には根のタイプは二項演算子であるこ
// とを保証すること．
void
BNetDecomp::decomp_type2_sub(BNode* orig_node,
			     ymuint max_fanin,
			     const Expr& expr,
			     BNode* root_node,
			     bool no_xor)
{
  assert_cond(!expr.is_zero() && !expr.is_one() && !expr.is_literal(),
	      __FILE__, __LINE__);

  ymuint ni = expr.child_num();
  vector<pair<BNode*, bool> > tmp_fanins(ni);
  for (ymuint i = 0; i < ni; i ++) {
    Expr opr1 = expr.child(i);
    assert_cond(!opr1.is_zero() && !opr1.is_one(), __FILE__, __LINE__);

    BNode* node1;
    bool inv1;
    if ( opr1.is_literal() ) {
      VarId var = opr1.varid();
      ymuint pos = var.val();
      node1 = orig_node->fanin(pos);
      inv1 = opr1.is_negaliteral();
    }
    else {
      node1 = mManip->new_logic();
      decomp_type2_sub(orig_node, max_fanin, opr1, node1, no_xor);
      inv1 = false;
    }
    tmp_fanins[i] = make_pair(node1, inv1);
  }

  if ( no_xor && expr.is_xor() ) {
    max_fanin = 2;
  }
  else if ( max_fanin < 2 ) {
    max_fanin = ni;
  }

  // tmp_fanins をランダムに並べ替える．
  RandPermGen permgen(ni);
  permgen.generate(*mRandGen);
  vector<pair<BNode*, bool> > tmp_fanins2(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint pos = permgen.elem(i);
    tmp_fanins2[i] = tmp_fanins[pos];
  }
  // ランダムに分割した論理木を作る．
  build_tree(0, ni, tmp_fanins2, max_fanin, expr, root_node, no_xor);
}

// tmp_fanins[b: b+ni-1] を入力とする type_expr の
// max_fanin 分木を network 上のノードとして作る．
// root_node が NULL でない場合には root_node を根のノードとする．
// root_node が NULL の場合には新しいノードを作る．
// いずれの場合でも根のノードを返す．
BNode*
BNetDecomp::build_tree(ymuint b,
		       ymuint ni,
		       const vector<pair<BNode*, bool> >& tmp_fanins,
		       ymuint max_fanin,
		       const Expr& type_expr,
		       BNode* root_node,
		       bool no_xor)
{
  assert_cond(ni > 1, __FILE__, __LINE__);
  assert_cond(max_fanin > 1, __FILE__, __LINE__);

  // 個々の部分木の入力数
  ymuint new_ni = ni / max_fanin;
  // ただし 0 から (nodd - 1) までは + 1する．
  ymuint nodd = ni % max_fanin;

  ymuint real_fanin = ni;
  if ( real_fanin > max_fanin ) {
    real_fanin = max_fanin;
  }

  vector<BNode*> fanins(real_fanin);
  vector<Expr> literals(real_fanin);

  ymuint b0 = b;
  for (ymuint i = 0; i < real_fanin; ++ i) {
    ymuint b1 = b0;
    ymuint ni1 = new_ni;
    b0 += ni1;
    if ( i < nodd ) {
      ++ ni1;
    }
    bool inv;
    if ( ni1 == 1 ) {
      fanins[i] = tmp_fanins[b1].first;
      inv = tmp_fanins[b1].second;
    }
    else {
      fanins[i] = build_tree(b1, ni1, tmp_fanins, max_fanin,
			     type_expr, NULL, no_xor);
      inv = false;
    }
    literals[i] = Expr::make_literal(VarId(i), inv);
  }

  Expr expr;
  if ( type_expr.is_and() ) {
    expr = Expr::make_and(literals);
  }
  else if ( type_expr.is_or() ) {
    expr = Expr::make_or(literals);
  }
  else if ( type_expr.is_xor() ) {
    if ( no_xor ) {
      assert_cond(real_fanin == 2, __FILE__, __LINE__);
      Expr lit1 = literals[0];
      Expr lit2 = literals[1];
      BNode* and1 = mManip->new_logic();
      bool stat1 = mManip->change_logic(and1, lit1 & ~lit2, fanins);
      assert_cond(stat1, __FILE__, __LINE__);
      BNode* and2 = mManip->new_logic();
      bool stat2 = mManip->change_logic(and2, ~lit1 & lit2, fanins);
      assert_cond(stat2, __FILE__, __LINE__);
      fanins[0] = and1;
      fanins[1] = and2;
      expr = lit1 | lit2;
    }
    else {
      expr = Expr::make_xor(literals);
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( root_node == NULL ) {
    root_node = mManip->new_logic();
  }
  bool stat = mManip->change_logic(root_node, expr, fanins);
  assert_cond(stat, __FILE__, __LINE__);

  return root_node;
}

// depth_map に基づいて node の深さを計算する．
// 同時にその値を depth_map に書き込む．
// node の直接のファンインのノードの深さは depth_map に入っているものとする．
int
BNetDecomp::calc_depth(BNode* node)
{
  int d = 0;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    BNode* inode = node->fanin(i);
    int level = mDepthMap[inode->id()];
    if ( d < level ) {
      d = level;
    }
  }
  ++ d;
  mDepthMap[node->id()] = d;
  return d;
}

END_NAMESPACE_YM_NETWORKS_BNET
