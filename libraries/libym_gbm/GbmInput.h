#ifndef GBMINPUT_H
#define GBMINPUT_H

/// @file GbmInput.h
/// @brief GbmInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNodeImpl.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
/// @class GbmInput GbmInput.h "GbmInput.h"
/// @brief 外部入力ノードを表す GbmNode の派生クラス
//////////////////////////////////////////////////////////////////////
class GbmInput :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid 変数番号
  GbmInput(ymuint id,
	   VarId vid);

  /// @brief デストラクタ
  virtual
  ~GbmInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_input() const;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const;

};

END_NAMESPACE_YM_GBM

#endif // GBMINPUT_H
