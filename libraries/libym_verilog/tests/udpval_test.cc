
/// @file udpval_test.cc
/// @brief VlUdpVal のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/VlUdpVal.h"


BEGIN_NAMESPACE_YM

struct Operation
{

  /// @brief 演算を実行する仮想関数
  virtual
  bool
  operator()(const VlUdpVal& left,
	     const VlUdpVal& right) const = 0;

  /// @brief 演算を表す文字列
  virtual
  const char*
  opr_str() const = 0;

};

struct LeOp :
  public Operation
{

  /// @brief 演算を実行する仮想関数
  virtual
  bool
  operator()(const VlUdpVal& left,
	     const VlUdpVal& right) const;

  /// @brief 演算を表す文字列
  virtual
  const char*
  opr_str() const;

};

struct GeOp :
  public Operation
{

  /// @brief 演算を実行する仮想関数
  virtual
  bool
  operator()(const VlUdpVal& left,
	     const VlUdpVal& right) const;

  /// @brief 演算を表す文字列
  virtual
  const char*
  opr_str() const;

};

// @brief 演算を実行する仮想関数
bool
LeOp::operator()(const VlUdpVal& left,
		 const VlUdpVal& right) const
{
  return left <= right;
}

// @brief 演算を表す文字列
const char*
LeOp::opr_str() const
{
  return " <= ";
}

// @brief 演算を実行する仮想関数
bool
GeOp::operator()(const VlUdpVal& left,
		 const VlUdpVal& right) const
{
  return left >= right;
}

// @brief 演算を表す文字列
const char*
GeOp::opr_str() const
{
  return " >= ";
}

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

bool
check_val(const char* prefix,
	  const char* rhs,
	  const VlUdpVal& val,
	  const char* expected_string,
	  bool is_level,
	  bool is_edge,
	  bool is_nc,
	  bool is_composite)
{
  bool stat = true;

  cout << prefix << " : "
       << rhs << " = \"" << val.to_string() << "\"" << endl
       << "  val.is_level_symbol() = " << bool_str(val.is_level_symbol()) << endl
       << "  val.is_edge_symbol() = " << bool_str(val.is_edge_symbol()) << endl
       << "  val.is_nc_symbol() = " << bool_str(val.is_nc_symbol()) << endl
       << "  val.is_composite_symbol() = "
       << bool_str(val.is_composite_symbol()) << endl;
  if ( val.to_string() != expected_string ) {
    cout << "  ERROR, expected string = " << expected_string << endl;
    stat = false;
  }
  if ( val.is_level_symbol() != is_level ) {
    cout << "  ERROR, val.is_level_symbol() should be "
	 << bool_str(is_level) << endl;
    stat = false;
  }
  if ( val.is_edge_symbol() != is_edge ) {
    cout << "  ERROR, val.is_edge_symbol() should be "
	 << bool_str(is_edge) << endl;
    stat = false;
  }
  if ( val.is_nc_symbol() != is_nc ) {
    cout << "  ERROR, val.is_nc_symbol() should be "
	 << bool_str(is_nc) << endl;
    stat = false;
  }
  if ( val.is_composite_symbol() != is_composite ) {
    cout << "  ERROR, val.is_composite_symbol() should be "
	 << bool_str(is_composite) << endl;
    stat = false;
  }
  return stat;
}


bool
check_opr(const char* prefix,
	  const Operation& opr,
	  const vector<VlUdpVal>& val_table,
	  const vector<bool>& out_table)
{
  bool stat = true;
  ymuint n = val_table.size();
  for (ymuint i = 0; i < n; ++ i) {
    VlUdpVal in1 = val_table[i];
    for (ymuint j = 0; j < n; ++ j) {
      VlUdpVal in2 = val_table[j];
      bool result = opr(in1, in2);
      bool out = out_table[i * n + j];
      cout << prefix << " : "
	   << in1 << opr.opr_str() << in2
	   << " = " << result;
      if ( result != out ) {
	cout << ", ERROR, expected value is " << out;
	stat = false;
      }
      cout << endl;
    }
  }
  return stat;
}

bool
check_le(ymuint i,
	 ymuint j)
{
  switch ( i ) {
  case 0:
    switch ( j ) {
    case 0:
    case 3:
    case 4:
      return true;
    }
    break;

  case 1:
    switch ( j ) {
    case 1:
    case 3:
    case 4:
      return true;
    }
    break;

  case 2:
    switch ( j ) {
    case 2:
    case 4:
      return true;
    }
    break;

  case 3:
    switch ( j ) {
    case 3:
    case 4:
      return true;
    }
    break;

  case 4:
    switch ( j ) {
    case 4:
      return true;
    }
    break;
  }
  return false;
}

