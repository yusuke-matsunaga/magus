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
  const SmtAttr*
  eval_to_attr(const SmtLibNode* node);

  /// @brief 識別子から関数に変換する．
  /// @param[in] name 関数名
  const SmtFun*
  find_fun(const SmtId* name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SmtIdMgr mIdMgr;

  SmtSortMgr mSortMgr;

  SmtFunMgr mFunMgr;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLIBMGR_H
