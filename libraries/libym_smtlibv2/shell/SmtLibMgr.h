#ifndef SMTLIBMGR_H
#define SMTLIBMGR_H

/// @file SmtLibMgr.h
/// @brief SmtLibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/SimpleAlloc.h"

#include "ym_smtlibv2/SmtSortedVar.h"
#include "ym_smtlibv2/SmtVarBinding.h"

#include "SmtIdMgr.h"
#include "SmtSortMgr.h"
#include "SmtFunMgr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibNode;

//////////////////////////////////////////////////////////////////////
/// @class SmtLibMgr SmtLibMgr.h "SmtLibMgr.h"
/// @brief SmtLib の S式の evaluation を行うクラス
//////////////////////////////////////////////////////////////////////
class SmtLibMgr
{
public:

  /// @brief コンストラクタ
  SmtLibMgr();

  /// @brief デストラクタ
  ~SmtLibMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief S式を数値に変換する．
  /// @param[in] node S式を表すノード
  ymint32
  eval_to_int(const SmtLibNode* node);

  /// @brief S式をシンボルに変換する．
  /// @param[in] node S式を表すノード
  ShString
  eval_to_symbol(const SmtLibNode* node);

  /// @brief S式をキーワードに変換する．
  /// @param[in] node S式を表すノード
  ShString
  eval_to_keyword(const SmtLibNode* node);

  /// @brief S式を識別子に変換する．
  /// @param[in] node S式を表すノード
  const SmtId*
  eval_to_id(const SmtLibNode* node);

  /// @brief S式を sort に変換する．
  /// @param[in] node S式を表すノード
  const SmtSort*
  eval_to_sort(const SmtLibNode* node);

  /// @brief S式を term に変換する．
  /// @param[in] node S式を表すノード
  const SmtTerm*
  eval_to_term(const SmtLibNode* node);

  /// @brief S式を s-expr に変換する．
  /// @param[in] node S式を表すノード
  const SmtTerm*
  eval_to_expr(const SmtLibNode* node);

  /// @brief S式を qual_identifier に変換する．
  /// @param[in] node S式を表すノード
  const SmtTerm*
  eval_to_qid(const SmtLibNode* node);

  /// @brief S式を sorted_var に変換する．
  /// @param[in] node S式を表すノード
  /// @param[out] sorted_var 結果を格納する変数
  /// @retval true 変換が成功した．
  /// @retval false 変換が失敗した．
  bool
  eval_to_sorted_var(const SmtLibNode* node,
		     SmtSortedVar& sorted_var);

  /// @brief S式を var_binding に変換する．
  /// @param[in] node S式を表すノード
  /// @param[out] var_binding 結果を格納する変数
  /// @retval true 変換が成功した．
  /// @retval false 変換が失敗した．
  bool
  eval_to_var_binding(const SmtLibNode* node,
		      SmtVarBinding& var_binding);

  /// @brief S式を attribute に変換する．
  /// @param[in] node S式を表すノード
  /// @param[out] attr_list 結果の attribute のリストを格納する変数
  bool
  eval_to_attr(const SmtLibNode* node,
	       vector<const SmtAttr*>& attr_list);

  /// @brief 識別子から関数に変換する．
  /// @param[in] name 関数名
  const SmtFun*
  find_fun(const SmtId* name);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief <numeric> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_numeric(ymint32 val);

  /// @brief <decimal> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_decimal(const ShString& val);

  /// @brief <hexadecimal> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_hexadecimal(const ShString& val);

  /// @brief <binary> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_binary(const ShString& val);

  /// @brief <string> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_string(const ShString& val);

  /// @brief <symbol> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_symbol(const ShString& val);

  /// @brief <keyword> を作る．
  /// @param[in] val 値
  const SmtTerm*
  new_keyword(const ShString& val);

  /// @brief identifier を作る．
  /// @param[in] id 識別子
  const SmtTerm*
  new_identifier(const SmtId* id);

  /// @brief qualified identifier を作る．
  /// @param[in] id 識別子
  /// @param[in] sort 型
  const SmtTerm*
  new_qual_identifier(const SmtId* id,
		      const SmtSort* sort);

  /// @brief term list を作る．
  /// @param[in] term_list を項のリスト
  const SmtTerm*
  new_term_list(const vector<const SmtTerm*>& term_list);

  /// @brief let 文を作る．
  /// @param[in] var_binding 変数割り当てのリスト
  /// @param[in] body 本体
  const SmtTerm*
  new_let(const vector<SmtVarBinding>& var_binding,
	  const SmtTerm* body);

  /// @brief forall 文を作る．
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  const SmtTerm*
  new_forall(const vector<SmtSortedVar>& var_list,
	     const SmtTerm* body);

  /// @brief exists 文を作る．
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  const SmtTerm*
  new_exists(const vector<SmtSortedVar>& var_list,
	     const SmtTerm* body);

  /// @brief attr 文を作る．
  /// @param[in] body 本体
  /// @param[in] attr_list 属性リスト
  const SmtTerm*
  new_attr_term(const SmtTerm* body,
		const vector<const SmtAttr*>& attr_list);

  /// @brief attribute を作る．
  /// @param[in] keyword キーワード
  /// @param[in] expr 値
  const SmtAttr*
  new_attr(const ShString& keyword,
	   const SmtTerm* expr = NULL);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  SmtIdMgr mIdMgr;

  SmtSortMgr mSortMgr;

  SmtFunMgr mFunMgr;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLIBMGR_H
