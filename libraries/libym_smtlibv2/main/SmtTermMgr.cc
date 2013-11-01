
/// @file SmtTermMgr.cc
/// @brief SmtTermMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermMgr.h"

#include "SmtCompTerm.h"
#include "SmtConstTerm.h"
#include "SmtFunTerm.h"
#include "SmtListTerm.h"
#include "SmtVarTerm.h"


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

// @brief <numeral> 型の term を作る．
// @param[in] val 値
const SmtTerm*
SmtTermMgr::make_numeral(ymuint32 val)
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

#if 0
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
#else
// @brief 変数型の term を作る．
// @param[in] var 変数
const SmtTerm*
SmtTermMgr::make_var(const SmtVarFun* var)
{
  void* p = mAlloc.get_memory(sizeof(SmtVarTerm));
  return new (p) SmtVarTerm(var);
}
#endif

// @brief function term を作る．
// @param[in] function 関数
// @param[in] input_list 入力のリスト
const SmtTerm*
SmtTermMgr::make_fun(const SmtVarFun* function,
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

// @brief forall 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtTermMgr::make_forall(const vector<const SmtVarFun*>& var_list,
			const SmtTerm* body)
{
  ymuint n = var_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtForall) + sizeof(const SmtVarFun*) * (n - 1));
    return new (p) SmtForall(var_list, body);
  }
}

// @brief exists 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtTermMgr::make_exists(const vector<const SmtVarFun*>& var_list,
			const SmtTerm* body)
{
  ymuint n = var_list.size();
  if ( n == 0 ) {
    return NULL;
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtExists) + sizeof(const SmtVarFun*) * (n - 1));
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
