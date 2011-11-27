
/// @file AigMgr.cc
/// @brief AigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/AigMgr.h"
#include "AigNode.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgr::AigMgr() :
  mImpl(new AigMgrImpl)
{
}

// @brief デストラクタ
AigMgr::~AigMgr()
{
  delete mImpl;
}

// @brief 入力ノード数を得る．
ymuint
AigMgr::input_num() const
{
  return mImpl->input_num();
}

// @brief 入力ノードを取り出す．
const AigNode*
AigMgr::input_node(VarId id) const
{
  return mImpl->input_node(id);
}

// @brief ノード数を得る．
ymuint
AigMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < input_num() )
// @note ANDノードの他に入力ノードも含まれる．
const AigNode*
AigMgr::node(ymuint pos) const
{
  return mImpl->node(pos);
}


BEGIN_NONAMESPACE

void
dfs(ostream& s,
    AigNode* node,
    hash_set<ymuint>& mark)
{
  if ( node == NULL ) {
    return;
  }
  ymuint id = node->node_id();
  if ( mark.count(id) > 0 ) {
    return;
  }
  mark.insert(id);
  s << "Node#" << id << ": ";
  if ( node->is_input() ) {
    s << "Input#" << node->input_id() << endl;
  }
  else {
    s << "And(" << node->fanin0() << ", "
      << node->fanin1() << ")" << endl;
    dfs(s, node->fanin0_node(), mark);
    dfs(s, node->fanin1_node(), mark);
  }
}

END_NONAMESPACE

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] handle_list 対象のハンドルのリスト
void
AigMgr::print_handles(ostream& s,
		      const list<Aig>& handle_list) const
{
  ymuint i = 0;
  hash_set<ymuint> mark;
  for (list<Aig>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p) {
    Aig handle = *p;
    s << "Root#" << i << ": " << handle << endl;
    ++ i;
    dfs(s, handle.node(), mark);
  }
}

// @brief 定数0関数をつくる．
Aig
AigMgr::make_zero()
{
  return mImpl->make_zero();
}

// @brief 定数1関数をつくる．
Aig
AigMgr::make_one()
{
  return mImpl->make_one();
}

// @brief 外部入力を作る．
// @param[in] id 入力番号
Aig
AigMgr::make_input(VarId id)
{
  return mImpl->make_input(id);
}

// @brief AND ノードを作る．
Aig
AigMgr::make_and(Aig handle1,
		 Aig handle2)
{
  return mImpl->make_and(handle1, handle2);
}

// @brief 複数のノードの AND を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
Aig
AigMgr::make_and(const vector<Aig>& edge_list)
{
  ymuint n = edge_list.size();
  if ( n == 0 ) {
    return make_one();
  }
  Aig ans = edge_list[0];
  for (ymuint i = 1; i < n; ++ i) {
    ans = make_and(ans, edge_list[i]);
  }
  return ans;
}

// @brief 複数のノードの AND を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
Aig
AigMgr::make_and(const list<Aig>& edge_list)
{
  if ( edge_list.empty() ) {
    return make_one();
  }
  list<Aig>::const_iterator p = edge_list.begin();
  Aig ans = *p;
  for (++ p ; p != edge_list.end(); ++ p) {
    ans = make_and(ans, *p);
  }
  return ans;
}

// @brief 複数のノードの OR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
Aig
AigMgr::make_or(const vector<Aig>& edge_list)
{
  ymuint n = edge_list.size();
  if ( n == 0 ) {
    return make_zero();
  }
  Aig ans = edge_list[0];
  for (ymuint i = 1; i < n; ++ i) {
    ans = make_or(ans, edge_list[i]);
  }
  return ans;
}

// @brief 複数のノードの OR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
Aig
AigMgr::make_or(const list<Aig>& edge_list)
{
  if ( edge_list.empty() ) {
    return make_zero();
  }
  list<Aig>::const_iterator p = edge_list.begin();
  Aig ans = *p;
  for (++ p ; p != edge_list.end(); ++ p) {
    ans = make_or(ans, *p);
  }
  return ans;
}

