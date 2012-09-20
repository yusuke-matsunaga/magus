#ifndef GBMNODEIMPL_H
#define GBMNODEIMPL_H

/// @file GbmNodeImpl.h
/// @brief GbmNodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gbm/GbmNode.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
/// @class GbmNodeImpl GbmNodeImpl.h "GbmNodeImpl.h"
/// @brief GbmNode の実装クラス
//////////////////////////////////////////////////////////////////////
class GbmNodeImpl :
  public GbmNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid 変数番号
  GbmNodeImpl(ymuint id,
	      VarId vid);

  /// @brief デストラクタ
  virtual
  ~GbmNodeImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const;

  /// @brief SATソルバ用の変数番号を返す．
  virtual
  VarId
  var_id() const;

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_input() const;

  /// @brief ANDノードの時 true を返す．
  virtual
  bool
  is_and() const;

  /// @brief LUTノードの時 true を返す．
  virtual
  bool
  is_lut() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // SAT ソルバ用の変数番号
  VarId mVarId;

};

END_NAMESPACE_YM_GBM

#endif // GBMNODEIMPL_H
