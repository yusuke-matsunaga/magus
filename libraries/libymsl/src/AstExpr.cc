
/// @file AstImpl.cc
/// @brief AstImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncCall.h"
#include "AstUniOp.h"
#include "AstBinOp.h"
#include "AstVarExpr.h"
#include "AstIntConst.h"
#include "AstFloatConst.h"
#include "AstStringConst.h"

#include "AstVarDecl.h"


BEGIN_NAMESPACE_YM_YMSL

#include "grammer.hh"


//////////////////////////////////////////////////////////////////////
// クラス AstExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstExpr::AstExpr(const FileRegion& loc) :
  Ast(loc)
{
  mNext = NULL;
}

// @brief デストラクタ
AstExpr::~AstExpr()
{
}

// @brief 次の要素を得る．
AstExpr*
AstExpr::next() const
{
  return mNext;
}

// @brief 前の要素をセットする．
void
AstExpr::set_prev(AstExpr* prev)
{
  if ( prev != NULL ) {
    prev->mNext = this;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AstFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] expr_list 引数リスト
// @param[in] loc ファイル位置
AstFuncCall::AstFuncCall(AstSymbol* name,
			 AstExpr* expr_list,
			 const FileRegion& loc) :
  AstExpr(loc),
  mName(name)
{
}

// @brief デストラクタ
AstFuncCall::~AstFuncCall()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstFuncCall::print(ostream& s) const
{
#if 0
  mName->print(s);
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
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス AstBinOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] token トークン
// @param[in] left, right オペランド
AstBinOp::AstBinOp(TokenType token,
		   AstExpr* left,
		   AstExpr* right) :
  AstExpr(FileRegion(left->file_region(), right->file_region())),
  mToken(token),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstBinOp::~AstBinOp()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstBinOp::print(ostream& s) const
{
  s << "(";
  mLeft->print(s);
  switch ( mToken) {
  case PLUS:   s << "+"; break;
  case MINUS:  s << "-"; break;
  case MULT:   s << "*"; break;
  case DIV:    s << "/"; break;
  case MOD:    s << "%"; break;
  case BITAND: s << "&"; break;
  case BITOR:  s << "|"; break;
  case BITXOR: s << "^"; break;
  case LOGAND: s << " and "; break;
  case LOGOR:  s << " or "; break;
  case LOGXOR: s << " xor "; break;
  case EQEQ:   s << "=="; break;
  case NOTEQ:  s << "!="; break;
  case LT:     s << "<"; break;
  case GT:     s << ">"; break;
  case LE:     s << "<="; break;
  case GE:     s << ">="; break;
  default: ASSERT_NOT_REACHED;
  }
  mRight->print(s);
  s << ")";
}


//////////////////////////////////////////////////////////////////////
// クラス AstUniOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] token トークン
// @param[in] opr オペランド
// @param[in] loc ファイル位置
AstUniOp::AstUniOp(TokenType token,
		   AstExpr* opr,
		   const FileRegion& loc) :
  AstExpr(FileRegion(loc, opr->file_region())),
  mToken(token),
  mOperand(opr)
{
}

// @brief デストラクタ
AstUniOp::~AstUniOp()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstUniOp::print(ostream& s) const
{
  switch ( mToken ) {
  case PLUS:   s << "+"; break;
  case MINUS:  s << "-"; break;
  case BITNEG: s << "~"; break;
  case LOGNOT: s << "!"; break;
  default: ASSERT_NOT_REACHED;
  }
  mOperand->print(s);
}


//////////////////////////////////////////////////////////////////////
// クラス AstVarExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
// @param[in] loc ファイル位置
AstVarExpr::AstVarExpr(AstVarDecl* var,
		       const FileRegion& loc) :
  AstExpr(loc),
  mVar(var)
{
}

// @brief デストラクタ
AstVarExpr::~AstVarExpr()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstVarExpr::print(ostream& s) const
{
  s << mVar->name();
}


//////////////////////////////////////////////////////////////////////
// クラス AstIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstIntConst::AstIntConst(int val,
			 const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstIntConst::~AstIntConst()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstIntConst::print(ostream& s) const
{
  s << mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス AstFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstFloatConst::AstFloatConst(double val,
			     const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstFloatConst::~AstFloatConst()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstFloatConst::print(ostream& s) const
{
  s << mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス AstStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstStringConst::AstStringConst(const char* val,
			       const FileRegion& loc) :
  AstExpr(loc)
{
}

// @brief デストラクタ
AstStringConst::~AstStringConst()
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstStringConst::print(ostream& s) const
{
  s << mVal;
}

END_NAMESPACE_YM_YMSL
