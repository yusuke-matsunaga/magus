#ifndef YMSLAST_H
#define YMSLAST_H

/// @file YmslAst.h
/// @brief YmslAst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_YMSL

/// @brief 抽象構文木の型
enum AstType
{
  kAstSymbol,
  kAstString,
  kAstInt,
  kAstFloat,

  kAstStringType,
  kAstIntType,
  kAstFloatType,
  kAstUserType,

  kAstList,

  kAstVarDecl,
  kAstFuncDecl,

  kAstAssignment,

  kAstIf,
  kAstElif,
  kAstElse,
  kAstFor,
  kAstWhile,
  kAstDoWhile,
  kAstSwitch,
  kAstCaseItem,
  kAstGoto,
  kAstLabel,
  kAstBreak,
  kAstContinue,
  kAstReturn,
  kAstBlock,

  kAstUniPlus,
  kAstUniMinus,
  kAstPlus,
  kAstMinus,
  kAstMult,
  kAstDiv,
  kAstMod,

  kAstBitNeg,
  kAstBitAnd,
  kAstBitOr,
  kAstBitXor,

  kAstLogNot,
  kAstLogAnd,
  kAstLogOr,
  kAstLogXor,

  kAstEqEq,
  kAstNotEq,
  kAstLt,
  kAstGt,
  kAstLe,
  kAstGe,

  kAstFuncCall,
};


//////////////////////////////////////////////////////////////////////
/// @class YmslAst YmslAst.h "YmslAst.h"
/// @brief YMSL の抽象構文木クラス
//////////////////////////////////////////////////////////////////////
class YmslAst
{
public:

  /// @brief デストラクタ
  virtual
  ~YmslAst() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を得る．
  virtual
  const FileRegion&
  file_region() const = 0;

  /// @brief 型を得る．
  virtual
  AstType
  type() const = 0;

  /// @brief 文字列型の値を返す．
  virtual
  const char*
  str_val() const = 0;

  /// @brief 整数型の値を返す．
  virtual
  int
  int_val() const = 0;

  /// @brief 浮動小数点型の値を返す．
  virtual
  double
  float_val() const = 0;

  /// @brief 子供の数を返す．
  virtual
  ymuint
  child_num() const = 0;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  virtual
  YmslAst*
  child(ymuint pos) const = 0;

  /// @brief 子供を追加する．
  virtual
  void
  add_child(YmslAst* child) = 0;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const = 0;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLAST_H
