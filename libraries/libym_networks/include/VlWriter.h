#ifndef VLWRITER_H
#define VLWRITER_H

/// @file VlWriter.h
/// @brief VlWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/networks_nsdef.h"
#include "YmLogic/expr_nsdef.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class VlWriter VlWriter.h "VlWriter.h"
/// @brief Verilog 出力用の基底クラス
//////////////////////////////////////////////////////////////////////
class VlWriter
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力ストリーム
  VlWriter(ostream& s);

  /// @brief デストラクタ
  ~VlWriter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 'module' 記述の開始
  /// @param[in] name モジュール名
  /// @note module モジュール名 '(' までを出力する．
  void
  begin_module(const char* name);

  /// @brief 'module' 記述の開始
  /// @param[in] name モジュール名
  /// @note module モジュール名 '(' までを出力する．
  void
  begin_module(const string& name);

  /// @brief 'module' 記述の終了
  /// @note ')' ';' nl を出力する．
  /// @note 'endmodule' ではない．
  void
  end_module();

  /// @brief 'endmodule' 記述の開始
  /// @note endmodule nl を出力する．
  /// @note 終了はない．
  void
  begin_endmodule();

  /// @brief 'input' 記述の開始
  /// @note input までを出力する．
  void
  begin_input();

  /// @brief 'input' 記述の開始
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  /// @note input [msb:lsb] までを出力する．
  void
  begin_input(int msb,
	      int lsb);

  /// @brief 'input' 記述の終了
  /// @note ';' nl を出力する．
  void
  end_input();

  /// @brief 'output' 記述の開始
  /// @note output までを出力する．
  void
  begin_output();

  /// @brief 'output' 記述の開始
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  /// @note output [msb:lsb] までを出力する．
  void
  begin_output(int msb,
	       int lsb);

  /// @brief 'output' 記述の終了
  /// @note ';' nl を出力する．
  void
  end_output();

  /// @brief 'inout' 記述の開始
  /// @note inout までを出力する．
  void
  begin_inout();

  /// @brief 'inout' 記述の開始
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  /// @note inout [msb:lsb] までを出力する．
  void
  begin_inout(int msb,
	      int lsb);

  /// @brief 'inout' 記述の終了
  /// @note ';' nl を出力する．
  void
  end_inout();

  /// @brief 'wire' 記述の開始
  /// @note wire までを出力する．
  void
  begin_wire();

  /// @brief 'wire' 記述の開始
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  /// @note wire [msb:lsb] までを出力する．
  void
  begin_wire(int msb,
	     int lsb);

  /// @brief 'wire' 記述の終了
  /// @note ';' nl を出力する．
  void
  end_wire();

  /// @brief 'reg' 記述の開始
  /// @note reg までを出力する．
  void
  begin_reg();

  /// @brief 'reg' 記述の開始
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  /// @note reg [msb:lsb] までを出力する．
  void
  begin_reg(int msb,
	    int lsb);

  /// @brief 'reg' 記述の終了
  /// @note ';' nl を出力する．
  void
  end_reg();

  /// @brief 宣言要素を出力する．
  /// @param[in] name 識別子名
  void
  put_elem(const char* name);

  /// @brief 宣言要素を出力する．
  /// @param[in] name 識別子名
  void
  put_elem(const string& name);

  /// @brief 'assign' 記述の開始
  /// @note assign までを出力する．
  void
  begin_assign();

  /// @brief 'assign' 記述の終了
  /// @note ';' nl を出力する．
  void
  end_assign();

  /// @brief インスタンス記述の開始
  /// @param[in] def_name インスタンスの定義名
  void
  begin_inst(const char* def_name);

  /// @brief インスタンス記述の開始
  /// @param[in] def_name インスタンスの定義名
  void
  begin_inst(const string& def_name);

  /// @brief インスタンス記述の終了
  /// @note ';' nl を出力する．
  void
  end_inst();

  /// @brief インスタンスの要素の開始
  /// @param[in] inst_name インスタンス名
  /// @note インスタンス名 '(' までを出力する．
  void
  begin_inst_elem(const char* inst_name);

  /// @brief インスタンスの要素の開始
  /// @param[in] inst_name インスタンス名
  /// @note インスタンス名 '(' までを出力する．
  void
  begin_inst_elem(const string& inst_name);

  /// @brief インスタンスの要素の終了
  /// @note ')' を出力する．
  void
  end_inst_elem();


public:
  //////////////////////////////////////////////////////////////////////
  // 低レベル関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲を出力する．
  /// @param[in] msb 範囲の MSB
  /// @param[in] lsb 範囲の LSB
  void
  put_range(int msb,
	    int lsb);

  /// @brief 識別子を出力する．
  /// @param[in] name 識別子名
  /// @note 場合によってはエスケープ文字列にする．
  void
  put_idstr(const char* name);

  /// @brief 識別子を出力する．
  /// @param[in] name 識別子名
  /// @note 場合によってはエスケープ文字列にする．
  void
  put_idstr(const string& name);

  /// @brief 文字列をそのまま出力する．
  /// @param[in] str 文字列
  void
  put_str(const char* str);

  /// @brief 文字列をそのまま出力する．
  /// @param[in] str 文字列
  void
  put_str(const string& str);

  /// @brief 整数を出力する．
  /// @param[in] num 数値
  void
  put_num(int num);

  /// @brief 論理式を出力する．
  /// @param[in] expr 論理式
  /// @param[in] name_map 変数名の対応表
  /// @note expr 中の i 番めの変数の名前が name_map[i] に入っている．
  void
  put_expr(const Expr& expr,
	   const VarStrMap& name_map);

  /// @brief 行末を出力する．
  /// @note 具体的には ';' nl
  void
  put_eol();

  /// @brief 空白を出力する．
  /// @param[in] n 空白の数
  void
  put_sp(ymuint n = 1);

  /// @brief 改行を出力する．
  void
  put_nl();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mS;

  // 先頭の要素のときに true となるフラグ
  bool mFirst;

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWRITER_H
