#ifndef ASTSYMBOL_H
#define ASTSYMBOL_H

/// @file AstSymbol.h
/// @brief AstSymbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstImpl.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstSymbol AstSymbol.h "AstSymbol.h"
/// @brief シンボルを表す Ast
//////////////////////////////////////////////////////////////////////
class AstSymbol :
  public AstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstSymbol(ShString val,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstSymbol();


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


#endif // ASTSYMBOL_H
