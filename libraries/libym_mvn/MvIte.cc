
/// @file libym_mvn/MvIte.cc
/// @brief MvIte の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvIte.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvIte
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvIte::MvIte(MvModule* module,
	     ymuint bit_width) :
  MvTernaryOp(module, 1, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvIte::~MvIte()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvIte::type() const
{
  return kIte;
}

END_NAMESPACE_YM_MVN
