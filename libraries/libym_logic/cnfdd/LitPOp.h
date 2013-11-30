#ifndef LITPOP_H
#define LITPOP_H

/// @file LitPOp.h
/// @brief LitPOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "UniVOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class LitPOp LitPOp.h "LitPOp.h"
/// @brief add_posiliteral 演算を求めるクラス
//////////////////////////////////////////////////////////////////////
class LitPOp :
  public UniVOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  LitPOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~LitPOp();


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

#endif // LITPOP_H
