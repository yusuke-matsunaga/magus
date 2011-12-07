#ifndef SWAPOP_H
#define SWAPOP_H

/// @file SwapOp.h
/// @brief SwapOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddOp.h"
#include "ZddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class SwapOp SwapOp.h "SwapOp.h"
/// @brief swap 演算を求めるクラス
//////////////////////////////////////////////////////////////////////
class SwapOp :
  public ZddUnOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ZddMgrImpl
  SwapOp(ZddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~SwapOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief swap 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] var 変数番号
  virtual
  ZddEdge
  apply(ZddEdge left,
	VarId var);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  ZddEdge
  swap_step(ZddEdge f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のマネージャ
  ZddMgrImpl& mMgr;

  // 演算テーブル
  CompTbl2 mSwapTable;

  // 変数番号
  VarId mVar;

  // 変数番号を表す枝
  ZddEdge mVarEdge;

};

END_NAMESPACE_YM_ZDD

#endif // SWAPOP_H
