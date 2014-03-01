
/// @file BdnNodeHandle.sip
/// @brief BdnNodeHandle の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnNodeHandle
{
%TypeHeaderCode
#include "networks/BdnNodeHandle.h"
using nsYm::BdnNodeHandle;
%End

public:

  /// @brief 空のコンストラクタ
  BdnNodeHandle();

  /// @brief 内容を設定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  BdnNodeHandle(BdnNode* node,
		bool inv = false);

  /// @brief 内容を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転している時に true とするフラグ
  void
  set(BdnNode* node,
      bool inv);

  /// @brief 定数0を返す．
  static
  BdnNodeHandle
  make_zero();

  /// @brief 定数1を返す．
  static
  BdnNodeHandle
  make_one();

  /// @brief 極性を否定したハンドルを返す．
  BdnNodeHandle
  operator~() const;

  /// @brief ノードを得る．
  BdnNode*
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
  operator==(BdnNodeHandle right) const;

};