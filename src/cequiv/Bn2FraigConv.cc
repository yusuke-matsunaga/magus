
/// @file Bn2FraigConv.cc
/// @brief Bn2FraigConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Bn2FraigConv.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/BnFuncType.h"
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
// @param[in] inputs 外部入力のハンドルのリスト
void
Bn2FraigConv::convert(const BnNetwork& src_network,
		      const vector<FraigHandle>& inputs)
{
  ymuint bn_nn = src_network.node_num();
  ymuint ni = src_network.input_num();
  ymuint nl = src_network.logic_num();

  ASSERT_COND( ni == inputs.size() );

  mHandleMap.clear();
  mHandleMap.resize(bn_nn);

  //////////////////////////////////////////////////////////////////////
  // 外部入力のマップを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < ni; ++ i) {
    const BnNode* bnnode = src_network.input(i);
    FraigHandle h = inputs[i];
    mHandleMap[bnnode->id()] = h;
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  vector<const BnNode*> sorted_node_list;
  src_network.topological_sort(sorted_node_list);
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = sorted_node_list[i];
    FraigHandle h = node2handle(bnnode);
    mHandleMap[bnnode->id()] = h;
  }
}

// @brief ノード番号に対応するハンドルを返す．
// @param[in] node_id ノード番号
FraigHandle
Bn2FraigConv::get_handle(ymuint node_id)
{
  return mHandleMap[node_id];
}

// @brief BnNode に対応するハンドルを作る．
// @param[in] node 対象のノード
//
// node のファンイン側の構造は Fraig 化されていると仮定する．
FraigHandle
Bn2FraigConv::node2handle(const BnNode* node)
{
  // ファンインのノードに対応するハンドルを求める．
  ymuint ni = node->fanin_num();
  vector<FraigHandle> fanin_handles(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint inode_id = node->fanin_id(i);
    FraigHandle ih = get_handle(inode_id);
    fanin_handles[i] = ih;
  }

  // 個々の関数タイプに従って fraig を生成する．
  const BnFuncType* func_type = node->func_type();
  switch ( func_type->type() ) {
  case BnFuncType::kFt_C0:
    return mFraigMgr.make_zero();

  case BnFuncType::kFt_C1:
    return mFraigMgr.make_one();

  case BnFuncType::kFt_BUFF:
    return fanin_handles[0];

  case BnFuncType::kFt_NOT:
    return ~fanin_handles[0];

  case BnFuncType::kFt_AND:
    return mFraigMgr.make_and(fanin_handles);

  case BnFuncType::kFt_NAND:
    return ~mFraigMgr.make_and(fanin_handles);

  case BnFuncType::kFt_OR:
    return mFraigMgr.make_or(fanin_handles);

  case BnFuncType::kFt_NOR:
    return ~mFraigMgr.make_or(fanin_handles);

  case BnFuncType::kFt_XOR:
    return mFraigMgr.make_xor(fanin_handles);

  case BnFuncType::kFt_XNOR:
    return ~mFraigMgr.make_xor(fanin_handles);

  case BnFuncType::kFt_CELL:
    {
      const Cell* cell = func_type->cell();
    }
    break;

  case BnFuncType::kFt_EXPR:
    return mFraigMgr.make_logic(func_type->expr(), fanin_handles);

  case BnFuncType::kFt_TV:
    {
      TvFunc tv = func_type->truth_vector();
    }
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return FraigHandle();
}

END_NAMESPACE_YM_CEC
