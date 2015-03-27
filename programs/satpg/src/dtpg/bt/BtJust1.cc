
/// @file BtJust1.cc
/// @brief BtJust1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJust1.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'Just1' タイプの生成を行なう．
BackTracer*
new_BtJust1()
{
  return new BtJust1();
}


//////////////////////////////////////////////////////////////////////
// クラス BtJust1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtJust1::BtJust1()
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
// @param[out] assign_list 値の割当リスト
void
BtJust1::operator()(TpgNode* fnode,
		    const vector<Bool3>& model,
		    const vector<TpgNode*>& input_list,
		    const vector<TpgNode*>& output_list,
		    NodeValList& assign_list)
{
  assign_list.clear();

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
  ASSERT_COND( onode != NULL );

  // 正当化を行う．
  justify(onode, model, assign_list);

  // 一連の処理でつけたマークを消す．
  clear_justified();
}

// @brief solve 中で変数割り当ての正当化を行なう．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @note node の値割り当てを正当化する．
// @note 正当化に用いられているノードには mJustifiedMark がつく．
// @note mJustifiedMmark がついたノードは mJustifiedNodeList に格納される．
void
BtJust1::justify(TpgNode* node,
		 const vector<Bool3>& model,
		 NodeValList& assign_list)
{
  if ( justified_mark(node) ) {
    return;
  }
  set_justified(node);

  if ( node->is_input() ) {
    // val を記録
    record_value(node, model, assign_list);
    return;
  }

  Bool3 gval = node_gval(node, model);
  Bool3 fval = node_fval(node, model);

  if ( gval != fval ) {
    // 正常値と故障値が異なっていたら
    // すべてのファンインをたどる．
    just_sub1(node, model, assign_list);
    return;
  }

  switch ( node->gate_type() ) {
  case kTgGateBuff:
  case kTgGateNot:
    // 無条件で唯一のファンインをたどる．
    justify(node->fanin(0), model, assign_list);
    break;

  case kTgGateAnd:
    if ( gval == kB3True ) {
      // すべてのファンインノードをたどる．
      just_sub1(node, model, assign_list);
    }
    else if ( gval == kB3False ) {
      // 0の値を持つ最初のノードをたどる．
      just_sub2(node, model, kB3False, assign_list);
    }
    break;

  case kTgGateNand:
    if ( gval == kB3True ) {
      // 0の値を持つ最初のノードをたどる．
      just_sub2(node, model, kB3False, assign_list);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインノードをたどる．
      just_sub1(node, model, assign_list);
    }
    break;

  case kTgGateOr:
    if ( gval == kB3True ) {
      // 1の値を持つ最初のノードをたどる．
      just_sub2(node, model, kB3True, assign_list);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインノードをたどる．
      just_sub1(node, model, assign_list);
    }
    break;

  case kTgGateNor:
    if ( gval == kB3True ) {
      // すべてのファンインノードをたどる．
      just_sub1(node, model, assign_list);
    }
    else if ( gval == kB3False ) {
      // 1の値を持つ最初のノードをたどる．
      just_sub2(node, model, kB3True, assign_list);
    }
    break;

  case kTgGateXor:
  case kTgGateXnor:
    // すべてのファンインノードをたどる．
    just_sub1(node, model, assign_list);
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
}

// @brief すべてのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[out] assign_list 値の割当リスト
void
BtJust1::just_sub1(TpgNode* node,
		   const vector<Bool3>& model,
		   NodeValList& assign_list)
{
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    justify(inode, model, assign_list);
  }
}

// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] val 値
// @param[out] assign_list 値の割当リスト
void
BtJust1::just_sub2(TpgNode* node,
		   const vector<Bool3>& model,
		   Bool3 val,
		   NodeValList& assign_list)
{
  bool gfound = false;
  bool ffound = false;
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Bool3 igval = node_gval(inode, model);
    Bool3 ifval = node_fval(inode, model);
    if ( !gfound && igval == val ) {
      justify(inode, model, assign_list);
      gfound = true;
      if ( ifval == val ) {
	break;
      }
    }
    else if ( !ffound && ifval == val ) {
      justify(inode, model, assign_list);
      ffound = true;
    }
    if ( gfound && ffound ) {
      break;
    }
  }
}

END_NAMESPACE_YM_SATPG
