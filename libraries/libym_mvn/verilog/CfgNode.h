#ifndef LIBYM_MVN_VERILOG_CFGNODE_H
#define LIBYM_MVN_VERILOG_CFGNODE_H

/// @file libym_mvn/verilog/CfgNode.h
/// @brief CfgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class CfgNode CfgNode.h "CfgNode.h"
/// @brief CFG のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class CfgNode
{
  friend class CfgMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] stmt 対応するステートメント
  CfgNode(const VlStmt* stmt);

  /// @brief デストラクタ
  ~CfgNode();


public:

  /// @brief 対応するステートメントを返す．
  const VlStmt*
  stmt() const;

  /// @brief 次のノードを返す．
  CfgNode*
  next() const;

  /// @brief 条件分岐ノードの場合の then ブロックの先頭のノードを返す．
  CfgNode*
  then_block() const;

  /// @brief 条件分岐ノードの場合の else ブロックの先頭のノードを返す．
  CfgNode*
  else_block() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応するステートメント
  const VlStmt* mStmt;

  // 次のノード
  CfgNode* mNext;

  // then ブロックの先頭
  CfgNode* mThen;

  // else ブロックの先頭
  CfgNode* mElse;

  // mNext ノードが親への復帰の時に真となるフラグ
  bool mLast;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 対応するステートメントを返す．
inline
const VlStmt*
CfgNode::stmt() const
{
  return mStmt;
}

// 次のノードを返す．
inline
CfgNode*
CfgNode::next() const
{
  return mNext;
}

// @brief 条件分岐ノードの場合の then ブロックの先頭のノードを返す．
inline
CfgNode*
CfgNode::then_block() const
{
  return mThen;
}

// @brief 条件分岐ノードの場合の else ブロックの先頭のノードを返す．
inline
CfgNode*
CfgNode::else_block() const
{
  return mElse;
}

END_NAMESPACE_YM_MVN


#endif // LIBYM_MVN_VERILOG_CFGNODE_H
