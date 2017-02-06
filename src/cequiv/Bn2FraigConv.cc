
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
// @param[in] inputs 外部入力のハンドルのリスト
void
Bn2FraigConv::convert(const BnNetwork& src_network,
		      const vector<FraigHandle>& inputs)
{
  ymuint ni = src_network.input_num();
  ymuint nl = src_network.logic_num();

  ASSERT_COND( ni == inputs.size() );

  mHandleMap.clear();

  //////////////////////////////////////////////////////////////////////
  // 外部入力のマップを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < ni; ++ i) {
    const BnNode* bnnode = src_network.input(i);
    FraigHandle h = inputs[i];
    mHandleMap.add(bnnode->id(), h);
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for (ymuint i = 0; i < nl; ++ i) {
    const BnNode* bnnode = src_network.logic(i);
    FraigHandle h = node2handle(bnnode);
    mHandleMap.add(bnnode->id(), h);
  }
}

// @brief ノード番号に対応するハンドルを返す．
// @param[in] node_id ノード番号
FraigHandle
Bn2FraigConv::get_handle(ymuint node_id)
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
  ymuint ni = node->fanin_num();
  vector<FraigHandle> fanin_handles(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint inode_id = node->fanin(i);
    FraigHandle ih = get_handle(inode_id);
    fanin_handles[i] = ih;
  }

  // 個々の関数タイプに従って fraig を生成する．
  BnLogicType logic_type = node->logic_type();
  switch ( logic_type ) {
  case kBnLt_C0:
    return mFraigMgr.make_zero();

  case kBnLt_C1:
    return mFraigMgr.make_one();

  case kBnLt_BUFF:
    return fanin_handles[0];

  case kBnLt_NOT:
    return ~fanin_handles[0];

  case kBnLt_AND:
    return mFraigMgr.make_and(fanin_handles);

  case kBnLt_NAND:
    return ~mFraigMgr.make_and(fanin_handles);

  case kBnLt_OR:
    return mFraigMgr.make_or(fanin_handles);

  case kBnLt_NOR:
    return ~mFraigMgr.make_or(fanin_handles);

  case kBnLt_XOR:
    return mFraigMgr.make_xor(fanin_handles);

  case kBnLt_XNOR:
    return ~mFraigMgr.make_xor(fanin_handles);

  case kBnLt_EXPR:
    return mFraigMgr.make_logic(node->expr(), fanin_handles);

  case kBnLt_TV:
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
