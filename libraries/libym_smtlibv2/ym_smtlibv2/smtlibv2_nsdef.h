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

//////////////////////////////////////////////////////////////////////
/// @brief SmtLib の処理結果を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tSmtLibResponse {
  /// @brief 成功
  kSmtLibSuccess,
  /// @brief エラー
  kSmtLibError,
  /// @brief 未サポート
  kSmtLibUnsupported
};


// クラス名の宣言
class SmtAttr;
class SmtConst;
class SmtExpr;
class SmtFun;
class SmtId;
class SmtSort;
class SmtSortedVar;
class SmtTerm;

END_NAMESPACE_YM_SMTLIBV2

BEGIN_NAMESPACE_YM

using nsSmtLibV2::SmtAttr;
using nsSmtLibV2::SmtConst;
using nsSmtLibV2::SmtExpr;
using nsSmtLibV2::SmtFun;
using nsSmtLibV2::SmtId;
using nsSmtLibV2::SmtSort;
using nsSmtLibV2::SmtSortedVar;
using nsSmtLibV2::SmtTerm;

END_NAMESPACE_YM

#endif // YM_SMTLIBV2_SMTLIBV2_NSDEF_H
