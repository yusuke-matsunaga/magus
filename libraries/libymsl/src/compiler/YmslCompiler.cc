
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
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
// @return コンパイルしたモジュールを返す．
//
// エラーが起きたら NULL を返す．
YmslModule*
YmslCompiler::compile(IDO& ido)
{
  AstMgr ast_mgr;

  bool stat1 = ast_mgr.read_source(ido);
  if ( !stat1 ) {
    return NULL;
  }

  IrMgr ir_mgr;

  // 中間表現を作る．
  AstStatement* toplevel = ast_mgr.toplevel();
  IrToplevel* ir_toplevel = new IrToplevel;
  bool stat2 = ir_mgr.elaborate(toplevel, *ir_toplevel);
  if ( !stat2 ) {
    return NULL;
  }

  return NULL;
}

END_NAMESPACE_YM_YMSL
