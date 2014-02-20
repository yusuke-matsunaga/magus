#ifndef ITEOP_H
#define ITEOP_H

/// @file IteOp.h
/// @brief IteOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddTriOp.h"


BEGIN_NAMESPACE_YM_BDD

class BddBinOp;

//////////////////////////////////////////////////////////////////////
/// @class IteOp IteOp.h "IteOp.h"
/// @brief if-then-else 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class IteOp :
  public BddTriOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] and_op AND 演算オブジェクト
  /// @param[in] xor_op XOR 演算オブジェクト
  IteOp(BddMgrImpl* mgr,
	BddBinOp* and_op,
	BddBinOp* xor_op);

  /// @brief デストラクタ
  virtual
  ~IteOp();


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

  // XOR 演算オブジェクト
  BddBinOp* mXorOp;

};

END_NAMESPACE_YM_BDD

#endif // ITEOP_H
