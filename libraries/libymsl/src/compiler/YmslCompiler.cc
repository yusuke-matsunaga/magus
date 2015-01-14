
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"
#include "AstMgr.h"
#include "AstStatement.h"
#include "YmslScope.h"


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
  AstMgr mgr;

  bool stat = mgr.read_source(ido);
  if ( !stat ) {
    return false;
  }

  AstStatement* toplevel = mgr.toplevel();

  YmslScope* toplevel_scope = new YmslScope(NULL, ShString("__main__"));
  toplevel->phase1(toplevel_scope);
  toplevel->phase2(toplevel_scope);

  return true;
}

END_NAMESPACE_YM_YMSL
