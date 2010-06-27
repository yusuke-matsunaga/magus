
/// @file libym_mvn/MvRelOp.cc
/// @brief MvRelOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvRelOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvEq::MvEq(MvModule* module,
	   ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, 1)
{
}

// @brief デストラクタ
MvEq::~MvEq()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvEq::type() const
{
  return kEq;
}


//////////////////////////////////////////////////////////////////////
// クラス MvLt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvLt::MvLt(MvModule* module,
	   ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, 1)
{
}

// @brief デストラクタ
MvLt::~MvLt()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvLt::type() const
{
  return kLt;
}

END_NAMESPACE_YM_MVN
