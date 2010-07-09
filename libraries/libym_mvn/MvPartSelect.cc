
/// @file libym_mvn/MvPartSelect.cc
/// @brief MvPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvPartSelect.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 範囲指定入力(base位置)のビット幅
// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
MvPartSelect::MvPartSelect(MvModule* module,
			   ymuint bit_width1,
			   ymuint bit_width2,
			   ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvPartSelect::~MvPartSelect()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvPartSelect::type() const
{
  return kPartSelect;
}

END_NAMESPACE_YM_MVN
