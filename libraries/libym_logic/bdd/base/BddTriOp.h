#ifndef BDDTRIOP_H
#define BDDTRIOP_H

/// @file BddTriOp.h
/// @brief BddTriOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"
#include "CompTbl3.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddTriOp BddTriOp.h "BddTriOp.h"
/// @brief BDD 間の二項演算を表すクラス
//////////////////////////////////////////////////////////////////////
class BddTriOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name テーブル名
  BddTriOp(BddMgrImpl* mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~BddTriOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e1, e2, e3 オペランド
  /// @return 演算結果を買えす．
  virtual
  BddEdge
  apply(BddEdge e1,
	BddEdge e2,
	BddEdge e3) = 0;

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算結果テーブルを検索する．
  /// @param[in] e1, e2, e3 オペランドの枝
  BddEdge
  get(BddEdge e1,
      BddEdge e2,
      BddEdge e3);

  /// @brief 演算結果テーブルに登録する．
  /// @param[in] e1, e2, e3 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(BddEdge e1,
      BddEdge e2,
      BddEdge e3,
      BddEdge ans);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl3 mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1, e2, e3 オペランドの枝
inline
BddEdge
BddTriOp::get(BddEdge e1,
	      BddEdge e2,
	      BddEdge e3)
{
  return mCompTbl.get(e1, e2, e3);
}

// @brief 演算結果テーブルに登録する．
// @param[in] e1, e2, e3 オペランドの枝
// @param[in] ans 結果の枝
inline
void
BddTriOp::put(BddEdge e1,
	      BddEdge e2,
	      BddEdge e3,
	      BddEdge ans)
{
  mCompTbl.put(e1, e2, e3, ans);
}

END_NAMESPACE_YM_BDD

#endif // BDDBINOP_H
