
/// @file ElbParamAssign.cc
/// @brief ElbParamAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
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
