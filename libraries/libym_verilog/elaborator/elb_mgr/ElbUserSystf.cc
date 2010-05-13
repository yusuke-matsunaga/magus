
/// @file libym_verilog/elb/ElbUserSystf.cc
/// @brief ElbUserSystf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbUserSystf.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "ElbUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbUserSystf
//////////////////////////////////////////////////////////////////////

// @brief 名前を返す．
string
ElbUserSystf::name() const
{
  return _name();
}

END_NAMESPACE_YM_VERILOG
