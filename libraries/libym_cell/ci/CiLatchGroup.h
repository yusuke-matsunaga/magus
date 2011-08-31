#ifndef CILATCHGROUP_H
#define CILATCHGROUP_H

/// @file CiLatchGroup.h
/// @brief CiLatchGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiLatchGroup CiLatchGroup.h" "CiLatchGroup.h"
/// @brief FFセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLatchGroup :
  public CiGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CiLatchGroup();

  /// @brief デストラクタ
  virtual
  ~CiLatchGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CELL

#endif // CILATCHGROUP_H
