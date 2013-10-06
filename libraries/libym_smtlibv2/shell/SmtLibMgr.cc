
/// @file SmtLibMgr.cc
/// @brief SmtLibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibMgr.h"
#include "SmtLibNode.h"

#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtFun.h"
#include "SmtConstTerm.h"
#include "SmtIdTerm.h"
#include "SmtCompTerm.h"
#include "SmtFunTerm.h"
#include "SmtListTerm.h"
#include "SmtAttrImpl.h"


BEGIN_NONAMESPACE

const bool debug = true;

inline
const char*
bool_str(bool val)
{
  if ( val ) {
    return "true";
  }
  else {
    return "false";
  }
}

inline
const char*
stat_str(bool val)
{
  if ( val ) {
    return "success";
  }
  else {
    return "error";
  }
}

END_NONAMESPACE

BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibMgr::SmtLibMgr() :
  mAlloc(4096),
  mIdMgr(mAlloc),
  mSortMgr(mAlloc),
  mFunMgr(mAlloc)
{
}

// @brief デストラクタ
SmtLibMgr::~SmtLibMgr()
{
}

// @brief set-logic の処理を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::set_logic(const SmtLibNode* arg_top)
{
  // このコマンドは1度しか使えない．
  if ( mLogic != kSmtLogic_NONE ) {
    if ( debug ) {
      cerr << "set-logic failed: already set" << endl;
    }
    return false;
  }

  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ||
       arg_top->type() != kSymbolToken ) {
    if ( debug ) {
      cerr << "set-logic failed: syntax error" << endl;
    }
    return false;
  }

  const char* str = arg_top->str_value();
  if ( strcmp(str, "AUFLIA") == 0 ) {
    mLogic = kSmtLogic_AUFLIA;

    core_init();
  }
  else if ( strcmp(str, "AUFLIRA") == 0 ) {
    mLogic = kSmtLogic_AUFLIRA;

    core_init();
  }
  else if ( strcmp(str, "AUFNIRA") == 0 ) {
    mLogic = kSmtLogic_AUFNIRA;

    core_init();
  }
  else if ( strcmp(str, "LRA") == 0 ) {
    mLogic = kSmtLogic_LRA;

    core_init();
  }
  else if ( strcmp(str, "QF_ABV") == 0 ) {
    mLogic = kSmtLogic_QF_ABV;

    core_init();
  }
  else if ( strcmp(str, "QF_AUFBV") == 0 ) {
    mLogic = kSmtLogic_QF_AUFBV;

    core_init();
  }
  else if ( strcmp(str, "QF_AUFLIA") == 0 ) {
    mLogic = kSmtLogic_QF_AUFLIA;

    core_init();
  }
  else if ( strcmp(str, "QF_AX") == 0 ) {
    mLogic = kSmtLogic_QF_AX;

    core_init();
  }
  else if ( strcmp(str, "QF_BV") == 0 ) {
    mLogic = kSmtLogic_QF_BV;

    core_init();
  }
  else if ( strcmp(str, "QF_IDL") == 0 ) {
    mLogic = kSmtLogic_QF_IDL;

    core_init();
  }
  else if ( strcmp(str, "QF_LIA") == 0 ) {
    mLogic = kSmtLogic_QF_LIA;

    core_init();
  }
  else if ( strcmp(str, "QF_LRA") == 0 ) {
    mLogic = kSmtLogic_QF_LRA;

    core_init();
  }
  else if ( strcmp(str, "QF_NIA") == 0 ) {
    mLogic = kSmtLogic_QF_NIA;

    core_init();
  }
  else if ( strcmp(str, "QF_NRA") == 0 ) {
    mLogic = kSmtLogic_QF_NRA;

    core_init();
  }
  else if ( strcmp(str, "QF_RDL") == 0 ) {
    mLogic = kSmtLogic_QF_RDL;

    core_init();
  }
  else if ( strcmp(str, "QF_UF") == 0 ) {
    mLogic = kSmtLogic_QF_UF;

    core_init();
  }
  else if ( strcmp(str, "QF_UFBF") == 0 ) {
    mLogic = kSmtLogic_QF_UFBV;

    core_init();
  }
  else if ( strcmp(str, "QF_UFIDL") == 0 ) {
    mLogic = kSmtLogic_QF_UFIDL;

    core_init();
  }
  else if ( strcmp(str, "QF_UFLIA") == 0 ) {
    mLogic = kSmtLogic_QF_UFLIA;

    core_init();
  }
  else if ( strcmp(str, "QF_UFLRA") == 0 ) {
    mLogic = kSmtLogic_QF_UFLRA;

    core_init();
  }
  else if ( strcmp(str, "QF_UFNRA") == 0 ) {
    mLogic = kSmtLogic_QF_UFNRA;

    core_init();
  }
  else if ( strcmp(str, "UFLRA") == 0 ) {
    mLogic = kSmtLogic_UFLRA;

    core_init();
  }
  else if ( strcmp(str, "UFNIA") == 0 ) {
    mLogic = kSmtLogic_UFNIA;

    core_init();
  }
  else {
    if ( debug ) {
      cerr << "set-logic failed: unknown logic: " << str << endl;
    }
    return false;
  }

  return true;
}

