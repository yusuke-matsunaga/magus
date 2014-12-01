#ifndef YMSLASTRETURN_H
#define YMSLASTRETURN_H

/// @file YmslAstReturn.h
/// @brief YmslAstReturn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstReturn YmslAstReturn.h "YmslAstReturn.h"
/// @brief return 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstReturn :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 値
  /// @param[in] loc ファイル位置
  YmslAstReturn(YmslAst* expr,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstReturn();


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

  // 値
  YmslAst* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTRETURN_H
