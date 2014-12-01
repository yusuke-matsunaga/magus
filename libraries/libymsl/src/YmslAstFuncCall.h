#ifndef YMSLASTFUNCCALL_H
#define YMSLASTFUNCCALL_H

/// @file YmslAstFuncCall.h
/// @brief YmslAstFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstFuncCall YmslAstFuncCall.h "YmslAstFuncCall.h"
/// @brief 関数呼び出しを表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstFuncCall :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id 関数名
  /// @param[in] expr_list 引数のリスト
  /// @param[in] loc ファイル位置
  YmslAstFuncCall(YmslAst* id,
		  YmslAst* expr_list,
		  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstFuncCall();


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


#endif // YMSLASTFUNCCALL_H
