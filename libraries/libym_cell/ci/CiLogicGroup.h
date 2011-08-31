#ifndef CILOGICGROUP_H
#define CILOGICGROUP_H

/// @file CiLogicGroup.h
/// @brief CiLogicGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiLogicGroup CiLogicGroup.h" "CiLogicGroup.h"
/// @brief 論理セルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLogicGroup :
  public CiGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CiLogicGroup();

  /// @brief デストラクタ
  virtual
  ~CiLogicGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CELL

#endif // CILOGICGROUP_H
