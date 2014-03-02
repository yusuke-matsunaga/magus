#ifndef FLOWEDGE_H
#define FLOWEDGE_H

/// @file FlowEdge.h
/// @brief FlowEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class FlowNode;

//////////////////////////////////////////////////////////////////////
/// @class FlowEdge FlowEdge.h "FlowEdge.h"
/// @brief フローグラフの枝を表すクラス
//////////////////////////////////////////////////////////////////////
class FlowEdge
{
  friend class FlowGraph;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] from 入力元のノード
  /// @param[in] to 出力先のノード
  /// @param[in] cost コスト
  FlowEdge(ymuint id,
	   FlowNode* from,
	   FlowNode* to,
	   ymuint cost);

  /// @brief デストラクタ
  ~FlowEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 入力元のノードを返す．
  FlowNode*
  from() const;

  /// @brief 出力先のノードを返す．
  FlowNode*
  to() const;

  /// @brief コストを返す．
  ymuint
  cost() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 入力元のノード
  FlowNode* mFrom;

  // 出力先のノード
  FlowNode* mTo;

  // コスト
  ymuint32 mCost;

  // 選ばれている時に true とするフラグ
  bool mSelected;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] from 入力元のノード
// @param[in] to 出力先のノード
// @param[in] cost コスト
inline
FlowEdge::FlowEdge(ymuint id,
		   FlowNode* from,
		   FlowNode* to,
		   ymuint cost) :
  mId(id),
  mFrom(from),
  mTo(to),
  mCost(cost)
{
}

// @brief デストラクタ
inline
FlowEdge::~FlowEdge()
{
}

// @brief ID番号を返す．
inline
ymuint
FlowEdge::id() const
{
  return mId;
}

// @brief 入力元のノードを返す．
inline
FlowNode*
FlowEdge::from() const
{
  return mFrom;
}

// @brief 出力先のノードを返す．
inline
FlowNode*
FlowEdge::to() const
{
  return mTo;
}

// @brief コストを返す．
inline
ymuint
FlowEdge::cost() const
{
  return mCost;
}

END_NAMESPACE_YM

#endif // FLOWEDGE_H
