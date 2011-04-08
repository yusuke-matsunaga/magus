
/// @file libym_mvn/MvnConstBitSelect.cc
/// @brief MvnConstBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConstBitSelect.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bitpos ビット位置
// @param[in] bit_width 入力のビット幅
MvnConstBitSelect::MvnConstBitSelect(MvnModule* module,
				     ymuint bitpos,
				     ymuint bit_width) :
  MvnUnaryOp(module, bit_width, 1),
  mBitPos(bitpos)
{
}

// @brief デストラクタ
MvnConstBitSelect::~MvnConstBitSelect()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnConstBitSelect::type() const
{
  return kConstBitSelect;
}

// @brief ビット位置を得る．
// @note type() が kConstBitSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnConstBitSelect::bitpos() const
{
  return mBitPos;
}

// @brief bit-selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bitpos ビット位置
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_constbitselect(MvnModule* module,
			   ymuint bitpos,
			   ymuint bit_width)
{
  MvnNode* node = new MvnConstBitSelect(module, bitpos, bit_width);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
