#ifndef TOKENTYPE_H
#define TOKENTYPE_H

/// @file TokenType.h
/// @brief tTokenType の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @brief トークンの種類
//////////////////////////////////////////////////////////////////////
enum tTokenType {
  /// @brief NUMERAL
  kNumToken,
  /// @brief DECIMAL
  kDecToken,
  /// @brief HEXADECIMAL
  kHexToken,
  /// @brief BINARY
  kBinToken,
  /// @brief STRING
  kStringToken,
  /// @brief SYMBOL
  kSymbolToken,
  /// @brief KEYWORD
  kKeywordToken,
  /// @brief LIST
  kListToken,
  /// @brief (
  kLpToken,
  /// @brief )
  kRpToken,
  /// @brief NL
  kNlToken,
  /// @brief ERROR
  kErrorToken,
  /// @brief EOF
  kEofToken,
  /// @brief unexpected EOF
  kUnexpEofToken
};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTLIBV2_NSDEF_H
