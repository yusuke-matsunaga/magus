#ifndef SMTTERMMGR_H
#define SMTTERMMGR_H

/// @file SmtTermMgr.h
/// @brief SmtTermMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "YmUtils/ShString.h"
#include "YmUtils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtTermMgr SmtTermMgr.h "SmtTermMgr.h"
/// @brief SmtTerm に対する操作を行うクラス
//////////////////////////////////////////////////////////////////////
class SmtTermMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  SmtTermMgr(Alloc& alloc);

  /// @brief デストラクタ
  ~SmtTermMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief <numeral> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_numeral(ymuint32 val);

  /// @brief <decimal> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_decimal(const ShString& val);

  /// @brief <hexadecimal> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_hexadecimal(const ShString& val);

  /// @brief <binary> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_binary(const ShString& val);

  /// @brief <string> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_string(const ShString& val);

  /// @brief <symbol> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_symbol(const ShString& val);

  /// @brief <keyword> 型の term を作る．
  /// @param[in] val 値
  const SmtTerm*
  make_keyword(const ShString& val);

#if 0
  /// @brief <identifier> 型の term を作る．
  /// @param[in] id 識別子
  const SmtTerm*
  make_identifier(const SmtId* id);

  /// @brief <qualified identifier> 型の term を作る．
  /// @param[in] id 識別子
  /// @param[in] sort 型
  const SmtTerm*
  make_qual_identifier(const SmtId* id,
		       const SmtSort* sort);
#else
  /// @brief 変数型の term を作る．
  /// @param[in] var 変数
  const SmtTerm*
  make_var(const SmtVarFun* var);
#endif

  /// @brief function term を作る．
  /// @param[in] function 関数
  /// @param[in] input_list 入力のリスト
  /// @note input_list のサイズは 1 以上
  const SmtTerm*
  make_fun(const SmtVarFun* function,
	   const vector<const SmtTerm*>& input_list);

#if 0
  /// @brief let 文を作る．
  /// @param[in] var_binding 変数割り当てのリスト
  /// @param[in] body 本体
  /// @note var_binding のサイズは 1 以上
  const SmtTerm*
  make_let(const vector<SmtVarBinding>& var_binding,
	   const SmtTerm* body);
#endif

  /// @brief forall 文を作る．
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  /// @note var_list のサイズは 1 以上
  const SmtTerm*
  make_forall(const vector<const SmtVarFun*>& var_list,
	      const SmtTerm* body);

  /// @brief exists 文を作る．
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  /// @note var_list のサイズは 1 以上
  const SmtTerm*
  make_exists(const vector<const SmtVarFun*>& var_list,
	      const SmtTerm* body);

  /// @brief attr 文を作る．
  /// @param[in] body 本体
  /// @param[in] attr_list 属性リスト
  /// @note attr_list のサイズは 1 以上
  const SmtTerm*
  make_attr(const SmtTerm* body,
	    const vector<SmtAttr>& attr_list);

  /// @brief list term を作る．
  /// @param[in] term_list 要素のリスト
  const SmtTerm*
  make_list(const vector<const SmtTerm*>& term_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  Alloc& mAlloc;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTTERMMGR_H
