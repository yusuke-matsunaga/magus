
/// @file AstParam.cc
/// @brief AstParam の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstParam.h"
#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstParam::AstParam(ShString name,
		   ValueType type,
		   AstExpr* init_expr,
		   const FileRegion& loc) :
  Ast(loc),
  mName(name),
  mIndex(-1),
  mType(type),
  mInitExpr(init_expr)
{
}

// @brief デストラクタ
AstParam::~AstParam()
{
}

// @brief 名前を得る．
ShString
AstParam::name() const
{
  return mName;
}

// @brief インデックス番号を返す．
ymuint
AstParam::index() const
{
  return mIndex;
}

// @brief 型を得る．
ValueType
AstParam::type() const
{
  return mType;
}

// @brief 初期化式を返す．
//
// NULL の場合もある．
AstExpr*
AstParam::init_expr() const
{
  return mInitExpr;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstParam::print(ostream& s) const
{
  s << name() << ": " << type();
  if ( init_expr() != NULL ) {
    s << " = ";
    init_expr()->print(s);
  }
}

END_NAMESPACE_YM_YMSL
