#ifndef DGNODE_H
#define DGNODE_H

/// @file DgNode.h
/// @brief DgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class DgNode DgNode.h "DgNode.h"
/// @brief dependence graph(DG)のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgNode
{
  friend class DgGraph;
private:

  /// @brief コンストラクタ
  DgNode();

  /// @brief デストラクタ
  ~DgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 隣接リストを返す．
  const vector<DgNode*>&
  adj_link() const;

  /// @brief アクティブフラグを返す．
  bool
  is_active() const;

  /// @brief アクティブでなくす．
  void
  inactivate();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 隣接ノードのリスト
  vector<DgNode*> mAdjLink;

  // アクティブフラグ
  bool mActive;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DgNode::DgNode()
{
}

// @brief デストラクタ
inline
DgNode::~DgNode()
{
}

// @brief ID番号を返す．
inline
ymuint
DgNode::id() const
{
  return mId;
}

// @brief 隣接リストを返す．
inline
const vector<DgNode*>&
DgNode::adj_link() const
{
  return mAdjLink;
}

// @brief アクティブフラグを返す．
inline
bool
DgNode::is_active() const
{
  return mActive;
}

// @brief アクティブでなくす．
inline
void
DgNode::inactivate()
{
  mActive = false;
}

END_NAMESPACE_YM_LUTMAP

#endif // DGNODE_H
