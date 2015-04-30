#ifndef BTJUST1_H
#define BTJUST1_H

/// @file BtJust1.h
/// @brief BtJust1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJustBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtJust1 BtJust1.h "BtJust1.h"
/// @brief 必要なノードのみ正当化する BackTracer
//////////////////////////////////////////////////////////////////////
class BtJust1 :
  public BtJustBase
{
public:

  /// @brief コンストラクタ
  BtJust1();

  /// @brief デストラクタ
  virtual
  ~BtJust1();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] node_set 故障に関係するノード集合
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  run(const TpgNode* fnode,
      const NodeSet& node_set,
      const ValMap& val_map,
      NodeValList& assign_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify(const TpgNode* node,
	  const ValMap& val_map,
	  NodeValList& assign_list);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  void
  just_sub1(const TpgNode* node,
	    const ValMap& val_map,
	    NodeValList& assign_list);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] val 値
  /// @param[out] assign_list 値の割当リスト
  void
  just_sub2(const TpgNode* node,
	    const ValMap& val_map,
	    Val3 val,
	    NodeValList& assign_list);

};

END_NAMESPACE_YM_SATPG

#endif // BTJUST1_H
