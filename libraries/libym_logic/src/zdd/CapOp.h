#ifndef CAPOP_H
#define CAPOP_H

/// @file CapOp.h
/// @brief CapOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddBinOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class CapOp CapOp.h "CapOp.h"
/// @brief インターセクションを求めるクラス
//////////////////////////////////////////////////////////////////////
class CapOp :
  public ZddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  CapOp(ZddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~CapOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief \f$\cap\f$演算を行う関数
  /// @param[in] left, right オペランド
  virtual
  ZddEdge
  apply(ZddEdge left,
	ZddEdge right);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  ZddEdge
  apply_step(ZddEdge f,
	     ZddEdge g);

};

END_NAMESPACE_YM_ZDD

#endif // CAPOP_H
