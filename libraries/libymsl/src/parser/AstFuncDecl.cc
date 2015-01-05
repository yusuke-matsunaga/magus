
/// @file AstDeclBase.cc
/// @brief AstDeclBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncDecl.h"
#include "AstVarDecl.h"


BEGIN_NAMESPACE_YM_YMSL

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
  Ast(loc),
  mName(name),
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

// @brief 名前を得る．
ShString
AstFuncDecl::name() const
{
  return mName;
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
