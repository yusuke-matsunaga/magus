
/// @file Ast.cc
/// @brief Ast の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "AstList.h"
#include "AstModule.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstToplevel.h"


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
// クラス AstSymbol
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstSymbol::AstSymbol(ShString val,
		     const FileRegion& loc) :
  Ast(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstSymbol::~AstSymbol()
{
}

// @brief 文字列型の値を返す．
ShString
AstSymbol::str_val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス AstModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module モジュール名
// @param[in] alias エイリアス名
// @param[in] loc ファイル位置
AstModule::AstModule(AstSymbol* module,
		     AstSymbol* alias,
		     const FileRegion& loc) :
  Ast(loc),
  mModuleName(module),
  mAliasName(alias)
{
}

// @brief デストラクタ
AstModule::~AstModule()
{
}

// @brief モジュール名を返す．
AstSymbol*
AstModule::module_name() const
{
  return mModuleName;
}

// @brief エイリアス名を返す．
//
// 場合によっては NULL を返す．
AstSymbol*
AstModule::alias_name() const
{
  return mAliasName;
}


//////////////////////////////////////////////////////////////////////
// クラス AstToplevel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_list ステートメントリスト
// @param[in] loc ファイル位置
AstToplevel::AstToplevel(AstStmtList* stmt_list,
			 const FileRegion& loc) :
  Ast(loc),
  mStmtList(stmt_list->size())
{
  ymuint pos = 0;
  for (AstStmtList::Iterator p = stmt_list->begin();
       !p.is_end(); p.next()) {
    mStmtList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstToplevel::~AstToplevel()
{
}

// @brief 出力する
// @param[in] s 出力先のストリーム
void
AstToplevel::print(ostream& s) const
{
  for (vector<AstStatement*>::const_iterator p = mStmtList.begin();
       p != mStmtList.end(); ++ p) {
    AstStatement* stmt = *p;
    stmt->print(s, 0);
  }
}


//////////////////////////////////////////////////////////////////////
// misc
//////////////////////////////////////////////////////////////////////

// @brief ValueType を出力する．
ostream&
operator<<(ostream& s,
	   ValueType vt)
{
  switch ( vt ) {
  case kVoidType:    s << "void"; break;
  case kBooleanType: s << "boolean"; break;
  case kIntType:     s << "int"; break;
  case kFloatType:   s << "float"; break;
  case kStringType:  s << "string"; break;
  case kUserType:    s << "__user__"; break;
  }
  return s;
}

END_NAMESPACE_YM_YMSL
