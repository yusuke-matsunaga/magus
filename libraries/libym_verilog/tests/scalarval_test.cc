
/// @file scalarval_test.cc
/// @brief VlScalarVal のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/VlScalarVal.h"


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

int
val2int(const VlScalarVal& val)
{
  if ( val.is_zero() ) {
    return 0;
  }
  if ( val.is_one() ) {
    return 1;
  }
  if ( val.is_x() ) {
    return 2;
  }
  if ( val.is_z() ) {
    return 3;
  }
  assert_not_reached(__FILE__, __LINE__);
  return 255;
}

bool
equal(const VlScalarVal& left,
      const VlScalarVal& right)
{
  return val2int(left) == val2int(right);
}

bool
check_val(const char* prefix,
	  const char* lhs,
	  const VlScalarVal& val,
	  bool is_zero,
	  bool is_one,
	  bool is_x,
	  bool is_z)
{
  bool stat = true;
  cout << prefix << " : " << lhs << " = " << val << endl;
  if ( val.is_zero() != is_zero ) {
    cout << prefix << " : 'is_zero() = " << bool_str(is_zero)
	 << "' fails." << endl;
    stat = false;
  }
  if ( val.is_one() != is_one ) {
    cout << prefix << " : 'is_one() = " << bool_str(is_one)
	 << "' fails." << endl;
    stat = false;
  }
  if ( val.is_x() != is_x ) {
    cout << prefix << " : 'is_x() = " << bool_str(is_x)
	 << "' fails." << endl;
    stat = false;
  }
  if ( val.is_z() != is_z ) {
    cout << prefix << " : 'is_z() = " << bool_str(is_z)
	 << "' fails." << endl;
    stat = false;
  }
  if ( val.is_xz() != (is_x || is_z) ) {
    cout << prefix << " : 'is_xz() = " << bool_str(is_x || is_z)
	 << "' fails." << endl;
    stat = false;
  }
  return stat;
}

bool
check_not(const char* prefix,
	  VlScalarVal val_table[],
	  VlScalarVal out_table[])
{
  bool stat = true;
  for (ymuint i = 0; i < 4; ++ i) {
    VlScalarVal in = val_table[i];
    VlScalarVal result = !in;
    VlScalarVal out = out_table[i];
    cout << prefix << " : !" << in
	 << " =" << result;
    if ( !equal(result, out) ) {
      cout << ", ERROR, expected value is " << out;
      stat = false;
    }
    cout << endl;
  }
  return stat;
}

bool
check_eq(const char* prefix,
	 VlScalarVal val_table[],
	 VlScalarVal out_table[])
{
  bool stat = true;
  for (ymuint i = 0; i < 4; ++ i ) {
    VlScalarVal in0 = val_table[i];
    for (ymuint j = 0; j < 4; ++ j ) {
      VlScalarVal in1 = val_table[j];
      VlScalarVal result = in0 == in1;
      VlScalarVal out = out_table[i * 4 + j];
      cout << prefix << " : "
	   << in0 << " == " << in1
	   << " = " << result;
      if ( !equal(result, out) ) {
	cout << ", ERROR, expected value is " << out;
	stat = false;
      }
      cout << endl;
    }
  }
  return stat;
}

bool
check_neq(const char* prefix,
	 VlScalarVal val_table[],
	 VlScalarVal out_table[])
{
  bool stat = true;
  for (ymuint i = 0; i < 4; ++ i ) {
    VlScalarVal in0 = val_table[i];
    for (ymuint j = 0; j < 4; ++ j ) {
      VlScalarVal in1 = val_table[j];
      VlScalarVal result = in0 != in1;
      VlScalarVal out = out_table[i * 4 + j];
      cout << prefix << " : "
	   << in0 << " != " << in1
	   << " = " << result;
      if ( !equal(result, out) ) {
	cout << ", ERROR, expected value is " << out;
	stat = false;
      }
      cout << endl;
    }
  }
  return stat;
}

