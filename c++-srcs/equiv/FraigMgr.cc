
/// @file FraigMgr.cc
/// @brief FraigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgr.h"
#include "FraigMgrImpl.h"
#include "FraigNode.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/BnNodeType.h"
#include "ym/Range.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif

#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif


BEGIN_NAMESPACE_FRAIG

BEGIN_NONAMESPACE

const int debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// FraigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigMgr::FraigMgr(
  SizeType sig_size,
  const SatSolverType& solver_type
) : mRep{new FraigMgrImpl(sig_size, solver_type)}
{
}

// @brief デストラクタ
FraigMgr::~FraigMgr()
{
}

// @brief 入力ノード数を得る．
SizeType
FraigMgr::input_num() const
{
  return mRep->input_num();
}

// @brief 入力ノードを取り出す．
FraigNode*
FraigMgr::input_node(
  SizeType pos
) const
{
  return mRep->input_node(pos);
}

// @brief ノード数を得る．
SizeType
FraigMgr::node_num() const
{
  return mRep->node_num();
}

// @brief ノードを取り出す．
FraigNode*
FraigMgr::node(
  SizeType pos
) const
{
  return mRep->node(pos);
}

// @brief 外部入力を作る．
FraigHandle
FraigMgr::make_input()
{
  return mRep->make_input();
}

// @brief 2つのノードの AND を取る．
FraigHandle
FraigMgr::make_and(
  FraigHandle handle1,
  FraigHandle handle2
)
{
  return mRep->make_and(handle1, handle2);
}

// @brief 論理式に対応するノード(木)をつくる．
FraigHandle
FraigMgr::make_expr(
  const Expr& expr,
  const vector<FraigHandle>& inputs
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
    ASSERT_COND( id < inputs.size() );
    return inputs[id];
  }
  if ( expr.is_nega_literal() ) {
    VarId var = expr.varid();
    int id = var.val();
    ASSERT_COND( id < inputs.size() );
    return ~inputs[id];
  }

  SizeType n = expr.child_num();
  vector<FraigHandle> edge_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    edge_list[i] = make_expr(expr.child(i), inputs);
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

// @brief 真理値表に対応するノード(木)を作る．
FraigHandle
FraigMgr::make_tv(
  const TvFunc& func,
  const vector<FraigHandle>& inputs
)
{
  // 単純に Shannon's decomposition を用いる．
  auto ans = _make_tv(func, inputs, 0);
  return ans;
}

// @brief make_tv() の下請け関数
FraigHandle
FraigMgr::_make_tv(
  const TvFunc& func,
  const vector<FraigHandle>& inputs,
  SizeType pos
)
{
  if ( func.is_zero() ) {
    return FraigHandle::zero();
  }
  if ( func.is_one() ) {
    return FraigHandle::one();
  }

  for ( ; pos < inputs.size(); ++ pos ) {
    VarId var{pos};
    auto f0 = func.cofactor(var, true);
    auto f1 = func.cofactor(var, false);
    if ( f0 != f1 ) {
      auto r0 = _make_tv(f0, inputs, pos + 1);
      auto r1 = _make_tv(f1, inputs, pos + 1);
      return merge(inputs[pos], r0, r1);
    }
  }
  ASSERT_NOT_REACHED;
  return FraigHandle::zero(); // ダミー
}

// @brief BDDに対応するノード(木)を作る．
FraigHandle
FraigMgr::make_bdd(
  const Bdd& func,
  const vector<FraigHandle>& inputs
)
{
  if ( func.is_zero() ) {
    return FraigHandle::zero();
  }
  if ( func.is_one() ) {
    return FraigHandle::one();
  }
  Bdd f0;
  Bdd f1;
  auto top = func.root_decomp(f0, f1);
  auto r0 = make_bdd(f0, inputs);
  auto r1 = make_bdd(f1, inputs);
  return merge(inputs[top.val()], r0, r1);
}

// @brief Shanon 展開のマージを行う．
FraigHandle
FraigMgr::merge(
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

// @brief コファクターを計算する．
FraigHandle
FraigMgr::make_cofactor(
  FraigHandle edge,
  int input_id,
  bool inv
)
{
  if ( edge.is_const() ) {
    // edge が定数の時は変更なし
    return edge;
  }

  FraigNode* node = edge.node();
  FraigHandle ans;
  if ( node->is_input() ) {
    // 入力ノード時は番号が input_id どうかで処理が変わる．
    if ( node->input_id() == input_id ) {
      if ( inv ) {
	ans = make_zero();
      }
      else {
	ans = make_one();
      }
    }
    else {
      ans = FraigHandle(node, false);
    }
  }
  else {
    // AND ノードの場合
    // 2つの子供に再帰的な処理を行って結果の AND を計算する．
    FraigHandle new_handle0 = make_cofactor(node->fanin0_handle(), input_id, inv);
    FraigHandle new_handle1 = make_cofactor(node->fanin1_handle(), input_id, inv);
    FraigHandle ans = make_and(new_handle0, new_handle1);
  }
  if ( edge.inv() ) {
    ans = ~ans;
  }
  return ans;
}

// @brief BnNetwork をインポートする．
vector<FraigHandle>
FraigMgr::import_subnetwork(
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
    int id = network.input_id(i);
    h_map[id] = input_handles[i];
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  SizeType nl = network.logic_num();
  for ( auto i: Range(nl) ) {
    int id = network.logic_id(i);
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
      ans = make_expr(network.expr(node.expr_id()), fanin_handles);
      break;

    case BnNodeType::TvFunc:
      ans = make_tv(network.func(node.func_id()), fanin_handles);
      break;

    case BnNodeType::Bdd:
      ans = make_bdd(node.bdd(), fanin_handles);
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

// @brief 複数のノードの AND を取る．
FraigHandle
FraigMgr::_make_and(
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

// @brief 複数のノードの XOR を取る．
FraigHandle
FraigMgr::_make_xor(
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
  return make_xor(h0, h1);
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
FraigMgr::check_equiv(
  FraigHandle aig1,
  FraigHandle aig2
)
{
  return mRep->check_equiv(aig1, aig2);
}

// @brief ログレベルを設定する．
void
FraigMgr::set_loglevel(
  SizeType level
)
{
  mRep->set_loglevel(level);
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgr::set_logstream(
  ostream* out
)
{
  mRep->set_logstream(out);
}

// @brief 内部の統計情報を出力する．
void
FraigMgr::dump_stats(
  ostream& s
)
{
  mRep->dump_stats(s);
}

END_NAMESPACE_FRAIG
