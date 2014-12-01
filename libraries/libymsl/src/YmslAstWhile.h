#ifndef YMSLASTWHILE_H
#define YMSLASTWHILE_H

/// @file YmslAstWhile.h
/// @brief YmslAstWhile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstWhile YmslAstWhile.h "YmslAstWhile.h"
/// @brief while 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstWhile :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] body 本文
  /// @param[in] loc ファイル位置
  YmslAstWhile(YmslAst* cond,
	       YmslAst* body,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstWhile();


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
  YmslAst* mChildList[2];

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTWHILE_H
