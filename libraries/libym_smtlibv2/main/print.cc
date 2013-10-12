
/// @file print.cc
/// @brief SmtId, SmtSort, SmtFun, SmtTerm の出力関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtId.h"
#include "ym_smtlibv2/SmtSort.h"
#include "ym_smtlibv2/SmtFun.h"
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
  ymint pid = sort->param_id();
  if ( pid >= 0 ) {
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
// クラス SmtFun
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
string
fun_str(const SmtFun* fun)
{
  ostringstream buf;
  buf << id_str(fun->name()) << " (";
  for (ymuint i = 0; i < fun->input_num(); ++ i) {
    const SmtSort* sort = fun->input_sort(i);
    buf << " " << sort_str(sort);
  }
  buf << ") "
      << sort_str(fun->output_sort());

  if ( fun->body() ) {
    buf << " (";
    for (ymuint i = 0; i < fun->input_num(); ++ i) {
      const SmtId* id = fun->input_var(i);
      buf << " " << id_str(id);
    }
    buf << " ) "
	<< term_str(fun->body());
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

  case SmtTerm::kIdentifier:
    buf << id_str(term->identifier());
    break;

  case SmtTerm::kQualIdentifier:
    buf << "( as " << id_str(term->identifier())
	<< " " << sort_str(term->identifier_sort())
	<< " )";
    break;

  case SmtTerm::kFunTerm:
    buf << "( " << id_str(term->function()->name());
    for (ymuint i = 0; i < term->input_num(); ++ i) {
      buf << " " << term_str(term->input(i));
    }
    buf << " )";
    break;

  case SmtTerm::kLet:
    buf << "( let (";
    for (ymuint i = 0; i < term->let_binding_num(); ++ i) {
      SmtVarBinding vb = term->let_binding(i);
      buf << " (" << id_str(vb.mVar) << " " << term_str(vb.mExpr)
	  << " )";
    }
    buf << " ) " << term_str(term->body());
    break;

  case SmtTerm::kForall:
    buf << "( forall (";
    for (ymuint i = 0; i < term->var_num(); ++ i) {
      SmtSortedVar sv = term->sorted_var(i);
      buf << " (" << id_str(sv.mVar) << " " << sort_str(sv.mSort) << " )";
    }
    buf << " ) " << term_str(term->body());
    break;

  case SmtTerm::kExists:
    buf << "( exists (";
    for (ymuint i = 0; i < term->var_num(); ++ i) {
      SmtSortedVar sv = term->sorted_var(i);
      buf << " (" << id_str(sv.mVar) << " " << sort_str(sv.mSort) << " )";
    }
    buf << " ) " << term_str(term->body());
    break;

  case SmtTerm::kAttr:
    buf << "( ! " << term_str(term->body());
    for (ymuint i = 0; i < term->attr_num(); ++ i) {
      const SmtAttr* attr = term->attr(i);
      buf << " " << attr_str(attr);
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


//////////////////////////////////////////////////////////////////////
// クラス SmtAttr
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
string
attr_str(const SmtAttr* attr)
{
  ostringstream buf;
  buf << attr->keyword();
  if ( attr->value() ) {
    buf << " " << term_str(attr->value());
  }
  return buf.str();
}

END_NAMESPACE_YM_SMTLIBV2
