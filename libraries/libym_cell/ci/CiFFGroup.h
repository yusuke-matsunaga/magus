#ifndef CIFFGROUP_H
#define CIFFGROUP_H

/// @file CiFFGroup.h
/// @brief CiFFGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiFFGroup CiFFGroup.h" "CiFFGroup.h"
/// @brief FFセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class CiFFGroup :
  public CiGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CiFFGroup();

  /// @brief デストラクタ
  virtual
  ~CiFFGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CELL

#endif // CIFFGROUP_H
