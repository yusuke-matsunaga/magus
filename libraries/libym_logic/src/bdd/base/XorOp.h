﻿#ifndef XOROP_H
#define XOROP_H

/// @file XorOp.h
/// @brief XorOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class XorOp XorOp.h "XorOp.h"
/// @brief XOR 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class XorOp :
  public BddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  XorOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~XorOp();


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

#endif // XOROP_H
