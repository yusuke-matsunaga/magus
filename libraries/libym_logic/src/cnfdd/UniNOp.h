#ifndef UNINOP_H
#define UNINOP_H

/// @file UniNOp.h
/// @brief UniNOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Op.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class UniNOp UniNOp.h "UniNOp.h"
/// @brief CNFDD の二項演算(一つは数値)を行うクラス
//////////////////////////////////////////////////////////////////////
class UniNOp :
  public Op
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name テーブル名
  UniNOp(CNFddMgrImpl& mgr,
	 const char* name);

  /// @brief デストラクタ
  virtual
  ~UniNOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] right 第2オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left,
	ymuint right) = 0;

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
  /// @param[in] n 第2オペランド
  CNFddEdge
  get(CNFddEdge e1,
      ymuint n);

  /// @brief 演算結果テーブルに登録する．
  /// @param[in] e1 オペランドの枝
  /// @param[in] n 第2オペランド
  /// @param[in] ans 結果の枝
  void
  put(CNFddEdge e1,
      ymuint n,
      CNFddEdge ans);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl1n mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1 オペランドの枝
// @param[in] n 第2オペランド
inline
CNFddEdge
UniNOp::get(CNFddEdge e1,
	    ymuint n)
{
  return mCompTbl.get(e1, n);
}

// @brief 演算結果テーブルに登録する．
// @param[in] e1 オペランドの枝
// @param[in] n 第2オペランド
// @param[in] ans 結果の枝
inline
void
UniNOp::put(CNFddEdge e1,
	    ymuint n,
	    CNFddEdge ans)
{
  mCompTbl.put(e1, n, ans);
}

END_NAMESPACE_YM_CNFDD

#endif // UNINOP_H
