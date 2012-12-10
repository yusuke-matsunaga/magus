#ifndef DTPGFFR_H
#define DTPGFFR_H

/// @file DtpgFFR.h
/// @brief DtpgFFR のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

class DtpgFault;

//////////////////////////////////////////////////////////////////////
/// @class DtpgFFR DtpgFFR.h "DtpgFFR.h"
/// @brief DtpgNetwork の FFR を表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgFFR
{
  friend class DtpgNetwork;

public:

  /// @brief 根のノードを取り出す．
  DtpgNode*
  root() const;

  /// @brief ノードのリストを返す．
  const vector<DtpgNode*>&
  node_list() const;

  /// @brief 故障のリストを返す．
  const vector<DtpgFault*>&
  fault_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  DtpgNode* mRoot;

  // FFR のノードのリスト
  // mRoot も含む．
  vector<DtpgNode*> mNodeList;

  // FFR 内の故障のリスト
  vector<DtpgFault*> mFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 根のノードを取り出す．
inline
DtpgNode*
DtpgFFR::root() const
{
  return mRoot;
}

// @brief ノードのリストを返す．
inline
const vector<DtpgNode*>&
DtpgFFR::node_list() const
{
  return mNodeList;
}

// @brief 故障のリストを返す．
inline
const vector<DtpgFault*>&
DtpgFFR::fault_list() const
{
  return mFaultList;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGFFR_H
