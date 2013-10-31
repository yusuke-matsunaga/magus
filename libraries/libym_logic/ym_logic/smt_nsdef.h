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


// クラス名の宣言
class SmtAttr;
class SmtFun;
class SmtSolver;
class SmtSort;
class SmtTerm;
class SmtVar;

END_NAMESPACE_YM_SMT

BEGIN_NAMESPACE_YM

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
/// @class 組み込み型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tSmtSort {
  kSmtSort_UserDef,    // 一般の型
  kSmtSort_Bool,       // Bool 型
  kSmtSort_Int,        // 整数型
  kSmtSort_Real        // 実数型
};


//////////////////////////////////////////////////////////////////////
/// @brief 組み込み関数を表す型
//////////////////////////////////////////////////////////////////////
enum tSmtFun {
  kSmtFun_UserDef,     // 一般の関数
  kSmtFun_True,        // logical TRUE
  kSmtFun_False,       // logical FALSE
  kSmtFun_Not,         // logical NOT
  kSmtFun_And,         // logical AND
  kSmtFun_Or,          // logical OR
  kSmtFun_Xor,         // logical XOR
  kSmtFun_Imp,         // logical IMPLICATAION (=>)
  kSmtFun_Eq,          // equality (=)
  kSmtFun_Diseq,       // disequalit (distinct)
  kSmtFun_Ite,         // if-then-else
  kSmtFun_Uminus,      // arithmetic unary minus (-)
  kSmtFun_Add,         // arithmetic binary addition (+)
  kSmtFun_Sub,         // arifthmetic binary subtraction (-)
  kSmtFun_Mul,         // arithmetic binary multiplication (*)
  kSmtFun_Div,         // arithmetic binary division (/)
  kSmtFun_Le,          // less than or equal (<=)
  kSmtFun_Lt,          // less than (<)
  kSmtFun_Ge,          // greater than or equal (>=)
  kSmtFun_Gt           // greater than (>)
};


//////////////////////////////////////////////////////////////////////
/// @brief 任意のオペランドをとる関数の属性
///
/// 関数の属性は以下のとおり
///  - right assoc: 右方結合性．マルチオペランドのときに右から対にする．
///      cf) (R a b c d e) は (R a (R b (R c (R d e)))) と等価
///  - left assoc: 左方結合性．マルチオペランドのときに左から対にする．
///      cf) (L a b c d e) は (L (L (L (L a b) c) d) e) と等価
///  - chainable: マルチオペランドのときに各オペランドの間に演算子を挿入する．
///      cf) (C a b c d e) は (and (C a b) (C b c) (C c d) (C d e)) と等価
///          ちなに and は left assoc
///  - pairwise: マルチオペランドのときに全ての対を考える．
///      cf) (P a b c d e) は (and (P a b) (P a c) (P a d) (P a e)
///                                (P b c) (P b d) (P b e)
///                                (P c d) (P c e)
///                                (P d e)
///          と等価
//////////////////////////////////////////////////////////////////////
enum tSmtFunAttr {
  kSmtFunAttr_None,       // なし
  kSmtFunAttr_RightAssoc, // 右方結合
  kSmtFunAttr_LeftAssoc,  // 左方結合
  kSmtFunAttr_Chainable,  // chainable
  kSmtFunAttr_Pairwise    // pairwise
};


//////////////////////////////////////////////////////////////////////
/// @brief 変数の種類を表す型
//////////////////////////////////////////////////////////////////////
enum tSmtVar {
  kSmtVar_Global, // グローバル変数
  kSmtVar_FunArg, // 関数の引数
  kSmtVar_Forall, // forall の束縛変数
  kSmtVar_Exists  // exists の束縛変数
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

using nsSmt::SmtAttr;
using nsSmt::SmtFun;
using nsSmt::SmtSolver;
using nsSmt::SmtSort;
using nsSmt::SmtTerm;
using nsSmt::SmtVar;

END_NAMESPACE_YM

#endif // YM_LOGIC_SMT_NSDEF_H
