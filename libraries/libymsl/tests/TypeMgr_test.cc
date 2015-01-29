
/// @file TypeMgr_test.cc
/// @brief TypeMgr_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "TypeMgr.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

bool
init_test()
{
  TypeMgr mgr;

  bool ok = true;

  {
    const Type* type1 = mgr.void_type();
    if ( type1 == NULL ) {
      cerr << " init_test: void_type() == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kVoidType ) {
      cerr << " init_test: void_type() is not kVoidType" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.void_type();
      if ( type1 != type2 ) {
	cerr << " init_test: void_type() is not unique" << endl;
	ok = false;
      }
    }
  }

  {
    const Type* type1 = mgr.boolean_type();
    if ( type1 == NULL ) {
      cerr << " init_test: boolean_type() == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kBooleanType ) {
      cerr << " init_test: boolean_type() is not kBooleanType" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.boolean_type();
      if ( type1 != type2 ) {
	cerr << " init_test: boolean_type() is not unique" << endl;
	ok = false;
      }
    }
  }

  {
    const Type* type1 = mgr.int_type();
    if ( type1 == NULL ) {
      cerr << " init_test: int_type() == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kIntType ) {
      cerr << " init_test: int_type() is not kIntType" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.int_type();
      if ( type1 != type2 ) {
	cerr << " init_test: int_type() is not unique" << endl;
	ok = false;
      }
    }
  }

  {
    const Type* type1 = mgr.float_type();
    if ( type1 == NULL ) {
      cerr << " init_test: float_type() == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kFloatType ) {
      cerr << " init_test: float_type() is not kFloatType" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.float_type();
      if ( type1 != type2 ) {
	cerr << " init_test: float_type() is not unique" << endl;
	ok = false;
      }
    }
  }

  {
    const Type* type1 = mgr.string_type();
    if ( type1 == NULL ) {
      cerr << " init_test: string_type() == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kStringType ) {
      cerr << " init_test: string_type() is not kStringType" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.string_type();
      if ( type1 != type2 ) {
	cerr << " init_test: string_type() is not unique" << endl;
	ok = false;
      }
    }
  }

  return ok;
}

bool
array_test()
{
  TypeMgr mgr;

  bool ok = true;

  const Type* int_type = mgr.int_type();
  {
    const Type* type1 = mgr.array_type(int_type);
    if ( type1 == NULL ) {
      cerr << " array_test: array_type(int_type) == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kArrayType ||
	      type1->elem_type() != int_type ) {
      cerr << " array_test: array_type(int_type) is not array(int)" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.array_type(int_type);
      if ( type1 != type2 ) {
	cerr << " array_test: array_type(int_type) is not unique" << endl;
	ok = false;
      }
    }
  }

  const Type* float_type = mgr.float_type();
  {
    const Type* type1 = mgr.array_type(float_type);
    if ( type1 == NULL ) {
      cerr << " array_test: array_type(float_type) == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kArrayType ||
	      type1->elem_type() != float_type ) {
      cerr << " array_test: array_type(float_type) is not array(float)" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.array_type(float_type);
      if ( type1 != type2 ) {
	cerr << " array_test: array_type(float_type) is not unique" << endl;
	ok = false;
      }
    }
  }

  return ok;
}

bool
set_test()
{
  TypeMgr mgr;

  bool ok = true;

  const Type* int_type = mgr.int_type();
  {
    const Type* type1 = mgr.set_type(int_type);
    if ( type1 == NULL ) {
      cerr << " set_test: set_type(int_type) == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kSetType ||
	      type1->elem_type() != int_type ) {
      cerr << " set_test: set_type(int_type) is not set(int)" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.set_type(int_type);
      if ( type1 != type2 ) {
	cerr << " set_test: set_type(int_type) is not unique" << endl;
	ok = false;
      }
    }
  }

  const Type* float_type = mgr.float_type();
  {
    const Type* type1 = mgr.set_type(float_type);
    if ( type1 == NULL ) {
      cerr << " set_test: set_type(float_type) == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kSetType ||
	      type1->elem_type() != float_type ) {
      cerr << " set_test: set_type(float_type) is not set(float)" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.set_type(float_type);
      if ( type1 != type2 ) {
	cerr << " set_test: set_type(float_type) is not unique" << endl;
	ok = false;
      }
    }
  }

  return ok;
}

bool
map_test()
{
  TypeMgr mgr;

  bool ok = true;

  const Type* int_type = mgr.int_type();
  const Type* float_type = mgr.float_type();
  {
    const Type* type1 = mgr.map_type(int_type, float_type);
    if ( type1 == NULL ) {
      cerr << " map_test: map_type(int_type, float_type) == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kMapType ||
	      type1->key_type() != int_type ||
	      type1->elem_type() != float_type ) {
      cerr << " map_test: map_type(int_type, float_type) is not map(int, float)" << endl;
      ok = false;
    }
    else {
      const Type* type2 = mgr.map_type(int_type, float_type);
      if ( type1 != type2 ) {
	cerr << " map_test: map_type(int_type, float) is not unique" << endl;
	ok = false;
      }
    }
  }

  return ok;
}

bool
func_test()
{
  TypeMgr mgr;

  bool ok = true;

  const Type* int_type = mgr.int_type();
  const Type* float_type = mgr.float_type();

  {
    vector<const Type*> input_type_list(1, int_type);
    const Type* type1 = mgr.function_type(float_type, input_type_list);
    if ( type1 == NULL ) {
      cerr << " func_test: function_type(float_type, (int_type)) == NULL" << endl;
      ok = false;
    }
    else if ( type1->type_id() != kFuncType ||
	      type1->function_output_type() != float_type ) {
      cerr << " func_test: function_type(float_type, (int_type)) is illegal" << endl;
      ok = false;
    }
    else {
      ymuint n = type1->function_input_num();
      if ( n != input_type_list.size() ) {
	cerr << " func_test: function_type(float_type, (int_type)) is illegal" << endl;
	ok = false;
      }
      else {
	for (ymuint i = 0; i < n; ++ i) {
	  if ( type1->function_input_type(i) != input_type_list[i] ) {
	    cerr << " func_test: function_type(float_type, (int_type)) is illegal" << endl;
	    ok = false;
	    break;
	  }
	}
      }
      const Type* type2 = mgr.function_type(float_type, input_type_list);
      if ( type1 != type2 ) {
	cerr << " func_test: function_type(float_type, (int_type)) is not unique" << endl;
	ok = false;
      }
    }
  }
  return ok;
}

int
TypeMgr_test(int argc,
	     char** argv)
{

  int nerr = 0;

  if ( !init_test() ) {
    cerr << "init_test failed" << endl;
    ++ nerr;
  }

  if ( !array_test() ) {
    cerr << "array_test failed" << endl;
    ++ nerr;
  }

  if ( !set_test() ) {
    cerr << "set_test failed" << endl;
    ++ nerr;
  }

  if ( !map_test() ) {
    cerr << "map_test failed" << endl;
    ++ nerr;
  }

  if ( !func_test() ) {
    cerr << "func_test failed" << endl;
    ++ nerr;
  }

  return nerr;
}

END_NAMESPACE_YM_YMSL


int
main(int argc,
     char** argv)
{
  return nsYm::nsYmsl::TypeMgr_test(argc, argv);
}
