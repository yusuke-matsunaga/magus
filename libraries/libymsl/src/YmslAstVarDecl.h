#ifndef YMSLASTVARDECL_H
#define YMSLASTVARDECL_H

/// @file YmslAstVarDecl.h
/// @brief YmslAstVarDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstVarDecl YmslAstVarDecl.h "YmslAstVarDecl.h"
/// @brief 変数宣言を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstVarDecl :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  YmslAstVarDecl(YmslAst* id,
		 YmslAst* type,
		 YmslAst* init_expr,
		 const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstVarDecl();


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
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の配列
  YmslAst* mChildList[3];

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTVARDECL_H
