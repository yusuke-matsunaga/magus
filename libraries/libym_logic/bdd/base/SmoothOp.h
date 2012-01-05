#ifndef SMOOTHOP_H
#define SMOTTHOP_H

/// @file SmoothOp.h
/// @brief SmoothOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddUniOp.h"


BEGIN_NAMESPACE_YM_BDD

class BddBinOp;

//////////////////////////////////////////////////////////////////////
/// @class SmoothOp SmoothOp.h "SmoothOp.h"
/// @brief smooth 演算を行うクラス
//////////////////////////////////////////////////////////////////////
class SmoothOp :
  public BddUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] and_op AND 演算オブジェクト
  SmoothOp(BddMgrImpl* mgr,
	   BddBinOp* and_op);

  /// @brief デストラクタ
  virtual
  ~SmoothOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e 根の枝
  /// @param[in] v_list 消去する変数のレベルのリスト
  /// @return 演算結果を返す．
  BddEdge
  apply(BddEdge e,
	const vector<ymuint32>& v_list);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief apply() の下請け関数
  BddEdge
  apply_step(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 消去するレベルに印をつけた配列
  vector<bool> mEmark;

  // 消去する変数のレベルの最大値
  ymuint32 mMaxLevel;

  // AND 演算オブジェクト
  BddBinOp* mAndOp;

};

END_NAMESPACE_YM_BDD

#endif // SMOTTHOP_H
