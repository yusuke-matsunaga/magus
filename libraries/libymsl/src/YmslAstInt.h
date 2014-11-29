#ifndef YMSLASTINT_H
#define YMSLASTINT_H

/// @file YmslAstInt.h
/// @brief YmslAstInt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstInt YmslAstInt.h "YmslAstInt.h"
/// @brief 整数型定数を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstInt :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAstInt(int val,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstInt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 整数型の値を返す．
  virtual
  int
  int_val() const;

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
  int mVal;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTINT_H
