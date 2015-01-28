
/// @file TypeMgr.cc
/// @brief TypeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "TypeMgr.h"
#include "Type.h"
#include "PrimType.h"
#include "ArrayType.h"
#include "SetType.h"
#include "MapType.h"
#include "FuncType.h"
#include "EnumType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス TypeMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
//
// この時点で組込型だけは登録されている．
TypeMgr::TypeMgr()
{
  init();
}

// @brief デストラクタ
TypeMgr::~TypeMgr()
{
  for (vector<Type*>::iterator p = mTypeList.begin();
       p != mTypeList.end(); ++ p) {
    delete *p;
  }
}

// @brief void 型を得る．
const Type*
TypeMgr::void_type()
{
  return mVoidType;
}

// @brief boolean 型を得る．
const Type*
TypeMgr::boolean_type()
{
  return mBooleanType;
}

// @brief int 型を得る．
const Type*
TypeMgr::int_type()
{
  return mIntType;
}

// @brief float 型を得る．
const Type*
TypeMgr::float_type()
{
  return mFloatType;
}

// @brief string 型を得る．
const Type*
TypeMgr::string_type()
{
  return mStringType;
}

// @brief array 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const Type*
TypeMgr::array_type(const Type* elem_type)
{
}

// @brief set 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const Type*
TypeMgr::set_type(const Type* elem_type)
{
}

// @brief map 型を得る．
// @param[in] elem_type 要素の型
// @param[in] key_type キーの型
//
// 登録されていなければ新たに登録する．
const Type*
TypeMgr::map_type(const Type* elem_type,
		  const Type* key_type)
{
}

// @brief function 型を得る．
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
//
// 登録されていなければ新たに登録する．
const Type*
TypeMgr::function_type(const Type* output_type,
		       const vector<const Type*>& input_type_list)
{
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
const Type*
TypeMgr::enum_type(ShString name,
		   const vector<pair<ShString, int> >& elem_list)
{
  const Type* type = new_EnumType(name, elem_list);
  return type;
}

// @brief 組み込み型を登録する．
void
TypeMgr::init()
{
  mVoidType = new_PrimType(kVoidType);
  mBooleanType = new_PrimType(kBooleanType);
  mIntType = new_PrimType(kIntType);
  mFloatType = new_PrimType(kFloatType);
  mStringType = new_PrimType(kStringType);
}

// @brief プリミティブ型を作る．
// @param[in] type_id 型番号
const Type*
TypeMgr::new_PrimType(TypeId type_id)
{
  Type* type = new PrimType(type_id);
  reg_type(type);
  return type;
}

// @brief array 型を作る．
// @param[in] elem_type 要素の型
const Type*
TypeMgr::new_ArrayType(const Type* elem_type)
{
  Type* type = new ArrayType(elem_type);
  reg_type(type);
  return type;
}

// @brief set 型を作る．
// @param[in] elem_type 要素の型
const Type*
TypeMgr::new_SetType(const Type* elem_type)
{
  Type* type = new SetType(elem_type);
  reg_type(type);
  return type;
}

// @brief map 型を作る．
// @param[in] key_type キーの
// @param[in] elem_type 要素の型
const Type*
TypeMgr::new_MapType(const Type* key_type,
		     const Type* elem_type)
{
  Type* type = new MapType(key_type, elem_type);
  reg_type(type);
  return type;
}

// @brief function 型を作る．
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
const Type*
TypeMgr::new_FuncType(const Type* output_type,
		      const vector<const Type*>& input_type_list)
{
  Type* type = new FuncType(output_type, input_type_list);
  reg_type(type);
  return type;
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
const Type*
TypeMgr::new_EnumType(ShString name,
		      const vector<pair<ShString, int> >& elem_list)
{
  Type* type = new EnumType(name, elem_list);
  reg_type(type);
  return type;
}

// @brief 型を登録する．
// @param[in] type 登録する型
void
TypeMgr::reg_type(Type* type)
{
  type->mId = mTypeList.size();
  mTypeList.push_back(type);
}

END_NAMESPACE_YM_YMSL
