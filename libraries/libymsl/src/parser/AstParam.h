#ifndef ASTPARAM_H
#define ASTPARAM_H

/// @file AstParam.h
/// @brief AstParam のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstParam AstParam.h "AstParam.h"
/// @brief 関数のパラメータを表すクラス
//////////////////////////////////////////////////////////////////////
class AstParam :
  public Ast
{
  friend class YmslModule;

public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  AstParam(ShString name,
	   AstType* type,
	   AstExpr* init_expr,
	   const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstParam();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  ShString
  name() const;

  /// @brief インデックス番号を返す．
  ymuint
  index() const;

  /// @brief 型を得る．
  AstType*
  type() const;

  /// @brief 初期化式を返す．
  ///
  /// NULL の場合もある．
  AstExpr*
  init_expr() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // インデックス番号
  ymuint mIndex;

  // 変数の型
  AstType* mType;

  // 初期化式
  AstExpr* mInitExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTPARAM_H
