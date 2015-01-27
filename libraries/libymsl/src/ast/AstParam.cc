
/// @file AstParam.cc
/// @brief AstParam の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstParam.h"


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
		   AstType* type,
		   AstExpr* init_expr,
		   const FileRegion& loc) :
  Ast(loc),
  mName(name),
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

// @brief 型を得る．
const AstType*
AstParam::type() const
{
  return mType;
}

// @brief 初期化式を返す．
//
// NULL の場合もある．
const AstExpr*
AstParam::init_expr() const
{
  return mInitExpr;
}

END_NAMESPACE_YM_YMSL
