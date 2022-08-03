
/// @file FraigEnc.cc
/// @brief FraigEnc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigEnc.h"
#include "ym/BnNetwork.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
// クラス FraigEnc
//////////////////////////////////////////////////////////////////////

// @brief ネットワークの出力の論理関数 を AIG に変換する．
vector<FraigHandle>
FraigEnc::operator()(
  const BnNetwork& network,
  const vector<FraigHandle>& input_handles
)
{
  // network のノードの番号をキーとして対応するハンドルを収める配列
  vector<FraigHandle> h_map(network.node_num());

  //////////////////////////////////////////////////////////////////////
  // 外部入力に対応するハンドルを登録する．
  //////////////////////////////////////////////////////////////////////
  SizeType ni = network.input_num();
  ASSERT_COND( input_handles.size() == ni );
  for ( auto i: Range(ni) ) {
    SizeType id = network.input_id(i);
    h_map[id] = input_handles[i];
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  SizeType nl = network.logic_num();
  for ( auto i: Range(nl) ) {
    SizeType id = network.logic_id(i);
    auto& node = network.node(id);

    // ファンインのノードに対応するハンドルを求める．
    SizeType ni = node.fanin_num();
    vector<FraigHandle> fanin_handles(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      fanin_handles[i] = h_map[node.fanin_id(i)];
    }

    // 個々の関数タイプに従って fraig を生成する．
    BnNodeType logic_type = node.type();
    FraigHandle ans;
    switch ( logic_type ) {
    case BnNodeType::C0:
      ans = make_zero();
      break;

    case BnNodeType::C1:
      ans = make_one();
      break;

    case BnNodeType::Buff:
      ans = make_buff(fanin_handles[0]);
      break;

    case BnNodeType::Not:
      ans = make_not(fanin_handles[0]);
      break;

    case BnNodeType::And:
      ans = make_and(fanin_handles);
      break;

    case BnNodeType::Nand:
      ans = make_nand(fanin_handles);
      break;

    case BnNodeType::Or:
      ans = make_or(fanin_handles);
      break;

    case BnNodeType::Nor:
      ans = make_nor(fanin_handles);
      break;

    case BnNodeType::Xor:
      ans = make_xor(fanin_handles);
      break;

    case BnNodeType::Xnor:
      ans = make_xnor(fanin_handles);
      break;

    case BnNodeType::Expr:
      ans = expr2aig(network.expr(node.expr_id()), fanin_handles);
      break;

    case BnNodeType::TvFunc:
      mTvMap.clear();
      ans = tv2aig(network.func(node.func_id()), 0, fanin_handles);
      break;

    case BnNodeType::Bdd:
      ans = bdd2aig(node.bdd(), fanin_handles);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }

    // 登録しておく．
    h_map[id] = ans;
  }

  //////////////////////////////////////////////////////////////////////
  // 外部出力のマップを作成する．
  //////////////////////////////////////////////////////////////////////
  SizeType no = network.output_num();
  vector<FraigHandle> output_handles(no);
  for ( auto i: Range(no) ) {
    SizeType iid = network.output_src_id(i);
    output_handles[i] = h_map[iid];
  }

  return output_handles;
}

// @brief 論理式に対応するノード(木)をつくる．
FraigHandle
FraigEnc::expr2aig(
  const Expr& expr,
  const vector<FraigHandle>& fanin_handles
)
{
  if ( expr.is_zero() ) {
    return make_zero();
  }
  if ( expr.is_one() ) {
    return make_one();
  }
  if ( expr.is_posi_literal() ) {
    VarId var = expr.varid();
    int id = var.val();
    ASSERT_COND( id < fanin_handles.size() );
    return fanin_handles[id];
  }
  if ( expr.is_nega_literal() ) {
    VarId var = expr.varid();
    int id = var.val();
    ASSERT_COND( id < fanin_handles.size() );
    return ~fanin_handles[id];
  }

  SizeType n = expr.child_num();
  vector<FraigHandle> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    edge_list[i] = expr2aig(expr.child(i), fanin_handles);
  }
  if ( expr.is_and() ) {
    return make_and(edge_list);
  }
  if ( expr.is_or() ) {
    return make_or(edge_list);
  }
  if ( expr.is_xor() ) {
    return make_xor(edge_list);
  }

  ASSERT_NOT_REACHED;
  return make_zero();
}

// @brief TvFunc を AIG に変換する．
FraigHandle
FraigEnc::tv2aig(
  const TvFunc& func,
  SizeType pos,
  const vector<FraigHandle>& fanin_handles
)
{
  if ( func.is_zero() ) {
    return make_zero();
  }
  if ( func.is_one() ) {
    return make_one();
  }

  if ( mTvMap.count(func) > 0 ) {
    return mTvMap.at(func);
  }

  for ( ; pos < fanin_handles.size(); ++ pos ) {
    VarId var{pos};
    auto f0 = func.cofactor(var, true);
    auto f1 = func.cofactor(var, false);
    if ( f0 != f1 ) {
      auto r0 = tv2aig(f0, pos + 1, fanin_handles);
      auto r1 = tv2aig(f1, pos + 1, fanin_handles);
      return make_mux(fanin_handles[pos], r0, r1);
    }
  }
  ASSERT_NOT_REACHED;
  return make_zero(); // ダミー
}

