
/// @file AstBlock.cc
/// @brief AstBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlock.h"
#include "AstFuncDecl.h"
#include "AstStatement.h"
#include "AstVarDecl.h"
#include "SymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBlock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent_dict 親の辞書
AstBlock::AstBlock(YmslDict* parent_dict) :
  mDict(parent_dict)
{
}

// @brief コンストラクタ
// @param[in] parent 親のブロック
AstBlock::AstBlock(AstBlock* parent) :
  mDict(&parent->mDict)
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
  ShString label = statement->label();
  if ( label != ShString() ) {
    mDict.add_statement(statement);
  }
}

// @brief 変数定義を追加する．
// @param[in] item 追加する要素
void
AstBlock::add_vardecl(AstVarDecl* item)
{
  mDict.add_vardecl(item);
}

// @brief 名前からラベルステートメントを見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
AstStatement*
AstBlock::find_label(ShString name) const
{
  return mDict.find_label(name);
}

// @brief 名前から変数宣言を見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
AstVarDecl*
AstBlock::find_vardecl(ShString name) const
{
  return mDict.find_vardecl(name);
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBlock::print(ostream& s,
		ymuint indent) const
{
  ymuint n = mStatementList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStatementList[i]->print(s, indent);
  }
}

END_NAMESPACE_YM_YMSL
