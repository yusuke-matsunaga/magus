#ifndef YMSLASTSTRING_H
#define YMSLASTSTRING_H

/// @file YmslAstString.h
/// @brief YmslAstString のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstString YmslAstString.h "YmslAstString.h"
/// @brief 整数型定数を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstString :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAstString(const char* val,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstString();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 文字列型の値を返す．
  virtual
  const char*
  str_val() const;

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
  const char* mVal;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTSTRING_H
