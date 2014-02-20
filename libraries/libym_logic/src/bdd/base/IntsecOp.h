#ifndef INTSECOP_H
#define INTSECOP_H

/// @file IntsecOp.h
/// @brief IntsecOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class IntsecOp IntsecOp.h "IntsecOp.h"
/// @brief intersect 演算を行うクラス
/// 2つの論理関数が共通部分を持つか(両方の関数を1にする入力が存在するか)
/// を調べる演算
/// 答えは true/false の論理値だが演算結果テーブルを流用するため
/// BddEdge の 0/1 を用いている．
//////////////////////////////////////////////////////////////////////
class IntsecOp :
  public BddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  IntsecOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~IntsecOp();


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

#endif // INTSECOP_H
