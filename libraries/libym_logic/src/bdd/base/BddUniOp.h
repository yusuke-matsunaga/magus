#ifndef BDDUNIOP_H
#define BDDUNIOP_H

/// @file BddUniOp.h
/// @brief BddUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddUniOp BddUniOp.h "BddUniOp.h"
/// @brief BDD 単項演算を表すクラス
//////////////////////////////////////////////////////////////////////
class BddUniOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  BddUniOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~BddUniOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算結果テーブルを検索する．
  /// @param[in] e1 オペランドの枝
  BddEdge
  get(BddEdge e1);

  /// @brief 演算結果テーブルに登録する．
  /// @param[in] e1 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(BddEdge e1,
      BddEdge ans);

  /// @brief 演算結果テーブルをクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  HashMap<BddEdge, BddEdge> mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1 オペランドの枝
inline
BddEdge
BddUniOp::get(BddEdge e1)
{
  BddEdge ans;
  if ( mCompTbl.find(e1, ans) ) {
    return ans;
  }
  else {
    return BddEdge::make_error();
  }
}

// @brief 演算結果テーブルに登録する．
// @param[in] e1 オペランドの枝
// @param[in] ans 結果の枝
inline
void
BddUniOp::put(BddEdge e1,
	      BddEdge ans)
{
  mCompTbl.add(e1, ans);
}

// @brief 演算結果テーブルをクリアする．
inline
void
BddUniOp::clear()
{
  mCompTbl.clear();
}

END_NAMESPACE_YM_BDD

#endif // BDDBINOP_H
