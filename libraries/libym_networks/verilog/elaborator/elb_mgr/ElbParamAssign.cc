
/// @file libym_verilog/elb/ElbParamAssign.cc
/// @brief ElbParamAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParamAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbParamAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbParamAssign::ElbParamAssign() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbParamAssign::~ElbParamAssign()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDefParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbDefParam::ElbDefParam() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbDefParam::~ElbDefParam()
{
}

END_NAMESPACE_YM_VERILOG
