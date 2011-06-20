
/// @file libym_verilog/elb/ElbProcess.cc
/// @brief ElbProcess の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbProcess.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbProcess
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbProcess::ElbProcess() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbProcess::~ElbProcess()
{
}

END_NAMESPACE_YM_VERILOG