bool
check_and(const char* prefix,
	  VlScalarVal val_table[],
	  VlScalarVal out_table[])
{
  bool stat = true;
  for (ymuint i = 0; i < 4; ++ i ) {
    VlScalarVal in0 = val_table[i];
    for (ymuint j = 0; j < 4; ++ j ) {
      VlScalarVal in1 = val_table[j];
      VlScalarVal result = in0 && in1;
      VlScalarVal out = out_table[i * 4 + j];
      cout << prefix << " : "
	   << in0 << " && " << in1
	   << " = " << result;
      if ( !equal(result, out) ) {
	cout << ", ERROR, expected value is " << out;
	stat = false;
      }
      cout << endl;
    }
  }
  return stat;
}

bool
check_or(const char* prefix,
	 VlScalarVal val_table[],
	 VlScalarVal out_table[])
{
  bool stat = true;
  for (ymuint i = 0; i < 4; ++ i ) {
    VlScalarVal in0 = val_table[i];
    for (ymuint j = 0; j < 4; ++ j ) {
      VlScalarVal in1 = val_table[j];
      VlScalarVal result = in0 || in1;
      VlScalarVal out = out_table[i * 4 + j];
      cout << prefix << " : "
	   << in0 << " || " << in1
	   << " = " << result;
      if ( !equal(result, out) ) {
	cout << ", ERROR, expected value is " << out;
	stat = false;
      }
      cout << endl;
    }
  }
  return stat;
}

