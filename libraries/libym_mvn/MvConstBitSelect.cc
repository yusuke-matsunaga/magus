
/// @file libym_mvn/MvConstBitSelect.cc
/// @brief MvConstBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConstBitSelect.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bitpos ビット位置
// @param[in] bitwidth 入力のビット幅
MvConstBitSelect::MvConstBitSelect(MvModule* module,
				   ymuint bitpos,
				   ymuint bitwidth) :
  MvNode(module, 1, 1),
  mBitPos(bitpos)
{
  set_bit_width(input(0), bitwidth);
  set_bit_width(output(0), 1);
}

// @brief デストラクタ
MvConstBitSelect::~MvConstBitSelect()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvConstBitSelect::type() const
{
  return kConstBitSelect;
}

// @brief ビット位置を得る．
// @note type() が kConstBitSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvConstBitSelect::bitpos() const
{
  return mBitPos;
}

END_NAMESPACE_YM_MVN
