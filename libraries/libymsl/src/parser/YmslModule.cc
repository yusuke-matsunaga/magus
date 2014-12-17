
/// @file YmslModule.cc
/// @brief YmslModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslModule.h"
#include "AstBlock.h"

#include "../builtin/YmslPrint.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name モジュール名
YmslModule::YmslModule(ShString name) :
  mName(name)
{
  mToplevelBlock = new AstBlock(&mGlobalDict);
  mBlockStack.push_back(mToplevelBlock);

#if 0
  YmslFunc* func1 = new YmslPrint(ShString("print"), vector<YmslVar*>(0));
  mGlobalDict.add_function(func1);
#endif
}

// @brief デストラクタ
YmslModule::~YmslModule()
{
}

// @brief モジュール名を返す．
ShString
YmslModule::name() const
{
  return mName;
}

// @brief トップレベルブロックを返す．
AstBlock*
YmslModule::toplevel_block() const
{
  return mToplevelBlock;
}

// @brief 現在のブロックを返す．
AstBlock*
YmslModule::cur_block() const
{
  ASSERT_COND( !mBlockStack.empty() );
  return mBlockStack.back();
}

// @brief 新しいブロックを作りスタックに積む．
// @return 新しいブロックを返す．
AstBlock*
YmslModule::push_new_block()
{
  AstBlock* parent = cur_block();
  AstBlock* block = new AstBlock(parent);
  mBlockStack.push_back(block);
  return block;
}

// @brief ブロックをスタックから取り去る．
void
YmslModule::pop_block()
{
  mBlockStack.pop_back();
}

// @brief 関数を追加する．
void
YmslModule::add_function(AstFuncDecl* funcdecl)
{
  mGlobalDict.add_function(funcdecl);
}

// @brief グローバル変数を追加する．
// @param[in] vardecl 変数宣言
void
YmslModule::add_global_var(AstVarDecl* vardecl)
{
  mGlobalDict.add_vardecl(vardecl);
}

// @brief 現在のブロックに変数を追加する．
void
YmslModule::add_local_var(AstVarDecl* vardecl)
{
  mBlockStack.back()->add_vardecl(vardecl);
}

// @brief 現在のブロックに statement を追加する．
void
YmslModule::add_statement(AstStatement* stmt)
{
  mBlockStack.back()->add_statement(stmt);
}

// @brief 関数を探す．
// @param[in] name 関数名
//
// 見つからなければ NULL を返す．
AstFuncDecl*
YmslModule::find_function(ShString name) const
{
  return mGlobalDict.find_function(name);
}

// @brief 変数を探す．
// @param[in] name 変数名
//
// 見つからなければ NULL を返す．
AstVarDecl*
YmslModule::find_var(ShString name) const
{
  AstBlock* block = cur_block();
  return block->find_vardecl(name);
}

END_NAMESPACE_YM_YMSL