BEGIN_NONAMESPACE

// @brief 枝をAIGに変換する．
inline
FraigHandle
edge2aig(
  SizeType edge,
  const vector<FraigHandle>& h_map
)
{
  if ( edge == 0 ) {
    return FraigHandle::zero();
  }
  if ( edge == 1 ) {
    return FraigHandle::one();
  }
  SizeType node = BddInfo::edge2node(edge);
  bool inv = BddInfo::edge2inv(edge);
  return h_map[node] ^ inv;
}

END_NONAMESPACE

// @brief BDD を AIG に変換する．
FraigHandle
FraigEnc::bdd2aig(
  const Bdd& bdd,
  const vector<FraigHandle>& fanin_handles
)
{
  SizeType root_edge;
  auto node_list = bdd.node_info(root_edge);
  // インデックスごとのノードリストを作る．
  SizeType max_index = 0;
  for ( auto node: node_list ) {
    if ( max_index < node.index ) {
      max_index = node.index;
    }
  }
  ++ max_index;
  vector<vector<BddInfo>> indexed_node_list(max_index);
  for ( auto node: node_list ) {
    indexed_node_list[node.index].push_back(node);
  }
  // 下位のインデックスから AIG を作る．
  vector<FraigHandle> h_map(node_list.size() + 1);
  for ( SizeType i = 0; i < max_index; ++ i ) {
    auto cedge = fanin_handles[max_index - i - 1];
    for ( auto node: indexed_node_list[max_index - i - 1] ) {
      auto r0 = edge2aig(node.edge0, h_map);
      auto r1 = edge2aig(node.edge1, h_map);
      auto r = make_mux(cedge, r0, r1);
      h_map[node.id] = r;
    }
  }
  return edge2aig(root_edge, h_map);
}

// @brief Shanon 展開のマージを行う．
FraigHandle
FraigEnc::make_mux(
  FraigHandle cedge,
  FraigHandle edge0,
  FraigHandle edge1
)
{
  if ( edge0.is_zero() ) {
    if ( edge1.is_zero() ) {
      return edge0;
    }
    if ( edge1.is_one() ) {
      return cedge;
    }
    return make_and(cedge, edge1);
  }
  if ( edge0.is_one() ) {
    if ( edge1.is_zero() ) {
      return ~cedge;
    }
    if ( edge1.is_one() ) {
      return edge0;
    }
    return make_or(~cedge, edge1);
  }
  if ( edge1.is_zero() ) {
    return make_and(~cedge, edge0);
  }
  if ( edge1.is_one() ) {
    return make_or(cedge, edge0);
  }
  auto tmp0 = make_and(~cedge, edge0);
  auto tmp1 = make_and(cedge, edge1);
  auto ans = make_or(tmp0, tmp1);
  return ans;
}

// @brief make_and() の下請け関数
FraigHandle
FraigEnc::_make_and(
  const vector<FraigHandle>& edge_list,
  SizeType start_pos,
  SizeType end_pos,
  bool iinv
)
{
  ASSERT_COND( start_pos < end_pos );

  SizeType n = end_pos - start_pos;
  if ( n == 1 ) {
    FraigHandle h = edge_list[start_pos];
    if ( iinv ) {
      h = ~h;
    }
    return h;
  }
  // n >= 2
  SizeType mid_pos = start_pos + (n + 1) / 2;
  FraigHandle h0 = _make_and(edge_list, start_pos, mid_pos, iinv);
  FraigHandle h1 = _make_and(edge_list, mid_pos, end_pos, iinv);
  return make_and(h0, h1);
}

// @brief make_xor() の下請け関数
FraigHandle
FraigEnc::_make_xor(
  const vector<FraigHandle>& edge_list,
  SizeType start_pos,
  SizeType end_pos
)
{
  ASSERT_COND( start_pos < end_pos );

  SizeType n = end_pos - start_pos;
  if ( n == 1 ) {
    return edge_list[start_pos];
  }
  // n >= 2
  SizeType mid_pos = start_pos + (n + 1) / 2;
  FraigHandle h0 = _make_xor(edge_list, start_pos, mid_pos);
  FraigHandle h1 = _make_xor(edge_list, mid_pos, end_pos);
  FraigHandle tmp1 = make_and( h0, ~h1);
  FraigHandle tmp2 = make_and(~h0,  h1);
  return make_or(tmp1, tmp2);
}

// @brief 2つのノードの AND を作る．
FraigHandle
FraigEnc::make_and(
  FraigHandle edge1,
  FraigHandle edge2
)
{
  return mMgr.make_and(edge1, edge2);
}

END_NAMESPACE_FRAIG
