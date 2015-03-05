
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"

#include "AstMgr.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "IrMgr.h"
#include "IrToplevel.h"
#include "VsmGen.h"

#include "YmUtils/FileIDO.h"


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
// @param[in] name モジュール名
// @return コンパイルしたモジュールを返す．
//
// エラーが起きたら NULL を返す．
VsmModule*
YmslCompiler::compile(IDO& ido,
		      ShString name)
{
  AstMgr ast_mgr;

  bool stat1 = ast_mgr.read_source(ido);
  if ( !stat1 ) {
    return NULL;
  }

  IrMgr ir_mgr;

  // 中間表現を作る．
  AstStatement* ast_toplevel = ast_mgr.toplevel();
  IrToplevel* ir_toplevel = ir_mgr.elaborate(ast_toplevel, name, *this);
  if ( ir_toplevel == NULL ) {
    return NULL;
  }

  // コード生成を行う．
  VsmGen vsmgen;

  VsmModule* module = vsmgen.code_gen(ir_toplevel, name);

  return module;
}

// @brief モジュールを import する．
// @param[in] name モジュール名
// @return モジュールを返す．
//
// エラーが起きたら NULL を返す．
VsmModule*
YmslCompiler::import(ShString name)
{
  // 実際に import する．
  string body = static_cast<const char*>(name);
  for ( ; ; ) {
    // 実はループじゃないけど break を使いたいので
    PathName path = body + ".ym";
    // サーチパスを考慮してファイルを探す．
    PathName fullpath = mPathList.search(path);
    if ( !fullpath.is_valid() ) {
      cout << path.str() << " not found" << endl;
      break;
    }
    FileIDO ido;
    bool stat = ido.open(fullpath.str());
    ASSERT_COND( stat );
    VsmModule* module = compile(ido, name);
    return module;
  }

  for ( ; ; ) {
    // 実はループじゃないけど break を使いたいので
    PathName path = body + ".ymc";
    // サーチパスを考慮してファイルを探す．
    PathName fullpath = mPathList.search(path);
    if ( !fullpath.is_valid() ) {
      break;
    }
    FileIDO ido;
    bool stat = ido.open(fullpath.str());
    ASSERT_COND( stat );
    //VsmModule* module = load(ido);
    VsmModule* module = NULL;
    return module;
  }

  return NULL;
}

// @brief サーチパスの先頭に path を追加する．
// @param[in] path 追加するパス
// @note path は ':' を含んでいても良い
void
YmslCompiler::add_searchpath_top(const string& path)
{
  mPathList.add_top(path);
}

// @brief サーチパスの末尾に path を追加する．
// @param[in] path 追加するパス
// @note path は ':' を含んでいても良い
void
YmslCompiler::add_searchpath_end(const string& path)
{
  mPathList.add_end(path);
}

END_NAMESPACE_YM_YMSL
