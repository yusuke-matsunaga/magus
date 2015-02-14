#ifndef ASTEXPR_H
#define ASTEXPR_H

/// @file AstExpr.h
/// @brief AstExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "OpCode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstExpr AstExpr.h "AstExpr.h"
/// @brief 式を表すクラス
//////////////////////////////////////////////////////////////////////
class AstExpr :
  public Ast
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief 式の種類
  //////////////////////////////////////////////////////////////////////
  enum Type {
    // 定数
    kTrue,
    kFalse,
    kIntConst,
    kFloatConst,
    kStringConst,
    // 終端
    kSymbolExpr,
    kArrayRef,
    kMemberRef,
    // 演算
    kUniOp,
    kBinOp,
    kTriOp,
    // 関数呼び出し
    kFuncCall
  };


public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstExpr(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  expr_type() const = 0;

  /// @brief 整数値を返す．
  ///
  /// kIntConst のみ有効
  virtual
  Ymsl_INT
  int_val() const;

  /// @brief 浮動小数点値を返す．
  ///
  /// kFloatConst のみ有効
  virtual
  Ymsl_FLOAT
  float_val() const;

  /// @brief 文字列値を返す．
  ///
  /// kStringConst のみ有効
  virtual
  const char*
  string_val() const;

  /// @brief シンボルを返す．
  ///
  /// kSymbolExpr のみ有効
  virtual
  const AstSymbol*
  symbol() const;

  /// @brief 本体を返す．
  ///
  /// kArrayRef, kMemberRef のみ有効
  virtual
  const AstExpr*
  body() const;

  /// @brief メンバ名を返す．
  virtual
  const AstSymbol*
  member() const;

  /// @brief 配列のインデックスを返す．
  ///
  /// kArrayRef のみ有効
  virtual
  const AstExpr*
  index() const;

  /// @brief オペコードを返す．
  ///
  /// 演算子のみ有効
  virtual
  OpCode
  opcode() const;

  /// @brief オペランド数を返す．
  ///
  /// 演算子のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num()
  ///
  /// 演算子のみ有効
  virtual
  const AstExpr*
  operand(ymuint pos) const;

  /// @brief 関数本体を返す．
  virtual
  const AstExpr*
  func() const;

  /// @brief 引数リストの要素数を返す．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 引数リストの要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  const AstExpr*
  arglist_elem(ymuint pos) const;

};

/// @brief AstExpr::Type を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] et 式の種類
ostream&
operator<<(ostream& s,
	   AstExpr::Type et);

END_NAMESPACE_YM_YMSL


#endif // ASTEXPR_H
