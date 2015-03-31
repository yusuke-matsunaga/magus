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
  /// @param[in] val_map ノードの値の割当を保持するクラス
  /// @param[in] input_list テストパタンに関係のある入力のリスト
  /// @param[in] output_list 故障伝搬の可能性のある出力のリスト
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  operator()(TpgNode* fnode,
	     const ValMap& val_map,
	     const vector<TpgNode*>& input_list,
	     const vector<TpgNode*>& output_list,
	     NodeValList& assign_list);

};

END_NAMESPACE_YM_SATPG

#endif // BTSIMPLE_H
