
/// @file Bn2FraigConv.cc
/// @brief Bn2FraigConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Bn2FraigConv.h"
#include "FraigMgr.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/TvFunc.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_EQUIV

// @brief コンストラクタ
// @param[in] fraig_mgr FraigMgr
// @param[in] node_num ノード数(ノードIDの最大値+1)
Bn2FraigConv::Bn2FraigConv(FraigMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
Bn2FraigConv::~Bn2FraigConv()
{
}

// @brief ネットワークの構造に対応する Fraig を作る．
// @param[in] src_network 元となるネットワーク
// @param[in] input_list 入力ノード番号のリスト
// @param[in] output_list 出力ノード番号のリスト
// @param[in] input_handles 入力のハンドルのリスト
// @param[out] output_handles 出力のハンドルのリスト
void
Bn2FraigConv::operator()(const BnNetwork& src_network,
			 const vector<int>& input_list,
			 const vector<int>& output_list,
			 const vector<FraigHandle>& input_handles,
			 vector<FraigHandle>& output_handles)
{
  mHandleMap.clear();
  mHandleMap.resize(src_network.node_num());

  //////////////////////////////////////////////////////////////////////
  // 外部入力に対応するハンドルを登録する．
  //////////////////////////////////////////////////////////////////////
  int ni = src_network.input_num();
  for ( auto i: Range(ni) ) {
    auto id = input_list[i];
    put_handle(id, input_handles[i]);
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  int nl = src_network.logic_num();
  for ( auto i: Range(nl) ) {
    auto bnnode = src_network.logic(i);
    make_handle(bnnode);
  }

  //////////////////////////////////////////////////////////////////////
  // 外部出力のマップを作成する．
  //////////////////////////////////////////////////////////////////////
  int no = src_network.output_num();
  for ( auto i: Range(no) ) {
    auto bnnode = src_network.output(i);
    make_handle(bnnode);
  }

  //////////////////////////////////////////////////////////////////////
  // 順序を考慮して output_handles に入れる．
  //////////////////////////////////////////////////////////////////////
  for ( auto i: Range(no) ) {
    auto id = output_list[i];
    output_handles[i] = get_handle(id);
  }
}

// @brief BnNode に対応するハンドルを作る．
// @param[in] node 対象のノード
// @return 生成したハンドルを返す．
//
// node のファンイン側の構造は Fraig 化されていると仮定する．
FraigHandle
Bn2FraigConv::make_handle(const BnNode* node)
{
  if ( node->is_output() ) {
    auto h = get_handle(node->fanin());
    put_handle(node->id(), h);
  }

  // ファンインのノードに対応するハンドルを求める．
  int ni = node->fanin_num();
  vector<FraigHandle> fanin_handles(ni);
  for ( int i = 0; i < ni; ++ i ) {
    fanin_handles[i] = get_handle(node->fanin(i));
  }

  // 個々の関数タイプに従って fraig を生成する．
  BnNodeType logic_type = node->type();
  FraigHandle ans;
  switch ( logic_type ) {
  case BnNodeType::C0:
    ans = mMgr.make_zero();
    break;

  case BnNodeType::C1:
    ans = mMgr.make_one();
    break;

  case BnNodeType::Buff:
    ans = mMgr.make_buff(fanin_handles[0]);
    break;

  case BnNodeType::Not:
    ans = mMgr.make_not(fanin_handles[0]);
    break;

  case BnNodeType::And:
    ans = mMgr.make_and(fanin_handles);
    break;

  case BnNodeType::Nand:
    ans = mMgr.make_nand(fanin_handles);
    break;

  case BnNodeType::Or:
    ans = mMgr.make_or(fanin_handles);
    break;

  case BnNodeType::Nor:
    ans = mMgr.make_nor(fanin_handles);
    break;

  case BnNodeType::Xor:
    ans = mMgr.make_xor(fanin_handles);
    break;

  case BnNodeType::Xnor:
    ans = mMgr.make_xnor(fanin_handles);
    break;

  case BnNodeType::Expr:
    ans = mMgr.make_expr(node->expr(), fanin_handles);
    break;

  case BnNodeType::TvFunc:
    {
      TvFunc tv = node->func();
      // 未完
    }
    ASSERT_NOT_REACHED;
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  // 登録しておく．
  put_handle(node->id(), ans);

  return ans;
}

END_NAMESPACE_EQUIV