// @brief Core theory の初期化を行う．
void
SmtLibMgr::core_init()
{
  // (declare-sort Bool 0)
  const SmtId* bool_id = mIdMgr.new_id(ShString("Bool"));
  assert_cond( bool_id != NULL, __FILE__, __LINE__);
  mSortMgr.reg_sort(bool_id, 0);

  const SmtSort* bool_sort = mSortMgr.new_sort(bool_id, vector<const SmtSort*>(0));
  assert_cond( bool_sort != NULL, __FILE__, __LINE__);

  // (declare-fun true () Bool)
  const SmtId* true_id = mIdMgr.new_id(ShString("true"));
  assert_cond( true_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(true_id, vector<const SmtSort*>(0), bool_sort);

  // (declare-fun false () Bool)
  const SmtId* false_id = mIdMgr.new_id(ShString("false"));
  assert_cond( false_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(false_id, vector<const SmtSort*>(0), bool_sort);

  vector<const SmtSort*> b1_list(1);
  b1_list[0] = bool_sort;

  // (declare-fun not (Bool) Bool)
  const SmtId* not_id = mIdMgr.new_id(ShString("not"));
  assert_cond( not_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(not_id, b1_list, bool_sort);

  vector<const SmtSort*> b2_list(2);
  b2_list[0] = bool_sort;
  b2_list[1] = bool_sort;

  // (declare-fun and (Bool Bool) Bool)
  const SmtId* and_id = mIdMgr.new_id(ShString("and"));
  assert_cond( and_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(and_id, b2_list, bool_sort);

  // (declare-fun or (Bool Bool) Bool)
  const SmtId* or_id = mIdMgr.new_id(ShString("or"));
  assert_cond( or_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(or_id, b2_list, bool_sort);

  // (declare-fun xor (Bool Bool) Bool)
  const SmtId* xor_id = mIdMgr.new_id(ShString("xor"));
  assert_cond( xor_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(xor_id, b2_list, bool_sort);

  // (declare-fun => (Bool Bool) Bool)
  const SmtId* imp_id = mIdMgr.new_id(ShString("=>"));
  assert_cond( imp_id != NULL, __FILE__, __LINE__);
  mFunMgr.reg_fun(imp_id, b2_list, bool_sort);
}

// @brief sort の宣言を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::declare_sort(const SmtLibNode* arg_top)
{
  // このコマンドは2つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  ymuint num = 0;
  if ( !parse_args(arg_top, 2, arg_list) ) {
    goto syntax_error;
  }

  // 1つめは型名
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    goto syntax_error;
  }

  // 2つめは引数の数
  if ( arg_list[1]->type() != kNumToken ) {
    goto syntax_error;
  }
  num = arg_list[1]->int_value();

  // 型を登録する．
  {
    bool stat = mSortMgr.reg_sort(name, num);

    if ( debug ) {
      cerr << "declare-sort " << id_str(name) << " " << num << endl
	   << "  ==> " << stat_str(stat) << endl;
    }

    return stat;
  }

 syntax_error:
  return false;
}

// @brief sort の alias を定義する．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::define_sort(const SmtLibNode* arg_top)
{
  // このコマンドは3つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  vector<const SmtId*> param_list;
  const SmtSort* sort = NULL;
  if ( !parse_args(arg_top, 3, arg_list) ) {
    goto syntax_error;
  }

  // 1つめは型名
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    goto syntax_error;
  }

  // 2つめはパラメータリスト
  if ( arg_list[1]->type() != kListToken ) {
    goto syntax_error;
  }
  param_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node1 = arg_list[1]->child();
       node1 != NULL; node1 = node1->sibling()) {
    const SmtId* id = eval_to_id(node1);
    if ( id == NULL ) {
      goto syntax_error;
    }
    param_list.push_back(id);
  }

  // 3つめは型テンプレート
  sort = eval_to_sort_template(arg_list[2], param_list);

  // 型テンプレートを登録する．
  {
    bool stat = mSortMgr.reg_alias(name, param_list.size(), sort);

    if ( debug ) {
      cerr << "define-sort " << id_str(name) << " (";
      for (vector<const SmtId*>::const_iterator p = param_list.begin();
	   p != param_list.end(); ++ p) {
	const SmtId* id = *p;
	cerr << " " << id_str(id);
      }
      cerr << " ) " << sort_str(sort) << endl
	   << "  ==> " << stat_str(stat) << endl;
    }
    return stat;
  }

 syntax_error:
  return false;
}

// @brief 関数の宣言を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::declare_fun(const SmtLibNode* arg_top)
{
  // このコマンドは3つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  vector<const SmtSort*> input_sort_list;
  const SmtSort* output_sort = NULL;

  if ( !parse_args(arg_top, 3, arg_list) ) {
    goto syntax_error;
  }

  // 1つめは名前
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    goto syntax_error;
  }

  // 2つめは入力の型のリスト
  if ( arg_list[1]->type() != kListToken ) {
    goto syntax_error;
  }
  input_sort_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    const SmtSort* sort = eval_to_sort(node);
    if ( sort == NULL ) {
      goto syntax_error;
    }
    input_sort_list.push_back(sort);
  }

  // 3つめは出力の型のリスト
  output_sort = eval_to_sort(arg_list[2]);
  if ( output_sort == NULL ) {
    goto syntax_error;
  }

  // 関数を登録する．
  {
    bool stat = mFunMgr.reg_fun(name, input_sort_list, output_sort);

    if ( debug ) {
      cerr << "declare-fun " << id_str(name) << " (";
      for (vector<const SmtSort*>::const_iterator p = input_sort_list.begin();
	   p != input_sort_list.end(); ++ p) {
	const SmtSort* sort = *p;
	cerr << " " << sort_str(sort);
      }
      cerr << " ) " << sort_str(output_sort) << endl
	   << "  ==> " << stat_str(stat) << endl;
    }

    return stat;
  }

 syntax_error:
  return false;
}

// @brief 関数の定義を行う．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::define_fun(const SmtLibNode* arg_top)
{
  // このコマンドは4つの引数をとる．
  vector<const SmtLibNode*> arg_list;
  const SmtId* name = NULL;
  vector<SmtSortedVar> var_list;
  const SmtSort* output_sort = NULL;
  const SmtTerm* body = NULL;

  if ( !parse_args(arg_top, 4, arg_list) ) {
    goto syntax_error;
  }

  // 1つめは名前
  name = eval_to_id(arg_list[0]);
  if ( name == NULL ) {
    goto syntax_error;
  }

  // 2つめは変数のリスト
  if ( arg_list[1]->type() != kListToken ) {
    goto syntax_error;
  }
  var_list.reserve(arg_list[1]->child_num());
  for (const SmtLibNode* node = arg_list[1]->child();
       node != NULL; node = node->sibling()) {
    SmtSortedVar sorted_var;
    if ( !eval_to_sorted_var(node, sorted_var) ) {
      goto syntax_error;
    }
    var_list.push_back(sorted_var);
  }

  // 3つめは出力の型
  output_sort = eval_to_sort(arg_list[2]);

  // 4つめは本体の式
  body = eval_to_term(arg_list[3]);

  // 関数を登録する．
  {
    bool stat = mFunMgr.reg_fun(name, var_list, output_sort, body);

    if ( debug ) {
      cerr << "define-fun " << id_str(name) << " (";
      for (vector<SmtSortedVar>::const_iterator p = var_list.begin();
	   p != var_list.end(); ++ p) {
	const SmtSortedVar& sv = *p;
	cerr << " (" << id_str(sv.mVar)
	     << " " << sort_str(sv.mSort)
	     << " )";
      }
      cerr << " ) "
	   << sort_str(output_sort)
	   << " " << term_str(body)
	   << endl
	   << "  ==> " << stat_str(stat) << endl;
    }

    return stat;
  }

 syntax_error:
  return false;
}

// @brief assertion を追加する．
// @param[in] arg_top 引数の先頭ノード
bool
SmtLibMgr::assert(const SmtLibNode* arg_top)
{
  // このコマンドは1つの引数をとる．
  if ( arg_top == NULL || arg_top->sibling() != NULL ) {
    return false;
  }

  const SmtTerm* term = eval_to_term(arg_top);
  if ( term == NULL ) {
    if ( debug ) {
      cerr << " syntax error in assert" << endl;
    }
    return false;
  }

  if ( debug ) {
    cerr << "assert " << term_str(term) << endl;
  }

  return true;
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

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
SmtLibMgr::eval_to_sort_template(const SmtLibNode* node,
				 const vector<const SmtId*>& param_list)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  if ( id != NULL ) {
    ymuint n = param_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( id == param_list[i] ) {
	return mSortMgr.new_sort_param(i);
      }
    }
  }
  else {
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
      const SmtSort* sort1 = eval_to_sort_template(node2, param_list);
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
    const SmtId* fid = eval_to_id(node1);
    if ( fid == NULL ) {
      if ( debug ) {
	cerr << "eval_to_term failed: undefined function: "
	     << id_str(fid) << endl;
      }
      return NULL;
    }
    const SmtFun* fun = find_fun(fid);
    if ( fun != NULL ) {
      if ( fun->input_num() != n - 1) {
	// 引数の数が合わない．
	if ( debug ) {
	  cerr << "eval_to_term failed: # of args mismatch: " << endl;
	  display(cerr, node, 0);
	}
	return NULL;
      }
      vector<const SmtTerm*> input_list;
      input_list.reserve(n - 1);
      for (const SmtLibNode* node2 = node1->sibling();
	   node2 != NULL; node2 = node2->sibling()) {
	const SmtTerm* term1 = eval_to_term(node2);
	input_list.push_back(term1);
      }
      return new_fun_term(fun, input_list);
    }
    else if ( node1->type() != kSymbolToken ||
	      strcmp("let", static_cast<const char*>(node1->str_value())) == 0 ) {
      if ( node->child_num() != 3 ) {
	// syntax error
	cerr << "eval_to_term failed: # of args mismatch: 3 expected: "
	     << node->child_num() << endl;
	goto syntax_error;
      }
      const SmtLibNode* node2 = node1->sibling();
      if ( node2->type() != kListToken ) {
	// syntax error
	cerr << "eval_to_term failed: LIST expected" << endl;
	goto syntax_error;
      }

      vector<SmtVarBinding> var_binding_list;
      var_binding_list.reserve(node2->child_num());
      for (const SmtLibNode* node21 = node2->child();
	   node21 != NULL; node21 = node21->sibling()) {
	SmtVarBinding var_binding;
	if ( !eval_to_var_binding(node21, var_binding) ) {
	  // syntax error
	  cerr << "eval_to_var_binding failed:" << endl;
	  display(cerr, node21, 0);
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
      cerr << "eval_to_term failed: " << node1->str_value()
	   << " : undefined function" << endl;
      goto syntax_error;
    }
  }

 syntax_error:
  cerr << "eval_to_term failed: syntax error" << endl;
  display(cerr, node, 0);
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
  return new_list_term(expr_list);
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

// @brief 引数のリストをパーズする．
// @param[in] arg_top 引数の先頭のノード
// @param[in] arg_num 引数の数
// @param[out] arg_list 引数を格納するリスト
// @retval true 引数の数が arg_num と一致した．
// @retval false 引数の数が arg_num と一致しなかった．
bool
SmtLibMgr::parse_args(const SmtLibNode*  arg_top,
		      ymuint arg_num,
		      vector<const SmtLibNode*>& arg_list)
{
  arg_list.clear();
  arg_list.reserve(arg_num);
  const SmtLibNode* node = arg_top;
  for (ymuint i = 0; i < arg_num; ++ i) {
    if ( node == NULL ) {
      return false;
    }
    arg_list.push_back(node);
    node = node->sibling();
  }
  if ( node != NULL ) {
    return false;
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

// @brief function term を作る．
// @param[in] function 関数
// @param[in] input_list 入力のリスト
const SmtTerm*
SmtLibMgr::new_fun_term(const SmtFun* function,
			const vector<const SmtTerm*>& input_list)
{
  ymuint n = input_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtFunTerm) + sizeof(const SmtTerm*) * ( n - 1));
  return new (p) SmtFunTerm(function, input_list);
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

// @brief term list を作る．
// @param[in] term_list 要素のリスト
const SmtTerm*
SmtLibMgr::new_list_term(const vector<const SmtTerm*>& term_list)
{
  ymuint n = term_list.size();
  void* p = mAlloc.get_memory(sizeof(SmtListTerm) + sizeof(const SmtTerm*) * (n - 1));
  return new (p) SmtListTerm(term_list);
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
