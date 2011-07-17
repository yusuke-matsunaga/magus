
/// @file udpval_test.cc
/// @brief VlUdpVal のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/VlUdpVal.h"


BEGIN_NAMESPACE_YM

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

void
udpval_test()
{
  bool stat = true;

  // 空のコンストラクタのテスト
  if ( !check_val("test_default_constructor",
		  "VlUdpVal()",
		  VlUdpVal(),
		  "-",
		  true,
		  false,
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
