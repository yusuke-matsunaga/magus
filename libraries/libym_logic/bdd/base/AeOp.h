#ifndef AEOP_H
#define AEOP_H

/// @file AeOp.h
/// @brief AeOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddTriOp.h"


BEGIN_NAMESPACE_YM_BDD

class BddBinOp;

//////////////////////////////////////////////////////////////////////
/// @class AeOp AeOp.h "AeOp.h"
/// @brief and-exist 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class AeOp :
  public BddTriOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] and_op AND 演算オブジェクト
  /// @param[in] smooth_op smoothing 演算オブジェクト
  AeOp(BddMgrImpl* mgr,
       BddBinOp* and_op,
       BddBinOp* smooth_op);

  /// @brief デストラクタ
  virtual
  ~AeOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e1, e2, e3 オペランド
  /// @return 演算結果を返す．
  virtual
  BddEdge
  apply(BddEdge e1,
	BddEdge e2,
	BddEdge e3);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際の演算を行う関数
  BddEdge
  apply_step(BddEdge f,
	     BddEdge g,
	     BddEdge h);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AND 演算オブジェクト
  BddBinOp* mAndOp;

  // smoothing 演算オブジェクト
  BddBinOp* mSmoothOp;

};

END_NAMESPACE_YM_BDD

#endif // ITEOP_H
