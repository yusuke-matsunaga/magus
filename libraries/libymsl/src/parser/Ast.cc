
/// @file Ast.cc
/// @brief Ast の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "AstIdentifier.h"
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
// クラス AstIdentifier
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] symbol_list シンボルリスト
// @param[in] loc ファイル位置
AstIdentifier::AstIdentifier(AstSymbolList* symbol_list,
			     const FileRegion& loc) :
  Ast(loc),
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
AstIdentifier::~AstIdentifier()
{
}

// @brief シンボルリストを返す．
const vector<AstSymbol*>&
AstIdentifier::symbol_list() const
{
  return mSymbolList;
}

// @brief 内容を出力する．
void
AstIdentifier::print(ostream& s) const
{
  ymuint n = mSymbolList.size();
  const char* dot = "";
  for (ymuint i = 0; i < n; ++ i) {
    AstSymbol* sym = mSymbolList[i];
    s << dot << sym->str_val();
    dot = ".";
  }
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
