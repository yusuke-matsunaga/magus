#ifndef YMSLCOMPILER_H
#define YMSLCOMPILER_H

/// @file YmslCompiler.h
/// @brief YmslCompiler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmslTypeMgr.h"
#include "YmUtils/IDO.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslCompiler YmslCompiler.h "YmslCompiler.h"
/// @brief YMSL 用のコンパイラ
//////////////////////////////////////////////////////////////////////
class YmslCompiler
{
public:

  /// @brief コンストラクタ
  YmslCompiler();

  /// @brief デストラクタ
  ~YmslCompiler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コンパイルする．
  /// @param[in] ido 入力データ
  /// @return 成功したら true を返す．
  bool
  compile(IDO& ido);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の生成を行う．
  /// @param[in] stmt 文
  void
  phase1(const AstStatement* stmt);

  /// @brief enum 型の定義を行う．
  /// @param[in] stmt 文
  ///
  /// stmt は kEnumDecl でなければならない．
  void
  reg_enum(const AstStatement* stmt);

  /// @brief 関数の定義を行う．
  /// @param[in] stmt 文
  ///
  /// stmt は kFuncDecl でなければならない．
  void
  reg_func(const AstStatement* stmt);

  /// @brief 変数の定義を行う．
  /// @param[in] stmt 文
  ///
  /// stmt は kVarDecl でなければならない．
  void
  reg_var(const AstStatement* stmt);

  /// @brief 型の参照を解決する．
  /// @param[in] asttype 型を表す構文木
  ///
  /// 解決できない時には NULL を返す．
  const YmslType*
  resolve_type(const AstType* asttype);

  /// @brief 参照の解決を行う．
  /// @param[in] stmt 文
  void
  phase2(const AstStatement* stmt);

  /// @brief 参照の解決を行う．
  /// @param[in] expr 式
  void
  phase2(const AstExpr* expr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型を管理するマネージャ
  YmslTypeMgr mTypeMgr;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLCOMPILER_H
