
/// @file SmtLibMgr.cc
/// @brief SmtLibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibMgr.h"
#include "SmtLibNode.h"

#include "SmtConstTerm.h"
#include "SmtIdTerm.h"
#include "SmtCompTerm.h"
#include "SmtTermList.h"
#include "SmtAttrImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibMgr::SmtLibMgr() :
  mAlloc(4096)
{
}

// @brief デストラクタ
SmtLibMgr::~SmtLibMgr()
{
}

// @brief S式を識別子に変換する．
// @param[in] node S式を表すノード
const SmtId*
SmtLibMgr::eval_to_id(const SmtLibNode* node)
{
  ShString name;
  vector<ymint32> index_list;
  if ( node->type() == kSymbolToken ) {
    // 単純な形式の場合
    name = node->str_value();
  }
  else if ( node->type() == kListToken ) {
    // 最初は '_'
    const SmtLibNode* node1 = node->child();
    if ( node1->type() != kSymbolToken ||
	 strcmp("_", static_cast<const char*>(node1->str_value())) != 0 ) {
      // syntax error
      goto syntax_error;
    }

    // 2番めが名前
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 == NULL ||
	 node2->type() != kSymbolToken ) {
      // syntax error
      goto syntax_error;
    }
    name = node2->str_value();

    // 3番め以降にインデックス
    index_list.reserve(node->child_num() - 2);
    for (const SmtLibNode* node3 = node2->sibling();
	 node3 != NULL; node3 = node3->sibling()) {
      if ( node3->type() != kNumToken ) {
	// syntax error
	goto syntax_error;
      }
      index_list.push_back(node3->int_value());
    }
    if ( index_list.empty() ) {
      // syntax error
      goto syntax_error;
    }
  }

  return mIdMgr.new_id(name, index_list);

 syntax_error:
  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
SmtLibMgr::eval_to_sort(const SmtLibNode* node)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  if ( id == NULL ) {
    if ( node->type() != kListToken ) {
      // syntax error
      goto syntax_error;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_to_id(node1);
    if ( id == NULL ) {
      // syntax error
      goto syntax_error;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtSort* sort1 = eval_to_sort(node2);
      if ( sort1 == NULL ) {
	// syntax error
	goto syntax_error;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      // syntax error
      goto syntax_error;
    }
  }

  return mSortMgr.new_sort(id, elem_list);

 syntax_error:
  return NULL;
}

// @brief S式を term に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_term(const SmtLibNode* node)
{
  switch ( node->type() ) {
  case kNumToken: return new_numeric(node->int_value());
  case kDecToken: return new_decimal(node->str_value());
  case kHexToken: return new_hexadecimal(node->str_value());
  case kBinToken: return new_binary(node->str_value());
  case kStringToken: return new_string(node->str_value());
  default: ;
  }

  const SmtTerm* term = eval_to_qid(node);
  if ( term != NULL ) {
    return term;
  }

  if ( node->type() == kListToken ) {
    ymuint n = node->child_num();
    const SmtLibNode* node1 = node->child();
    const SmtTerm* qid = eval_to_qid(node1);
    if ( qid != NULL ) {
      vector<const SmtTerm*> term_list;
      term_list.reserve(n);
      term_list.push_back(qid);
      for (const SmtLibNode* node2 = node1->sibling();
	   node2 != NULL; node2 = node2->sibling()) {
	const SmtTerm* term1 = eval_to_term(node2);
	term_list.push_back(term1);
      }
      return new_term_list(term_list);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("let", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	// syntax error
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	// syntax error
	goto syntax_error;
      }

      vector<SmtVarBinding> var_binding_list;
      var_binding_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtVarBinding var_binding;
	if ( !eval_to_var_binding(node21, var_binding) ) {
	  // syntax error
	  goto syntax_error;
	}
	var_binding_list.push_back(var_binding);
      }

      const SmtLibNode* node3 = node2->sibling();
      const SmtTerm* term3 = eval_to_term(node3);
      if ( term3 == NULL ) {
	// syntax error
	goto syntax_error;
      }
      return new_let(var_binding_list, term3);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("forall", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	// syntax error
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	// syntax error
	goto syntax_error;
      }

      vector<SmtSortedVar> sorted_var_list;
      sorted_var_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtSortedVar sorted_var;
	if ( !eval_to_sorted_var(node21, sorted_var) ) {
	  // syntax error
	  goto syntax_error;
	}
	sorted_var_list.push_back(sorted_var);
      }

      const SmtLibNode* node3 = node2->sibling();
      const SmtTerm* term3 = eval_to_term(node3);
      if ( term3 == NULL ) {
	// syntax error
	goto syntax_error;
      }
      return new_forall(sorted_var_list, term3);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("exists", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	// syntax error
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	// syntax error
	goto syntax_error;
      }

      vector<SmtSortedVar> sorted_var_list;
      sorted_var_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtSortedVar sorted_var;
	if ( !eval_to_sorted_var(node21, sorted_var) ) {
	  // syntax error
	  goto syntax_error;
	}
	sorted_var_list.push_back(sorted_var);
      }

      const SmtLibNode* node3 = node2->sibling();
      const SmtTerm* term3 = eval_to_term(node3);
      if ( term3 == NULL ) {
	// syntax error
	goto syntax_error;
      }
      return new_exists(sorted_var_list, term3);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("!", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() < 3 ) {
	// syntax error
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      const SmtTerm* term = eval_to_term(node2);
      if ( term == NULL ) {
	// syntax error
	goto syntax_error;
      }

      vector<const SmtAttr*> attr_list;
      attr_list.reserve(node->child_num() - 2);
      if ( !eval_to_attr(node2->sibling(), attr_list) ) {
	// syntax error
	goto syntax_error;
      }
      return new_attr_term(term, attr_list);
    }
    else {
      // syntax error
      goto syntax_error;
    }
  }

 syntax_error:
  return NULL;
}

