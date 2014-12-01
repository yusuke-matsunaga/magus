#ifndef YMSLASTASSIGNMENT_H
#define YMSLASTASSIGNMENT_H

/// @file YmslAstAssignment.h
/// @brief YmslAstAssignment のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstAssignment YmslAstAssignment.h "YmslAstAssignment.h"
/// @brief 代入文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstAssignment :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  YmslAstAssignment(YmslAst* left,
		    YmslAst* right);

  /// @brief デストラクタ
  virtual
  ~YmslAstAssignment();


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

#endif // YMSLASTASSIGNMENT_H
