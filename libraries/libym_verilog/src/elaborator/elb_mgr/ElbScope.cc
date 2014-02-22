
/// @file ElbScope.cc
/// @brief ElbScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbScope::ElbScope() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbScope::~ElbScope()
{
}

END_NAMESPACE_YM_VERILOG
