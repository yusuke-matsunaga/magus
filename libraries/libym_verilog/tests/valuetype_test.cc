
/// @file valuetype_test.cc
/// @brief VlValueType のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/VlValueType.h"


BEGIN_NAMESPACE_YM_VERILOG

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
check_type(const char* prefix,
	   const char* lhs,
	   const VlValueType& type,
	   bool no_type,
	   bool int_type,
	   bool real_type,
	   bool time_type,
	   bool bv_type,
	   bool is_signed,
	   bool is_sized,
	   ymuint size)
{
  cout << prefix << ": " << lhs << " = " << type << endl;
  bool stat = true;
  if ( type.is_no_type() != no_type ) {
    cout << prefix << ": 'is_no_type() == "
	 << bool_str(no_type) << "' fails." << endl;
    stat = false;
  }
  if ( type.is_int_type() != int_type ) {
    cout << prefix << ": 'is_int_type() == "
	 << bool_str(int_type) << "' fails." << endl;
    stat = false;
  }
  if ( type.is_real_type() != real_type ) {
    cout << prefix << ": 'is_real_type() == "
	 << bool_str(real_type) << "' fails." << endl;
    stat = false;
  }
  if ( type.is_time_type() != time_type ) {
    cout << prefix << ": 'is_time_type() == "
	 << bool_str(time_type) << "' fails." << endl;
    stat = false;
  }
  if ( type.is_bitvector_type() != bv_type ) {
    cout << prefix << ": 'is_bv_type() == "
	 << bool_str(bv_type) << "' fails." << endl;
    stat = false;
  }
  if ( type.is_signed() != is_signed ) {
    cout << prefix << ": 'is_signed() == "
	 << bool_str(is_signed) << "' fails." << endl;
    stat = false;
  }
  if ( type.is_sized() != is_sized ) {
    cout << prefix << ": 'is_sized() == "
	 << bool_str(is_sized) << "' fails." << endl;
    stat = false;
  }
  if ( type.size() != size ) {
    cout << prefix << ": 'size() == " << size
	 << "' fails." << endl;
    stat = false;
  }
  return stat;
}

void
valuetype_test()
{
  bool stat = true;

  // 型なしのコンストラクタのテスト
  if ( !check_type("test0",
		   "VlValueType()",
		   VlValueType(),
		   true,
		   false,
		   false,
		   false,
		   false,
		   false,
		   false,
		   0) ) {
    stat = false;
  }

  // 整数型の生成関数のテスト
  if ( !check_type("test1",
		   "VlValueType::int_type()",
		   VlValueType::int_type(),
		   false,
		   true,
		   false,
		   false,
		   true,
		   true,
		   true,
		   kVpiSizeInteger) ) {
    stat = false;
  }

  // 実数型の生成関数のテスト
  if ( !check_type("test2",
		   "VlValueType::real_type()",
		   VlValueType::real_type(),
		   false,
		   false,
		   true,
		   false,
		   false,
		   true,
		   true,
		   kVpiSizeReal) ) {
    stat = false;
  }

  // time 型の生成関数のテスト
  if ( !check_type("test3",
		   "VlValueType::time_type()",
		   VlValueType::time_type(),
		   false,
		   false,
		   false,
		   true,
		   true,
		   false,
		   true,
		   kVpiSizeTime) ) {
    stat = false;
  }

  // unsigned, unsized のテスト
  if ( !check_type("test4a",
		   "VlValueType(false, false, 0)",
		   VlValueType(false, false, 0),
		   false,
		   false,
		   false,
		   false,
		   true,
		   false,
		   false,
		   0) ) {
    stat = false;
  }

  // unsigned, unsized のテスト
  if ( !check_type("test4b",
		   "VlValueType(false, false, 32)",
		   VlValueType(false, false, 32),
		   false,
		   false,
		   false,
		   false,
		   true,
		   false,
		   false,
		   32) ) {
    stat = false;
  }

  // unsigned, sized のテスト
  if ( !check_type("test5",
		   "VlValueType(false, true, 32)",
		   VlValueType(false, true, 32),
		   false,
		   false,
		   false,
		   false,
		   true,
		   false,
		   true,
		   32) ) {
    stat = false;
  }

  // signed, unsized のテスト
  if ( !check_type("test6a",
		   "VlValueType(true, false, 0)",
		   VlValueType(true, false, 0),
		   false,
		   false,
		   false,
		   false,
		   true,
		   true,
		   false,
		   0) ) {
    stat = false;
  }

  // unsigned, unsized のテスト
  if ( !check_type("test6b",
		   "VlValueType(true, false, 32)",
		   VlValueType(true, false, 32),
		   false,
		   false,
		   false,
		   false,
		   true,
		   true,
		   false,
		   32) ) {
    stat = false;
  }

  // signed, sized のテスト
  if ( !check_type("test7a",
		   "VlValueType(true, true, 16)",
		   VlValueType(true, true, 16),
		   false,
		   false,
		   false,
		   false,
		   true,
		   true,
		   true,
		   16) ) {
    stat = false;
  }

  // signed, sized のテスト
  if ( !check_type("test7b",
		   "VlValueType(true, true, 32)",
		   VlValueType(true, true, 32),
		   false,
		   true,
		   false,
		   false,
		   true,
		   true,
		   true,
		   32) ) {
    stat = false;
  }

  if ( stat ) {
    cout << "All test passed." << endl;
  }
}

END_NAMESPACE_YM_VERILOG


int
main(int argc,
     char** argv)
{
  using nsYm::nsVerilog::valuetype_test;

  valuetype_test();

  return 0;
}
