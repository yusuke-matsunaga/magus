
/// @file YmslAstUniOp.cc
/// @brief YmslAstUniOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
YmslAstUniOp::YmslAstUniOp(YmslAst* child,
			   const FileRegion& loc) :
  YmslAstImpl(FileRegion(loc, child->file_region())),
  mChild(child)
{
}

// @brief デストラクタ
YmslAstUniOp::~YmslAstUniOp()
{
}

// @brief 子供の数を返す．
ymuint
YmslAstUniOp::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstUniOp::child(ymuint pos) const
{
  ASSERT_COND( pos == 0 );
  return mChild;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
YmslAstUniOp::print(ostream& s) const
{
  switch ( type() ) {
  case kAstUniPlus: s << "+"; break;
  case kAstUniMinus: s << "-"; break;
  case kAstBitNeg: s << "~"; break;
  case kAstLogNot: s << "!"; break;
  default: ASSERT_NOT_REACHED;
  }
  child(0)->print(s);
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstUniPlus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
YmslAstUniPlus::YmslAstUniPlus(YmslAst* child,
			       const FileRegion& loc) :
  YmslAstUniOp(child, loc)
{
}

// @brief デストラクタ
YmslAstUniPlus::~YmslAstUniPlus()
{
}

// @brief 型を得る．
AstType
YmslAstUniPlus::type() const
{
  return kAstUniPlus;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstUniMinus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
YmslAstUniMinus::YmslAstUniMinus(YmslAst* child,
				 const FileRegion& loc) :
  YmslAstUniOp(child, loc)
{
}

// @brief デストラクタ
YmslAstUniMinus::~YmslAstUniMinus()
{
}

// @brief 型を得る．
AstType
YmslAstUniMinus::type() const
{
  return kAstUniMinus;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBitNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
YmslAstBitNeg::YmslAstBitNeg(YmslAst* child,
			     const FileRegion& loc) :
  YmslAstUniOp(child, loc)
{
}

// @brief デストラクタ
YmslAstBitNeg::~YmslAstBitNeg()
{
}

// @brief 型を得る．
AstType
YmslAstBitNeg::type() const
{
  return kAstBitNeg;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLogNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
YmslAstLogNot::YmslAstLogNot(YmslAst* child,
			     const FileRegion& loc) :
  YmslAstUniOp(child, loc)
{
}

// @brief デストラクタ
YmslAstLogNot::~YmslAstLogNot()
{
}

// @brief 型を得る．
AstType
YmslAstLogNot::type() const
{
  return kAstLogNot;
}


END_NAMESPACE_YM_YMSL
