#ifndef YMSL_INT_H
#define YMSL_INT_H

/// @file ymsl_int.h
/// @brief ymsl の内部定義ファイル
/// n@author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief ymsl の名前空間の開始
#define BEGIN_NAMESPACE_YM_YMSL \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsYmsl)

/// @brief ymsl の名前空間の終了
#define END_NAMESPACE_YM_YMSL \
END_NAMESPACE(nsYmsl) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_YMSL

class YmslCodeList;
class YmslFunction;
class YmslObj;
class YmslVSM;

class IrCodeBlock;
class IrHandle;
class IrFunction;
class IrNode;
class IrToplevel;

class Function;
class Scope;
class Type;
class TypeMgr;
class Var;

class AstCaseItem;
class AstEnumConst;
class AstExpr;
class AstMgr;
class AstModule;
class AstParam;
class AstStatement;
class AstSymbol;
class AstToplevel;
class AstType;

template <typename T>
class AstList;

typedef AstList<AstModule> AstModuleList;
typedef AstList<AstStatement> AstStmtList;
typedef AstList<AstCaseItem> AstCaseList;
typedef AstList<AstParam> AstParamList;
typedef AstList<AstExpr> AstExprList;
typedef AstList<AstEnumConst> AstEnumConstList;
typedef AstList<AstSymbol> AstSymbolList;


/// @brief 論理型
typedef bool Ymsl_BOOLEAN;

/// @brief 整数型
typedef ymint32 Ymsl_INT;

/// @brief 浮動小数点型
typedef double Ymsl_FLOAT;

/// @brief 文字列型
typedef const char* Ymsl_STRING;

/// @brief オブジェクト型
///
/// 汎用のポインタ型
typedef YmslObj* Ymsl_OBJPTR;

/// @brief プログラムのコード
typedef ymuint32 Ymsl_CODE;


//////////////////////////////////////////////////////////////////////
/// @brief 型の種類
//////////////////////////////////////////////////////////////////////
enum TypeId {
  kVoidType,
  kBooleanType,
  kIntType,
  kFloatType,
  kStringType,
  kArrayType,
  kSetType,
  kMapType,
  kFuncType,
  kNamedType,
  kEnumType,
  kClassType,
  kUserDefType
};

/// @brief TypeId を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] vt 型番号
ostream&
operator<<(ostream& s,
	   TypeId vt);

END_NAMESPACE_YM_YMSL

#endif // YMSL_INT_H
