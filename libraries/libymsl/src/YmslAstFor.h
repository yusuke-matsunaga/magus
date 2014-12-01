#ifndef YMSLASTFOR_H
#define YMSLASTFOR_H

/// @file YmslAstFor.h
/// @brief YmslAstFor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstFor YmslAstFor.h "YmslAstFor.h"
/// @brief for 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstFor :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] init 初期化文
  /// @param[in] cond 条件式
  /// @param[in] next 増加文
  /// @param[in] body 本文
  /// @param[in] loc ファイル位置
  YmslAstFor(YmslAst* init,
	     YmslAst* cond,
	     YmslAst* next,
	     YmslAst* body,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstFor();


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
  YmslAst* mChildList[4];

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTFOR_H
