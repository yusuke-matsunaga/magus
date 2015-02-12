
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"

#include "AstMgr.h"
#include "IrMgr.h"
#include "IrToplevel.h"

BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslCompiler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslCompiler::YmslCompiler()
{
}

// @brief デストラクタ
YmslCompiler::~YmslCompiler()
{
}

// @brief コンパイルする．
// @param[in] ido 入力データ
// @return 成功したら true を返す．
bool
YmslCompiler::compile(IDO& ido)
{
  AstMgr ast_mgr;

  bool stat1 = ast_mgr.read_source(ido);
  if ( !stat1 ) {
    return false;
  }

  IrMgr ir_mgr;

  // 中間表現を作る．
  AstStatement* toplevel = ast_mgr.toplevel();
  IrToplevel* ir_toplevel = new IrToplevel;
  bool stat2 = ir_mgr.elaborate(toplevel, *ir_toplevel);
  if ( !stat2 ) {
    return false;
  }

  return true;
}

END_NAMESPACE_YM_YMSL
