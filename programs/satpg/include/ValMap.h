#ifndef VALMAP_H
#define VALMAP_H

/// @file ValMap.h
/// @brief ValMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "Val3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class ValMap ValMap.h "ValMap.h"
/// @brief ノードに関連付けられた値を保持するクラス
///
/// 下を見ればわかるけどインターフェイスを定義しただけの
/// 純粋仮想クラス
//////////////////////////////////////////////////////////////////////
class ValMap
{
public:

  /// @brief デストラクタ
  virtual
  ~ValMap() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの正常値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  gval(TpgNode* node) const = 0;

  /// @brief ノードの故障値を返す．
  /// @param[in] node 対象のノード
  virtual
  Val3
  fval(TpgNode* node) const = 0;

};

END_NAMESPACE_YM_SATPG

#endif // VALMAP_H
