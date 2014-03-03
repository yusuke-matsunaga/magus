
/// @file /NpnVmap.sip
/// @brief NpnVmap の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class NpnVmap
{
%TypeHeaderCode
#include "logic/NpnVmap.h"
using nsYm::NpnVmap;
%End

public:

  /// @brief 空のコンストラクタ．
  /// @note 内容は不定
  NpnVmap();

  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] var 変数番号
  /// @param[in] inv 反転属性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  NpnVmap(VarId var,
	  bool inv = false);

  /// @brief 不正な値を返すクラス関数
  NpnVmap
  invalid();

  /// @brief 変換先の変数番号を得る．
  VarId
  var() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 不正な値の時に true を返す．
  bool
  is_invalid() const;

  /// @brief 等価比較演算
  bool
  operator==(const NpnVmap& right) const;

};