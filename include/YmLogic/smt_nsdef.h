#ifndef YMYMLOGIC_SMT_NSDEF_H
#define YMYMLOGIC_SMT_NSDEF_H

/// @file YmLogic/smt_nsdef.h
/// @brief smt の共通定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup SmtGroup SMT ソルバ
///
/// SMT ソルバを構成するクラスライブラリ


#include "YmTools.h"


/// @brief smt 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SMT \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSmt)

/// @brief smt 用の名前空間の終了
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
class SmtFun;
class SmtFunValidator;
class SmtSolver;
class SmtSortInfo;
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


/// @brief 型番号の型
typedef ymuint32 tSmtSortId;

/// @brief tSmtSortId の不適切な値
const tSmtSortId kSmtSort_None = 0;

/// @brief Bool 型を表す定数
const tSmtSortId kSmtSort_Bool = 1;

/// @brief 整数型を表す定数
const tSmtSortId kSmtSort_Int = 2;

/// @brief 実数型を表す定数
const tSmtSortId kSmtSort_Real = 3;

/// @brief 任意の型を表す定数
const tSmtSortId kSmtSort_Any = 4;

/// @brief 整数型か実数型を表す定数
const tSmtSortId kSmtSort_Num = 5;

/// @brief ユーザー定義型の定数の最初の値
const tSmtSortId kSmtSort_User1 = 6;


//////////////////////////////////////////////////////////////////////
/// @brief 組み込み関数を表す型
//////////////////////////////////////////////////////////////////////
enum tSmtFunType {
  kSmtFun_UserDef =  0,        // 一般の関数
  kSmtFun_True    =  1,        // logical TRUE
  kSmtFun_False   =  2,        // logical FALSE
  kSmtFun_Not     =  3,        // logical NOT
  kSmtFun_And     =  4,        // logical AND
  kSmtFun_Or      =  5,        // logical OR
  kSmtFun_Xor     =  6,        // logical XOR
  kSmtFun_Imp     =  7,        // logical IMPLICATAION (=>)
  kSmtFun_Eq      =  8,        // equality (=)
  kSmtFun_Diseq   =  9,        // disequalit (distinct)
  kSmtFun_Ite     = 10,        // if-then-else
  kSmtFun_Uminus  = 11,        // arithmetic unary minus (-)
  kSmtFun_Add     = 12,        // arithmetic binary addition (+)
  kSmtFun_Sub     = 13,        // arifthmetic binary subtraction (-)
  kSmtFun_Mul     = 14,        // arithmetic binary multiplication (*)
  kSmtFun_Div     = 15,        // arithmetic binary division (/)
  kSmtFun_Le      = 16,        // less than or equal (<=)
  kSmtFun_Lt      = 17,        // less than (<)
  kSmtFun_Ge      = 18,        // greater than or equal (>=)
  kSmtFun_Gt      = 19,        // greater than (>)
  kSmtFun_Last    = kSmtFun_Gt // end-marker
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


/// @brief 関数の型から属性を得る．
/// @param[in] fun_type 関数の型
/// @return 属性を返す．
extern
tSmtFunAttr
fun_attr(tSmtFunType fun_type);


//////////////////////////////////////////////////////////////////////
/// @brief 変数の種類を表す型
//////////////////////////////////////////////////////////////////////
enum tSmtVarType {
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

using nsSmt::SmtFun;
using nsSmt::SmtFunValidator;
using nsSmt::SmtSolver;
using nsSmt::SmtSortInfo;
using nsSmt::SmtTerm;
using nsSmt::SmtVar;

END_NAMESPACE_YM

#endif // YMYMLOGIC_SMT_NSDEF_H
