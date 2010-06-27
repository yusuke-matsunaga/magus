
/// @file libym_mvn/MvThrough.cc
/// @brief MvThrough の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvThrough.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvThrough::MvThrough(MvModule* module,
		     ymuint bit_width) :
  MvUnaryOp(module, bit_width, bit_width)
{
}

// @brief デストラクタ
MvThrough::~MvThrough()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvThrough::type() const
{
  return kThrough;
}

END_NAMESPACE_YM_MVN
