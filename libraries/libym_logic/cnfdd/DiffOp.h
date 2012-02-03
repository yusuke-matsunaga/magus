#ifndef DIFFOP_H
#define DIFFOP_H

/// @file DiffOp.h
/// @brief DiffOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class DiffOp DiffOp.h "DiffOp.h"
/// @brief 集合差を求めるクラス
//////////////////////////////////////////////////////////////////////
class DiffOp :
  public BinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  DiffOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~DiffOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief \f$\setdiff\f$演算を行う関数
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

#endif // DIFFOP_H
