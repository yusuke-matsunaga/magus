
/// @file libym_mvn/MvRop.cc
/// @brief MvRop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvRop.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvRand
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvRand::MvRand(MvModule* module,
	       ymuint bit_width) :
  MvUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvRand::~MvRand()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvRand::type() const
{
  return kRand;
}


//////////////////////////////////////////////////////////////////////
// クラス MvRor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvRor::MvRor(MvModule* module,
	       ymuint bit_width) :
  MvUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvRor::~MvRor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvRor::type() const
{
  return kRor;
}


//////////////////////////////////////////////////////////////////////
// クラス MvRxor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvRxor::MvRxor(MvModule* module,
	       ymuint bit_width) :
  MvUnaryOp(module, bit_width, 1)
{
}

// @brief デストラクタ
MvRxor::~MvRxor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvRxor::type() const
{
  return kRxor;
}

END_NAMESPACE_YM_MVN
