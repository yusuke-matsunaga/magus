
/// @file AstExpr.cc
/// @brief AstExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstExpr::AstExpr(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstExpr::~AstExpr()
{
}

// @brief 整数値を返す．
//
// kIntConst のみ有効
int
AstExpr::int_val() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 実数値を返す．
//
// kFloatConst のみ有効
double
AstExpr::float_val() const
{
  ASSERT_NOT_REACHED;
  return 0.0;
}

// @brief 文字列値を返す．
//
// kStringConst のみ有効
const char*
AstExpr::string_val() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief シンボルを返す．
//
// kSymbolExpr のみ有効
const AstSymbol*
AstExpr::symbol() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 本体を返す．
//
// kArrayRef, kMemberRef のみ有効
const AstExpr*
AstExpr::body() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief メンバ名を返す．
const AstSymbol*
AstExpr::member() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 配列のインデックスを返す．
//
// kArrayRef のみ有効
const AstExpr*
AstExpr::index() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief オペランド数を返す．
//
// 演算子のみ有効
ymuint
AstExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置
//
// pos の範囲は演算子によって異なる．
const AstExpr*
AstExpr::operand(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 関数本体を返す．
const AstExpr*
AstExpr::func() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 引数の数を返す．
//
// kFuncCall のみ有効
ymuint
AstExpr::arglist_num() const
{
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 引数を返す．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
const AstExpr*
AstExpr::arglist_elem(ymuint pos) const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

#if 0
//////////////////////////////////////////////////////////////////////
// クラス AstVarExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_name 変数名
// @param[in] loc ファイル位置
AstVarExpr::AstVarExpr(AstIdentifier* var_name,
		       const FileRegion& loc) :
  AstExpr(loc),
  mVarName(var_name),
  mVarDecl(NULL)
{
}

// @brief デストラクタ
AstVarExpr::~AstVarExpr()
{
}

// @brief 変数の参照を解決する．
void
AstVarExpr::resolve_var(YmslScope* parent_scope)
{
#if 0
  mVarDecl = parent_scope->find_var(mVarName);
  if ( mVarDecl == NULL ) {
    ostringstream buf;
    mVarName->print(buf);
    buf << ": Undefined";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    mVarName->file_region(),
		    kMsgError,
		    "PARS",
		    buf.str());
  }
#endif
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstVarExpr::type_analysis()
{
  return true;
}

// @brief 式の型を返す．
const YmslType*
AstVarExpr::type()
{
  return mVarDecl->type();
}

// @brief 命令コードのサイズを計算する．
ymuint
AstVarExpr::calc_size()
{
  return 2;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstVarExpr::compile(YmslDriver& driver,
		    YmslCodeList& code_list,
		    Ymsl_INT& addr)
{
#if 0
  Ymsl_CODE op;
  switch ( mVarDecl->type() ) {
  case kVoidType:
    break;

  case kBooleanType:
  case kIntType:
    if ( mVarDecl->global() ) {
      op = YMVM_LOAD_GLOBAL_INT;
    }
    else {
      op = YMVM_LOAD_LOCAL_INT;
    }
    break;

  case kFloatType:
    if ( mVarDecl->global() ) {
      op = YMVM_LOAD_GLOBAL_FLOAT;
    }
    else {
      op = YMVM_LOAD_LOCAL_FLOAT;
    }
    break;

  case kStringType:

    break;

  case kUserType:
    if ( mVarDecl->global() ) {
      op = YMVM_LOAD_GLOBAL_OBJ;
    }
    else {
      op = YMVM_LOAD_LOCAL_OBJ;
    }
    break;
  }
  code_list.write_opcode(addr, op);
  code_list.write_int(addr, mVarDecl->index());
#endif
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstVarExpr::print(ostream& s) const
{
  mVarName->print(s);
}
#endif

END_NAMESPACE_YM_YMSL
