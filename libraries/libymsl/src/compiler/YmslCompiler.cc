
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"
#include "AstEnumConst.h"
#include "AstExpr.h"
#include "AstMgr.h"
#include "AstParam.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"
#include "YmslScope.h"



BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslCompiler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslCompiler::YmslCompiler()
{
}

// @brief デストラクタ
YmslCompiler::~YmslCompiler()
{
}

// @brief コンパイルする．
// @param[in] ido 入力データ
// @return 成功したら true を返す．
bool
YmslCompiler::compile(IDO& ido)
{
  AstMgr mgr;

  bool stat = mgr.read_source(ido);
  if ( !stat ) {
    return false;
  }

  AstStatement* toplevel = mgr.toplevel();

  phase1(toplevel);

  phase2(toplevel);

  // YmslScope* toplevel_scope = new YmslScope(NULL, ShString("__main__"));

  return true;
}

// @brief 要素の生成を行う．
// @param[in] stmt 文
void
YmslCompiler::phase1(const AstStatement* stmt)
{
  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    {
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	phase1(stmt1);
      }
    }
    break;

  case kBreak:
    break;

  case kContinue:
    break;

  case kDoWhile:
    phase1(stmt->stmt());
    break;

  case kEnumDecl:
    reg_enum(stmt);
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
    break;

  case kFuncDecl:
    reg_func(stmt);
    break;

  case kGoto:
    break;

  case kIf:
    phase1(stmt->stmt());
    phase1(stmt->else_stmt());
    break;

  case kImport:
    break;

  case kLabel:
    break;

  case kReturn:
    break;

  case kSwitch:
    break;

  case kToplevel:
    // トップレベルのスコープを追加
    phase1(stmt->stmt());
    break;

  case kVarDecl:
    reg_var(stmt);
    break;

  case kFor:
    // for 文のスコープを追加
    phase1(stmt->init_stmt());
    phase1(stmt->next_stmt());
    phase1(stmt->stmt());
    break;

  case kWhile:
    phase1(stmt->stmt());
    break;
  }
}

// @brief enum 型の定義を行う．
// @param[in] stmt 文
//
// stmt は kEnumDecl でなければならない．
void
YmslCompiler::reg_enum(const AstStatement* stmt)
{
  ASSERT_COND( stmt->stmt_type() == kEnumDecl );

  ShString name = stmt->name();
  ymint n = stmt->enum_num();
  vector<const AstEnumConst*> ec_list(n);
  vector<ShString> elem_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    const AstEnumConst* ec = stmt->enum_const(i);
    ec_list[i] = ec;
    elem_list[i] = ec->name()->str_val();
  }

  // 定数に式がついている場合の処理
  HashMap<ymuint, int> val_dict;
  YmslType* type = mTypeMgr.enum_type(name, elem_list, val_dict);


  // type をカレントスコープに登録する．
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
//
// stmt は kFuncDecl でなければならない．
void
YmslCompiler::reg_func(const AstStatement* stmt)
{
  ASSERT_COND( stmt->stmt_type() == kFuncDecl );

  ShString name = stmt->name();
  const AstType* asttype = stmt->type();
  const YmslType* type = resolve_type(asttype);

  ymuint np = stmt->param_num();
  vector<const YmslType*> input_type_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    const AstParam* param = stmt->param(i);
    const AstType* asttype = param->type();
    const YmslType* type = resolve_type(asttype);
    input_type_list[i] = type;
  }
  // name: type, input_type_list という関数を作り，カレントスコープに登録する．
}

// @brief 変数の定義を行う．
// @param[in] stmt 文
//
// stmt は kVarDecl でなければならない．
void
YmslCompiler::reg_var(const AstStatement* stmt)
{
  ASSERT_COND( stmt->stmt_type() == kVarDecl );

  ShString name = stmt->name();
  const AstType* asttype = stmt->type();
  const YmslType* type = resolve_type(asttype);

  // name: type という変数を作り，カレントスコープに登録する．
}

// @brief 型の参照を解決する．
// @param[in] asttype 型を表す構文木
//
// 解決できない時には NULL を返す．
const YmslType*
YmslCompiler::resolve_type(const AstType* asttype)
{
  if ( asttype->named_type() ) {
    // スコープから名前の解決を行う
    return NULL;
  }

  switch ( asttype->type_id() ) {
  case kVoidType:
    return mTypeMgr.void_type();

  case kBooleanType:
    return mTypeMgr.boolean_type();

  case kIntType:
    return mTypeMgr.int_type();

  case kFloatType:
    return mTypeMgr.float_type();

  case kStringType:
    return mTypeMgr.string_type();

  case kArrayType:
    {
      const YmslType* elem_type = resolve_type(asttype->elem_type());
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.array_type(elem_type);
    }
    break;

  case kSetType:
    {
      const YmslType* elem_type = resolve_type(asttype->elem_type());
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.set_type(elem_type);
    }
    break;

  case kMapType:
    {
      const YmslType* key_type = resolve_type(asttype->key_type());
      if ( key_type == NULL ) {
	break;
      }
      const YmslType* elem_type = resolve_type(asttype->elem_type());
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.map_type(key_type, elem_type);
    }
    break;

  case kFuncType:
  case kClassType:
  case kEnumType:
  case kUserDefType:
    // AstType でこれらの型はありえない．
    ASSERT_NOT_REACHED;
    break;

  }
  return NULL;
}

// @brief 参照の解決を行う．
// @param[in] stmt 文
void
YmslCompiler::phase2(const AstStatement* stmt)
{
}

// @brief 参照の解決を行う．
// @param[in] expr 式
void
YmslCompiler::phase2(const AstExpr* expr)
{
}

END_NAMESPACE_YM_YMSL
