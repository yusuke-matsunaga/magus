
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
#include "AstType.h"


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
    mS << "break;" << endl;
    break;

  case kConstDecl:
    print_indent(indent);
    mS << "const " << stmt->name()->str_val() << ": ";
    print_type(stmt->type());
    {
      const AstExpr* expr = stmt->expr();
      mS << " = ";
      print_expr(expr);
    }
    mS << ";" << endl;
    break;

  case kContinue:
    print_indent(indent);
    mS << "continue;" << endl;
    break;

  case kDecr:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " --;" << endl;
    break;

  case kDoWhile:
    print_indent(indent);
    mS << "do" << endl;
    print_statement(stmt->stmt(), indent + 1);
    print_indent(indent);
    mS << "while ";
    print_expr(stmt->expr());
    mS << endl;
    break;

  case kEnumDecl:
    print_indent(indent);
    mS << "enum " << stmt->name()->str_val() << " {" << endl;
    {
      ymuint n = stmt->enum_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstSymbol* name = stmt->enum_const(i);
	const AstExpr* expr = stmt->enum_const_expr(i);
	print_indent(indent + 1);
	mS << name->str_val();
	if ( expr != NULL ) {
	  mS << " = ";
	  print_expr(expr);
	}
	if ( i < n - 1 ) {
	  mS << ",";
	}
	mS << endl;
      }
    }
    print_indent(indent);
    mS << "}" << endl;
    break;

  case kAssignment:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " = ";
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kInplaceOp:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    switch ( stmt->opcode() ) {
    case kOpAdd:    mS << " += "; break;
    case kOpSub:    mS << " -= "; break;
    case kOpMul:    mS << " *= "; break;
    case kOpDiv:    mS << " /= "; break;
    case kOpMod:    mS << " %= "; break;
    case kOpLshift: mS << " <<= "; break;
    case kOpRshift: mS << " >>= "; break;
    case kOpBitAnd: mS << " &= "; break;
    case kOpBitOr:  mS << " |= "; break;
    case kOpBitXor: mS << " ^= "; break;
    default: ASSERT_NOT_REACHED; break;
    }
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kExprStmt:
    print_indent(indent);
    print_expr(stmt->expr());
    mS << ";" << endl;
    break;

  case kFor:
    print_indent(indent);
    mS << "for (" << endl;
    print_statement(stmt->init_stmt(), indent + 1);
    print_indent(indent + 1);
    print_expr(stmt->expr());
    mS << ";" << endl;
    print_statement(stmt->next_stmt(), indent + 1);
    print_statement(stmt->stmt(), indent + 1);
    break;

  case kFuncDecl:
    {
      print_indent(indent);
      mS << "function " << stmt->name()->str_val() << "(";
      ymuint np = stmt->param_num();
      const char* comma = "";
      for (ymuint i = 0; i < np; ++ i) {
	mS << comma << stmt->param_name(i)->str_val() << ": ";
	print_type(stmt->param_type(i));
	const AstExpr* expr = stmt->param_expr(i);
	if ( expr != NULL ) {
	  mS << " = ";
	  print_expr(expr);
	}
	comma = ", ";
      }
      mS << ")" << endl;
      print_statement(stmt->stmt(), indent + 1);
    }
    break;

  case kGoto:
    print_indent(indent);
    mS << "goto " << stmt->label()->str_val() << ";" << endl;
    break;

  case kIf:
    print_indent(indent);
    mS << "if ";
    print_expr(stmt->expr());
    mS << endl;
    print_statement(stmt->stmt(), indent + 1);
    if ( stmt->else_stmt() != NULL ) {
      print_indent(indent);
      mS << "else" << endl;
      print_statement(stmt->else_stmt(), indent + 1);
    }
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

  case kIncr:
    print_indent(indent);
    print_expr(stmt->lhs_expr());
    mS << " ++;" << endl;
    break;

  case kLabel:
    print_indent(indent);
    mS << stmt->label()->str_val() << ":" << endl;
    break;

  case kNullStmt:
    print_indent(indent);
    mS << ";" << endl;
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
    print_indent(indent);
    mS << "switch ";
    print_expr(stmt->expr());
    mS << "{" << endl;
    {
      ymuint n = stmt->switch_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstExpr* label = stmt->case_label(i);
	const AstStatement* body = stmt->case_stmt(i);
	print_indent(indent);
	if ( label != NULL ) {
	  mS << "case ";
	  print_expr(label);
	}
	else {
	  mS << "default";
	}
	mS << ": " << endl;
	print_statement(body, indent + 1);
	mS << endl;
      }
    }
    print_indent(indent);
    mS << "}" << endl;
    break;

  case kToplevel:
    for (ymuint i = 0; i < stmt->stmtlist_num(); ++ i) {
      const AstStatement* stmt1 = stmt->stmtlist_elem(i);
      print_statement(stmt1, indent + 1);
    }
    break;

  case kVarDecl:
    print_indent(indent);
    mS << "var " << stmt->name()->str_val() << ": ";
    print_type(stmt->type());
    {
      const AstExpr* expr = stmt->expr();
      if ( expr != NULL ) {
	mS << " = ";
	print_expr(expr);
      }
    }
    mS << ";" << endl;
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
  case AstExpr::kTrue:
    mS << "true";
    break;

  case AstExpr::kFalse:
    mS << "false";
    break;

  case AstExpr::kIntConst:
    mS << expr->int_val();
    break;

  case AstExpr::kFloatConst:
    mS << expr->float_val();
    break;

  case AstExpr::kStringConst:
    mS << '"' << expr->string_val() << '"';
    break;

  case AstExpr::kSymbolExpr:
    mS << expr->symbol()->str_val();
    break;

  case AstExpr::kMemberRef:
    print_expr(expr->body());
    mS << "." << expr->member()->str_val();
    break;

  case AstExpr::kArrayRef:
    print_expr(expr->body());
    mS << "[";
    print_expr(expr->index());
    mS << "]";
    break;

  case AstExpr::kUniOp:
    switch ( expr->opcode() ) {
    case kOpUniMinus:
      mS << "(";
      mS << " - ";
      print_expr(expr->operand(0));
      mS << ")";
      break;

    case kOpBitNeg:
      mS << "(";
      mS << " ~ ";
      print_expr(expr->operand(0));
      mS << ")";
      break;

    case kOpLogNot:
      mS << "(";
      mS << " not ";
      print_expr(expr->operand(0));
      mS << ")";
      break;

    case kOpCastInt:
      mS << "int(";
      print_expr(expr->operand(0));
      mS << ")";
      break;

    case kOpCastBoolean:
      mS << "boolean(";
      print_expr(expr->operand(0));
      mS << ")";
      break;

    case kOpCastFloat:
      mS << "float(";
      print_expr(expr->operand(0));
      mS << ")";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case AstExpr::kBinOp:
    switch ( expr->opcode() ) {
    case kOpBitAnd:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " & ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpBitOr:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " | ";
      print_expr(expr->operand(1));
      mS << ")";
    break;

    case kOpBitXor:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " ^ ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpLogAnd:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " and ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpLogOr:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " or ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpEqual:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " == ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpNotEq:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " != ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpLt:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " < ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpLe:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " <= ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpAdd:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " + ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpSub:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " - ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpMul:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " * ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpDiv:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " / ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpMod:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " % ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpLshift:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " << ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    case kOpRshift:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " >> ";
      print_expr(expr->operand(1));
      mS << ")";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case AstExpr::kTriOp:
    switch ( expr->opcode() ) {
    case kOpIte:
      mS << "(";
      print_expr(expr->operand(0));
      mS << " ? ";
      print_expr(expr->operand(1));
      mS << " : ";
      print_expr(expr->operand(2));
      mS << ")";
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case AstExpr::kFuncCall:
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
  }
}

// @brief 型を出力する．
// @param[in] type 型
void
AstPrinter::print_type(const AstType* type)
{
  switch ( type->type_id() ) {
  case kVoidType:    mS << "void"; break;
  case kBooleanType: mS << "boolean"; break;
  case kIntType:     mS << "int"; break;
  case kFloatType:   mS << "float"; break;
  case kStringType:  mS << "string"; break;

  case kArrayType:
    mS << "array(";
    print_type(type->elem_type());
    mS << ")";
    break;

  case kSetType:
    mS << "set(";
    print_type(type->elem_type());
    mS << ")";
    break;

  case kMapType:
    mS << "map(";
    print_type(type->key_type());
    mS << ", ";
    print_type(type->elem_type());
    mS << ")";
    break;

  case kNamedType:
    {
      ymuint n = type->scope_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstSymbol* scope_symbol = type->scope(i);
	mS << scope_symbol->str_val() << ".";
      }
      const AstSymbol* name_symbol = type->name();
      mS << name_symbol->str_val();
    }
    break;

  default:
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
