#ifndef AST_H
#define AST_H

/// @file Ast.h
/// @brief Ast のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

#if 0
/// @brief 抽象構文木の型
enum AstType
{
  kAstSymbol,

  kAstInt,
  kAstFloat,
  kAstString,

  kAstStringType,
  kAstIntType,
  kAstFloatType,
  kAstUserType,

  kAstList,

  kAstVarDecl,
  kAstFuncDecl,

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
#endif


//////////////////////////////////////////////////////////////////////
/// @class Ast Ast.h "Ast.h"
/// @brief YMSL の抽象構文木クラス
//////////////////////////////////////////////////////////////////////
class Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  Ast(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~Ast();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を得る．
  const FileRegion&
  file_region() const;

#if 0
  /// @brief 型を得る．
  virtual
  AstType
  type() const = 0;

  /// @brief 名前を得る．
  ///
  /// 名前を持つ要素のみ意味を持つ．
  virtual
  ShString
  name() const = 0;

  /// @brief ブロックを返す．
  ///
  /// ブロックを持たない要素の場合 NULL を返す．
  virtual
  YmslBlock*
  block() const = 0;

  /// @brief 文字列型の値を返す．
  virtual
  ShString
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
  Ast*
  child(ymuint pos) const = 0;

  /// @brief 子供を追加する．
  virtual
  void
  add_child(Ast* child) = 0;

  /// @brief ファイル位置を設定する．
  virtual
  void
  set_file_region(const FileRegion& loc) = 0;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const = 0;
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mLoc;

};

END_NAMESPACE_YM_YMSL

#endif // AST_H
