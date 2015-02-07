
/// @file Ast.cc
/// @brief Ast の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Ast
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
Ast::Ast(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
Ast::~Ast()
{
}

// @brief ファイル位置を得る．
const FileRegion&
Ast::file_region() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

// @brief StmtType を出力する．
// @param[in] s 出力先のストリーム
// @param[in] st 文の種類
ostream&
operator<<(ostream& s,
	   StmtType st)
{
  switch ( st ) {
  case kBlockStmt: s << "BlockStmt"; break;
  case kBreak:     s << "Break";     break;
  case kConstDecl: s << "ConstDecl"; break;
  case kContinue:  s << "Continue";  break;
  case kDecr:      s << "Decr";      break;
  case kDoWhile:   s << "DoWhile";   break;
  case kEnumDecl:  s << "EnumDecl";  break;
  case kEqAssign:  s << "EqAssign";  break;
  case kEqPlus:    s << "EqPlus";    break;
  case kEqMinus:   s << "EqMinus";   break;
  case kEqMult:    s << "EqMult";    break;
  case kEqDiv:     s << "EqDiv";     break;
  case kEqMod:     s << "EqMod";     break;
  case kEqLshift:  s << "EqLshift";  break;
  case kEqRshift:  s << "EqRshift";  break;
  case kEqAnd:     s << "EqAnd";     break;
  case kEqOr:      s << "EqOr";      break;
  case kEqXor:     s << "EqXor";     break;
  case kExprStmt:  s << "ExprStmt";  break;
  case kFor:       s << "For";       break;
  case kFuncDecl:  s << "FuncDecl";  break;
  case kGoto:      s << "Goto";      break;
  case kIf:        s << "If";        break;
  case kIncr:      s << "Incr";      break;
  case kImport:    s << "Import";    break;
  case kLabel:     s << "Label";     break;
  case kNullStmt:  s << "NullStmt";  break;
  case kReturn:    s << "Return";    break;
  case kSwitch:    s << "Switch";    break;
  case kToplevel:  s << "Topleevel"; break;
  case kVarDecl:   s << "VarDecl";   break;
  case kWhile:     s << "While";     break;
  default: ASSERT_NOT_REACHED;       break;
  }

  return s;
}

// @brief ExprType を出力する．
// @param[in] s 出力先のストリーム
// @param[in] et 式の種類
ostream&
operator<<(ostream& s,
	   ExprType et)
{
  switch ( et ) {
  }

  return s;
}

END_NAMESPACE_YM_YMSL
