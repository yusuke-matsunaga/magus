
/// @file YmslAstBinOp.cc
/// @brief YmslAstBinOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

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
void
YmslAstBinOp::print(ostream& s) const
{
  s << "(";
  child(0)->print(s);
  switch ( type() ) {
  case kAstPlus: s << "+"; break;
  case kAstMinus: s << "-"; break;
  case kAstMult: s << "*"; break;
  case kAstDiv: s << "/"; break;
  case kAstMod: s << "%"; break;
  case kAstBitAnd: s << "&"; break;
  case kAstBitOr: s << "|"; break;
  case kAstBitXor: s << "^"; break;
  case kAstLogAnd: s << " and "; break;
  case kAstLogOr: s << " or "; break;
  case kAstLogXor: s << " xor "; break;
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

END_NAMESPACE_YM_YMSL
