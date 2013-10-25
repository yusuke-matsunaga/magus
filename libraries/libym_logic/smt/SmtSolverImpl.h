#ifndef SMTSOLVERIMPL_H
#define SMTSOLVERIMPL_H

/// @file SmtSolverImpl.h
/// @brief SmtSolverImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SmtSolver.h"
#include "ym_logic/SmtSort.h"
#include "ym_logic/SmtFun.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtSolverImpl SmtSolverImpl.h "SmtSolverImpl.h"
/// @brief SmtSolver の実装ファイル
//////////////////////////////////////////////////////////////////////
class SmtSolverImpl :
  public SmtSolver
{
public:

  /// @brief コンストラクタ
  SmtSolverImpl();

  /// @brief デストラクタ
  virtual
  ~SmtSolverImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用するロジックを設定する
  /// @param[in] logic 設定するロジック
  virtual
  bool
  set_logic(tSmtLogic logic);

  /// @brief 型を作る．
  /// @param[in] elem_list 要素の型のリスト
  /// @return 作成した型を返す．
  virtual
  const SmtSort*
  make_sort(const vector<const SmtSort*>& elem_list);

  /// @brief 組み込み型を作る．
  /// @param[in] type 型の種類
  virtual
  const SmtSort*
  make_builtin_sort(tSmtSort type);

  /// @brief 変数を作る．
  /// @param[in] sort 変数の型
  /// @param[in] type 変数の種類
  /// @return 作成した変数を返す．
  virtual
  const SmtVar*
  make_var(const SmtSort* sort,
	   tSmtVar type);

  /// @brief 関数を作る．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @return 作成した関数を返す．
  virtual
  const SmtFun*
  make_fun(const vector<const SmtSort*>& input_sort_list,
	   const SmtSort* output_sort);

  /// @brief 内容を持った関数を作る．
  /// @param[in] input_var_list 入力の変数のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] body 本体を式
  /// @return 作成した関数を返す．
  virtual
  const SmtFun*
  make_fun(const vector<const SmtVar*>& input_var_list,
	   const SmtSort* output_sort,
	   const SmtTerm* body);

  /// @brief 組み込み関数を作る．
  /// @param[in] fun_type 関数の種類
  /// @param[in] input_sort_list 入力の型のリスト
  /// @note 関数によっては input_sort_list が必要ない場合もある．
  virtual
  const SmtFun*
  make_builtin_fun(tSmtFun fun_type,
		   const vector<const SmtSort*>& input_sort_list = vector<const SmtSort*>(0));

  /// @brief <numeric> 型の term を作る．
  /// @param[in] val 値
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_numeric_term(ymuint32 val);

  /// @brief <decimal> 型の term を作る．
  /// @param[in] val 値
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_decimal_term(const ShString& val);

  /// @brief <hexadecimal> 型の term を作る．
  /// @param[in] val 値
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_hexadecimal_term(const ShString& val);

  /// @brief <binary> 型の term を作る．
  /// @param[in] val 値
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_binary_term(const ShString& val);

  /// @brief <string> 型の term を作る．
  /// @param[in] val 値
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_string_term(const ShString& val);

  /// @brief 変数型の term を作る．
  /// @param[in] var 変数
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_var_term(const SmtVar* var);

  /// @brief 関数呼び出しの term を作る．
  /// @param[in] fun 関数
  /// @param[in] arg_list 引数のリスト
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_fun_term(const SmtFun* fun,
		const vector<const SmtTerm*>& arg_list);

  /// @brief forall の term を作る．
  /// @param[in] var_list 束縛変数のリスト
  /// @param[in] body 本体の式
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_forall_term(const vector<const SmtVar*>& var_list,
		   const SmtTerm* body);

  /// @brief exists の term を作る．
  /// @param[in] var_list 束縛変数のリスト
  /// @param[in] body 本体の式
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_exists_term(const vector<const SmtVar*>& var_list,
		   const SmtTerm* body);

  /// @brief 属性付きの term を作る．
  /// @param[in] body 本体の式
  /// @param[in] attr_list 属性リスト
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_annotated_term(const SmtTerm* body,
		      const vector<SmtAttr>& attr_list);

  /// @brief 充足可能性を調べる．
  /// @param[in] assert_list 仮定する式のリスト
  virtual
  tSmtLibResponse
  check_sat(const vector<const SmtTerm*>& assert_list);

  /// @brief 直前の check_sat() の証明を得る．
  /// @note check_sat() の結果が UNSAT の時のみ意味を持つ．
  virtual
  tSmtLibResponse
  get_proof();

  /// @brief 直前の check_sat() の unsat-core を得る．
  /// @param[out] term_list unsat-core の式を格納するリスト
  /// @note check_sat() の結果が UNSAT の時のみ意味を持つ．
  virtual
  tSmtLibResponse
  get_unsat_core(vector<const SmtTerm*>& term_list);

  /// @brief 直前の check_sat() の値割り当てを得る．
  /// @param[in] expr_list expr 式のリスト
  /// @param[out] value_list 値のリスト
  /// @note expr_list[i] に対応する値が value_list[i] に格納される．
  virtual
  tSmtLibResponse
  get_value(const vector<const SmtTerm*>& expr_list);

  /// @brief 直前の check_sat() の値割り当てを得る．
  virtual
  tSmtLibResponse
  get_assignment();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CORE logic の初期化を行う．
  void
  Core_init();

  /// @brief INTS logic の初期化を行う．
  void
  Ints_init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // ロジック
  tSmtLogic mLogic;

  // SmtSort の次の ID番号
  ymuint32 mSortId;

  // SmtVar の次の ID番号
  ymuint32 mVarId;

  // Bool 型
  const SmtSort* mBoolSort;

  // Int 型
  const SmtSort* mIntSort;

  // Real 型
  const SmtSort* mRealSort;

  // true 関数
  const SmtFun* mTrueFun;

  // false 関数
  const SmtFun* mFalseFun;

  // not 関数
  const SmtFun* mNotFun;

  // and 関数
  const SmtFun* mAndFun;

  // or 関数
  const SmtFun* mOrFun;

  // xor 関数
  const SmtFun* mXorFun;

  // => 関数
  const SmtFun* mImpFun;

  // = 関数を保持するハッシュ表
  // 入力の型の ID 番号をキーにする．
  hash_map<ymuint32, const SmtFun*> mEqFunMap;

  // distinct 関数を保持するハッシュ表
  // 入力の型の ID 番号をキーにする．
  hash_map<ymuint32, const SmtFun*> mDiseqFunMap;

  // ite 関数を保持するハッシュ表
  // データの型のID番号をキーにする．
  hash_map<ymuint32, const SmtFun*> mIteFunMap;

  // 整数の単項マイナス
  const SmtFun* mIntUminusFun;

  // 整数の加算
  const SmtFun* mIntAddFun;

  // 整数の減算
  const SmtFun* mIntSubFun;

  // 整数の乗算
  const SmtFun* mIntMulFun;

  // 整数の <= 演算子
  const SmtFun* mIntLeFun;

  // 整数の < 演算子
  const SmtFun* mIntLtFun;

  // 整数の >= 演算子
  const SmtFun* mIntGeFun;

  // 整数の > 演算子
  const SmtFun* mIntGtFun;

};

END_NAMESPACE_YM_SMT

#endif // SMTSOLVERIMPL_H
