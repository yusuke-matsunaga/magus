
/// @file AstPrimary.cc
/// @brief AstPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstPrimary.h"
#include "AstVarDecl.h"

#include "YmslScope.h"
#include "YmslVM.h"

#include "ObjHandle.h"

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] symbol_list 変数名
// @param[in] loc ファイル位置
AstPrimary::AstPrimary(AstSymbolList* symbol_list,
		       const FileRegion& loc) :
  AstExpr(loc),
  mSymbolList(symbol_list->size())
{
  ymuint pos = 0;
  for (AstSymbolList::Iterator p = symbol_list->begin();
       !p.is_end(); p.next()) {
    mSymbolList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstPrimary::~AstPrimary()
{
}

// @brief 種類を返す．
ExprType
AstPrimary::expr_type() const
{
  return kPrimary;
}

// @brief シンボルリストの要素数を返す．
//
// kPrimary, kArrayRef, kFuncCall のみ有効
ymuint
AstPrimary::symbollist_num() const
{
  return mSymbolList.size();
}

// @brief シンボルリストの要素を返す．
// @param[in] pos 位置 ( 0 <= pos < symbollist_num() )
//
// kPrimary, kArrayRef, kFuncCall のみ有効
const AstSymbol*
AstPrimary::symbollist_elem(ymuint pos) const
{
  ASSERT_COND( pos < symbollist_num() );
  return mSymbolList[pos];
}

#if 0
// @brief 変数の参照を解決する．
void
AstPrimary::resolve_var(YmslScope* parent_scope)
{
  ObjHandle* h = parent_scope->find(mVarName);
  if ( h == NULL ) {
    ostringstream buf;
    buf << mVarName << ": Undefined";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region(),
		    kMsgError,
		    "PARS",
		    buf.str());
    return;
  }

  mVar = h->var();
  if ( mVar != NULL ) {
    return;
  }

  // h が他の型の場合．
  if ( h->function() != NULL ) {
    ostringstream buf;
    buf << mVarName << ": variable expected";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region(),
		    kMsgError,
		    "PARS",
		    buf.str());
    return;
  }

  YmslScope* scope = h->scope();

  #warning "嘘"
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstPrimary::type_analysis()
{
}

// @brief 式の型を返す．
const YmslType*
AstPrimary::type()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstPrimary::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstPrimary::compile(YmslDriver& driver,
		    YmslCodeList& code_list,
		    Ymsl_INT& addr)
{
}

// @brief 変数を返す．
YmslVar*
AstPrimary::var() const
{
  return mVar;
}

// @brief 変数に対する代入命令を返す．
Ymsl_CODE
AstPrimary::opcode() const
{
#if 0
  Ymsl_CODE op;
  switch ( mVar->type() ) {
  case kVoidType:
    break;

  case kBooleanType:
  case kIntType:
    if ( mVar->global() ) {
      op = YMVM_STORE_GLOBAL_INT;
    }
    else {
      op = YMVM_STORE_LOCAL_INT;
    }
    break;

  case kFloatType:
    if ( mVar->global() ) {
      op = YMVM_STORE_GLOBAL_FLOAT;
    }
    else {
      op = YMVM_STORE_LOCAL_FLOAT;
    }
    break;

  case kStringType:

    break;

  case kUserType:
    if ( mVar->global() ) {
      op = YMVM_STORE_GLOBAL_OBJ;
    }
    else {
      op = YMVM_STORE_LOCAL_OBJ;
    }
    break;
  }
  return op;
#endif
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstPrimary::print(ostream& s) const
{
  const char* dot = "";
  for (ymuint i = 0; i < mSymbolList.size(); ++ i) {
    s << dot << mSymbolList[i]->str_val();
    dot = ".";
  }
}
#endif

END_NAMESPACE_YM_YMSL
