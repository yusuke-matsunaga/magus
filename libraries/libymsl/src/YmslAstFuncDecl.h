#ifndef YMSLASTFUNCDECL_H
#define YMSLASTFUNCDECL_H

/// @file YmslAstFuncDecl.h
/// @brief YmslAstFuncDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstFuncDecl YmslAstFuncDecl.h "YmslAstFuncDecl.h"
/// @brief 関数宣言を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstFuncDecl :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id 変数名
  /// @param[in] type 型
  /// @param[in] param_list パラメータリスト
  /// @param[in] statement_list 本文のリスト
  /// @param[in] loc ファイル位置
  YmslAstFuncDecl(YmslAst* id,
		  YmslAst* type,
		  YmslAst* param_list,
		  YmslAst* statement_list,
		  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstFuncDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 子供の数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置( 0 <= pos < child_num() )
  virtual
  YmslAst*
  child(ymuint pos) const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の配列
  YmslAst* mChildList[4];

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTFUNCDECL_H
