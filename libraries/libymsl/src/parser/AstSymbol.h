#ifndef ASTSYMBOL_H
#define ASTSYMBOL_H

/// @file AstSymbol.h
/// @brief AstSymbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstSymbol AstSymbol.h "AstSymbol.h"
/// @brief シンボルを表す Ast
//////////////////////////////////////////////////////////////////////
class AstSymbol :
  public Ast
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

  /// @brief 次の要素をセットする．
  void
  set_next(AstSymbol* next);

  /// @brief 文字列型の値を返す．
  ShString
  str_val() const;

  /// @brief 次の要素を得る．
  ///
  /// なければ NULL を返す．
  AstSymbol*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mVal;

  // 次の要素
  AstSymbol* mNext;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSYMBOL_H
