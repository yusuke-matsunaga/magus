#ifndef ASTENUMCONST_H
#define ASTENUMCONST_H

/// @file AstEnumConst.h
/// @brief AstEnumConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstEnumConst AstEnumConst.h "AstEnumConst.h"
/// @brief enum 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class AstEnumConst :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] expr 値を表す式
  /// @param[in] loc ファイル位置
  AstEnumConst(AstSymbol* name,
	       AstExpr* expr,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstEnumConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const AstSymbol*
  name() const;

  /// @brief 値を表す式を返す．
  ///
  /// NULL の場合もある．
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  AstSymbol* mName;

  // 値を表す式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTENUMCONST_H
