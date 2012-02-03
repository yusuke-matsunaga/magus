#ifndef UNIOP_H
#define UNIOP_H

/// @file UniOp.h
/// @brief UniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "Op.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class UniOp UniOp.h "UniOp.h"
/// @brief CNFDD の単項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class UniOp :
  public Op
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] name テーブル名
  UniOp(CNFddMgrImpl& mgr,
	const char* name);

  /// @brief デストラクタ
  virtual
  ~UniOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl1 mCompTbl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 演算結果テーブルを検索する．
// @param[in] e1 オペランドの枝
inline
CNFddEdge
UniOp::get(CNFddEdge e1)
{
  return mCompTbl.get(e1);
}

// @brief 演算結果テーブルに登録する．
// @param[in] e1 オペランドの枝
// @param[in] ans 結果の枝
inline
void
UniOp::put(CNFddEdge e1,
	   CNFddEdge ans)
{
  mCompTbl.put(e1, ans);
}

END_NAMESPACE_YM_CNFDD

#endif // UNIOP_H
