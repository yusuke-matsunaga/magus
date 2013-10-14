#ifndef YM_SMTLIBV2_SMTMGR_H
#define YM_SMTLIBV2_SMTMGR_H

/// @file ym_smtlibv2/SmtMgr.h
/// @brief SmtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/SimpleAlloc.h"

#include "ym_smtlibv2/SmtSortedVar.h"
#include "ym_smtlibv2/SmtVarBinding.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtIdMgr;
class SmtSortMgr;
class SmtFunMgr;
class StackPage;

//////////////////////////////////////////////////////////////////////
/// @class SmtMgr SmtMgr.h "ym_smtlibv2/SmtMgr.h"
/// @brief SmtLib に対する操作を行うクラス
//////////////////////////////////////////////////////////////////////
class SmtMgr
{
public:

  /// @brief コンストラクタ
  SmtMgr();

  /// @brief デストラクタ
  ~SmtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // コマンド処理関数
  //////////////////////////////////////////////////////////////////////

  /// @brief set-logic の処理を行う．
  /// @param[in] logic tSmtLogic ロジック
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - 以前に set_logic が実行されていた．
  bool
  set_logic(tSmtLogic logic);

  /// @brief オプションを設定する．
  /// @param[in] keyword キーワード
  /// @param[in] value 属性値
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  set_option(const ShString& keyword,
	     const SmtTerm* value);

  /// @brief set-info の処理を行う．
  /// @param[in] keyword キーワード
  /// @param[in] value 属性値
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  set_info(const ShString& keyword,
	   const SmtTerm* value);

  /// @brief sort の宣言を行う．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] param_num パラメータの数
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - 同名の型が別に宣言されている．
  bool
  declare_sort(const SmtId* name_id,
	       ymuint param_num);

  /// @brief sort の alias を定義する．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] sort_tmpl 型テンプレート
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - 同名の型が既に宣言されている．
  ///  - sort_tmpl のパラメータに不備がある．
  ///
  /// 型テンプレートは見かけは SmtSort だが，
  /// 要素の型として `パラメータ型' を一つ以上
  /// 含んでいる．
  /// sort_tmpl 中に現れるパラメータ型の番号
  /// に空きがある場合，例えば，2つの要素型を
  /// 持つ複合型の定義で2つの要素型が，
  /// それぞれパラメータ番号0 とパラメータ番号2
  /// だった場合，パラメータ番号1が抜けているので
  /// エラーとなる．
  bool
  define_sort(const SmtId* name_id,
	      const SmtSort* sort_tmpl);

  /// @brief 関数の宣言を行う．
  /// @param[in] name_id 関数名を表す識別子
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - 同名の関数が既に宣言されている．
  bool
  declare_fun(const SmtId* name_id,
	      const vector<const SmtSort*>& input_sort_list,
	      const SmtSort* output_sort);

  /// @brief 関数の定義を行う．
  /// @param[in] name_id 関数名を表す識別子
  /// @param[in] input_var_list 型つきの入力変数のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] body 本体の式
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - 同名の関数が既に定義されている．
  bool
  define_fun(const SmtId* name_id,
	     const vector<SmtSortedVar>& input_var_list,
	     const SmtSort* output_sort,
	     const SmtTerm* body);

  /// @brief assertion を追加する．
  /// @param[in] term 式
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// この関数は通常は成功するはず．
  bool
  assert(const SmtTerm* term);

  /// @brief assertion スタックにプッシュする．
  /// @param[in] num プッシュするレベル
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// この関数は通常は成功するはず．
  bool
  push(ymuint num);

  /// @brief assertion スタックからポップする．
  /// @param[in] num ポップするレベル．
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - num がスタックのサイズと等しいか大きかった．
  bool
  pop(ymuint num);

  /// @brief 充足可能性判定を行なう．
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  check_sat();

  /// @brief assertion を得る．
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_assertions();

  /// @brief 証明を得る．
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_proof();

  /// @brief unsat core を得る．
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_unsat_core();

  /// @brief 値を得る．
  /// @param[in] term_list 項のリスト
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_value(const vector<const SmtTerm*>& term_list);

  /// @brief 割り当てを得る．
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_assignment();

  /// @brief オプションを得る．
  /// @param[in] keyword キーワード
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_option(const ShString& keyword);

