#ifndef BTBASE_H
#define BTBASE_H

/// @file BtBase.h
/// @brief BtBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "BackTracer.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtBase BtBase.h "BtBase.h"
/// @brief BackTracer の基本クラス
//////////////////////////////////////////////////////////////////////
class BtBase :
  public BackTracer
{
protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @note node の値を mValList に記録する．
  /// @note 単純だが mModel 上のインデックスと mValList の符号化は異なる．
  void
  record_value(TpgNode* node);

  /// @brief justified マークをつける．
  /// @param[in] node 対象のノード
  void
  set_justified_mark(TpgNode* node);

  /// @brief justified マークを消す．
  /// @param[in] node 対象のノード
  void
  clear_justified_mark(TpgNode* node);

  /// @brief justified マークを読む．
  /// @param[in] node 対象のノード
  bool
  justified_mark(TpgNode* node);

  /// @brief ノードの正常値を読み出す．
  /// @param[in] node 対象のノード
  /// @note mModel の値を読む．
  Bool3
  node_gval(TpgNode* node);

  /// @brief ノードの故障値を読み出す．
  /// @param[in] node 対象のノード
  /// @note mModel の値を読む．
  Bool3
  node_fval(TpgNode* node);

  /// @brief プリミティブの正常値を読み出す．
  /// @param[in] prim 対象のプリミティブ
  /// @note mModel の値を読む．
  Bool3
  primitive_gval(TpgPrimitive* prim);

  /// @brief プリミティブの故障値を読み出す．
  /// @param[in] prim 対象のプリミティブ
  /// @note mModel の値を読む．
  Bool3
  primitive_fval(TpgPrimitive* prim);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT の割り当て結果
  vector<Bool3> mModel;

  // 正当化されたノードのリスト
  vector<TpgNode*> mJustifiedNodeList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノードの正常値を読み出す．
// @note mModel の値を読む．
inline
Bool3
BtBase::node_gval(TpgNode* node)
{
  return mModel[node->gvar().val()];
}

// @brief ノードの故障値を読み出す．
// @note mModel の値を読む．
inline
Bool3
BtBase::node_fval(TpgNode* node)
{
  return mModel[node->fvar().val()];
}

// @brief プリミティブの正常値を読み出す．
// @note mModel の値を読む．
inline
Bool3
BtBase::primitive_gval(TpgPrimitive* prim)
{
  Literal lit = prim->glit();
  Bool3 val = mModel[lit.varid().val()];
  if ( lit.pol() == kPolNega ) {
    val = ~val;
  }
  return val;
}

// @brief プリミティブの故障値を読み出す．
// @note mModel の値を読む．
inline
Bool3
BtBase::primitive_fval(TpgPrimitive* prim)
{
  Literal lit = prim->flit();
  Bool3 val = mModel[lit.varid().val()];
  if ( lit.pol() == kPolNega ) {
    val = ~val;
  }
  return val;
}

END_NAMESPACE_YM_SATPG

#endif // BTBASE_H
