#ifndef YMSLASTFLOAT_H
#define YMSLASTFLOAT_H

/// @file YmslAstFloat.h
/// @brief YmslAstFloat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstFloat YmslAstFloat.h "YmslAstFloat.h"
/// @brief 浮動小数点型を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstFloat :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  YmslAstFloat(double val,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstFloat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 浮動小数点型の値を返す．
  virtual
  double
  float_val() const;

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
  double mVal;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTFLOAT_H
