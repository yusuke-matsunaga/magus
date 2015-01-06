
/// @file AstPrimary.cc
/// @brief AstPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstPrimary.h"
#include "AstList.h"
#include "AstVarDecl.h"

#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_name 変数名
// @param[in] loc ファイル位置
AstPrimary::AstPrimary(AstSymbolList* var_name,
		       const FileRegion& loc) :
  Ast(loc),
  mVarName(var_name->size()),
  mVar(NULL)
{
  ymuint pos = 0;
  for (AstSymbolList::Iterator p = var_name->begin();
       !p.is_end(); p.next()) {
    mVarName[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstPrimary::~AstPrimary()
{
}

// @brief 変数を返す．
AstVarDecl*
AstPrimary::var() const
{
  return mVar;
}

// @brief 変数に対する代入命令を返す．
Ymsl_CODE
AstPrimary::opcode() const
{
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
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstPrimary::print(ostream& s) const
{
  s << mVar->name();
}

END_NAMESPACE_YM_YMSL
