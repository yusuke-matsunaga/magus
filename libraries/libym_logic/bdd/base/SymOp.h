#ifndef SYMOP_H
#define SYMOP_H

/// @file SymOp.h
/// @brief SymOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"
#include "CompTbl2.h"
#include "CompTbl3.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class SymOp SymOp.h "SymOp.h"
/// @brief 対称変数のチェックを行うクラス
//////////////////////////////////////////////////////////////////////
class SymOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  SymOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~SymOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e オペランド
  /// @param[in] id1, id2 対称性を調べる変数番号
  /// @param[in] pol 極性
  /// @retval true 対称だった．
  /// @retval false 対称でなかった．
  bool
  apply(BddEdge e,
	VarId id1,
	VarId id2,
	tPol pol);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数のレベルが x よりも上の場合の関数
  BddEdge
  apply_step1(BddEdge e);

  /// @brief 変数のレベルが x と y の間の場合の関数
  BddEdge
  apply_step2(BddEdge e0,
	      BddEdge e1);

  /// @brief y のレベルのノードを含んでいないか調べる関数
  BddEdge
  apply_step3(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数その1のレベル
  ymuint32 mX;

  // 変数その2のレベル
  ymuint32 mY;

  // 極性
  tPol mPol;

  // 演算結果テーブル用に mX, mY を表す BDD
  BddEdge mXYedge;

  // 演算結果テーブル用に mY を表す BDD
  BddEdge mYedge;

  // apply_step1 で用いる演算結果テーブル
  CompTbl2 mCompTbl1;

  // apply_step2 で用いる演算結果テーブル
  CompTbl3 mCompTbl2;

  // apply_step3 で用いる演算結果テーブル
  CompTbl2 mCompTbl3;

};

END_NAMESPACE_YM_BDD

#endif // SYMOP_H