// @brief 複数のノードの XOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
Aig
AigMgr::make_xor(const vector<Aig>& edge_list)
{
  ymuint n = edge_list.size();
  if ( n == 0 ) {
    return make_zero();
  }
  Aig ans = edge_list[0];
  for (ymuint i = 1; i < n; ++ i) {
    ans = make_xor(ans, edge_list[i]);
  }
  return ans;
}

// @brief 複数のノードの XOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
Aig
AigMgr::make_xor(const list<Aig>& edge_list)
{
  if ( edge_list.empty() ) {
    return make_zero();
  }
  list<Aig>::const_iterator p = edge_list.begin();
  Aig ans = *p;
  for (++ p ; p != edge_list.end(); ++ p) {
    ans = make_xor(ans, *p);
  }
  return ans;
}

// @brief 論理式に対応するノード(木)をつくる．
// @param[in] expr 対象の論理式
// @param[in] input_map 入力とAIGの対応表
Aig
AigMgr::make_logic(const LogExpr& expr,
		   const hash_map<VarId, Aig>& input_map)
{
  if ( expr.is_zero() ) {
    return make_zero();
  }
  if ( expr.is_one() ) {
    return make_one();
  }
  if ( expr.is_posiliteral() ) {
    VarId id = expr.varid();
    hash_map<VarId, Aig>::const_iterator p = input_map.find(id);
    if ( p != input_map.end() ) {
      return p->second;
    }
    // なかったらそのままの入力ノードに変換する．
    return make_input(id);
  }
  if ( expr.is_negaliteral() ) {
    VarId id = expr.varid();
    hash_map<VarId, Aig>::const_iterator p = input_map.find(id);
    if ( p != input_map.end() ) {
      return ~p->second;
    }
    // なかったらそのままの入力ノードに変換する．
    return ~make_input(id);
  }
  if ( expr.is_and() ) {
    ymuint n = expr.child_num();
    vector<Aig> tmp_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      tmp_list[i] = make_logic(expr.child(i), input_map);
    }
    return make_and(tmp_list);
  }
  if ( expr.is_or() ) {
    ymuint n = expr.child_num();
    vector<Aig> tmp_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      tmp_list[i] = make_logic(expr.child(i), input_map);
    }
    return make_or(tmp_list);
  }
  if ( expr.is_xor() ) {
    ymuint n = expr.child_num();
    vector<Aig> tmp_list(n);
    for (ymuint i = 0; i < n; ++ i) {
      tmp_list[i] = make_logic(expr.child(i), input_map);
    }
    return make_xor(tmp_list);
  }
  assert_not_reached(__FILE__, __LINE__);
  return make_zero();
}

// @brief コファクターを計算する．
// @param[in] edge 対象の AIG ハンドル
// @param[in] id コファクターをとる変数番号
// @param[in] pol 極性
Aig
AigMgr::make_cofactor(Aig edge,
		      VarId id,
		      tPol pol)
{
  if ( edge.is_const() ) {
    // edge が定数の時は変更なし
    return edge;
  }

  AigNode* node = edge.node();
  Aig ans;
  if ( node->is_input() ) {
    // 入力ノード時は番号が id どうかで処理が変わる．
    if ( node->input_id() == id ) {
      if ( pol == kPolPosi ) {
	ans = make_one();
      }
      else {
	ans = make_zero();
      }
    }
    else {
      ans = Aig(node, false);
    }
  }
  else {
    // AND ノードの場合
    // 2つの子供に再帰的な処理を行って結果の AND を計算する．
    Aig new_handle0 = make_cofactor(node->fanin0(), id, pol);
    Aig new_handle1 = make_cofactor(node->fanin1(), id, pol);
    Aig ans = make_and(new_handle0, new_handle1);
  }
  if ( edge.inv() ) {
    ans = ~ans;
  }
  return ans;
}

END_NAMESPACE_YM_AIG
