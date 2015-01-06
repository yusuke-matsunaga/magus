#ifndef ASTPRIMARY_H
#define ASTPRIMARY_H

/// @file AstPrimary.h
/// @brief AstPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstPrimary AstPrimary.h "AstPrimary.h"
/// @brief primary を表すクラス
//////////////////////////////////////////////////////////////////////
class AstPrimary :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_name 変数名
  /// @param[in] loc ファイル位置
  AstPrimary(AstSymbol* var_name,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstPrimary();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を返す．
  AstVarDecl*
  var() const;

  /// @brief 変数に対する代入命令を返す．
  Ymsl_CODE
  opcode() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数名
  AstSymbol* mVarName;

  // 変数
  AstVarDecl* mVar;

};

END_NAMESPACE_YM_YMSL

#endif // ASTPRIMARY_H
