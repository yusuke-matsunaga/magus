
/// @file SmtLibMgr.cc
/// @brief SmtLibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibMgr.h"
#include "SmtLibNode.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibMgr::SmtLibMgr()
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
    name = node->value();
  }
  else if ( node->type() == kListToken ) {
    // 最初は '_'
    const SmtLibNode* node1 = node->child();
    if ( node1->type() != kSymbolToken ||
	 strcmp("_", static_cast<const char*>(node1->value())) != 0 ) {
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
    name = node2->value();

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
  case kNumToken:
  case kDecToken:
  case kHexToken:
  case kBinToken:
  case kStringToken:
    ;
  default:
    ;
  }

  const SmtTerm* term = eval_to_qid(node);
  if ( term != NULL ) {
    return term;
  }

  if ( node->type() != kListToken ) {
    // syntax error;
    goto syntax_error;
  }

  ymuint n = node->child_num();
  const SmtLibNode* node1 = node->child();
  const SmtTerm* qid = eval_to_qid(node1);
  if ( qid == NULL ) {
    if ( node1->type() != kSymbolToken ) {
      // syntax error
      goto syntax_error;
    }
    if ( strcmp("let", static_cast<const char*>(node1->value())) == 0 ) {
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
    }
    else if ( strcmp("forall", static_cast<const char*>(node1->value())) == 0 ) {
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
    }
    else if ( strcmp("exists", static_cast<const char*>(node1->value())) == 0 ) {
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
    }
    else if ( strcmp("!", static_cast<const char*>(node1->value())) == 0 ) {
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

      vector<SmtAttr*> attr_list;
      attr_list.reserve(node->child_num() - 2);
      for (const SmtLibNode* node3 = node2->sibling();
	   node3 != NULL; node3 = node3->sibling()) {
	SmtAttr* attr = eval_to_attr(node3);
	if ( attr == NULL ) {
	  // syntax error
	  goto syntax_error;
	}
	attr_list.push_back(attr);
      }
    }
  }
  else {
    vector<const SmtTerm*> term_list;
    term_list.reserve(n - 1);
    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtTerm* term1 = eval_to_term(node2);
      term_list.push_back(term1);
    }
  }

 syntax_error:
  return NULL;
}

// @brief S式を qual_identifier に変換する．
// @param[in] node S式を表すノード
const SmtTerm*
SmtLibMgr::eval_to_qid(const SmtLibNode* node)
{
  const SmtId* id = eval_to_id(node);
  if ( id != NULL ) {
  }

  if ( node->type() != kListToken ||
       node->child_num() != 3 ) {
    return NULL;
  }

  const SmtLibNode* node1 = node->child();
  if ( node1->type() != kSymbol ) {
    return NULL;
  }
  if ( strcmp("as", static_cast<const char*>(node1->value())) != 0 ) {
    return NULL;
  }

  const SmtLibNode* node2 = node1->sibling();
  const SmtId* id = eval_to_id(node2);
  if ( id == NULL ) {
    return NULL;
  }

  const SmtLibNode* node3 = node2->sibling();
  const SmtSort* sort = eval_to_sort(node3);
  if ( sort == NULL ) {
    return NULL;
  }

  //
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
const SmtAttr*
SmtLibMgr::eval_to_attr(const SmtLibNode* node)
{
  if ( node->type() != kKeywordToken ) {
    return NULL;
  }
  const SmtLibNode* node1 = node->sibling();
  if ( node1 != NULL ) {
    SmtTerm* expr = eval_to_expr(node1);
    if ( expr == NULL ) {
      return NULL;
    }
  }
}

// @brief 識別子から関数に変換する．
// @param[in] name 関数名
const SmtFun*
SmtLibMgr::find_fun(const SmtId* name)
{
  return mFunMgr.find_fun(name);
}

END_NAMESPACE_YM_SMTLIBV2
