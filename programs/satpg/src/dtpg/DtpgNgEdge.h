#ifndef DTPGNGEDGE_H
#define DTPGNGEDGE_H

/// @file DtpgNgEdge.h
/// @brief DtpgNgEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

class DtpgNodeGroup;

//////////////////////////////////////////////////////////////////////
/// @class DtpgNgEdge DtpgNgEdge.h "DtpgNgEdge.h"
/// @brief DtpgNodeGroup 間の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgNgEdge
{
  friend class DtpgNgMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] size サイズ
  /// @param[in] src_node 入力元のノード
  /// @param[in] dst_node 出力先のノード
  DtpgNgEdge(ymuint size,
	     DtpgNodeGroup* src_node,
	     DtpgNodeGroup* dst_node);

  /// @brief デストラクタ
  ~DtpgNgEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを返す．
  /// @note サイズとは src_node() と dst_node() のサイズの和
  ymuint
  size() const;

  /// @brief 入力元のノードを返す．
  DtpgNodeGroup*
  src_node() const;

  /// @brief 出力先のノードを返す．
  DtpgNodeGroup*
  dst_node() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // サイズ
  ymuint32 mSize;

  // 入力元のノード
  DtpgNodeGroup* mSrcNode;

  // 出力先のノード
  DtpgNodeGroup* mDstNode;

  // 前のリンク
  DtpgNgEdge* mPrev;

  // 後のリンク
  DtpgNgEdge* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size サイズ
// @param[in] src_node 入力元のノード
// @param[in] dst_node 出力先のノード
inline
DtpgNgEdge::DtpgNgEdge(ymuint size,
		       DtpgNodeGroup* src_node,
		       DtpgNodeGroup* dst_node) :
  mSize(size),
  mSrcNode(src_node),
  mDstNode(dst_node)
{
}

// @brief デストラクタ
inline
DtpgNgEdge::~DtpgNgEdge()
{
}

// @brief サイズを返す．
// @note サイズとは src_node() と dst_node() のサイズの和
inline
ymuint
DtpgNgEdge::size() const
{
  return mSize;
}

// @brief 入力元のノードを返す．
inline
DtpgNodeGroup*
DtpgNgEdge::src_node() const
{
  return mSrcNode;
}

// @brief 出力先のノードを返す．
inline
DtpgNodeGroup*
DtpgNgEdge::dst_node() const
{
  return mDstNode;
}

END_NAMESPACE_YM_SATPG

#endif // DTPGNGEDGE_H
