
/// @file PtiBase.cc
/// @brief PtiXXX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtiDecl.h"
#include "PtiExpr.h"
#include "YmVerilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PtiExpr
//////////////////////////////////////////////////////////////////////


BEGIN_NONAMESPACE

string
decompile_impl(const PtExpr* expr,
	       int ppri);

// @brief 演算子の場合の decompile() 処理
string
decompile_opr(const PtExpr* expr,
	      int ppri)
{
  // 各演算子を表す文字列のテーブル
  static const char* sym_table[] = {
    "",               // dummy
    "-",              // vpiMinusOp
    "+",              // vpiPlusOp
    "!",              // vpiNotOp
    "~",              // vpiBitNeg
    "&",              // vpiUnaryAndOp
    "~&",             // vpiUnaryNandOp
    "|",              // vpiUnaryOrOp
    "~|",             // vpiUnaryNorOp
    "^",              // vpiUnaryXorOp
    "~^",             // vpiUnaryXNorOp
    "-",              // vpiSubOp
    "/",              // vpiDivOp
    "%",              // vpiModOp
    "==",             // vpiEqOp
    "!=",             // vpiNeqOp
    "===",            // vpiCaseEqOp
    "!==",            // vpiCaseNeqOp
    ">",              // vpiGtOp
    ">=",             // vpiGeOp
    "<",              // vpiLtOp
    "<=",             // vpiLeOp
    "<<",             // vpiLShiftOp
    ">>",             // vpiRShiftOp
    "+",              // vpiAddOp
    "*",              // vpiMultOp
    "&&",             // vpiLogAndOp
    "||",             // vpiLogOrOp
    "&",              // vpiBitAndOp
    "|",              // vpiBitOrOp
    "^",              // vpiBitXorOp
    "~^",             // vpiBitXNorOp
    "?:",             // vpiConditionOp(dummy)
    "{}",             // vpiConcatOp(dummy)
    "{{}}",           // vpiMultiConcatOp(dummy)
    "or",             // vpiEventOrOp(dummy)
    ";",              // vpiNullOp(dummy)
    ",",              // vpiListOp(dummy)
    ":",              // vpiMinTypMaxOp(dummy)
    "posedge ",       // vpiPosedgeOp
    "negedge ",       // vpiNegedgeOp
    "<<<",            // vpiArithLShiftOp
    ">>>",            // vpiArithRShiftOp
    "**"              // vpiPowerOp
  };

  // 優先順位のテーブル
  static int pri_table[] = {
    0,                // dummy
    13,               // vpiMinusOp
    13,               // vpiPlusOp
    13,               // vpiNotOp
    13,               // vpiBitNegOp
    13,               // vpiUnaryAndOp
    13,               // vpiUnaryNandOp
    13,               // vpiUnaryOrOp
    13,               // vpiUnaryNorOp
    13,               // vpiUnaryXorOp
    13,               // vpiUnaryXNorOp
    10,               // vpiSubOp
    11,               // vpiDivOp
    11,               // vpiModOp
    7,                // vpiEqOp
    7,                // vpiNeqOp
    7,                // vpiCaseEqOp
    7,                // vpiCaseNeqOp
    8,                // vpiGtOp
    8,                // vpiGeOp
    8,                // vpiLtOp
    8,                // vpiLeOp
    9,                // vpiLShiftOp
    9,                // vpiRShiftOp
    10,               // vpiAddOp
    11,               // vpiMultOp
    3,                // vpiLogAndOp
    2,                // vpiLogOrOp
    6,                // vpiBitAndOp
    4,                // vpiBitOrOp
    5,                // vpiBitXorOp
    5,                // vpiBitXNorOp
    1,                // vpiConditionOp
    0,                // vpiConcatOp
    0,                // vpiMultiConcatOp
    0,                // vpiEventOrOp
    0,                // vpiNullOp
    0,                // vpiListOp
    0,                // vpiMinTypMaxOp
    0,                // vpiPosedgeOp
    0,                // vpiNegedgeOp
    9,                // vpiArithLShiftOp
    9,                // vpiArithRShiftOp
    12                // vpiPowerOp
  };

  string ans;

  tVlOpType optype = expr->op_type();
  // parent_optype の優先順位が自分の優先順位よりも高ければ括弧が必要
  bool need_par = false;
  int pri = pri_table[optype];
  if ( ppri > pri ) {
    need_par = true;
    ans += "(";
  }

  switch ( optype ) {
    // 空
  case kVlNullOp:
    ans += decompile_impl(expr->operand(0), 0);
    break;

    // 単項演算子
  case kVlMinusOp:
  case kVlNotOp:
  case kVlBitNegOp:
  case kVlPlusOp:
  case kVlUnaryAndOp:
  case kVlUnaryNandOp:
  case kVlUnaryNorOp:
  case kVlUnaryOrOp:
  case kVlUnaryXNorOp:
  case kVlUnaryXorOp:
  case kVlPosedgeOp:
  case kVlNegedgeOp:
    ans += sym_table[optype] + decompile_impl(expr->operand(0), pri);
    break;

    // 二項演算子
  case kVlAddOp:
  case kVlArithLShiftOp:
  case kVlArithRShiftOp:
  case kVlBitAndOp:
  case kVlBitOrOp:
  case kVlBitXNorOp:
  case kVlBitXorOp:
  case kVlCaseEqOp:
  case kVlCaseNeqOp:
  case kVlDivOp:
  case kVlEqOp:
  case kVlGeOp:
  case kVlGtOp:
  case kVlLShiftOp:
  case kVlLeOp:
  case kVlLogAndOp:
  case kVlLogOrOp:
  case kVlLtOp:
  case kVlModOp:
  case kVlMultOp:
  case kVlNeqOp:
  case kVlPowerOp:
  case kVlRShiftOp:
  case kVlSubOp:
    ans += decompile_impl(expr->operand(0), pri) + sym_table[optype] +
      decompile_impl(expr->operand(1), pri);
    break;

    // 三項演算子
  case kVlConditionOp:
    ans += decompile_impl(expr->operand(0), pri) + "?" +
      decompile_impl(expr->operand(1), pri) + ":" +
      decompile_impl(expr->operand(2), pri);
    break;

  case kVlMinTypMaxOp:
    ans += decompile_impl(expr->operand(0), pri) + ":" +
      decompile_impl(expr->operand(1), pri) + ":" +
      decompile_impl(expr->operand(2), pri);
    break;

  case kVlConcatOp:
    {
      ans += "{";
      const char* delim = "";
      ymuint n = expr->operand_num();
      for (ymuint i = 0; i < n; ++ i) {
	ans += delim + expr->operand(i)->decompile();
	delim = ",";
      }
      ans += "}";
    }
    break;

  case kVlMultiConcatOp:
    {
      ans = "{";
      ans += expr->operand(0)->decompile() + "{";
      const char* delim = "";
      ymuint n = expr->operand_num();
      for (ymuint i = 1; i < n; ++ i) {
	ans += delim + expr->operand(i)->decompile();
	delim = ",";
      }
      ans += "}}";
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( need_par ) {
    ans += ")";
  }
  return ans;
}

// @brief decompile() の下請け関数
// @param[in] ppri 直前の演算子の優先順位
string
decompile_impl(const PtExpr* expr,
	       int ppri)
{
  switch ( expr->type() ) {
  case kPtOprExpr:
    return decompile_opr(expr, ppri);
    break;

  case kPtConstExpr:
    {
      ostringstream buf;
      if ( expr->const_size() > 0 ) {
	buf << expr->const_size() << "'";
      }

      switch ( expr->const_type() ) {
      case kVpiRealConst:
	buf << expr->const_real();
	return buf.str();

      case kVpiStringConst:
	return expr->const_str();

      case kVpiIntConst:
	if ( expr->const_str() == NULL ) {
	  buf << expr->const_uint();
	  return buf.str();
	}
	break;

      case kVpiSignedBinaryConst:
	buf << "s";
      case kVpiBinaryConst:
	buf << "b";
	break;

      case kVpiSignedOctConst:
	buf << "s";
      case kVpiOctConst:
	buf << "b";
	break;

      case kVpiSignedDecConst:
	buf << "s";
      case kVpiDecConst:
	buf << "d";
	break;

      case kVpiSignedHexConst:
	buf << "s";
      case kVpiHexConst:
	buf << "h";
	break;
      }
      buf << expr->const_str();
      return buf.str();
    }

  case kPtFuncCallExpr:
  case kPtSysFuncCallExpr:
    {
      string ans(expr->name());
      ans += "(";
      ymuint n = expr->operand_num();
      const char* comma = "";
      for (ymuint i = 0; i < n; ++ i) {
	ans += comma + expr->operand(i)->decompile();
	comma = ", ";
      }
      ans += ")";
      return ans;
    }

  case kPtPrimaryExpr:
    {
      string ans(expand_full_name(expr->namebranch_array(), expr->name()));
      ymuint n = expr->index_num();
      for (ymuint i = 0; i < n; ++ i) {
	ans += "[" + expr->index(i)->decompile() + "]";
      }
      if ( expr->range_mode() != kVpiNoRange ) {
	const char* delim = NULL;
	switch ( expr->range_mode() ) {
	case kVpiConstRange: delim = ":"; break;
	case kVpiPlusRange:  delim = "+:"; break;
	case kVpiMinusRange: delim = "-:"; break;
	case kVpiNoRange: assert_not_reached(__FILE__, __LINE__);
	}
	ans += "[" + expr->left_range()->decompile() + delim +
	  expr->right_range()->decompile() + "]";
      }
      return ans;
    }

  default:
    assert_not_reached(__FILE__, __LINE__);
  }

  return string();
}

END_NONAMESPACE


// @brief 式を表す文字列表現の取得
string
PtiExpr::decompile() const
{
  return decompile_impl(this, 0);
}



//////////////////////////////////////////////////////////////////////
// PtNameBranch 用の関数
// 適当な場所がないのでここに置いている．
//////////////////////////////////////////////////////////////////////

// @brief 階層名を作り出す関数
// @param[in] nb 階層ブランチリストの先頭
// @param[in] name 末尾の名前
// @return 階層名を展開したものを返す．
string
expand_full_name(const PtNameBranchArray& nb_array,
		 const char* name)
{
  ostringstream buf;
  const char* period = "";
  for (ymuint i = 0; i < nb_array.size(); ++ i) {
    const PtNameBranch* nb = nb_array[i];
    buf << period << nb->name();
    if ( nb->has_index() ) {
      buf << "[" << nb->index() << "]";
    }
    period = ".";
  }
  if ( name != NULL ) {
    buf << period << name;
  }
  buf.flush();
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
