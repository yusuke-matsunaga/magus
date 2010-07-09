
/// @file libym_mvn/MvConstPartSelect.cc
/// @brief MvConstPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConstPartSelect.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
// @param[in] bitwidth 入力のビット幅
MvConstPartSelect::MvConstPartSelect(MvModule* module,
				     ymuint msb,
				     ymuint lsb,
				     ymuint bitwidth) :
  MvNode(module, 1, 1),
  mMsb(msb),
  mLsb(lsb)
{
  set_bit_width(input(0), bitwidth);
  set_bit_width(output(0), 1);
}

// @brief デストラクタ
MvConstPartSelect::~MvConstPartSelect()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvConstPartSelect::type() const
{
  return kConstPartSelect;
}

// @brief 範囲指定の MSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvConstPartSelect::msb() const
{
  return mMsb;
}

// @brief 範囲指定の LSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvConstPartSelect::lsb() const
{
  return mLsb;
}

END_NAMESPACE_YM_MVN
