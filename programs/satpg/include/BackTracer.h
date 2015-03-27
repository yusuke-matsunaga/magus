#ifndef BACKTRACER_H
#define BACKTRACER_H

/// @file BackTracer.h
/// @brief BackTracer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmLogic/Bool3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BackTracer BackTracer.h "BackTracer.h"
/// @brief テストパタンを求めるためのバックトレースを行なうファンクター
//////////////////////////////////////////////////////////////////////
class BackTracer
{
public:

  /// @brief デストラクタ
  virtual
  ~BackTracer() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  virtual
  void
  set_max_id(ymuint max_id) = 0;

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] model SATの値の割り当て結果を収めた配列
  /// @param[in] input_list テストパタンに関係のある入力のリスト
  /// @param[in] output_list 故障伝搬の可能性のある出力のリスト
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  operator()(TpgNode* fnode,
	     const vector<Bool3>& model,
	     const vector<TpgNode*>& input_list,
	     const vector<TpgNode*>& output_list,
	     NodeValList& assign_list) = 0;

};

/// @brief 'Simple' タイプの生成を行なう．
BackTracer*
new_BtSimple();

/// @brief 'Just1' タイプの生成を行なう．
BackTracer*
new_BtJust1();

/// @brief 'Just2' タイプの生成を行なう．
BackTracer*
new_BtJust2();

/// @brief 'Just3' タイプの生成を行なう．
BackTracer*
new_BtJust3();

END_NAMESPACE_YM_SATPG

#endif // BACKTRACER_H
