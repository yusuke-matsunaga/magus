
/// @file VlWriter.cc
/// @brief VlWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VlWriter.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlWriter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力ストリーム
VlWriter::VlWriter(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
VlWriter::~VlWriter()
{
}

// @brief 'module' 記述の開始
// @param[in] name モジュール名
// @note module モジュール名 '(' までを出力する．
void
VlWriter::begin_module(const char* name)
{
  put_str("module");
  put_sp();
  put_idstr(name);
  put_str("(");

  mFirst = true;
}

// @brief 'module' 記述の開始
// @param[in] name モジュール名
// @note module モジュール名 '(' までを出力する．
void
VlWriter::begin_module(const string& name)
{
  begin_module(name.c_str());
}

// @brief 'module' 記述の終了
// @note ')' ';' nl を出力する．
// @note 'endmodule' ではない．
void
VlWriter::end_module()
{
  put_str(")");
  put_eol();
}

// @brief 'endmodule' 記述の開始
// @note endmodule nl を出力する．
// @note 終了はない．
void
VlWriter::begin_endmodule()
{
  put_str("endmodule");
  put_nl();
}

// @brief 'input' 記述の開始
// @note input までを出力する．
void
VlWriter::begin_input()
{
  put_str("input");

  mFirst = true;
}

// @brief 'input' 記述の開始
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
// @note input [msb:lsb] までを出力する．
void
VlWriter::begin_input(int msb,
		      int lsb)
{
  begin_input();
  put_range(msb, lsb);
}

// @brief 'input' 記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_input()
{
  put_eol();
}

// @brief 'output' 記述の開始
// @note output までを出力する．
void
VlWriter::begin_output()
{
  put_str("output");

  mFirst = true;
}

// @brief 'output' 記述の開始
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
// @note output [msb:lsb] までを出力する．
void
VlWriter::begin_output(int msb,
		       int lsb)
{
  begin_output();
  put_range(msb, lsb);
}

// @brief 'output' 記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_output()
{
  put_eol();
}

// @brief 'inout' 記述の開始
// @note inout までを出力する．
void
VlWriter::begin_inout()
{
  put_str("inout");

  mFirst = true;
}

// @brief 'inout' 記述の開始
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
// @note inout までを出力する．
void
VlWriter::begin_inout(int msb,
		      int lsb)
{
  begin_inout();
  put_range(msb, lsb);
}

// @brief 'inout' 記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_inout()
{
  put_eol();
}

// @brief 'wire' 記述の開始
// @note wire までを出力する．
void
VlWriter::begin_wire()
{
  put_str("wire");

  mFirst = true;
}

// @brief 'wire' 記述の開始
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
// @note wire までを出力する．
void
VlWriter::begin_wire(int msb,
		     int lsb)
{
  begin_wire();
  put_range(msb, lsb);
}

// @brief 'wire' 記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_wire()
{
  put_eol();
}

// @brief 'reg' 記述の開始
// @note reg までを出力する．
void
VlWriter::begin_reg()
{
  put_str("reg");

  mFirst = true;
}

// @brief 'reg' 記述の開始
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
// @note reg までを出力する．
void
VlWriter::begin_reg(int msb,
		    int lsb)
{
  begin_reg();
  put_range(msb, lsb);
}

// @brief 'reg' 記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_reg()
{
  put_eol();
}

// @brief 宣言要素を出力する．
// @param[in] name 識別子名
void
VlWriter::put_elem(const char* name)
{
  if ( mFirst ) {
    mFirst = false;
  }
  else {
    put_str(",");
  }
  put_sp();
  put_idstr(name);
}

// @brief 宣言要素を出力する．
// @param[in] name 識別子名
void
VlWriter::put_elem(const string& name)
{
  put_elem(name.c_str());
}

// @brief 'assign' 記述の開始
// @note assign までを出力する．
void
VlWriter::begin_assign()
{
  put_str("assign");

  mFirst = true;
}

// @brief 'assign' 記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_assign()
{
  put_eol();
}

// @brief インスタンス記述の開始
// @param[in] def_name インスタンスの定義名
void
VlWriter::begin_inst(const char* def_name)
{
  put_idstr(def_name);
}

// @brief インスタンス記述の開始
// @param[in] def_name インスタンスの定義名
void
VlWriter::begin_inst(const string& def_name)
{
  put_idstr(def_name);
}

// @brief インスタンス記述の終了
// @note ';' nl を出力する．
void
VlWriter::end_inst()
{
  put_eol();
}

// @brief インスタンスの要素の開始
// @param[in] inst_name インスタンス名
// @note インスタンス名 '(' までを出力する．
void
VlWriter::begin_inst_elem(const char* inst_name)
{
  put_idstr(inst_name);
  put_str("(");
}

// @brief インスタンスの要素の開始
// @param[in] inst_name インスタンス名
// @note インスタンス名 '(' までを出力する．
void
VlWriter::begin_inst_elem(const string& inst_name)
{
  begin_inst_elem(inst_name.c_str());
}

// @brief インスタンスの要素の終了
// @note ')' を出力する．
void
VlWriter::end_inst_elem()
{
  put_str(")");
}

// @brief 範囲を出力する．
// @param[in] msb 範囲の MSB
// @param[in] lsb 範囲の LSB
void
VlWriter::put_range(int msb,
		    int lsb)
{
  put_str(" [");
  put_num(msb);
  put_str(":");
  put_num(lsb);
  put_str("]");
}

// @brief 識別子を出力する．
// @param[in] name 識別子名
// @note 場合によってはエスケープ文字列にする．
void
VlWriter::put_idstr(const char* name)
{
  const char* s = name;
  char c = *s;
  if ( c == '\0' ) {
    // かなり特殊なケース
    return;
  }

  bool need_escape = false;
  if ( !isalpha(c) && c != '_' ) {
    need_escape = true;
  }
  else {
    for (++s; (c = *s); ++ s) {
      if ( !isalnum(c) && c != '_' ) {
	need_escape = true;
	break;
      }
    }
  }
  if ( need_escape ) {
    mS << '\\' << name << ' ';
  }
  else {
    mS << name;
  }
}

// @brief 識別子を出力する．
// @param[in] name 識別子名
// @note 場合によってはエスケープ文字列にする．
void
VlWriter::put_idstr(const string& name)
{
  put_idstr(name.c_str());
}

// @brief 文字列をそのまま出力する．
// @param[in] str 文字列
void
VlWriter::put_str(const char* str)
{
  mS << str;
}

// @brief 文字列をそのまま出力する．
// @param[in] str 文字列
void
VlWriter::put_str(const string& str)
{
  mS << str;
}

// @brief 整数を出力する．
// @param[in] num 数値
void
VlWriter::put_num(int num)
{
  mS << num;
}

// @brief 論理式を出力する．
// @param[in] expr 論理式
// @param[in] name_map 変数名の対応表
// @note expr 中の i 番めの変数の名前が name_map[i] に入っている．
void
VlWriter::put_expr(const LogExpr& expr,
		  const VarStrMap& name_map)
{
  if ( expr.is_zero() ) {
    put_str("0");
  }
  else if ( expr.is_one() ) {
    put_str("1");
  }
  else if ( expr.is_literal() ) {
    if ( expr.is_negaliteral() ) {
      put_str("~");
    }
    VarId id = expr.varid();
    VarStrMap::const_iterator p = name_map.find(id);
    if ( p == name_map.end() ) {
#warning "TODO: 未対応"
      assert_not_reached(__FILE__, __LINE__);
    }
    string name = p->second;
    put_idstr(name);
  }
  else if ( expr.is_op() ){
    const char* op_str = NULL;
    if ( expr.is_and() ) {
      op_str = " & ";
    }
    else if ( expr.is_or() ) {
      op_str = " | ";
    }
    else if ( expr.is_xor() ) {
      op_str = " ^ ";
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
    put_str("(");
    ymuint n = expr.child_num();
    const char* delim = "";
    for (ymuint i = 0; i < n; ++ i) {
      put_str(delim);
      put_expr(expr.child(i), name_map);
      delim = op_str;
    }
    put_str(")");
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 行末を出力する．
// @note 具体的には ';' nl
void
VlWriter::put_eol()
{
  put_str(";");
  put_nl();
}

// @brief 空白を出力する．
// @param[in] n 空白の数
void
VlWriter::put_sp(ymuint n)
{
  for (ymuint i = 0; i < n; ++ i) {
    put_str(" ");
  }
}

// @brief 改行を出力する．
void
VlWriter::put_nl()
{
  mS << endl;
}

END_NAMESPACE_YM_NETWORKS

