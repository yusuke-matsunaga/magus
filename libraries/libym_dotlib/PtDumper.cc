
/// @file libym_dotlib/PtDumper.cc
/// @brief PtDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/PtDumper.h"
#include "ym_dotlib/DotlibNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtDumper::PtDumper()
{
}

// @brief デストラクタ
PtDumper::~PtDumper()
{
}


BEGIN_NONAMESPACE

string
indent_str(ymuint indent)
{
  string ans;
  for (ymuint i = 0; i < indent; ++ i) {
    ans += ' ';
  }
  return ans;
}

// str 中に [a-zA-Z0-9_] 以外の文字が含まれていたら
// " " で囲んで出力する．
void
dump_str(ostream& s,
	 const ShString& str)
{
  bool need_quote = false;
  for (const char* p = str; *p; ++ p) {
    int c = *p;
    if ( !isalnum(c) && c != '_' ) {
      need_quote = true;
      break;
    }
  }
  if ( need_quote ) {
    s << "\"";
  }
  s << str;
  if ( need_quote ) {
    s << "\"";
  }
}

void
dump_sub(ostream& s,
	 const DotlibNode* node,
	 ymuint indent)
{
  switch ( node->type() ) {
  case DotlibNode::kInt:
    s << node->int_value();
    break;

  case DotlibNode::kFloat:
    s << node->float_value();
    break;

  case DotlibNode::kString:
    dump_str(s, node->string_value());
    break;

  case DotlibNode::kPlus:
    s << "( ";
    dump_sub(s, node->opr1(), indent);
    s << " + ";
    dump_sub(s, node->opr2(), indent);
    s << " )";
    break;

  case DotlibNode::kMinus:
    s << "( ";
    dump_sub(s, node->opr1(), indent);
    s << " - ";
    dump_sub(s, node->opr2(), indent);
    s << " )";
    break;

  case DotlibNode::kMult:
    s << "( ";
    dump_sub(s, node->opr1(), indent);
    s << " * ";
    dump_sub(s, node->opr2(), indent);
    s << " )";
    break;

  case DotlibNode::kDiv:
    s << "( ";
    dump_sub(s, node->opr1(), indent);
    s << " / ";
    dump_sub(s, node->opr2(), indent);
    s << " )";
    break;

  case DotlibNode::kList:
    {
      s << "( ";
      const char* comma = "";
      for (const DotlibNode* v = node->top(); v; v = v->next()) {
	s << comma;
	dump_sub(s, v, indent);
	comma = ", ";
      }
      s << " )";
    }
    break;

  case DotlibNode::kGroup:
    s << " ";
    dump_sub(s, node->group_value(), 0);
    s << " {" << endl;
    for (const DotlibNode* attr = node->attr_top(); attr; attr = attr->next()) {
      assert_cond( attr->is_attr() == true, __FILE__, __LINE__);
      s << indent_str(indent + 2) << attr->attr_name() << ": ";
      const DotlibNode* value = attr->attr_value();
      dump_sub(s, value, indent + 2);
      if ( !value->is_group() ) {
	s << ";" << endl;
      }
    }
    s << indent_str(indent) << "}" << endl;
    break;

  case DotlibNode::kAttr:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

END_NONAMESPACE


// @brief root 以下の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] root パース木のノード
void
PtDumper::operator()(ostream& s,
		     const DotlibNode* root)
{
  s << "library ";
  dump_sub(s, root, 0);
}


END_NAMESPACE_YM_DOTLIB
