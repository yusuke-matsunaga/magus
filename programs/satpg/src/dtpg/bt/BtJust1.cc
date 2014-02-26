
/// @file BtJust1.cc
/// @brief BtJust1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJust1.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'Just1' タイプの生成を行なう．
// @param[in] tvmgr TvMgr
// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
BackTracer*
new_BtJust1(TvMgr& tvmgr,
	    ymuint max_id)
{
  return new BtJust1(tvmgr, max_id);
}


//////////////////////////////////////////////////////////////////////
// クラス BtJust1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tvmgr TvMgr
// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
BtJust1::BtJust1(TvMgr& tvmgr,
		 ymuint max_id) :
  BtJustBase(tvmgr, max_id)
{
}

// @brief デストラクタ
BtJust1::~BtJust1()
{
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
TestVector*
BtJust1::operator()(TpgNode* fnode,
		    const vector<Bool3>& model,
		    const vector<TpgNode*>& input_list,
		    const vector<TpgNode*>& output_list)
{
  TestVector* tv = new_vector();

  // 故障差の伝搬している外部出力を選ぶ．
  TpgNode* onode = NULL;
  for (vector<TpgNode*>::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    TpgNode* node = *p;
    if ( node_dval(node, model) == kB3True ) {
      onode = node;
      break;
    }
  }
  assert_cond( onode != NULL, __FILE__, __LINE__);

  // 正当化を行う．
  justify(onode, model);

  // 一連の処理でつけたマークを消す．
  clear_justified();

  return tv;
}

// @brief solve 中で変数割り当ての正当化を行なう．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @note node の値割り当てを正当化する．
// @note 正当化に用いられているノードには mJustifiedMark がつく．
// @note mJustifiedMmark がついたノードは mJustifiedNodeList に格納される．
void
BtJust1::justify(TpgNode* node,
		 const vector<Bool3>& model)
{
  if ( justified_mark(node) ) {
    return;
  }
  set_justified(node);

  if ( node->is_input() ) {
    // val を記録
    record_value(node, model);
    return;
  }

  Bool3 gval = node_gval(node, model);
  Bool3 fval = node_fval(node, model);

  if ( gval != fval ) {
    // 正常値と故障値が異なっていたら
    // すべてのファンインをたどる．
    just_sub1(node, model);
    return;
  }

  if ( node->is_cplx_logic() ) {
    ymuint np = node->primitive_num();
    TpgPrimitive* prim = node->primitive(np - 1);
    justify_primitive(prim, node, model);
  }
  else {
    switch ( node->gate_type() ) {
    case kTgGateBuff:
    case kTgGateNot:
      // 無条件で唯一のファンインをたどる．
      justify(node->fanin(0), model);
      break;

    case kTgGateAnd:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	just_sub1(node, model);
      }
      else if ( gval == kB3False ) {
	// 0の値を持つ最初のノードをたどる．
	just_sub2(node, model, kB3False);
      }
      break;

    case kTgGateNand:
      if ( gval == kB3True ) {
	// 0の値を持つ最初のノードをたどる．
	just_sub2(node, model, kB3False);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	just_sub1(node, model);
      }
      break;

    case kTgGateOr:
      if ( gval == kB3True ) {
	// 1の値を持つ最初のノードをたどる．
	just_sub2(node, model, kB3True);
      }
      else if ( gval == kB3False ) {
	// すべてのファンインノードをたどる．
	just_sub1(node, model);
      }
      break;

    case kTgGateNor:
      if ( gval == kB3True ) {
	// すべてのファンインノードをたどる．
	just_sub1(node, model);
      }
      else if ( gval == kB3False ) {
	// 1の値を持つ最初のノードをたどる．
	just_sub2(node, model, kB3True);
      }
      break;

    case kTgGateXor:
    case kTgGateXnor:
      // すべてのファンインノードをたどる．
      just_sub1(node, model);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief すべてのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
void
BtJust1::just_sub1(TpgNode* node,
		   const vector<Bool3>& model)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    justify(inode, model);
  }
}

// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] val 値
void
BtJust1::just_sub2(TpgNode* node,
		   const vector<Bool3>& model,
		   Bool3 val)
{
  bool gfound = false;
  bool ffound = false;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Bool3 igval = node_gval(inode, model);
    Bool3 ifval = node_fval(inode, model);
    if ( !gfound && igval == val ) {
      justify(inode, model);
      gfound = true;
      if ( ifval == val ) {
	break;
      }
    }
    else if ( !ffound && ifval == val ) {
      justify(inode, model);
      ffound = true;
    }
    if ( gfound && ffound ) {
      break;
    }
  }
}

