#ifndef ZDDUNIOP_H
#define ZDDUNIOP_H

/// @file ZddUniOp.h
/// @brief ZddUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ZddOp.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddUniOp ZddOp.h "ZddOp.h"
/// @brief ZDD の単項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddUniOp :
  public ZddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  ZddUniOp(ZddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~ZddUniOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] level 変数のレベル
  virtual
  ZddEdge
  apply(ZddEdge left,
	ymuint level) = 0;

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
  ZddEdge
  get(ZddEdge e1);

  /// @brief 演算結果テーブルに登録する．
  /// @param[in] e1 オペランドの枝
  /// @param[in] ans 結果の枝
  void
  put(ZddEdge e1,
      ZddEdge ans);

  /// @brief 演算結果テーブルをクリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一時的に結果を覚えておくハッシュ表
  hash_map<ZddEdge, ZddEdge> mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1 オペランドの枝
inline
ZddEdge
ZddUniOp::get(ZddEdge e1)
{
  hash_map<ZddEdge, ZddEdge>::iterator p = mCompTbl.find(e1);
  if ( p == mCompTbl.end() ) {
    return ZddEdge::make_error();
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
ZddUniOp::put(ZddEdge e1,
	      ZddEdge ans)
{
  mCompTbl.insert(make_pair(e1, ans));
}

// @brief 演算結果テーブルをクリアする．
inline
void
ZddUniOp::clear()
{
  mCompTbl.clear();
}

END_NAMESPACE_YM_ZDD

#endif // ZDDUNIOP_H
