#ifndef CNFDDUNIOP_H
#define CNFDDUNIOP_H

/// @file CNFddUniOp.h
/// @brief CNFddUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class CNFddUniOp CNFddOp.h "CNFddOp.h"
/// @brief CNFDD の単項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class CNFddUniOp :
  public CNFddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  CNFddUniOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~CNFddUniOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left) = 0;

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
  CNFddEdge
  get(CNFddEdge e1);

  /// @brief 演算結果テーブルに登録する．
  /// @param[in] e1 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(CNFddEdge e1,
      CNFddEdge ans);

  /// @brief 演算結果テーブルをクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一時的に結果を覚えておくハッシュ表
  hash_map<CNFddEdge, CNFddEdge> mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1 オペランドの枝
inline
CNFddEdge
CNFddUniOp::get(CNFddEdge e1)
{
  hash_map<CNFddEdge, CNFddEdge>::iterator p = mCompTbl.find(e1);
  if ( p == mCompTbl.end() ) {
    return CNFddEdge::make_error();
  }
  else {
    return p->second;
  }
}

// @brief 演算結果テーブルに登録する．
// @param[in] e1 オペランドの枝
// @param[in] ans 結果の枝
inline
void
CNFddUniOp::put(CNFddEdge e1,
		CNFddEdge ans)
{
  mCompTbl.insert(make_pair(e1, ans));
}

// @brief 演算結果テーブルをクリアする．
inline
void
CNFddUniOp::clear()
{
  mCompTbl.clear();
}

END_NAMESPACE_YM_CNFDD

#endif // CNFDDUNIOP_H
