#ifndef MERGEOP2_H
#define MERGEOP2_H

/// @file MergeOp2.h
/// @brief MergeOp2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ZddBinOp.h"


BEGIN_NAMESPACE_YM_ZDD

class NeOp;
class MsOp;

//////////////////////////////////////////////////////////////////////
/// @class MergeOp2 MergeOp2.h "MergeOp2.h"
/// @brief 各々の要素のマージするクラス
//////////////////////////////////////////////////////////////////////
class MergeOp2 :
  public ZddBinOpBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cup_op union 演算オブジェクト
  /// @param[in] ne_op n-element 演算オブジェクト
  /// @param[in] ms_op minimum set 演算オブジェクト
  MergeOp2(ZddMgrImpl* mgr,
	   ZddBinOp* cup_op,
	   NeOp* ne_op,
	   MsOp* ms_op);

  /// @brief デストラクタ
  virtual
  ~MergeOp2();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マージ演算を行う関数
  /// @param[in] left, right オペランド
  /// @param[in] limit 要素数の上限値
  virtual
  ZddEdge
  apply(ZddEdge left,
	ZddEdge right,
	ymuint limit);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  /// @param[in] limit
  ZddEdge
  apply_step(ZddEdge f,
	     ZddEdge g,
	     ymuint limit);

  /// @brief id1, id2, id3 をキーとして検索を行なう
  ZddEdge
  get(ZddEdge id1,
      ZddEdge id2,
      ymuint id3);

  /// @brief 結果を登録する
  void
  put(ZddEdge id1,
      ZddEdge id2,
      ymuint id3,
      ZddEdge ans);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算結果テーブル
  CompTbl2n mCompTbl;

  // union 演算オブジェクト
  ZddBinOp* mCupOp;

  // n-element 演算オブジェクト
  NeOp* mNeOp;

  // minimu set 演算オブジェクト
  MsOp* mMsOp;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief id1, id2, id3 をキーとして検索を行なう
inline
ZddEdge
MergeOp2::get(ZddEdge id1,
	      ZddEdge id2,
	      ymuint id3)
{
  return mCompTbl.get(id1, id2, id3);
}

// @brief 結果を登録する
inline
void
MergeOp2::put(ZddEdge id1,
	      ZddEdge id2,
	      ymuint id3,
	      ZddEdge ans)
{
  mCompTbl.put(id1, id2, id3, ans);
}

END_NAMESPACE_YM_ZDD

#endif // MERGEOP_H