  /// @brief 情報を得る．
  /// @param[in] keyword キーワード
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  bool
  get_info(const ShString& keyword);


public:
  //////////////////////////////////////////////////////////////////////
  // データ構造を作る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 識別子を返す．
  /// @param[in] name 名前
  /// @param[in] index_list インデックスのリスト
  /// @return 同じ識別子があればそれを返す．なければ作る．
  ///
  /// この関数は通常は成功するはず．
  const SmtId*
  make_id(const ShString& name,
	  const vector<ymuint32>& index_list = vector<ymuint32>(0));

  /// @brief 型を返す．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] elem_list 要素のリスト
  /// @return 同じ型があればそれを返す．なければ作る．
  /// @note エラーの場合には NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の型が登録されていなかった．
  ///  - 登録されている型と elem_list のサイズが異なった．
  const SmtSort*
  make_sort(const SmtId* name_id,
	    const vector<const SmtSort*>& elem_list = vector<const SmtSort*>(0));

  /// @brief 型パラメータを作る．
  /// @param[in] pid パラメータ番号
  const SmtSort*
  make_param_sort(ymuint pid);

  /// @brief 関数を返す．
  /// @param[in] name_id 関数名
  /// @return 指定された名前の関数を返す．
  /// @note エラーの場合には NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の関数が登録されていなかった．
  const SmtFun*
  find_fun(const SmtId* name_id);

  /// @brief <numeric> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_numeric_term(ymuint32 val);

  /// @brief <decimal> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_decimal_term(const ShString& val);

  /// @brief <hexadecimal> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_hexadecimal_term(const ShString& val);

  /// @brief <binary> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_binary_term(const ShString& val);

  /// @brief <string> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_string_term(const ShString& val);

  /// @brief <symbol> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_symbol_term(const ShString& val);

  /// @brief <keyword> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_keyword_term(const ShString& val);

  /// @brief <identifier> 型の term を作る．
  /// @param[in] id 識別子
  const SmtTerm*
  make_identifier_term(const SmtId* id);

  /// @brief <qualified identifier> 型の term を作る．
  /// @param[in] id 識別子
  /// @param[in] sort 型
  const SmtTerm*
  make_qual_identifier_term(const SmtId* id,
			    const SmtSort* sort);

  /// @brief function term を作る．
  /// @param[in] function 関数
  /// @param[in] input_list 入力のリスト
  const SmtTerm*
  make_fun_term(const SmtFun* function,
		const vector<const SmtTerm*>& input_list);

  /// @brief let 文を作る．
  /// @param[in] var_binding 変数割り当てのリスト
  /// @param[in] body 本体
  const SmtTerm*
  make_let_term(const vector<SmtVarBinding>& var_binding,
		const SmtTerm* body);

  /// @brief forall 文を作る．
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  const SmtTerm*
  make_forall_term(const vector<SmtSortedVar>& var_list,
		   const SmtTerm* body);

  /// @brief exists 文を作る．
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  const SmtTerm*
  make_exists_term(const vector<SmtSortedVar>& var_list,
		   const SmtTerm* body);

  /// @brief attr 文を作る．
  /// @param[in] body 本体
  /// @param[in] attr_list 属性リスト
  const SmtTerm*
  make_attr_term(const SmtTerm* body,
		 const vector<SmtAttr>& attr_list);

  /// @brief list term を作る．
  /// @param[in] term_list 要素のリスト
  const SmtTerm*
  make_list_term(const vector<const SmtTerm*>& term_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Core theory の初期化を行う．
  void
  Core_init();

  /// @brief Ints theory の初期化を行う．
  void
  Ints_init();


private:
  //////////////////////////////////////////////////////////////////////
  // Sort/Fun を管理するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の SortMgr を返す．
  SmtSortMgr&
  sort_mgr();

  /// @brief 現在の FunMgr を返す．
  SmtFunMgr&
  fun_mgr();

  /// @brief 現在の assertion リストを返す．
  vector<const SmtTerm*>&
  assertion_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // SmtId を管理するクラス
  SmtIdMgr* mIdMgr;

  // logic
  tSmtLogic mLogic;

  // スタック
  vector<StackPage*> mStack;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLIBMGR_H