// @brief S式を s-expr に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_expr(const SmtLibNode* node)
{
  switch ( node->type() ) {
  case kNumToken: return new_numeric(node->int_value());
  case kDecToken: return new_decimal(node->str_value());
  case kHexToken: return new_hexadecimal(node->str_value());
  case kBinToken: return new_binary(node->str_value());
  case kStringToken: return new_string(node->str_value());
  case kSymbolToken: return new_symbol(node->str_value());
  case kKeywordToken: return new_keyword(node->str_value());
  default: ;
  }

  if ( node->type() != kListToken ) {
    return NULL;
  }

  vector<const SmtTerm*> expr_list;
  expr_list.reserve(node->child_num());
  for (const SmtLibNode* node1 = node->child();
       node1 != NULL; node1 = node1->sibling()) {
    const SmtTerm* expr1 = eval_to_expr(node1);
    expr_list.push_back(expr1);
  }
  return new_term_list(expr_list);
}

// @brief S式を qual_identifier に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_qid(const SmtLibNode* node)
{
  const SmtId* id = eval_to_id(node);
  if ( id != NULL ) {
    return new_identifier(id);
  }

  if ( node->type() != kListToken ||
       node->child_num() != 3 ) {
    return NULL;
  }

  const SmtLibNode* node1 = node->child();
  if ( node1->type() != kSymbolToken ) {
    return NULL;
  }
  if ( strcmp("as", static_cast<const char*>(node1->str_value())) != 0 ) {
    return NULL;
  }

  const SmtLibNode* node2 = node1->sibling();
  id = eval_to_id(node2);
  if ( id == NULL ) {
    return NULL;
  }

  const SmtLibNode* node3 = node2->sibling();
  const SmtSort* sort = eval_to_sort(node3);
  if ( sort == NULL ) {
    return NULL;
  }

  return new_qual_identifier(id, sort);
}

// @brief S式を sorted_var に変換する．
// @param[in] node S式を表すノード
// @param[out] sorted_var 結果を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
bool
SmtLibMgr::eval_to_sorted_var(const SmtLibNode* node,
			      SmtSortedVar& sorted_var)
{
  if ( node->type() != kListToken ||
       node->child_num() != 2 ) {
    return false;
  }

  const SmtLibNode* node1 = node->child();
  const SmtId* id = eval_to_id(node1);
  if ( id == NULL ) {
    return false;
  }

  const SmtLibNode* node2 = node1->sibling();
  const SmtSort* sort = eval_to_sort(node2);
  if ( sort == NULL ) {
    return false;
  }

  sorted_var.mVar = id;
  sorted_var.mSort = sort;

  return true;
}

// @brief S式を var_binding に変換する．
// @param[in] node S式を表すノード
// @param[out] var_binding 結果を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．
bool
SmtLibMgr::eval_to_var_binding(const SmtLibNode* node,
			       SmtVarBinding& var_binding)
{
  if ( node->type() != kListToken ||
       node->child_num() != 2 ) {
    return false;
  }

  const SmtLibNode* node1 = node->child();
  const SmtId* id = eval_to_id(node1);
  if ( id == NULL ) {
    return false;
  }

  const SmtLibNode* node2 = node1->sibling();
  const SmtTerm* term = eval_to_term(node2);
  if ( term == NULL ) {
    return false;
  }

  var_binding.mVar = id;
  var_binding.mExpr = term;

  return true;
}

// @brief S式を attribute に変換する．
// @param[in] node S式を表すノード
// @param[out] attr_list 結果の attribute のリストを格納する変数
bool
SmtLibMgr::eval_to_attr(const SmtLibNode* node,
			vector<const SmtAttr*>& attr_list)
{
  attr_list.clear();
  for (const SmtLibNode* node1 = node; node1 != NULL; node1 = node1->sibling()) {
    if ( node1->type() != kKeywordToken ) {
      return false;
    }
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 != NULL && node2->type() != kKeywordToken ) {
      const SmtTerm* expr = eval_to_expr(node2);
      if ( expr == NULL ) {
	return false;
      }
      const SmtAttr* attr = new_attr(node1->str_value(), expr);
      attr_list.push_back(attr);

      node1 = node2;
    }
    else {
      const SmtAttr* attr = new_attr(node1->str_value());
      attr_list.push_back(attr);
    }
  }
  return true;
}

