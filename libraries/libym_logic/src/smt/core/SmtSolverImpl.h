#ifndef SMTSOLVERIMPL_H
#define SMTSOLVERIMPL_H

/// @file SmtSolverImpl.h
/// @brief SmtSolverImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/SmtSolver.h"
#include "YmLogic/SmtFun.h"
#include "YmUtils/SimpleAlloc.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_SMT

class SmtSortImpl;

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
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーとなる原因は以下のとおり
  ///  - 既に set_logic() が呼ばれていた．
  ///  - (unspported) サポートしていないロジックだった．
  virtual
  bool
  set_logic(tSmtLogic logic);

  /// @brief 型を作る．(単純型)
  /// @return 作成した型を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  tSmtSortId
  make_sort();

  /// @brief 型を作る．(複合型)
  /// @param[in] elem_list 要素の型のリスト
  /// @return 作成した型を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  tSmtSortId
  make_sort(const vector<tSmtSortId>& elem_list);

  /// @brief 型番号から SmtSort を得る．
  /// @param[in] id 型番号
  /// @return SmtSort を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  ///  - set_logic() で使える組み込み型ではない．
  ///  - 存在しない型番号だった．
  virtual
  const SmtSortInfo*
  get_sort(tSmtSortId id);

  /// @brief 変数を作る．
  /// @param[in] sort 変数の型
  /// @param[in] type 変数の種類
  /// @return 作成した変数を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  const SmtVar*
  make_var(tSmtSortId sort,
	   tSmtVarType type);

  /// @brief 関数を作る．(引数なし)
  /// @param[in] output_sort 出力の型
  /// @return 作成した関数を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  const SmtFun*
  make_fun(tSmtSortId output_sort);

  /// @brief 関数を作る．
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @return 作成した関数を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  const SmtFun*
  make_fun(const vector<tSmtSortId>& input_sort_list,
	   tSmtSortId output_sort);

  /// @brief 内容を持った関数を作る．(引数なし)
  /// @param[in] body 本体を式
  /// @return 作成した関数を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  const SmtFun*
  make_fun(const SmtTerm* body);

  /// @brief 内容を持った関数を作る．
  /// @param[in] input_var_list 入力の変数のリスト
  /// @param[in] body 本体を式
  /// @return 作成した関数を返す．
  ///
  /// エラーが起きた場合には NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - set_logic() が呼ばれていない．
  virtual
  const SmtFun*
  make_fun(const vector<const SmtVar*>& input_var_list,
	   const SmtTerm* body);

  /// @brief 組み込み関数用のチェッカーを得る．
  /// @param[in] fun_type 関数の型
  virtual
  const SmtFunValidator*
  get_fun_validator(tSmtFunType fun_type);

  /// @brief <numeral> 型の term を作る．
  /// @param[in] val 値
  /// @return 作成した式を返す．
  virtual
  const SmtTerm*
  make_numeral_term(ymuint32 val);

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

  /// @brief 関数呼び出しの term を作る．(引数なし)
  /// @param[in] fun 関数
  /// @return 作成した式を返す．
  ///
  /// エラーが起きたら NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - fun の引数の数か型が arg_list と合わない．
  virtual
  const SmtTerm*
  make_fun_term(const SmtFun* fun);

  /// @brief 関数呼び出しの term を作る．
  /// @param[in] fun 関数
  /// @param[in] arg_list 引数のリスト
  /// @return 作成した式を返す．
  ///
  /// エラーが起きたら NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - fun の引数の数か型が arg_list と合わない．
  virtual
  const SmtTerm*
  make_fun_term(const SmtFun* fun,
		const vector<const SmtTerm*>& arg_list);

  /// @brief 関数呼び出しの term を作る．(組み込み関数，引数なし)
  /// @param[in] fun_type 関数の型
  /// @return 作成した式を返す．
  ///
  /// エラーが起きたら NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - fun の引数の数か型が arg_list と合わない．
  virtual
  const SmtTerm*
  make_fun_term(tSmtFunType fun_type);

  /// @brief 関数呼び出しの term を作る．(組み込み関数)
  /// @param[in] fun_type 関数の型
  /// @param[in] arg_list 引数のリスト
  /// @return 作成した式を返す．
  ///
  /// エラーが起きたら NULL を返す．
  /// エラーとなる原因は以下のとおり
  ///  - fun の引数の数か型が arg_list と合わない．
  virtual
  const SmtTerm*
  make_fun_term(tSmtFunType fun_type,
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

  /// @brief REALS logic の初期化を行う．
  void
  Reals_init();

  /// @brief set_logic() のチェックを行う．
  bool
  check_logic();

  /// @brief SmtSort を登録する．
  /// @param[in] sort 登録するオブジェクト
  /// @return 割り当てたID番号を返す．
  tSmtSortId
  reg_sort(SmtSortImpl* sort);

  /// @brief SmtVar のID番号を得る．
  ymuint32
  new_var_id();


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

  // mSortArray のサイズ
  ymuint32 mSortArraySize;

  // SmtSortInfo のポインタ配列
  const SmtSortInfo** mSortArray;

  // SmtVar の次の ID番号
  ymuint32 mVarId;

  // SmtFunValidator 用のハッシュ表
  HashMap<ymuint32, const SmtFunValidator*> mFunValidatorMap;

};

END_NAMESPACE_YM_SMT

#endif // SMTSOLVERIMPL_H
