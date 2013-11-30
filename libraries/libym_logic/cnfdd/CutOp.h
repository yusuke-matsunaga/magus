#ifndef CUTOP_H
#define CUTOP_H

/// @file CutOp.h
/// @brief CutOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "UniNOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class CutOp CutOp.h "CutOp.h"
/// @brief cut_off 演算を求めるクラス
//////////////////////////////////////////////////////////////////////
class CutOp :
  public UniNOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  CutOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~CutOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cut_off 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] limit 要素数の制限値
  virtual
  CNFddEdge
  apply(CNFddEdge left,
	ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  CNFddEdge
  apply_step(CNFddEdge f,
	     ymuint limit);

};

END_NAMESPACE_YM_CNFDD

#endif // CUTOP_H
