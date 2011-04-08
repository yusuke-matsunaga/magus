
/// @file libym_mvn/MvnTernaryOp.cc
/// @brief MvnTernaryOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnTernaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnTernaryOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 入力3のビット幅
// @param[in] bit_width4 出力のビット幅
MvnTernaryOp::MvnTernaryOp(MvnModule* module,
			   ymuint bit_width1,
			   ymuint bit_width2,
			   ymuint bit_width3,
			   ymuint bit_width4) :
  MvnNode(module, 3, 1)
{
  set_ipin_bit_width(0, bit_width1);
  set_ipin_bit_width(1, bit_width2);
  set_ipin_bit_width(2, bit_width3);
  set_opin_bit_width(0, bit_width4);
}

// @brief デストラクタ
MvnTernaryOp::~MvnTernaryOp()
{
}

END_NAMESPACE_YM_MVN
