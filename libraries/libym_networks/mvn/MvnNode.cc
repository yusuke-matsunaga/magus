
/// @file libym_networks/MvnNode.cc
/// @brief MvnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

// @brief コンストラクタ
// @param[in] module 親のモジュール
MvnNode::MvnNode(MvnModule* parent) :
  mParent(parent)
{
}

// @brief デストラクタ
MvnNode::~MvnNode()
{
}

// @brief クロック信号の極性を得る．
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNode::clock_pol() const
{
  return 0;
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNode::control_pol(ymuint pos) const
{
  return 0;
}

// @brief ビット位置を得る．
// @note type() が kConstBitSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNode::bitpos() const
{
  return 0;
}

// @brief 範囲指定の MSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNode::msb() const
{
  return 0;
}

// @brief 範囲指定の LSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNode::lsb() const
{
  return 0;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
void
MvnNode::const_value(vector<ymuint32>& val) const
{
}

END_NAMESPACE_YM_NETWORKS
