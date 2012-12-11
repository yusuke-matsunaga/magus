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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  DtpgNode* mRoot;

  // FFR のノードのリスト
  // mRoot も含む．
  vector<DtpgNode*> mNodeList;

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

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGFFR_H
