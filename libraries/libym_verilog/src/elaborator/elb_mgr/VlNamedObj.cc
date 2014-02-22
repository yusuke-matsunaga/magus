
/// @file VlNamedObj.cc
/// @brief VlNamedObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlNamedObj.h"
#include "verilog/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlNamedObj
//////////////////////////////////////////////////////////////////////

// @brief このオブジェクトの属している module を返す．
const VlModule*
VlNamedObj::parent_module() const
{
  // デフォルトは parent() に同じ質問をする．
  return parent()->parent_module();
}

// @brief fullname の取得
string
VlNamedObj::full_name() const
{
  string tmp;
  if ( name() ) {
    tmp = name();
  }
  else {
    tmp = "<anonymous>";
  }
  if ( parent() ) {
    return parent()->full_name() + "." + tmp;
  }
  return tmp;
}


//////////////////////////////////////////////////////////////////////
// クラス VlModule
//////////////////////////////////////////////////////////////////////

// @brief 自分の属する module を得る．
// @note このクラスでは this を返す．
const VlModule*
VlModule::parent_module() const
{
  return this;
}


END_NAMESPACE_YM_VERILOG
