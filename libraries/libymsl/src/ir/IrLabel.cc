
/// @file IrLabel.cc
/// @brief IrLabel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrLabel::IrLabel() :
  IrNode(kOpLabel, NULL)
{
}

// @brief デストラクタ
IrLabel::~IrLabel()
{
}

END_NAMESPACE_YM_YMSL
