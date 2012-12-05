#ifndef TGFFR_H
#define TGFFR_H

/// @file TgFFR.h
/// @brief TgFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TgFFR TgFFR.h "TgFFR.h"
/// @brief TgNetwork の FFR を表すクラス
//////////////////////////////////////////////////////////////////////
class TgFFR
{
  friend class AtpgMgr;

public:

  /// @brief 根のノードを取り出す．
  const TgNode*
  root() const;

  /// @brief ノードのリストを返す．
  const vector<const TgNode*>&
  node_list() const;

  /// @brief 故障のリストを返す．
  const vector<SaFault*>&
  fault_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const TgNode* mRoot;

  // FFR のノードのリスト
  // mRoot も含む．
  vector<const TgNode*> mNodeList;

  // FFR 内の故障のリスト
  vector<SaFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 根のノードを取り出す．
inline
const TgNode*
TgFFR::root() const
{
  return mRoot;
}

// @brief ノードのリストを返す．
inline
const vector<const TgNode*>&
TgFFR::node_list() const
{
  return mNodeList;
}

// @brief 故障のリストを返す．
inline
const vector<SaFault*>&
TgFFR::fault_list() const
{
  return mFaultList;
}

END_NAMESPACE_YM_SATPG

#endif // TGFFR_H
