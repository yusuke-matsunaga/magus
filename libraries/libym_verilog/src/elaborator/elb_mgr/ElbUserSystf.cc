
/// @file ElbUserSystf.cc
/// @brief ElbUserSystf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


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
