#ifndef COFNOP_H
#define COFNOP_H

/// @file CofNOp.h
/// @brief CofNOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "UniVOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class CofNOp CofNOp.h "CofNOp.h"
/// @brief cofactorn 演算を求めるクラス
//////////////////////////////////////////////////////////////////////
class CofNOp :
  public UniVOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  CofNOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~CofNOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cofactorp 演算を行う関数
  /// @param[in] left オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  CNFddEdge
  apply_step(CNFddEdge f);

};

END_NAMESPACE_YM_CNFDD

#endif // COFNOP_H
