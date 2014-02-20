#ifndef ANDOP_H
#define ANDOP_H

/// @file AndOp.h
/// @brief AndOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class AndOp AndOp.h "AndOp.h"
/// @brief AND 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class AndOp :
  public BddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  AndOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~AndOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  /// @return 演算結果を返す．
  virtual
  BddEdge
  apply(BddEdge left,
	BddEdge right);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際の演算を行う関数
  BddEdge
  apply_step(BddEdge f,
	     BddEdge g);

};

END_NAMESPACE_YM_BDD

#endif // ANDOP_H
