#ifndef CONOP_H
#define CONOP_H

/// @file ConOp.h
/// @brief ConOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddOp.h"
#include "CNFddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class ConOp ConOp.h "ConOp.h"
/// @brief conjunction を求めるクラス
//////////////////////////////////////////////////////////////////////
class ConOp :
  public CNFddBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  ConOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~ConOp();


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
  cap_step(CNFddEdge f,
	   CNFddEdge g);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のマネージャ
  CNFddMgrImpl& mMgr;

  // 演算テーブル
  CompTbl2 mCapTable;

};

END_NAMESPACE_YM_CNFDD

#endif // CAPOP_H
