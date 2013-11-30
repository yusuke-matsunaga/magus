#ifndef SMOOTHOP_H
#define SMOTTHOP_H

/// @file SmoothOp.h
/// @brief SmoothOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class SmoothOp SmoothOp.h "SmoothOp.h"
/// @brief smooth 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class SmoothOp :
  public BddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] and_op AND 演算オブジェクト
  SmoothOp(BddMgrImpl* mgr,
	   BddBinOp* and_op);

  /// @brief デストラクタ
  virtual
  ~SmoothOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e 根の枝
  /// @param[in] s 消去する変数のリストの根
  /// @return 演算結果を返す．
  virtual
  BddEdge
  apply(BddEdge e,
	BddEdge s);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief apply() の下請け関数
  BddEdge
  apply_step(BddEdge e,
	     BddEdge s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AND 演算オブジェクト
  BddBinOp* mAndOp;

};

END_NAMESPACE_YM_BDD

#endif // SMOTTHOP_H
