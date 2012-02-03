#ifndef UNIVOP_H
#define UNIVOP_H

/// @file UniVOp.h
/// @brief UniVOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "Op.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class UniVOp UniVOp.h "UniVOp.h"
/// @brief CNFDD の二項演算(一つは変数番号)を行うクラス
//////////////////////////////////////////////////////////////////////
class UniVOp :
  public Op
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  UniVOp(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~UniVOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] level 変数のレベル
  CNFddEdge
  apply(CNFddEdge left,
	ymuint level);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を実際に行う関数
  /// @param[in] left オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left) = 0;

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

  /// @brief apply の第2引数を取り出す．
  ymuint
  level() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一時的に結果を覚えておくハッシュ表
  hash_map<CNFddEdge, CNFddEdge> mCompTbl;

  // apply の第2引数
  ymuint32 mLevel;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算を行う関数
// @param[in] left オペランド
// @param[in] level 変数のレベル
inline
CNFddEdge
UniVOp::apply(CNFddEdge left,
	      ymuint level)
{
  mLevel = level;
  return apply(left);
}

// @brief 演算結果テーブルを検索する．
// @param[in] e1 オペランドの枝
inline
CNFddEdge
UniVOp::get(CNFddEdge e1)
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
UniVOp::put(CNFddEdge e1,
	    CNFddEdge ans)
{
  mCompTbl.insert(make_pair(e1, ans));
}

// @brief 演算結果テーブルをクリアする．
inline
void
UniVOp::clear()
{
  mCompTbl.clear();
}

// @brief apply の第2引数を取り出す．
inline
ymuint
UniVOp::level() const
{
  return mLevel;
}

END_NAMESPACE_YM_CNFDD

#endif // UNIVOP_H
