#ifndef YM_SMTLIBV2_SMTLIBV2_NSDEF_H
#define YM_SMTLIBV2_SMTLIBV2_NSDEF_H

/// @file ym_smtlibv2/smtlibv2_nsdef.h
/// @brief smtlibv2 の共通定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


/// @brief ym_smtlibv2 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SMTLIBV2 \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSmtLibV2)

/// @brief ym_sat 用の名前空間の終了
#define END_NAMESPACE_YM_SMTLIBV2 \
END_NAMESPACE(nsSmtLibV2) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_SMTLIBV2

/// @brief トークンの種類
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
  kEofToken
};


// クラス名の宣言
class SmtLibNode;

END_NAMESPACE_YM_SMTLIBV2

BEGIN_NAMESPACE_YM

END_NAMESPACE_YM

#endif // YM_SMTLIBV2_SMTLIBV2_NSDEF_H
