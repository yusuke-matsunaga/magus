
/// @file libym_mvn/MvConcat.cc
/// @brief MvConcat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvConcat.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvConcat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvConcat::MvConcat(MvModule* module,
		   ymuint ni) :
  MvNode(module, ni, 1)
{
}

// @brief デストラクタ
MvConcat::~MvConcat()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvConcat::type() const
{
  return kConcat;
}

END_NAMESPACE_YM_MVN