void
udpval_test()
{
  bool stat = true;

  // 空のコンストラクタのテスト
  if ( !check_val("test_default_constructor",
		  "VlUdpVal()",
		  VlUdpVal(),
		  "-",
		  false,
		  true,
		  true,
		  false) ) {
    stat = false;
  }

  // 0
  if ( !check_val("test_0",
		  "VlUdpVal('0')",
		  VlUdpVal('0'),
		  "0",
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }

  // 1
  if ( !check_val("test_1",
		  "VlUdpVal('1')",
		  VlUdpVal('1'),
		  "1",
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }

  // x
  if ( !check_val("test_x",
		  "VlUdpVal('x')",
		  VlUdpVal('x'),
		  "x",
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }

  // b
  if ( !check_val("test_b",
		  "VlUdpVal('b')",
		  VlUdpVal('b'),
		  "b",
		  true,
		  false,
		  false,
		  true) ) {
    stat = false;
  }

  // ?
  if ( !check_val("test_q",
		  "VlUdpVal('?')",
		  VlUdpVal('?'),
		  "?",
		  true,
		  false,
		  false,
		  true) ) {
    stat = false;
  }

  // r
  if ( !check_val("test_r",
		  "VlUdpVal('r')",
		  VlUdpVal('r'),
		  "01",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // f
  if ( !check_val("test_f",
		  "VlUdpVal('f')",
		  VlUdpVal('f'),
		  "10",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // p
  if ( !check_val("test_p",
		  "VlUdpVal('p')",
		  VlUdpVal('p'),
		  "p",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // n
  if ( !check_val("test_n",
		  "VlUdpVal('n')",
		  VlUdpVal('n'),
		  "n",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // *
  if ( !check_val("test_star",
		  "VlUdpVal('*')",
		  VlUdpVal('*'),
		  "??",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // -
  if ( !check_val("test_nc",
		  "VlUdpVal('-')",
		  VlUdpVal('-'),
		  "-",
		  false,
		  true,
		  true,
		  false) ) {
    stat = false;
  }

  // 00
  if ( !check_val("test_00",
		  "VlUdpVal('0', '0')",
		  VlUdpVal('0', '0'),
		  "00",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 01
  if ( !check_val("test_01",
		  "VlUdpVal('0', '1')",
		  VlUdpVal('0', '1'),
		  "01",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 0X
  if ( !check_val("test_0X",
		  "VlUdpVal('0', 'x')",
		  VlUdpVal('0', 'x'),
		  "0x",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 0B
  if ( !check_val("test_0B",
		  "VlUdpVal('0', 'b')",
		  VlUdpVal('0', 'b'),
		  "0b",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 0Q
  if ( !check_val("test_0Q",
		  "VlUdpVal('0', '?')",
		  VlUdpVal('0', '?'),
		  "0?",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 10
  if ( !check_val("test_10",
		  "VlUdpVal('1', '0')",
		  VlUdpVal('1', '0'),
		  "10",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 11
  if ( !check_val("test_11",
		  "VlUdpVal('1', '1')",
		  VlUdpVal('1', '1'),
		  "11",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 1X
  if ( !check_val("test_1X",
		  "VlUdpVal('1', 'x')",
		  VlUdpVal('1', 'x'),
		  "1x",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 1B
  if ( !check_val("test_1B",
		  "VlUdpVal('1', 'b')",
		  VlUdpVal('1', 'b'),
		  "1b",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 1Q
  if ( !check_val("test_1Q",
		  "VlUdpVal('1', '?')",
		  VlUdpVal('1', '?'),
		  "1?",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // X0
  if ( !check_val("test_X0",
		  "VlUdpVal('x', '0')",
		  VlUdpVal('x', '0'),
		  "x0",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // X1
  if ( !check_val("test_X1",
		  "VlUdpVal('x', '1')",
		  VlUdpVal('x', '1'),
		  "x1",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // XX
  if ( !check_val("test_XX",
		  "VlUdpVal('x', 'x')",
		  VlUdpVal('x', 'x'),
		  "xx",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // XB
  if ( !check_val("test_XB",
		  "VlUdpVal('x', 'b')",
		  VlUdpVal('x', 'b'),
		  "xb",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // XQ
  if ( !check_val("test_XQ",
		  "VlUdpVal('x', '?')",
		  VlUdpVal('x', '?'),
		  "x?",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // B0
  if ( !check_val("test_B0",
		  "VlUdpVal('b', '0')",
		  VlUdpVal('b', '0'),
		  "b0",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // B1
  if ( !check_val("test_B1",
		  "VlUdpVal('b', '1')",
		  VlUdpVal('b', '1'),
		  "b1",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // BX
  if ( !check_val("test_BX",
		  "VlUdpVal('b', 'x')",
		  VlUdpVal('b', 'x'),
		  "bx",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // BB
  if ( !check_val("test_BB",
		  "VlUdpVal('b', 'b')",
		  VlUdpVal('b', 'b'),
		  "bb",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // BQ
  if ( !check_val("test_BQ",
		  "VlUdpVal('b', '?')",
		  VlUdpVal('b', '?'),
		  "b?",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // Q0
  if ( !check_val("test_Q0",
		  "VlUdpVal('?', '0')",
		  VlUdpVal('?', '0'),
		  "?0",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // Q1
  if ( !check_val("test_Q1",
		  "VlUdpVal('?', '1')",
		  VlUdpVal('?', '1'),
		  "?1",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // QX
  if ( !check_val("test_QX",
		  "VlUdpVal('?', 'x')",
		  VlUdpVal('?', 'x'),
		  "?x",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // QB
  if ( !check_val("test_QB",
		  "VlUdpVal('?', 'b')",
		  VlUdpVal('?', 'b'),
		  "?b",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // QQ
  if ( !check_val("test_QQ",
		  "VlUdpVal('?', '?')",
		  VlUdpVal('?', '?'),
		  "??",
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  vector<VlUdpVal> val_table;
  val_table.push_back(VlUdpVal('-'));
  val_table.push_back(VlUdpVal('0'));
  val_table.push_back(VlUdpVal('1'));
  val_table.push_back(VlUdpVal('x'));
  val_table.push_back(VlUdpVal('b'));
  val_table.push_back(VlUdpVal('?'));
  val_table.push_back(VlUdpVal('p'));
  val_table.push_back(VlUdpVal('n'));
  val_table.push_back(VlUdpVal('0', '0'));
  val_table.push_back(VlUdpVal('0', '1'));
  val_table.push_back(VlUdpVal('0', 'x'));
  val_table.push_back(VlUdpVal('0', 'b'));
  val_table.push_back(VlUdpVal('0', '?'));
  val_table.push_back(VlUdpVal('1', '0'));
  val_table.push_back(VlUdpVal('1', '1'));
  val_table.push_back(VlUdpVal('1', 'x'));
  val_table.push_back(VlUdpVal('1', 'b'));
  val_table.push_back(VlUdpVal('1', '?'));
  val_table.push_back(VlUdpVal('x', '0'));
  val_table.push_back(VlUdpVal('x', '1'));
  val_table.push_back(VlUdpVal('x', 'x'));
  val_table.push_back(VlUdpVal('x', 'b'));
  val_table.push_back(VlUdpVal('x', '?'));
  val_table.push_back(VlUdpVal('b', '0'));
  val_table.push_back(VlUdpVal('b', '1'));
  val_table.push_back(VlUdpVal('b', 'x'));
  val_table.push_back(VlUdpVal('b', 'b'));
  val_table.push_back(VlUdpVal('b', '?'));
  val_table.push_back(VlUdpVal('?', '0'));
  val_table.push_back(VlUdpVal('?', '1'));
  val_table.push_back(VlUdpVal('?', 'x'));
  val_table.push_back(VlUdpVal('?', 'b'));
  val_table.push_back(VlUdpVal('?', '?'));

  vector<bool> le_table(33 * 33, false);
  le_table[0 * 33 + 0] = true;
  for (ymuint i = 0; i < 5; ++ i) {
    ymuint p = i + 1;
    for (ymuint j = 0; j < 5; ++ j) {
      ymuint q = j + 1;
      if ( check_le(i, j) ) {
	le_table[p * 33 + q] = true;
      }
    }
  }
  for (ymuint i = 0; i < 5; ++ i) {
    for (ymuint j = 0; j < 5; ++ j) {
      ymuint p = i * 5 + j + 8;
      for (ymuint k = 0; k < 5; ++ k) {
	for (ymuint l = 0; l < 5; ++ l) {
	  ymuint q = k * 5 + l + 8;
	  if ( check_le(i, k) && check_le(j, l) ) {
	    le_table[p * 33 + q] = true;
	  }
	}
      }
    }
  }
  // p
  le_table[6 * 33 + 6] = true;
  le_table[6 * 33 + 32] = true;
  for (ymuint i = 0; i < 5; ++ i) {
    for (ymuint j = 0; j < 5; ++ j) {
      ymuint p = i * 5 + j + 8;
      bool stat = false;
      // vs 01
      if ( check_le(i, 0) && check_le(j, 1) ) {
	stat = true;
      }
      // vs 0x
      if ( check_le(i, 0) && check_le(j, 2) ) {
	stat = true;
      }
      // vs x1
      if ( check_le(i, 2) && check_le(j, 1) ) {
	stat = true;
      }
      if ( stat ) {
	le_table[p * 33 + 6] = true;
      }
    }
  }
  // n
  le_table[7 * 33 + 7] = true;
  le_table[7 * 33 + 32] = true;
  for (ymuint i = 0; i < 5; ++ i) {
    for (ymuint j = 0; j < 5; ++ j) {
      ymuint p = i * 5 + j + 8;
      bool stat = false;
      // vs 10
      if ( check_le(i, 1) && check_le(j, 0) ) {
	stat = true;
      }
      // vs 1x
      if ( check_le(i, 1) && check_le(j, 2) ) {
	stat = true;
      }
      // vs x0
      if ( check_le(i, 2) && check_le(j, 0) ) {
	stat = true;
      }
      if ( stat ) {
	le_table[p * 33 + 7] = true;
      }
    }
  }

  if ( !check_opr("test_le",
		  LeOp(),
		  val_table,
		  le_table) ) {
    stat = false;
  }

  if ( stat ) {
    cout << "All test passed." << endl;
  }
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using nsYm::udpval_test;

  udpval_test();

  return 0;
}
