#ifndef ASTBINOP_H
#define ASTBINOP_H

/// @file AstBinOp.h
/// @brief AstBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @ckass AstBinOp AstBinOp.h "AstBinOp.h"
/// @brief 二項演算子を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstBinOp :
  public AstExpr
{
public:

  /// @breif コンストラクタ
  /// @param[in] op 演算子のタイプ
  /// @param[in] left, right オペランド
  AstBinOp(ExprType op,
	   AstExpr* left,
	   AstExpr* right);

  /// @brief デストラクタ
  virtual
  ~AstBinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

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
  resolve_var(YmslScope* parent_scope);

  /// @brief 式の型を解析する．
  /// @return 引数の方が間違っていたら false を返す．
  ///
  /// 結果としてキャスト演算が挿入される場合もある．
  virtual
  bool
  type_analysis();

  /// @brief 式の型を返す．
  virtual
  const YmslType*
  type();

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size();

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
	  Ymsl_INT& addr);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子のタイプ
  ExprType mOp;

  // 型
  const YmslType* mType;

  // 左オペランド
  AstExpr* mLeft;

  // 右オペランド
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBINOP_H
