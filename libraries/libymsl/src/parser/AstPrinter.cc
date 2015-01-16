
/// @file AstPrinter.cc
/// @brief AstPrinter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstPrinter.h"
#include "AstExpr.h"
#include "AstStatement.h"
#include "AstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstPrinter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
AstPrinter::AstPrinter(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
AstPrinter::~AstPrinter()
{
}

// @brief 文を出力する．
// @param[in] stmt 文
// @param[in] indent インデントレベル
void
AstPrinter::print_statement(const AstStatement* stmt,
			    ymuint indent)
{
  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    print_indent(indent);
    mS << "{" << endl;
    for (ymuint i = 0; i < stmt->stmtlist_num(); ++ i) {
      const AstStatement* stmt1 = stmt->stmtlist_elem(i);
      print_statement(stmt1, indent + 1);
    }
    print_indent(indent);
    mS << "}" << endl;
    break;

  case kBreak:
    print_indent(indent);
    mS << "break;";
    break;

  case kContinue:
    print_indent(indent);
    mS << "continue;";
    break;

  case kDoWhile:
    print_indent(indent);
    mS << "do" << endl;
    print_statement(stmt->stmt(), indent + 1);
    mS << "while ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEnumDecl:
    break;

  case kEqAssign:
  case kEqPlus:
  case kEqMinus:
  case kEqMult:
  case kEqDiv:
  case kEqMod:
  case kEqLshift:
  case kEqRshift:
  case kEqAnd:
  case kEqOr:
  case kEqXor:
  case kExprStmt:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    switch ( stmt->stmt_type() ) {
    case kEqAssign: mS << " = "; break;
    case kEqPlus:   mS << " += "; break;
    case kEqMinus:  mS << " -= "; break;
    case kEqMult:   mS << " *= "; break;
    case kEqDiv:    mS << " /= "; break;
    case kEqMod:    mS << " %= "; break;
    case kEqLshift: mS << " <<= "; break;
    case kEqRshift: mS << " >>= "; break;
    case kEqAnd:    mS << " &= "; break;
    case kEqOr:     mS << " |= "; break;
    case kEqXor:    mS << " ^= "; break;
    default: ASSERT_NOT_REACHED; break;
    }
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kFor:
    break;

  case kFuncDecl:
    break;

  case kGoto:
    print_indent(indent);
    mS << "goto " << stmt->label()->str_val() << ";" << endl;
    break;

  case kIf:
    break;

  case kImport:
    break;

  case kLabel:
    print_indent(indent);
    mS << stmt->label()->str_val() << ":" << endl;
    break;

  case kReturn:
    print_indent(indent);
    mS << "return";
    if ( stmt->expr() != NULL ) {
      mS << " ";
      print_expr(stmt->expr());
    }
    mS << ";" << endl;
    break;

  case kSwitch:
    break;

  case kToplevel:
    for (ymuint i = 0; i < stmt->stmtlist_num(); ++ i) {
      const AstStatement* stmt1 = stmt->stmtlist_elem(i);
      print_statement(stmt1, indent + 1);
    }
    break;

  case kVarDecl:
    break;

  case kWhile:
    print_indent(indent);
    mS << "while ";
    print_expr(stmt->expr());
    mS << endl;
    print_statement(stmt->stmt(), indent + 1);
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
}

// @brief 式を出力する．
// @param[in] expr 式
void
AstPrinter::print_expr(const AstExpr* expr)
{
  expr->print(mS);
}

// @brief インデントする．
// @param[in] indent インデントレベル
void
AstPrinter::print_indent(ymuint indent)
{
  for (ymuint i = 0; i < indent; ++ i) {
    mS << "  ";
  }
}

END_NAMESPACE_YM_YMSL
