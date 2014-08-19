
/// @file print.cc
/// @brief SmtId, SmtSort, SmtFun, SmtTerm の出力関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtVarFun.h"
#include "ym_smtlibv2/SmtTerm.h"
#include "ym_smtlibv2/SmtAttr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtId
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
string
id_str(const SmtId* id)
{
  ostringstream buf;
  buf << id->name();
  for (ymuint i = 0; i < id->index_size(); ++ i) {
    buf << "[" << id->index(i) << "]";
  }
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス SmtSort
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
string
sort_str(const SmtSort* sort)
{
  ostringstream buf;
  if ( sort->is_param() ) {
    ymuint pid = sort->param_id();
    buf << "#" << pid;
  }
  else {
    buf << id_str(sort->name());
    if ( sort->elem_num() > 0 ) {
      buf << " (";
      for (ymuint i = 0; i < sort->elem_num(); ++ i) {
	buf << " " << sort_str(sort->elem(i));
      }
      buf << " )";
    }
  }
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス SmtVarFun
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
string
varfun_str(const SmtVarFun* obj)
{
  ostringstream buf;
  if ( obj->is_var() ) {
    const SmtSort* sort = obj->var_sort();
    if ( sort == NULL ) {
      buf << id_str(obj->name());
    }
    else {
      buf << "( as " << id_str(obj->name()) << " " << sort_str(sort) << " )";
    }
  }
  else {
    buf << id_str(obj->name()) << " (";
    for (ymuint i = 0; i < obj->input_num(); ++ i) {
      const SmtSort* sort = obj->input_sort(i);
      buf << " " << sort_str(sort);
    }
    buf << " ) "
	<< sort_str(obj->output_sort());

    if ( obj->fun_body() ) {
      buf << " (";
      for (ymuint i = 0; i < obj->input_num(); ++ i) {
	const SmtVarFun* var = obj->input_var(i);
	buf << " " << id_str(var->name());
      }
      buf << " ) "
	  << term_str(obj->fun_body());
    }
  }
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス SmtTerm
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
string
term_str(const SmtTerm* term)
{
  ostringstream buf;

  switch ( term->type() ) {
  case SmtTerm::kNumConst:
    buf << term->int_value();
    break;

  case SmtTerm::kDecConst:
  case SmtTerm::kHexConst:
  case SmtTerm::kBinConst:
  case SmtTerm::kSymbol:
  case SmtTerm::kKeyword:
    buf << term->str_value();
    break;

  case SmtTerm::kStrConst:
    buf << "\"" << term->str_value() << "\"";
    break;

#if 0
  case SmtTerm::kIdentifier:
    buf << id_str(term->identifier());
    break;

  case SmtTerm::kQualIdentifier:
    buf << "( as " << id_str(term->identifier())
	<< " " << sort_str(term->identifier_sort())
	<< " )";
    break;

#else
  case SmtTerm::kVarTerm:
    buf << varfun_str(term->var());
    break;
#endif

  case SmtTerm::kFunTerm:
    buf << "( " << id_str(term->function()->name());
    for (ymuint i = 0; i < term->input_num(); ++ i) {
      buf << " " << term_str(term->input(i));
    }
    buf << " )";
    break;

  case SmtTerm::kForall:
    buf << "( forall (";
    for (ymuint i = 0; i < term->var_num(); ++ i) {
      const SmtVarFun* var = term->bound_var(i);
      buf << " (" << id_str(var->name()) << " " << sort_str(var->var_sort()) << " )";
    }
    buf << " ) " << term_str(term->body());
    break;

  case SmtTerm::kExists:
    buf << "( exists (";
    for (ymuint i = 0; i < term->var_num(); ++ i) {
      const SmtVarFun* var = term->bound_var(i);
      buf << " (" << id_str(var->name()) << " " << sort_str(var->var_sort()) << " )";
    }
    buf << " ) " << term_str(term->body());
    break;

  case SmtTerm::kAttr:
    buf << "( ! " << term_str(term->body());
    for (ymuint i = 0; i < term->attr_num(); ++ i) {
      buf << " " << term->attr_keyword(i)
	  << " " << term->attr_value(i);
    }
    buf << " )";
    break;

  case SmtTerm::kList:
    buf << "(";
    for (ymuint i = 0; i < term->elem_num(); ++ i) {
      buf << " " << term_str(term->elem(i));
    }
    buf << " )";
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return buf.str();
}

END_NAMESPACE_YM_SMTLIBV2
