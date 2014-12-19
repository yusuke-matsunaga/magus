
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
// @param[in] global グローバル変数の時 true にするフラグ
// @param[in] loc ファイル位置
AstVarDecl::AstVarDecl(ShString name,
		       ValueType type,
		       AstExpr* init_expr,
		       bool global,
		       const FileRegion& loc) :
  AstDeclBase(name, loc),
  mIndex(-1),
  mType(type),
  mInitExpr(init_expr),
  mGlobal(global)
{
  mPrev = NULL;
}

// @brief デストラクタ
AstVarDecl::~AstVarDecl()
{
}

// @brief インデックス番号を返す．
ymuint
AstVarDecl::index() const
{
  return mIndex;
}

// @brief 型を得る．
ValueType
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

// @brief グローバル変数の時 true を返す．
bool
AstVarDecl::global() const
{
  return mGlobal;
}

// @brief 前の要素を取り出す．
AstVarDecl*
AstVarDecl::prev() const
{
  return mPrev;
}

// @brief 前の要素をセットする．
void
AstVarDecl::set_prev(AstVarDecl* prev)
{
  mPrev = prev;
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
  for (AstVarDecl* vd = param_list; vd != NULL; vd = vd->prev()) {
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
			 ValueType type,
			 AstVarDecl* param_list,
			 AstBlock* block,
			 const FileRegion& loc) :
  AstDeclBase(name, loc),
  mIndex(-1),
  mType(type),
  mParamList(count_size(param_list)),
  mBlock(block)
{
  ymuint i = mParamList.size();
  for (AstVarDecl* vd = param_list; vd != NULL; vd = vd->prev()) {
    -- i;
    mParamList[i] = vd;
  }
}

// @brief デストラクタ
AstFuncDecl::~AstFuncDecl()
{
}

// @brief インデックス番号を返す．
ymuint
AstFuncDecl::index() const
{
  return mIndex;
}

// @brief 出力の型を返す．
ValueType
AstFuncDecl::type() const
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
