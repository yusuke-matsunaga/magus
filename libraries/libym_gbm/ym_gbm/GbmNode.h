#ifndef YM_GBM_GBMNODE_H
#define YM_GBM_GBMNODE_H

/// @file ym_gbm/GbmNode.h
/// @brief GbmNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gbm/gbm_nsdef.h"
#include "ym_gbm/GbmNodeHandle.h"
#include "ym_logic/VarId.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
/// @class GbmNode GbmNode.h "ym_gbm/GbmNode.h"
/// @brief Gbm で用いるノードを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmNode
{
public:

  /// @brief コンストラクタ
  GbmNode() { }

  /// @brief デストラクタ
  virtual
  ~GbmNode() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const = 0;

  /// @brief SATソルバ用の変数番号を返す．
  virtual
  VarId
  var_id() const = 0;

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_input() const = 0;

  /// @brief ANDノードの時 true を返す．
  virtual
  bool
  is_and() const = 0;

  /// @brief LUTノードの時 true を返す．
  virtual
  bool
  is_lut() const = 0;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const = 0;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const = 0;

};

END_NAMESPACE_YM_GBM

#endif // YM_GBM_GBMNODE_H
