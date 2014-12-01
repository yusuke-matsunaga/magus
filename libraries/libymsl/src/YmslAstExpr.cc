
/// @file YmslAstImpl.cc
/// @brief YmslAstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstFuncCall.h"
#include "YmslAstUniOp.h"
#include "YmslAstBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslAstFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 関数名
// @param[in] expr_list 引数リスト
// @param[in] loc ファイル位置
YmslAstFuncCall::YmslAstFuncCall(YmslAst* id,
				 YmslAst* expr_list,
				 const FileRegion& loc) :
  YmslAstImpl(loc)
{
  mChildList[0] = id;
  mChildList[1] = expr_list;
}

// @brief デストラクタ
YmslAstFuncCall::~YmslAstFuncCall()
{
}

// @brief 型を得る．
AstType
YmslAstFuncCall::type() const
{
  return kAstFuncCall;
}

// @brief 子供の数を返す．
ymuint
YmslAstFuncCall::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstFuncCall::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstFuncCall::print(ostream& s,
		       ymuint indent) const
{
  print_indent(s, indent);
  mChildList[0]->print(s);
  s << "(";
  {
    ymuint n = mChildList[1]->child_num();
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      YmslAst* expr = mChildList[1]->child(i);
      s << comma;
      comma = ", ";
      expr->print(s);
    }
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBinOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] left, right オペランド
YmslAstBinOp::YmslAstBinOp(YmslAst* left,
			   YmslAst* right) :
  YmslAstImpl(FileRegion(left->file_region(), right->file_region()))
{
  mChild[0] = left;
  mChild[1] = right;
}

// @brief デストラクタ
YmslAstBinOp::~YmslAstBinOp()
{
}

// @brief 子供の数を返す．
ymuint
YmslAstBinOp::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
YmslAst*
YmslAstBinOp::child(ymuint pos) const
{
  ASSERT_COND( pos < 2 );
  return mChild[pos % 2];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslAstBinOp::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
  s << "(";
  child(0)->print(s);
  switch ( type() ) {
  case kAstPlus:   s << "+"; break;
  case kAstMinus:  s << "-"; break;
  case kAstMult:   s << "*"; break;
  case kAstDiv:    s << "/"; break;
  case kAstMod:    s << "%"; break;
  case kAstBitAnd: s << "&"; break;
  case kAstBitOr:  s << "|"; break;
  case kAstBitXor: s << "^"; break;
  case kAstLogAnd: s << " and "; break;
  case kAstLogOr:  s << " or "; break;
  case kAstLogXor: s << " xor "; break;
  case kAstEqEq:   s << "=="; break;
  case kAstNotEq:  s << "!="; break;
  case kAstLt:     s << "<"; break;
  case kAstGt:     s << ">"; break;
  case kAstLe:     s << "<="; break;
  case kAstGe:     s << ">="; break;
  default: ASSERT_NOT_REACHED;
  }
  child(1)->print(s);
  s << ")";
}



//////////////////////////////////////////////////////////////////////
// クラス YmslAstPlus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstPlus::YmslAstPlus(YmslAst* left,
			 YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstPlus::~YmslAstPlus()
{
}

// @brief 型を得る．
AstType
YmslAstPlus::type() const
{
  return kAstPlus;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstMinus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstMinus::YmslAstMinus(YmslAst* left,
			   YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstMinus::~YmslAstMinus()
{
}

// @brief 型を得る．
AstType
YmslAstMinus::type() const
{
  return kAstMinus;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstMult
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstMult::YmslAstMult(YmslAst* left,
			 YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstMult::~YmslAstMult()
{
}

// @brief 型を得る．
AstType
YmslAstMult::type() const
{
  return kAstMult;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstDiv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstDiv::YmslAstDiv(YmslAst* left,
			 YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstDiv::~YmslAstDiv()
{
}

// @brief 型を得る．
AstType
YmslAstDiv::type() const
{
  return kAstDiv;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstMod
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstMod::YmslAstMod(YmslAst* left,
			 YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstMod::~YmslAstMod()
{
}

// @brief 型を得る．
AstType
YmslAstMod::type() const
{
  return kAstMod;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBitAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstBitAnd::YmslAstBitAnd(YmslAst* left,
			     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstBitAnd::~YmslAstBitAnd()
{
}

// @brief 型を得る．
AstType
YmslAstBitAnd::type() const
{
  return kAstBitAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBitOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstBitOr::YmslAstBitOr(YmslAst* left,
			   YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstBitOr::~YmslAstBitOr()
{
}

// @brief 型を得る．
AstType
YmslAstBitOr::type() const
{
  return kAstBitOr;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstBitXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstBitXor::YmslAstBitXor(YmslAst* left,
			     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstBitXor::~YmslAstBitXor()
{
}

// @brief 型を得る．
AstType
YmslAstBitXor::type() const
{
  return kAstBitXor;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLogAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstLogAnd::YmslAstLogAnd(YmslAst* left,
			     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstLogAnd::~YmslAstLogAnd()
{
}

// @brief 型を得る．
AstType
YmslAstLogAnd::type() const
{
  return kAstLogAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLogOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstLogOr::YmslAstLogOr(YmslAst* left,
			   YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstLogOr::~YmslAstLogOr()
{
}

// @brief 型を得る．
AstType
YmslAstLogOr::type() const
{
  return kAstLogOr;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLogXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstLogXor::YmslAstLogXor(YmslAst* left,
			     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstLogXor::~YmslAstLogXor()
{
}

// @brief 型を得る．
AstType
YmslAstLogXor::type() const
{
  return kAstLogXor;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstEqEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstEqEq::YmslAstEqEq(YmslAst* left,
			 YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstEqEq::~YmslAstEqEq()
{
}

// @brief 型を得る．
AstType
YmslAstEqEq::type() const
{
  return kAstEqEq;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstNotEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstNotEq::YmslAstNotEq(YmslAst* left,
			   YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstNotEq::~YmslAstNotEq()
{
}

// @brief 型を得る．
AstType
YmslAstNotEq::type() const
{
  return kAstNotEq;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstLt::YmslAstLt(YmslAst* left,
		     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstLt::~YmslAstLt()
{
}

// @brief 型を得る．
AstType
YmslAstLt::type() const
{
  return kAstLt;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstGt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstGt::YmslAstGt(YmslAst* left,
		     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstGt::~YmslAstGt()
{
}

// @brief 型を得る．
AstType
YmslAstGt::type() const
{
  return kAstGt;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstLe
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstLe::YmslAstLe(YmslAst* left,
		     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstLe::~YmslAstLe()
{
}

// @brief 型を得る．
AstType
YmslAstLe::type() const
{
  return kAstLe;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslAstGe
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
YmslAstGe::YmslAstGe(YmslAst* left,
		     YmslAst* right) :
  YmslAstBinOp(left, right)
{
}

// @brief デストラクタ
YmslAstGe::~YmslAstGe()
{
}

// @brief 型を得る．
AstType
YmslAstGe::type() const
{
  return kAstGe;
}


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
// @param[in] indent インデントレベル
void
YmslAstUniOp::print(ostream& s,
		    ymuint indent) const
{
  print_indent(s, indent);
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
