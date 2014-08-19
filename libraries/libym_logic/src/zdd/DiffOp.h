#ifndef DIFFOP_H
#define DIFFOP_H

/// @file DiffOp.h
/// @brief DiffOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddBinOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class DiffOp DiffOp.h "DiffOp.h"
/// @brief 集合差を求めるクラス
//////////////////////////////////////////////////////////////////////
class DiffOp :
  public ZddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  DiffOp(ZddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~DiffOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief \f$\setminus\f$演算を行う関数
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

#endif // DIFFOP_H