// @brief <numeric> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_numeric(ymint32 val)
{
  void* p = mAlloc.get_memory(sizeof(SmtNumTerm));
  return new (p) SmtNumTerm(val);
}

// @brief <decimal> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_decimal(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtDecTerm));
  return new (p) SmtDecTerm(val);
}

// @brief <hexadecimal> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_hexadecimal(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtHexTerm));
  return new (p) SmtHexTerm(val);
}

// @brief <binary> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_binary(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtBinTerm));
  return new (p) SmtBinTerm(val);
}

// @brief <string> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_string(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtStrTerm));
  return new (p) SmtStrTerm(val);
}

// @brief <symbol> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_symbol(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtSymbolTerm));
  return new (p) SmtSymbolTerm(val);
}

// @brief <keyword> を作る．
// @param[in] val 値
const SmtTerm*
SmtLibMgr::new_keyword(const ShString& val)
{
  void* p = mAlloc.get_memory(sizeof(SmtKeywordTerm));
  return new (p) SmtKeywordTerm(val);
}

// @brief identifier を作る．
// @param[in] id 識別子
const SmtTerm*
SmtLibMgr::new_identifier(const SmtId* id)
{
  void* p = mAlloc.get_memory(sizeof(SmtIdTerm));
  return new (p) SmtIdTerm(id);
}

// @brief qualified identifier を作る．
// @param[in] id 識別子
// @param[in] sort 型
const SmtTerm*
SmtLibMgr::new_qual_identifier(const SmtId* id,
			       const SmtSort* sort)
{
  void* p = mAlloc.get_memory(sizeof(SmtQualIdTerm));
  return new (p) SmtQualIdTerm(id, sort);
}

// @brief term list を作る．
// @param[in] term_list を項のリスト
const SmtTerm*
SmtLibMgr::new_term_list(const vector<const SmtTerm*>& term_list)
{
  ymuint n = term_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtTermList) + sizeof(const SmtTerm*) * (n - 1));
  return new (p) SmtTermList(term_list);
}

// @brief let 文を作る．
// @param[in] var_binding 変数割り当てのリスト
// @param[in] body 本体
const SmtTerm*
SmtLibMgr::new_let(const vector<SmtVarBinding>& var_binding,
		   const SmtTerm* body)
{
  ymuint n = var_binding.size();
  void* p = mAlloc.get_memory(sizeof(SmtLet) + sizeof(SmtVarBinding) * (n - 1));
  return new (p) SmtLet(var_binding, body);
}

// @brief forall 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtLibMgr::new_forall(const vector<SmtSortedVar>& var_list,
		      const SmtTerm* body)
{
  ymuint n = var_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtForall) + sizeof(SmtSortedVar) * (n - 1));
  return new (p) SmtForall(var_list, body);
}

// @brief exists 文を作る．
// @param[in] var_list 変数リスト
// @param[in] body 本体
const SmtTerm*
SmtLibMgr::new_exists(const vector<SmtSortedVar>& var_list,
		      const SmtTerm* body)
{
  ymuint n = var_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtExists) + sizeof(SmtSortedVar) * (n - 1));
  return new (p) SmtExists(var_list, body);
}

// @brief attr 文を作る．
// @param[in] body 本体
// @param[in] attr_list 属性リスト
const SmtTerm*
SmtLibMgr::new_attr_term(const SmtTerm* body,
			 const vector<const SmtAttr*>& attr_list)
{
  ymuint n = attr_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtAttrTerm) + sizeof(const SmtAttr*) * (n - 1));
  return new (p) SmtAttrTerm(body, attr_list);
}

// @brief attribute を作る．
// @param[in] keyword キーワード
// @param[in] expr 値
const SmtAttr*
SmtLibMgr::new_attr(const ShString& keyword,
		    const SmtTerm* expr)
{
  SmtAttr* attr = NULL;
  if ( expr == NULL ) {
    void* p = mAlloc.get_memory(sizeof(SmtAttrImpl));
    attr = new (p) SmtAttrImpl(keyword);
  }
  else {
    void* p = mAlloc.get_memory(sizeof(SmtAttrImpl2));
    attr = new (p) SmtAttrImpl2(keyword, expr);
  }
  return attr;
}

// @brief 識別子から関数に変換する．
// @param[in] name 関数名
const SmtFun*
SmtLibMgr::find_fun(const SmtId* name)
{
  return mFunMgr.find_fun(name);
}

END_NAMESPACE_YM_SMTLIBV2
