
/// @file libym_verilog/elb/ElbContAssign.cc
/// @brief ElbContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbContAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbCaHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbCaHead::ElbCaHead()
{
}

// @brief デストラクタ
ElbCaHead::~ElbCaHead()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbContAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbContAssign::ElbContAssign() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbContAssign::~ElbContAssign()
{
}

END_NAMESPACE_YM_VERILOG
