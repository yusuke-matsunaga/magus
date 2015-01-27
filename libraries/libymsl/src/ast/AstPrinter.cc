
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
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " = ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqPlus:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " += ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqMinus:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " -= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqMult:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " *= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqDiv:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " /= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqMod:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " %= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqLshift:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " <<= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqRshift:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " >>= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqAnd:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " &= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqOr:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " |= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kEqXor:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " ^= ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kExprStmt:
    print_indent(indent);
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
    print_indent(indent);
    mS << "import";
    {
      ymuint n = stmt->import_num();
      const char* comma = " ";
      for (ymuint i = 0; i < n; ++ i) {
	const AstSymbol* module = stmt->import_module(i);
	const AstSymbol* alias = stmt->import_alias(i);
	mS << comma  << module->str_val();
	if ( alias != NULL ) {
	  mS << " as " << alias->str_val();
	}
	comma = ", ";
      }
    }
    mS << ";" << endl;
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
  switch ( expr->expr_type() ) {
  case kTrue:
    mS << "true";
    break;

  case kFalse:
    mS << "false";
    break;

  case kIntConst:
    mS << expr->int_val();
    break;

  case kFloatConst:
    mS << expr->float_val();
    break;

  case kStringConst:
    mS << expr->string_val();
    break;

  case kSymbolExpr:
    mS << expr->symbol()->str_val();
    break;

  case kMemberRef:
    print_expr(expr->body());
    mS << "." << expr->member()->str_val();
    break;

  case kArrayRef:
    print_expr(expr->body());
    mS << "[";
    print_expr(expr->index());
    mS << "]";
    break;

  case kUniPlus:
    mS << "(";
    mS << " + ";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kUniMinus:
    mS << "(";
    mS << " - ";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kBitNeg:
    mS << "(";
    mS << " ~ ";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kLogNot:
    mS << "(";
    mS << " not ";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kCastInt:
    mS << "int(";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kCastBoolean:
    mS << "boolean(";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kCastFloat:
    mS << "float(";
    print_expr(expr->operand(0));
    mS << ")";
    break;

  case kBitAnd:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " & ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kBitOr:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " | ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kBitXor:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " ^ ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kLogAnd:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " and ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kLogOr:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " or ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kEqual:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " == ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kNotEq:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " != ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kLt:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " < ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kLe:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " <= ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kGt:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " > ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kGe:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " >= ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kPlus:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " + ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kMinus:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " - ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kMult:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " * ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kDiv:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " / ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kMod:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " % ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kLshift:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " << ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kRshift:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " >> ";
    print_expr(expr->operand(1));
    mS << ")";
    break;

  case kIte:
    mS << "(";
    print_expr(expr->operand(0));
    mS << " ? ";
    print_expr(expr->operand(1));
    mS << " : ";
    print_expr(expr->operand(2));
    mS << ")";
    break;

  case kFuncCall:
    {
      print_expr(expr->func());
      mS << "(";
      ymuint n = expr->arglist_num();
      const char* comma = "";
      for (ymuint i = 0; i < n; ++ i) {
	mS << comma;
	print_expr(expr->arglist_elem(i));
	comma = ", ";
      }
      mS << ")";
    }
    break;

  case kScopeExpr:
  case kEnumExpr:
  case kFuncExpr:
    // AST ではありえない．
    ASSERT_NOT_REACHED;
    break;
  }
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
