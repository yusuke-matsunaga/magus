
/// @file AstStatement.cc
/// @brief AstStatement の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstStatement
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstStatement::AstStatement(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstStatement::~AstStatement()
{
}

// @brief ラベルステートメントの場合に名前を返す．
//
// それ以外では ShString() を返す．
ShString
AstStatement::label() const
{
  return ShString();
}

// @brief インデントする．
// @param[in] s 出力ストリーム
// @param[in] indent インデント量
void
AstStatement::print_indent(ostream& s,
			   ymuint indent)
{
  for (ymuint i = 0; i < indent; ++ i) {
    s << "  ";
  }
}

END_NAMESPACE_YM_YMSL
