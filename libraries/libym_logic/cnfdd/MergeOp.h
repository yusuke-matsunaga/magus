#ifndef MERGEOP_H
#define MERGEOP_H

/// @file MergeOp.h
/// @brief MergeOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BinOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class MergeOp MergeOp.h "MergeOp.h"
/// @brief 要素ごとのユニオンを求めるクラス
//////////////////////////////////////////////////////////////////////
class MergeOp :
  public BinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  /// @param[in] dis_op disjunction 演算オブジェクト
  MergeOp(CNFddMgrImpl& mgr,
	  BinOp* dis_op);

  /// @brief デストラクタ
  virtual
  ~MergeOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // disjunction 演算オブジェクト
  BinOp* mDisOp;

};

END_NAMESPACE_YM_CNFDD

#endif // MERGEOP_H
