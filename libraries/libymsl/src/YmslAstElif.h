#ifndef YMSLASTELIF_H
#define YMSLASTELIF_H

/// @file YmslAstElif.h
/// @brief YmslAstElif のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstElif YmslAstElif.h "YmslAstElif.h"
/// @brief case ブロックを表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstElif :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] statement_list 本体
  /// @param[in] loc ファイル位置
  YmslAstElif(YmslAst* cond,
	      YmslAst* statement_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstElif();


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
  YmslAst* mChildList[2];

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTELIF_H
