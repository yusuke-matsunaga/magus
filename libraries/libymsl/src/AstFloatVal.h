#ifndef ASTFLOATVAL_H
#define ASTFLOATVAL_H

/// @file AstFloatVal.h
/// @brief AstFloatVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFloatVal AstFloatVal.h "AstFloatVal.h"
/// @brief 浮動小数点型の定数を表す Ast
//////////////////////////////////////////////////////////////////////
class AstFloatVal :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstFloatVal(double val,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFloatVal();


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
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mVal;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTFLOATVAL_H
