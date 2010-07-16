
/// @file libym_mvn/MvOutput.cc
/// @brief MvOutput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvOutput.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pos 位置
// @param[in] bit_width ビット幅
MvOutput::MvOutput(MvModule* module,
		   ymuint pos,
		   ymuint bit_width) :
  MvNode(module, 1, 0),
  mPos(pos)
{
  set_bit_width(_input(0), bit_width);
}

// @brief デストラクタ
MvOutput::~MvOutput()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvOutput::type() const
{
  return kOutput;
}

// @brief 入力ノード/出力ノードの場合に位置を返す．
// @note type() が kInput と kOutput の時のみ意味を持つ．
ymuint
MvOutput::pos() const
{
  return mPos;
}

END_NAMESPACE_YM_MVN
