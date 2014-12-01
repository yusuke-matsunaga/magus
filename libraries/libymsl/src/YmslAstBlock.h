#ifndef YMSLASTBLOCK_H
#define YMSLASTBLOCK_H

/// @file YmslAstBlock.h
/// @brief YmslAstBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstBlock YmslAstBlock.h "YmslAstBlock.h"
/// @brief block文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstBlock :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] statement_list 文のリスト
  /// @param[in] loc ファイル位置
  YmslAstBlock(YmslAst* statement_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstBlock();


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

  // 文のリスト
  YmslAst* mStatementList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTBLOCK_H
