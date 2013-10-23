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
/// @brief logic を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tSmtLogic {
  kSmtLogic_NONE,
  kSmtLogic_AUFLIA,
  kSmtLogic_AUFLIRA,
  kSmtLogic_AUFNIRA,
  kSmtLogic_LRA,
  kSmtLogic_QF_ABV,
  kSmtLogic_QF_AUFBV,
  kSmtLogic_QF_AUFLIA,
  kSmtLogic_QF_AX,
  kSmtLogic_QF_BV,
  kSmtLogic_QF_IDL,
  kSmtLogic_QF_LIA,
  kSmtLogic_QF_LRA,
  kSmtLogic_QF_NIA,
  kSmtLogic_QF_NRA,
  kSmtLogic_QF_RDL,
  kSmtLogic_QF_UF,
  kSmtLogic_QF_UFBV,
  kSmtLogic_QF_UFIDL,
  kSmtLogic_QF_UFLIA,
  kSmtLogic_QF_UFLRA,
  kSmtLogic_QF_UFNRA,
  kSmtLogic_UFLRA,
  kSmtLogic_UFNIA
};


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
class SmtId;
class SmtMgr;
class SmtSort;
class SmtTerm;
class SmtVarFun;

END_NAMESPACE_YM_SMTLIBV2

BEGIN_NAMESPACE_YM

using nsSmtLibV2::SmtAttr;
using nsSmtLibV2::SmtId;
using nsSmtLibV2::SmtMgr;
using nsSmtLibV2::SmtSort;
using nsSmtLibV2::SmtTerm;
using nsSmtLibV2::SmtVarFun;

END_NAMESPACE_YM

#endif // YM_SMTLIBV2_SMTLIBV2_NSDEF_H
