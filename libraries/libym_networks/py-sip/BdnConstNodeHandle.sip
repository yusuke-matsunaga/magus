
/// @file BdnConstNodeHandle.sip
/// @brief BdnConstNodeHandle の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnConstNodeHandle
{
%TypeHeaderCode
#include "networks/BdnConstNodeHandle.h"
using nsYm::BdnConstNodeHandle;
%End

public:

  /// @brief 空のコンストラクタ
  BdnConstNodeHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  BdnConstNodeHandle(const BdnNode* node,
		     bool inv = false);

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(const BdnNode* node,
      bool inv);

  /// @brief 定数0を返す．
  static
  BdnConstNodeHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  BdnConstNodeHandle
  make_one();

  /// @brief 極性を否定したハンドルを返す．
  BdnConstNodeHandle
  operator~() const;

  /// @brief ノードを得る．
  const BdnNode*
  node() const;

  /// @brief 極性を得る．
  /// @return 反転している時 true を返す．
  bool
  inv() const;

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero();

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const;

  /// @brief 定数を指しているとき true を返す．
  bool
  is_const() const;

  /// @brief 等価比較演算子
  bool
  operator==(BdnConstNodeHandle right) const;

};