void
scalarval_test()
{
  bool stat = true;

  // 空のコンストラクタのテスト
  if ( !check_val("test0",
		  "VlScalarVal()",
		  VlScalarVal(),
		  false,
		  false,
		  true,
		  false) ) {
    stat = false;
  }

  // zero() 関数のテスト
  if ( !check_val("test1",
		  "VlScalarVal::zero()",
		  VlScalarVal::zero(),
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }

  // one() 関数のテスト
  if ( !check_val("test2",
		  "VlScalarVal::one()",
		  VlScalarVal::one(),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // x() 関数のテスト
  if ( !check_val("test3",
		  "VlScalarVal::x()",
		  VlScalarVal::x(),
		  false,
		  false,
		  true,
		  false) ) {
    stat = false;
  }

  // z() 関数のテスト
  if ( !check_val("test4",
		  "VlScalarVal::z()",
		  VlScalarVal::z(),
		  false,
		  false,
		  false,
		  true) ) {
    stat = false;
  }

  // 整数値からの変換のテスト
  if ( !check_val("test5a",
		  "VlScalarVal(0)",
		  VlScalarVal(0),
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test5b",
		  "VlScalarVal(1)",
		  VlScalarVal(1),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test5c",
		  "VlScalarVal(2)",
		  VlScalarVal(2),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 符号なし整数値からの変換のテスト
  if ( !check_val("test6a",
		  "VlScalarVal(0U)",
		  VlScalarVal(0U),
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test6b",
		  "VlScalarVal(1U)",
		  VlScalarVal(1U),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test6c",
		  "VlScalarVal(2U)",
		  VlScalarVal(2U),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // 実数値からの変換のテスト
  if ( !check_val("test7a",
		  "VlScalarVal(0.0)",
		  VlScalarVal(0.0),
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test7b",
		  "VlScalarVal(1.0)",
		  VlScalarVal(1.0),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test7c",
		  "VlScalarVal(2.0)",
		  VlScalarVal(2.0),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  // ブール値からの変換のテスト
  if ( !check_val("test8a",
		  "VlScalarVal(false)",
		  VlScalarVal(false),
		  true,
		  false,
		  false,
		  false) ) {
    stat = false;
  }
  if ( !check_val("test8b",
		  "VlScalarVal(true)",
		  VlScalarVal(true),
		  false,
		  true,
		  false,
		  false) ) {
    stat = false;
  }

  VlScalarVal val_0 = VlScalarVal::zero();
  VlScalarVal val_1 = VlScalarVal::one();
  VlScalarVal val_x = VlScalarVal::x();
  VlScalarVal val_z = VlScalarVal::z();

  VlScalarVal val_table[4];
  val_table[0] = val_0;
  val_table[1] = val_1;
  val_table[2] = val_x;
  val_table[3] = val_z;

  VlScalarVal not_table[4];
  not_table[0] = val_1;
  not_table[1] = val_0;
  not_table[2] = val_x;
  not_table[3] = val_x;

  if ( !check_not("test_not",
		  val_table,
		  not_table) ) {
    stat = false;
  }

  VlScalarVal and_table[16];
  and_table[0 * 4 + 0] = val_0;
  and_table[0 * 4 + 1] = val_0;
  and_table[0 * 4 + 2] = val_0;
  and_table[0 * 4 + 3] = val_0;
  and_table[1 * 4 + 0] = val_0;
  and_table[1 * 4 + 1] = val_1;
  and_table[1 * 4 + 2] = val_x;
  and_table[1 * 4 + 3] = val_x;
  and_table[2 * 4 + 0] = val_0;
  and_table[2 * 4 + 1] = val_x;
  and_table[2 * 4 + 2] = val_x;
  and_table[2 * 4 + 3] = val_x;
  and_table[3 * 4 + 0] = val_0;
  and_table[3 * 4 + 1] = val_x;
  and_table[3 * 4 + 2] = val_x;
  and_table[3 * 4 + 3] = val_x;

  if ( !check_and("test_and",
		  val_table,
		  and_table) ) {
    stat = false;
  }

  VlScalarVal or_table[16];
  or_table[0 * 4 + 0] = val_0;
  or_table[0 * 4 + 1] = val_1;
  or_table[0 * 4 + 2] = val_x;
  or_table[0 * 4 + 3] = val_x;
  or_table[1 * 4 + 0] = val_1;
  or_table[1 * 4 + 1] = val_1;
  or_table[1 * 4 + 2] = val_1;
  or_table[1 * 4 + 3] = val_1;
  or_table[2 * 4 + 0] = val_x;
  or_table[2 * 4 + 1] = val_1;
  or_table[2 * 4 + 2] = val_x;
  or_table[2 * 4 + 3] = val_x;
  or_table[3 * 4 + 0] = val_x;
  or_table[3 * 4 + 1] = val_1;
  or_table[3 * 4 + 2] = val_x;
  or_table[3 * 4 + 3] = val_x;

  if ( !check_or("test_or",
		 val_table,
		 or_table) ) {
    stat = false;
  }

  VlScalarVal eq_table[16];
  eq_table[0 * 4 + 0] = val_1;
  eq_table[0 * 4 + 1] = val_0;
  eq_table[0 * 4 + 2] = val_x;
  eq_table[0 * 4 + 3] = val_x;
  eq_table[1 * 4 + 0] = val_0;
  eq_table[1 * 4 + 1] = val_1;
  eq_table[1 * 4 + 2] = val_x;
  eq_table[1 * 4 + 3] = val_x;
  eq_table[2 * 4 + 0] = val_x;
  eq_table[2 * 4 + 1] = val_x;
  eq_table[2 * 4 + 2] = val_x;
  eq_table[2 * 4 + 3] = val_x;
  eq_table[3 * 4 + 0] = val_x;
  eq_table[3 * 4 + 1] = val_x;
  eq_table[3 * 4 + 2] = val_x;
  eq_table[3 * 4 + 3] = val_x;

  if ( !check_eq("test_eq",
		 val_table,
		 eq_table) ) {
    stat = false;
  }

  VlScalarVal neq_table[16];
  neq_table[0 * 4 + 0] = val_0;
  neq_table[0 * 4 + 1] = val_1;
  neq_table[0 * 4 + 2] = val_x;
  neq_table[0 * 4 + 3] = val_x;
  neq_table[1 * 4 + 0] = val_1;
  neq_table[1 * 4 + 1] = val_0;
  neq_table[1 * 4 + 2] = val_x;
  neq_table[1 * 4 + 3] = val_x;
  neq_table[2 * 4 + 0] = val_x;
  neq_table[2 * 4 + 1] = val_x;
  neq_table[2 * 4 + 2] = val_x;
  neq_table[2 * 4 + 3] = val_x;
  neq_table[3 * 4 + 0] = val_x;
  neq_table[3 * 4 + 1] = val_x;
  neq_table[3 * 4 + 2] = val_x;
  neq_table[3 * 4 + 3] = val_x;

  if ( !check_neq("test_neq",
		  val_table,
		  neq_table) ) {
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
  using nsYm::scalarval_test;

  scalarval_test();

  return true;
}
