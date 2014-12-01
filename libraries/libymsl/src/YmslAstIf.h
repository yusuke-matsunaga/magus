#ifndef YMSLASTIF_H
#define YMSLASTIF_H

/// @file YmslAstIf.h
/// @brief YmslAstIf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstIf YmslAstIf.h "YmslAstIf.h"
/// @brief if 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstIf :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] then_list then 文のリスト
  /// @param[in] elif_list elif ブロックのリスト
  /// @param[in] else_list else 文のリスト
  /// @param[in] loc ファイル位置
  YmslAstIf(YmslAst* cond,
	    YmslAst* then_list,
	    YmslAst* elif_list,
	    YmslAst* else_list,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstIf();


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


#endif // YMSLASTIF_H
