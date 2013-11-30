#ifndef XCOP_H
#define XCOP_H

/// @file XcOp.h
/// @brief XcOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddUniOp.h"


BEGIN_NAMESPACE_YM_BDD

class BddBinOp;

//////////////////////////////////////////////////////////////////////
/// @class XcOp XcOp.h "XcOp.h"
/// @brief XOR-cofactor 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class XcOp :
  public BddUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] xor_op XOR演算オブジェクト
  XcOp(BddMgrImpl* mgr,
       BddBinOp* xor_op);

  /// @brief デストラクタ
  virtual
  ~XcOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] id コファクタをとる変数番号
  /// @return 演算結果を返す．
  virtual
  BddEdge
  apply(BddEdge left,
	VarId id);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際の演算を行う関数
  BddEdge
  apply_step(BddEdge f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コファクターをとる変数のレベル
  ymuint32 mLevel;

  // XOR演算オブジェクト
  BddBinOp* mXorOp;

};

END_NAMESPACE_YM_BDD

#endif // XCOP_H
