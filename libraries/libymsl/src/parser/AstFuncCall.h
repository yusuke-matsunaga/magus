#ifndef ASTFUNCCALL_H
#define ASTFUNCCALL_H

/// @file AstFuncCall.h
/// @brief AstFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstPrimary.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncCall AstFuncCall.h "AstFuncCall.h"
/// @brief 関数呼び出しを表す Ast
//////////////////////////////////////////////////////////////////////
class AstFuncCall :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  /// @param[in] expr_list 引数のリスト
  /// @param[in] loc ファイル位置
  AstFuncCall(AstExpr* func,
	      AstExprList* expr_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 関数本体を返す．
  virtual
  const AstExpr*
  func_body() const;

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

  // 関数
  AstExpr* mFunc;

  // 引数のリスト
  vector<AstExpr*> mExprList;

};

END_NAMESPACE_YM_YMSL


#endif // ASTFUNCCALL_H
