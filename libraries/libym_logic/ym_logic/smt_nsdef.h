#ifndef YM_LOGIC_SMT_NSDEF_H
#define YM_LOGIC_SMT_NSDEF_H

/// @file ym_logic/smt_nsdef.h
/// @brief smt の共通定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup SmtGroup SMT ソルバ
///
/// SMT ソルバを構成するクラスライブラリ


#include "ymtools.h"


/// @brief ym_smt 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SMT \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSmt)

/// @brief ym_smt 用の名前空間の終了
#define END_NAMESPACE_YM_SMT \
END_NAMESPACE(nsSmt) \
END_NAMESPACE_YM

/// @namespace nsYm::nsSmt
/// @brief SMT ソルバ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_SMT

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
class SmtFun;
class SmtSolver;
class SmtSort;
class SmtTerm;
class SmtVar;

END_NAMESPACE_YM_SMT

BEGIN_NAMESPACE_YM

using nsSmt::tSmtLogic;
using nsSmt::kSmtLogic_NONE;
using nsSmt::kSmtLogic_AUFLIA;
using nsSmt::kSmtLogic_AUFLIRA;
using nsSmt::kSmtLogic_AUFNIRA;
using nsSmt::kSmtLogic_LRA;
using nsSmt::kSmtLogic_QF_ABV;
using nsSmt::kSmtLogic_QF_AUFBV;
using nsSmt::kSmtLogic_QF_AUFLIA;
using nsSmt::kSmtLogic_QF_AX;
using nsSmt::kSmtLogic_QF_BV;
using nsSmt::kSmtLogic_QF_IDL;
using nsSmt::kSmtLogic_QF_LIA;
using nsSmt::kSmtLogic_QF_LRA;
using nsSmt::kSmtLogic_QF_NIA;
using nsSmt::kSmtLogic_QF_NRA;
using nsSmt::kSmtLogic_QF_RDL;
using nsSmt::kSmtLogic_QF_UF;
using nsSmt::kSmtLogic_QF_UFBV;
using nsSmt::kSmtLogic_QF_UFIDL;
using nsSmt::kSmtLogic_QF_UFLIA;
using nsSmt::kSmtLogic_QF_UFLRA;
using nsSmt::kSmtLogic_QF_UFNRA;
using nsSmt::kSmtLogic_UFLRA;
using nsSmt::kSmtLogic_UFNIA;

using nsSmt::SmtAttr;
using nsSmt::SmtFun;
using nsSmt::SmtSolver;
using nsSmt::SmtSort;
using nsSmt::SmtTerm;
using nsSmt::SmtVar;

END_NAMESPACE_YM

#endif // YM_LOGIC_SMT_NSDEF_H
