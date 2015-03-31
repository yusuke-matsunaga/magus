#ifndef BTBASE_H
#define BTBASE_H

/// @file BtBase.h
/// @brief BtBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BackTracer.h"
#include "NodeValList.h"
#include "ValMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtBase BtBase.h "BtBase.h"
/// @brief BackTracer の基本クラス
//////////////////////////////////////////////////////////////////////
class BtBase :
  public BackTracer
{
public:

  /// @brief コンストラクタ
  BtBase();

  /// @brief デストラクタ
  virtual
  ~BtBase();


public:
  //////////////////////////////////////////////////////////////////////
  // Backtracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  ///
  /// このクラスの実装ではなにもしない．
  virtual
  void
  set_max_id(ymuint max_id);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  static
  void
  record_value(TpgNode* node,
	       const ValMap& val_map,
	       NodeValList& assign_list);

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードの値を記録する．
// @param[in] node 対象の外部入力ノード
// @param[in] val_map ノードの値の割当を保持するクラス
// @param[out] assign_list 値の割当リスト
inline
void
BtBase::record_value(TpgNode* node,
		     const ValMap& val_map,
		     NodeValList& assign_list)
{
  Val3 v = val_map.gval(node);
  if ( v == kVal0 ) {
    assign_list.add(node, false);
  }
  else if ( v == kVal1 ) {
    assign_list.add(node, true);
  }
}

END_NAMESPACE_YM_SATPG

#endif // BTBASE_H
