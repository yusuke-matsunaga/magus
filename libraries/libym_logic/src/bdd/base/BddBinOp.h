#ifndef BDDBINOP_H
#define BDDBINOP_H

/// @file BddBinOp.h
/// @brief BddBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"
#include "CompTbl2.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddBinOp BddBinOp.h "BddBinOp.h"
/// @brief BDD 間の二項演算を表すクラス
//////////////////////////////////////////////////////////////////////
class BddBinOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name テーブル名
  BddBinOp(BddMgrImpl* mgr,
	   const char* name = 0);

  /// @brief デストラクタ
  virtual
  ~BddBinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  /// @return 演算結果を買えす．
  virtual
  BddEdge
  apply(BddEdge left,
	BddEdge right) = 0;

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算結果テーブルを検索する．
  /// @param[in] e1, e2 オペランドの枝
  BddEdge
  get(BddEdge e1,
      BddEdge e2);

  /// @brief 演算結果テーブルに登録する．
  /// @param[in] e1, e2 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(BddEdge e1,
      BddEdge e2,
      BddEdge ans);

  /// @brief f と g のノードの子供のノードとレベルを求める．
  static
  ymuint
  split(BddEdge f,
	BddEdge g,
	BddEdge& f_0,
	BddEdge& f_1,
	BddEdge& g_0,
	BddEdge& g_1);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl2 mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1, e2 オペランドの枝
inline
BddEdge
BddBinOp::get(BddEdge e1,
	      BddEdge e2)
{
  return mCompTbl.get(e1, e2);
}

// @brief 演算結果テーブルに登録する．
// @param[in] e1, e2 オペランドの枝
// @param[in] ans 結果の枝
inline
void
BddBinOp::put(BddEdge e1,
	      BddEdge e2,
	      BddEdge ans)
{
  mCompTbl.put(e1, e2, ans);
}

// f と g のノードの子供のノードとレベルを求める．
inline
ymuint
BddBinOp::split(BddEdge f,
		BddEdge g,
		BddEdge& f_0,
		BddEdge& f_1,
		BddEdge& g_0,
		BddEdge& g_1)
{
  BddNode* f_vp = f.get_node();
  BddNode* g_vp = g.get_node();
  bool f_inv = f.inv();
  bool g_inv = g.inv();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( g_level < level ) {
    level = g_level;
  }
  split1(level, f_level, f, f_vp, f_inv, f_0, f_1);
  split1(level, g_level, g, g_vp, g_inv, g_0, g_1);
  return level;
}

END_NAMESPACE_YM_BDD

#endif // BDDBINOP_H
