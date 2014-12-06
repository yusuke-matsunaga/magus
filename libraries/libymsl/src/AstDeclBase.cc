
/// @file AstDeclBase.cc
/// @brief AstDeclBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstDeclBase.h"
#include "AstVarDecl.h"
#include "AstFuncDecl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstDeclBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] loc ファイル位置
AstDeclBase::AstDeclBase(ShString name,
			 const FileRegion& loc) :
  Ast(loc),
  mName(name)
{
}

// @brief デストラクタ
AstDeclBase::~AstDeclBase()
{
}

// @brief 名前を得る．
ShString
AstDeclBase::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス AstVarDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 変数名
// @param[in] type 型
// @param[in] init_expr 初期化式
// @param[in] loc ファイル位置
AstVarDecl::AstVarDecl(ShString name,
		       AstValueType* type,
		       AstExpr* init_expr,
		       const FileRegion& loc) :
  AstDeclBase(name, loc),
  mType(type),
  mInitExpr(init_expr)
{
}

// @brief デストラクタ
AstVarDecl::~AstVarDecl()
{
}

// @brief 型を得る．
AstValueType*
AstVarDecl::type() const
{
  return mType;
}

// @brief 初期化式を返す．
//
// NULL の場合もある．
AstExpr*
AstVarDecl::init_expr() const
{
  return mInitExpr;
}

// @brief 次の要素を取り出す．
AstVarDecl*
AstVarDecl::next()
{
  return mNext;
}

// @brief 前の要素をセットする．
void
AstVarDecl::set_prev(AstVarDecl* prev)
{
  if ( prev != NULL ) {
    prev->mNext = this;
  }
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstVarDecl::print(ostream& s,
		  ymuint indent) const
{
}


//////////////////////////////////////////////////////////////////////
// クラス AstFuncDecl
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

ymuint
count_size(AstVarDecl* param_list)
{
  ymuint n = 0;
  for (AstVarDecl* vd = param_list; vd != NULL; vd = vd->next()) {
    ++ n;
  }
  return n;
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] block 本文のブロック
// @param[in] loc ファイル位置
AstFuncDecl::AstFuncDecl(ShString name,
			 AstValueType* type,
			 AstVarDecl* param_list,
			 AstBlock* block,
			 const FileRegion& loc) :
  AstDeclBase(name, loc),
  mType(type),
  mParamList(count_size(param_list)),
  mBlock(block)
{
  ymuint i = mParamList.size();
  for (AstVarDecl* vd = param_list; vd != NULL; vd = vd->next()) {
    -- i;
    mParamList[i] = vd;
  }
}

// @brief デストラクタ
AstFuncDecl::~AstFuncDecl()
{
}

// @brief 出力の型を返す．
AstValueType*
AstFuncDecl::output_type() const
{
  return mType;
}

// @brief パラメータリストを返す．
const vector<AstVarDecl*>&
AstFuncDecl::param_list() const
{
  return mParamList;
}

// @brief ブロックを返す．
AstBlock*
AstFuncDecl::block() const
{
  return mBlock;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFuncDecl::print(ostream& s,
		   ymuint indent) const
{
}

END_NAMESPACE_YM_YMSL
