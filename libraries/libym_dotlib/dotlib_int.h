#ifndef LIBYM_DOTLIB_DOTLIB_INT_H
#define LIBYM_DOTLIB_DOTLIB_INT_H

/// @file libym_dotlib/dotlib_int.h
/// @brief dotlib サブモジュールの内部定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

/// @brief トークンの値
/// 基本的には dotlib のシンタックスにしたがっているが，
/// 一部，function 属性の文字列の中身をパーズする時のみ現れるシンボルがある．
/// AND, OR, XOR, PRIME がそれ．
enum tTokenType {
  /// @brief コロン(:)
  COLON,
  /// @brief セミコロン(;)
  SEMI,
  /// @brief コンマ(,)
  COMMA,
  /// @brief プラス(+)
  PLUS,
  /// @brief マイナス(-)
  MINUS,
  /// @brief かける(*)
  MULT,
  /// @brief わる(/)
  DIV,
  /// @brief NOT
  NOT,
  /// @brief AND
  AND,
  /// @brief OR
  OR,
  /// @brief XOR
  XOR,
  /// @brief 左括弧( ( )
  LP,
  /// @brief 右括弧( ) )
  RP,
  /// @brief 左中括弧( { )
  LCB,
  /// @brief 右中括弧( } )
  RCB,
  /// @brief プライム( ' )
  PRIME,
  /// @brief シンボル
  SYMBOL,
  /// @brief 整数値
  INT_NUM,
  /// @brief 浮動小数点数値
  FLOAT_NUM,
  /// @brief 式
  EXPRESSION,
  /// @brief 改行
  NL,
  /// @brief エラー
  ERROR,
  /// @brief ファイルの末尾
  END
};

// @brief tTokenType 内容をストリームに出力する．
ostream&
operator<<(ostream& s,
	   tTokenType type);

// クラス名の前方宣言
class DotlibParserImpl;
class DotlibHandler;
class GroupHandler;
class DotlibMgrImpl;
class DotlibNodeImpl;

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIB_INT_H
