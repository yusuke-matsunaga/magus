
/// @file BdnPort.sip
/// @brief BdnPort の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnPort
{
%TypeHeaderCode
#include "networks/BdnPort.h"
using nsYm::BdnPort;
%End

public:

  /// @brief ID 番号を得る．
  ymuint
  id() const;

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief ビットごとの方向を得る．
  /// @param[out] iovect ビットごとの方向を収める配列
  /// @note iovect の値の意味は以下の通り
  ///  - 0 : なし
  ///  - 1 : 入力のみ
  ///  - 2 : 出力のみ
  ///  - 3 : 入力と出力
  void
  get_iovect(vector<ymuint>& iovect) const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 出力ポートの場合には NULL を返す．
  const BdnNode*
  input(ymuint pos) const;

  /// @brief 出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < bit_width )
  /// @note 入力ポートの場合には NULL を返す．
  const BdnNode*
  output(ymuint pos) const;

};