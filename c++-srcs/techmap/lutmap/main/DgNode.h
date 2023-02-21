#ifndef DGNODE_H
#define DGNODE_H

/// @file DgNode.h
/// @brief DgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class DgNode DgNode.h "DgNode.h"
/// @brief dependence graph(DG)のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgNode
{
  friend class DgGraph;

private:

  /// @brief コンストラクタ
  DgNode() = default;

  /// @brief デストラクタ
  ~DgNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 隣接リストを返す．
  const vector<DgNode*>&
  adj_link() const
  {
    return mAdjLink;
  }

  /// @brief アクティブフラグを返す．
  bool
  is_active() const
  {
    return mActive;
  }

  /// @brief アクティブでなくす．
  void
  inactivate()
  {
    mActive = false;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // 隣接ノードのリスト
  vector<DgNode*> mAdjLink;

  // アクティブフラグ
  bool mActive{true};

};

END_NAMESPACE_LUTMAP

#endif // DGNODE_H
