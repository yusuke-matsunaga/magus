#ifndef ASTINTVAL_H
#define ASTINTVAL_H

/// @file AstInt.h
/// @brief AstInt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIntVal AstIntVal.h "AstIntVal.h"
/// @brief 整数型定数を表す Ast
//////////////////////////////////////////////////////////////////////
class AstIntVal :
  public AstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstIntVal(int val,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIntVal();


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
  int mVal;

};

END_NAMESPACE_YM_YMSL


#endif // ASTINTVAL_H
