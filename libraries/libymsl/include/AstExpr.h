#ifndef ASTEXPR_H
#define ASTEXPR_H

/// @file AstExpr.h
/// @brief AstExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstExpr AstExpr.h "AstExpr.h"
/// @brief 式を表すクラス
//////////////////////////////////////////////////////////////////////
class AstExpr :
  public Ast
{
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
  ExprType
  expr_type() const = 0;

  /// @brief 終端を返す．
  ///
  /// kLeafExpr のみ有効
  virtual
  const AstLeaf*
  leaf() const;

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

#if 0
  /// @brief 変数の参照を解決する．
  virtual
  void
  resolve_var(YmslScope* parent_scope) = 0;

  /// @brief 式の型を解析する．
  /// @return 引数の方が間違っていたら false を返す．
  ///
  /// 結果としてキャスト演算が挿入される場合もある．
  virtual
  bool
  type_analysis() = 0;

  /// @brief 式の型を返す．
  virtual
  const YmslType*
  type() = 0;

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size() = 0;

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  virtual
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr) = 0;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const = 0;
#endif

};

END_NAMESPACE_YM_YMSL


#endif // ASTEXPR_H
