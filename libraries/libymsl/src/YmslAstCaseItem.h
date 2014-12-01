#ifndef YMSLASTCASEITEM_H
#define YMSLASTCASEITEM_H

/// @file YmslAstCaseItem.h
/// @brief YmslAstCaseItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstCaseItem YmslAstCaseItem.h "YmslAstCaseItem.h"
/// @brief case ブロックを表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstCaseItem :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] statement_list 本体
  /// @param[in] loc ファイル位置
  YmslAstCaseItem(YmslAst* label,
		  YmslAst* statement_list,
		  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstCaseItem();


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


#endif // YMSLASTCASEITEM_H
