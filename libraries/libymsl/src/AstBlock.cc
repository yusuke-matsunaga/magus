
/// @file AstBlock.cc
/// @brief AstBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlock.h"
#include "YmslAst.h"
#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のブロック
AstBlock::AstBlock(AstBlock* parent) :
  mParent(parent)
{
}

// @brief デストラクタ
AstBlock::~AstBlock()
{
}

// @brief statement を追加する．
void
AstBlock::add_statement(AstStatement* statement)
{
  mStatementList.push_back(statement);
}

// @brief 関数定義を追加する．
// @param[in] item 追加する要素
void
AstBlock::add_funcdecl(AstFuncDecl* item)
{
  mSymbolTable.add(item->name(), item);
}

// @brief 変数定義を追加する．
// @param[in] item 追加する要素
void
AstBlock::add_vardecl(AstVarDecl* item)
{
  mSymbolTable.add(item->name(), item);
}
#if 0
// @brief 名前から要素を見つける．
// @param[in] name 名前
// @return name という名前の要素を返す．
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
YmslAst*
AstBlock::find_item(ShString name) const
{
  YmslAst* item;
  if ( mSymbolTable.find(name, item) ) {
    return item;
  }
  if ( mParent != NULL ) {
    return mParent->find_item(name);
  }
  return NULL;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBlock::print(ostream& s,
		ymuint indent) const
{
  YmslAstImpl::print_indent(s, indent);
  s << "{" << endl;

  ymuint n = mStatementList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStatementList[i]->print(s, indent + 1);
  }

  YmslAstImpl::print_indent(s, indent);
  s << "}" << endl;
}
#endif
END_NAMESPACE_YM_YMSL
