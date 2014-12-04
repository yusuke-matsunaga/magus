
/// @file AstImpl.cc
/// @brief AstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncCall.h"
#include "AstUniOp.h"
#include "AstBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 関数名
// @param[in] expr_list 引数リスト
// @param[in] loc ファイル位置
AstFuncCall::AstFuncCall(Ast* id,
			 Ast* expr_list,
			 const FileRegion& loc) :
  AstImpl(loc)
{
  mChildList[0] = id;
  mChildList[1] = expr_list;
}

// @brief デストラクタ
AstFuncCall::~AstFuncCall()
{
}

// @brief 型を得る．
AstType
AstFuncCall::type() const
{
  return kAstFuncCall;
}

// @brief 子供の数を返す．
ymuint
AstFuncCall::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstFuncCall::child(ymuint pos) const
{
  ASSERT_COND( pos < child_num() );
  return mChildList[pos];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFuncCall::print(ostream& s,
		   ymuint indent) const
{
  print_indent(s, indent);
  mChildList[0]->print(s);
  s << "(";
  {
    ymuint n = mChildList[1]->child_num();
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      Ast* expr = mChildList[1]->child(i);
      s << comma;
      comma = ", ";
      expr->print(s);
    }
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBinOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] left, right オペランド
AstBinOp::AstBinOp(Ast* left,
		   Ast* right) :
  AstImpl(FileRegion(left->file_region(), right->file_region()))
{
  mChild[0] = left;
  mChild[1] = right;
}

// @brief デストラクタ
AstBinOp::~AstBinOp()
{
}

// @brief 子供の数を返す．
ymuint
AstBinOp::child_num() const
{
  return 2;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstBinOp::child(ymuint pos) const
{
  ASSERT_COND( pos < 2 );
  return mChild[pos % 2];
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBinOp::print(ostream& s,
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
// クラス AstPlus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstPlus::AstPlus(Ast* left,
		 Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstPlus::~AstPlus()
{
}

// @brief 型を得る．
AstType
AstPlus::type() const
{
  return kAstPlus;
}


//////////////////////////////////////////////////////////////////////
// クラス AstMinus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstMinus::AstMinus(Ast* left,
		   Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstMinus::~AstMinus()
{
}

// @brief 型を得る．
AstType
AstMinus::type() const
{
  return kAstMinus;
}


//////////////////////////////////////////////////////////////////////
// クラス AstMult
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstMult::AstMult(Ast* left,
		 Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstMult::~AstMult()
{
}

// @brief 型を得る．
AstType
AstMult::type() const
{
  return kAstMult;
}


//////////////////////////////////////////////////////////////////////
// クラス AstDiv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstDiv::AstDiv(Ast* left,
	       Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstDiv::~AstDiv()
{
}

// @brief 型を得る．
AstType
AstDiv::type() const
{
  return kAstDiv;
}


//////////////////////////////////////////////////////////////////////
// クラス AstMod
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstMod::AstMod(Ast* left,
	       Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstMod::~AstMod()
{
}

// @brief 型を得る．
AstType
AstMod::type() const
{
  return kAstMod;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBitAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstBitAnd::AstBitAnd(Ast* left,
		     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstBitAnd::~AstBitAnd()
{
}

// @brief 型を得る．
AstType
AstBitAnd::type() const
{
  return kAstBitAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBitOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstBitOr::AstBitOr(Ast* left,
		   Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstBitOr::~AstBitOr()
{
}

// @brief 型を得る．
AstType
AstBitOr::type() const
{
  return kAstBitOr;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBitXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstBitXor::AstBitXor(Ast* left,
		     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstBitXor::~AstBitXor()
{
}

// @brief 型を得る．
AstType
AstBitXor::type() const
{
  return kAstBitXor;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLogAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstLogAnd::AstLogAnd(Ast* left,
		     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstLogAnd::~AstLogAnd()
{
}

// @brief 型を得る．
AstType
AstLogAnd::type() const
{
  return kAstLogAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLogOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstLogOr::AstLogOr(Ast* left,
		   Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstLogOr::~AstLogOr()
{
}

// @brief 型を得る．
AstType
AstLogOr::type() const
{
  return kAstLogOr;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLogXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstLogXor::AstLogXor(Ast* left,
		     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstLogXor::~AstLogXor()
{
}

// @brief 型を得る．
AstType
AstLogXor::type() const
{
  return kAstLogXor;
}


//////////////////////////////////////////////////////////////////////
// クラス AstEqEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstEqEq::AstEqEq(Ast* left,
		 Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstEqEq::~AstEqEq()
{
}

// @brief 型を得る．
AstType
AstEqEq::type() const
{
  return kAstEqEq;
}


//////////////////////////////////////////////////////////////////////
// クラス AstNotEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstNotEq::AstNotEq(Ast* left,
		   Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstNotEq::~AstNotEq()
{
}

// @brief 型を得る．
AstType
AstNotEq::type() const
{
  return kAstNotEq;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstLt::AstLt(Ast* left,
	     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstLt::~AstLt()
{
}

// @brief 型を得る．
AstType
AstLt::type() const
{
  return kAstLt;
}


//////////////////////////////////////////////////////////////////////
// クラス AstGt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstGt::AstGt(Ast* left,
	     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstGt::~AstGt()
{
}

// @brief 型を得る．
AstType
AstGt::type() const
{
  return kAstGt;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLe
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstLe::AstLe(Ast* left,
	     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstLe::~AstLe()
{
}

// @brief 型を得る．
AstType
AstLe::type() const
{
  return kAstLe;
}


//////////////////////////////////////////////////////////////////////
// クラス AstGe
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] left, right オペランド
AstGe::AstGe(Ast* left,
	     Ast* right) :
  AstBinOp(left, right)
{
}

// @brief デストラクタ
AstGe::~AstGe()
{
}

// @brief 型を得る．
AstType
AstGe::type() const
{
  return kAstGe;
}


//////////////////////////////////////////////////////////////////////
// クラス AstUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
AstUniOp::AstUniOp(Ast* child,
		   const FileRegion& loc) :
  AstImpl(FileRegion(loc, child->file_region())),
  mChild(child)
{
}

// @brief デストラクタ
AstUniOp::~AstUniOp()
{
}

// @brief 子供の数を返す．
ymuint
AstUniOp::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置( 0 <= pos < child_num() )
Ast*
AstUniOp::child(ymuint pos) const
{
  ASSERT_COND( pos == 0 );
  return mChild;
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstUniOp::print(ostream& s,
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
// クラス AstUniPlus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
AstUniPlus::AstUniPlus(Ast* child,
		       const FileRegion& loc) :
  AstUniOp(child, loc)
{
}

// @brief デストラクタ
AstUniPlus::~AstUniPlus()
{
}

// @brief 型を得る．
AstType
AstUniPlus::type() const
{
  return kAstUniPlus;
}


//////////////////////////////////////////////////////////////////////
// クラス AstUniMinus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
AstUniMinus::AstUniMinus(Ast* child,
			 const FileRegion& loc) :
  AstUniOp(child, loc)
{
}

// @brief デストラクタ
AstUniMinus::~AstUniMinus()
{
}

// @brief 型を得る．
AstType
AstUniMinus::type() const
{
  return kAstUniMinus;
}


//////////////////////////////////////////////////////////////////////
// クラス AstBitNeg
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
AstBitNeg::AstBitNeg(Ast* child,
		     const FileRegion& loc) :
  AstUniOp(child, loc)
{
}

// @brief デストラクタ
AstBitNeg::~AstBitNeg()
{
}

// @brief 型を得る．
AstType
AstBitNeg::type() const
{
  return kAstBitNeg;
}


//////////////////////////////////////////////////////////////////////
// クラス AstLogNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] child 子供のノード
// @param[in] loc ファイル位置
AstLogNot::AstLogNot(Ast* child,
		     const FileRegion& loc) :
  AstUniOp(child, loc)
{
}

// @brief デストラクタ
AstLogNot::~AstLogNot()
{
}

// @brief 型を得る．
AstType
AstLogNot::type() const
{
  return kAstLogNot;
}

END_NAMESPACE_YM_YMSL
