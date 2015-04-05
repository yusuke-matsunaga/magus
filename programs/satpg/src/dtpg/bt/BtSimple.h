#ifndef BTSIMPLE_H
#define BTSIMPLE_H

/// @file BtSimple.h
/// @brief BtSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtSimple BtSimple.h "BtSimple.h"
/// @brief 簡単な BackTracer
//////////////////////////////////////////////////////////////////////
class BtSimple :
  public BtBase
{
public:

  /// @brief コンストラクタ
  BtSimple();


public:
  //////////////////////////////////////////////////////////////////////
  // BtSimple の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] node_set 故障に関係するノード集合
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  operator()(const TpgNode* fnode,
	     const NodeSet& node_set,
	     const ValMap& val_map,
	     NodeValList& assign_list);

};

END_NAMESPACE_YM_SATPG

#endif // BTSIMPLE_H
