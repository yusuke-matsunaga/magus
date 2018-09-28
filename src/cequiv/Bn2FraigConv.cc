
/// @file Bn2FraigConv.cc
/// @brief Bn2FraigConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Bn2FraigConv.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_CEC

// @brief コンストラクタ
// @param[in] fraig_mgr FraigMgr
Bn2FraigConv::Bn2FraigConv(FraigMgr& fraig_mgr) :
  mFraigMgr(fraig_mgr)
{
}

// @brief デストラクタ
Bn2FraigConv::~Bn2FraigConv()
{
}

// @brief ネットワークの構造に対応する Fraig を作る．
// @param[in] src_network 元となるネットワーク
// @param[in] input_handles 入力のノード番号と対応するハンドルの対のリスト
void
Bn2FraigConv::convert(const BnNetwork& src_network,
		      const vector<pair<int, FraigHandle>>& input_handles)
{
  int ni = src_network.input_num();
  int nl = src_network.logic_num();

  ASSERT_COND( ni == input_handles.size() );

  mHandleMap.clear();

  //////////////////////////////////////////////////////////////////////
  // 外部入力のマップを作成する．
  //////////////////////////////////////////////////////////////////////
  for ( int i = 0; i < ni; ++ i ) {
    auto node_id = input_handles[i].first;
    auto h = input_handles[i].second;
    mHandleMap.add(node_id, h);
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for ( int i = 0; i < nl; ++ i ) {
    const BnNode* bnnode = src_network.logic(i);
    FraigHandle h = node2handle(bnnode);
    mHandleMap.add(bnnode->id(), h);
  }
}

// @brief ノード番号に対応するハンドルを返す．
// @param[in] node_id ノード番号
FraigHandle
Bn2FraigConv::get_handle(int node_id)
{
  FraigHandle handle;
  if ( mHandleMap.find(node_id, handle) ) {
    return handle;
  }
  return FraigHandle();
}

// @brief BnNode に対応するハンドルを作る．
// @param[in] node 対象のノード
//
// node のファンイン側の構造は Fraig 化されていると仮定する．
FraigHandle
Bn2FraigConv::node2handle(const BnNode* node)
{
  // ファンインのノードに対応するハンドルを求める．
  int ni = node->fanin_num();
  vector<FraigHandle> fanin_handles(ni);
  for ( int i = 0; i < ni; ++ i ) {
    int inode_id = node->fanin(i);
    FraigHandle ih = get_handle(inode_id);
    fanin_handles[i] = ih;
  }

  // 個々の関数タイプに従って fraig を生成する．
  BnNodeType logic_type = node->type();
  switch ( logic_type ) {
  case BnNodeType::C0:
    return mFraigMgr.make_zero();

  case BnNodeType::C1:
    return mFraigMgr.make_one();

  case BnNodeType::Buff:
    return fanin_handles[0];

  case BnNodeType::Not:
    return ~fanin_handles[0];

  case BnNodeType::And:
    return mFraigMgr.make_and(fanin_handles);

  case BnNodeType::Nand:
    return ~mFraigMgr.make_and(fanin_handles);

  case BnNodeType::Or:
    return mFraigMgr.make_or(fanin_handles);

  case BnNodeType::Nor:
    return ~mFraigMgr.make_or(fanin_handles);

  case BnNodeType::Xor:
    return mFraigMgr.make_xor(fanin_handles);

  case BnNodeType::Xnor:
    return ~mFraigMgr.make_xor(fanin_handles);

  case BnNodeType::Expr:
    return mFraigMgr.make_logic(node->expr(), fanin_handles);

  case BnNodeType::TvFunc:
    {
      TvFunc tv = node->func();
      // 未完
    }
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return FraigHandle();
}

END_NAMESPACE_YM_CEC
