#ifndef YM_DOTLIB_DOTLIB_NSDEF_H
#define YM_DOTLIB_DOTLIB_NSDEF_H

/// @file ym_dotlib/dotlib_nsdef.h
/// @brief dotlib サブモジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief dotlib の名前空間の開始
#define BEGIN_NAMESPACE_YM_DOTLIB \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsDotlib)

/// @brief dotlib の名前空間の終了
#define END_NAMESPACE_YM_DOTLIB \
END_NAMESPACE(nsDotlib) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_DOTLIB

/// @brief トークンの値
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
  /// @brief 左括弧( ( )
  LP,
  /// @brief 右括弧( ) )
  RP,
  /// @brief 左中括弧( { )
  LCB,
  /// @brief 右中括弧( } )
  RCB,
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
class DotlibParser;

class PtMgr;
class PtNode;
class PtAttr;

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIB_NSDEF_H
