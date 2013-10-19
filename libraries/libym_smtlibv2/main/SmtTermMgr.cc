
/// @file SmtTermMgr.cc
/// @brief SmtTermMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermMgr.h"

#include "SmtConstTerm.h"
#include "SmtIdTerm.h"
#include "SmtCompTerm.h"
#include "SmtFunTerm.h"
#include "SmtListTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtTermMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
SmtTermMgr::SmtTermMgr(Alloc& alloc) :
  mAlloc(alloc)
{
}

// @brief デストラクタ
SmtTermMgr::~SmtTermMgr()
{
}

// @brief <numeric> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_numeric(ymuint32 val)
{
  void* p = mAlloc.get_memory(sizeof(SmtNumTerm));
  return new (p) SmtNumTerm(val);
}

// @brief <decimal> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_decimal(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtDecTerm));
  return new (p) SmtDecTerm(val);
}

// @brief <hexadecimal> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_hexadecimal(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtHexTerm));
  return new (p) SmtHexTerm(val);
}

// @brief <binary> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_binary(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtBinTerm));
  return new (p) SmtBinTerm(val);
}

// @brief <string> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_string(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtStrTerm));
  return new (p) SmtStrTerm(val);
}

// @brief <symbol> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_symbol(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtSymbolTerm));
  return new (p) SmtSymbolTerm(val);
}

// @brief <keyword> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_keyword(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtKeywordTerm));
  return new (p) SmtKeywordTerm(val);
}

// @brief <identifier> 型の term を作る．
// @param[in] id 識別子
const SmtTerm*
SmtTermMgr::make_identifier(const SmtId* id)
{
  void* p = mAlloc.get_memory(sizeof(SmtIdTerm));
  return new (p) SmtIdTerm(id);
}

// @brief <qualified identifier> 型の term を作る．
// @param[in] id 識別子
// @param[in] sort 型
const SmtTerm*
SmtTermMgr::make_qual_identifier(const SmtId* id,
				 const SmtSort* sort)
{
  void* p = mAlloc.get_memory(sizeof(SmtQualIdTerm));
  return new (p) SmtQualIdTerm(id, sort);
}

// @brief function term を作る．
// @param[in] function 関数
// @param[in] input_list 入力のリスト
const SmtTerm*
SmtTermMgr::make_fun(const SmtFun* function,
		     const vector<const SmtTerm*>& input_list)
{
  ymuint n = input_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtFunTerm) + sizeof(const SmtTerm*) * ( n - 1));
    return new (p) SmtFunTerm(function, input_list);
  }
}

// @brief let 文を作る．
// @param[in] var_binding 変数割り当てのリスト
// @param[in] body 本体
const SmtTerm*
SmtTermMgr::make_let(const vector<SmtVarBinding>& var_binding,
		     const SmtTerm* body)
{
  ymuint n = var_binding.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtLet) + sizeof(SmtVarBinding) * (n - 1));
    return new (p) SmtLet(var_binding, body);
  }
}

// @brief forall 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtTermMgr::make_forall(const vector<SmtSortedVar>& var_list,
			const SmtTerm* body)
{
  ymuint n = var_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtForall) + sizeof(SmtSortedVar) * (n - 1));
    return new (p) SmtForall(var_list, body);
  }
}

// @brief exists 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtTermMgr::make_exists(const vector<SmtSortedVar>& var_list,
			const SmtTerm* body)
{
  ymuint n = var_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtExists) + sizeof(SmtSortedVar) * (n - 1));
    return new (p) SmtExists(var_list, body);
  }
}

// @brief attr 文を作る．
// @param[in] body 本体
// @param[in] attr_list 属性リスト
const SmtTerm*
SmtTermMgr::make_attr(const SmtTerm* body,
		      const vector<SmtAttr>& attr_list)
{
  ymuint n = attr_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtAttrTerm) + sizeof(SmtAttr) * (n - 1));
    return new (p) SmtAttrTerm(body, attr_list);
  }
}

// @brief list term を作る．
// @param[in] term_list 要素のリスト
const SmtTerm*
SmtTermMgr::make_list(const vector<const SmtTerm*>& term_list)
{
  ymuint n = term_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtListTerm) + sizeof(const SmtTerm*) * (n - 1));
    return new (p) SmtListTerm(term_list);
  }
}

END_NAMESPACE_YM_SMTLIBV2
