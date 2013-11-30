#ifndef CONOP_H
#define CONOP_H

/// @file ConOp.h
/// @brief ConOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class ConOp ConOp.h "ConOp.h"
/// @brief conjunction を求めるクラス
//////////////////////////////////////////////////////////////////////
class ConOp :
  public BinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  ConOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~ConOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief \f$\wedge\f$演算を行う関数
  /// @param[in] left, right オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left,
	CNFddEdge right);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  CNFddEdge
  apply_step(CNFddEdge f,
	     CNFddEdge g);

};

END_NAMESPACE_YM_CNFDD

#endif // CONOP_H
