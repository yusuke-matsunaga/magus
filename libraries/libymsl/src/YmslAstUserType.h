#ifndef YMSLASTUSERTYPE_H
#define YMSLASTUSERTYPE_H

/// @file YmslAstUserType.h
/// @brief YmslAstUserType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstUserType YmslAstUserType.h "YmslAstUserType.h"
/// @brief ユーザー定義型を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstUserType :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] type_name 型名
  /// @param[in] loc ファイル位置
  YmslAstUserType(YmslAst* type_name);

  /// @brief デストラクタ
  virtual
  ~YmslAstUserType();


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

  // 形名
  YmslAst* mTypeName;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTUSERTYPE_H
