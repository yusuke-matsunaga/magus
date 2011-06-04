#ifndef LIBYM_NETWORKS_CMN_CMNNODEINPUT_H
#define LIBYM_NETWORKS_CMN_CMNNODEINPUT_H

/// @file libym_networks/cmn/CmnNodeInput.h
/// @brief CmnNodeInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnNode.h"


BEGIN_NAMESPACE_YM_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnNodeInput CmnNodeInput.h "CmnNodeInput.h"
/// @brief 入力を表す CmnNode の派生クラス
//////////////////////////////////////////////////////////////////////
class CmnNodeInput :
  public CmnNode
{
  friend class CmnMgrImpl;

protected:

  /// @brief コンストラクタ
  CmnNodeInput();

  /// @brief デストラクタ
  virtual
  ~CmnNodeInput();


public:

  /// @brief タイプを得る．
  virtual
  tType
  type() const;

  /// @brief 入力ノードの時に true を返す．
  virtual
  bool
  is_input() const;

};

END_NAMESPACE_YM_CMN


#endif // LIBYM_NETWORKS_CMN_CMNNODEINPUT_H
