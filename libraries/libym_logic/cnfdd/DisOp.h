#ifndef DISCONOP_H
#define DISCONOP_H

/// @file DisOp.h
/// @brief DisOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class DisOp DisOp.h "DisOp.h"
/// @brief disjunction を求めるクラス
//////////////////////////////////////////////////////////////////////
class DisOp :
  public BinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  DisOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~DisOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief \f$\vee\f$演算を行う関数
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

#endif // DISCONOP_H
