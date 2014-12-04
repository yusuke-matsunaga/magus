#ifndef ASTSTRINGVAL_H
#define ASTSTRINGVAL_H

/// @file AstStringVal.h
/// @brief AstStringVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstStringVal AstStringVal.h "AstStringVal.h"
/// @brief 整数型定数を表す Ast
//////////////////////////////////////////////////////////////////////
class AstStringVal :
  public AstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstStringVal(ShString val,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstStringVal();


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
  ShString
  str_val() const;

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
  ShString mVal;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSTRINGVAL_H