// @brief justify の下請け関数
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @note node の値割り当てを正当化する．
void
BtJust1::justify_primitive(TpgPrimitive* prim,
			   TpgNode* node,
			   const vector<Bool3>& model)
{
  if ( prim->is_input() ) {
    ymuint ipos = prim->input_id();
    TpgNode* inode = node->fanin(ipos);
    justify(inode, model);
    return;
  }

  Bool3 gval = primitive_gval(prim, model);
  Bool3 fval = primitive_fval(prim, model);
  if ( gval != fval ) {
    // すべてのファンインノードをたどる．
    jp_sub1(prim, node, model);
    return;
  }

  switch ( prim->gate_type() ) {
  case kTgGateBuff:
  case kTgGateNot:
    // 唯一のファンインをたどる．
    justify_primitive(prim->fanin(0), node, model);
    break;

  case kTgGateAnd:
    if ( gval == kB3True ) {
      // すべてのファンインをたどる．
      jp_sub1(prim, node, model);
    }
    else if ( gval == kB3False ) {
      // 0 の値を持つ最初のファンインをたどる．
      jp_sub2(prim, node, model, kB3False);
    }
    break;

  case kTgGateNand:
    if ( gval == kB3True ) {
      // 0 の値を持つ最初のファンインをたどる．
      jp_sub2(prim, node, model, kB3False);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインをたどる．
      jp_sub1(prim, node, model);
    }
    break;

  case kTgGateOr:
    if ( gval == kB3True ) {
      // 1の値をもつ最初のファンインをたどる．
      jp_sub2(prim, node, model, kB3True);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインをたどる．
      jp_sub1(prim, node, model);
    }
    break;

  case kTgGateNor:
    if ( gval == kB3True ) {
      // すべてのファンインをたどる．
      jp_sub1(prim, node, model);
    }
    else if ( gval == kB3False ) {
      // 1の値をもつすべてのファンインをたどる．
      jp_sub2(prim, node, model, kB3True);
    }
    break;

  case kTgGateXor:
  case kTgGateXnor:
    // すべてのファンインをたどる．
    jp_sub1(prim, node, model);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
void
BtJust1::jp_sub1(TpgPrimitive* prim,
		 TpgNode* node,
		 const vector<Bool3>& model)
{
  ymuint ni = prim->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgPrimitive* iprim = prim->fanin(i);
    justify_primitive(iprim, node, model);
  }
}

// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
// @param[in] prim 対象のプリミティブ
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] val 値
void
BtJust1::jp_sub2(TpgPrimitive* prim,
		 TpgNode* node,
		 const vector<Bool3>& model,
		 Bool3 val)
{
  bool gfound = false;
  bool ffound = false;
  ymuint ni = prim->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgPrimitive* iprim = prim->fanin(i);
    Bool3 igval = primitive_gval(iprim, model);
    Bool3 ifval = primitive_fval(iprim, model);
    if ( !gfound && igval == val ) {
      justify_primitive(iprim, node, model);
      gfound = true;
      if ( ifval == val ) {
	break;
      }
    }
    else if ( !ffound && ifval == val ) {
      justify_primitive(iprim, node, model);
      ffound = true;
    }
    if ( gfound && ffound ) {
      break;
    }
  }
}

END_NAMESPACE_YM_SATPG
