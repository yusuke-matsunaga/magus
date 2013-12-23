#ifndef GBMNODE_H
#define GBMNODE_H

/// @file GbmNode.h
/// @brief GbmNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNodeHandle.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmNode GbmNode.h "GbmNode.h"
/// @brief Gbm で用いるノードを表すクラス
//////////////////////////////////////////////////////////////////////
class GbmNode
{
public:

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

  /// @brief MUX ノードの時 true を返す．
  virtual
  bool
  is_mux() const = 0;

  /// @brief 外部入力番号を返す．
  /// @note is_input() == true の時のみ意味を持つ．
  virtual
  ymuint
  input_id() const = 0;

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

  /// @brief LUT/MUX ノードの時の configuration 変数の最初の番号を得る．
  virtual
  ymuint
  conf_base() const = 0;

  /// @brief LUT/MUX ノードの時の configuration 変数の数を得る．
  virtual
  ymuint
  conf_size() const = 0;

  /// @brief 関数を計算する．
  virtual
  TvFunc
  calc_func(const vector<TvFunc>& func_array,
	    const vector<bool>& conf_bits) const = 0;
};

END_NAMESPACE_YM

#endif // YM_GBM_GBMNODE_H
