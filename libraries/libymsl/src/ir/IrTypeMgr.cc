
/// @file IrTypeMgr.cc
/// @brief IrTypeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrTypeMgr.h"
#include "IrType.h"
#include "IrPrimType.h"
#include "IrArrayType.h"
#include "IrSetType.h"
#include "IrMapType.h"
#include "IrEnumType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrTypeMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
//
// この時点で組込型だけは登録されている．
IrTypeMgr::IrTypeMgr()
{
  init();
}

// @brief デストラクタ
IrTypeMgr::~IrTypeMgr()
{
  for (vector<IrType*>::iterator p = mTypeList.begin();
       p != mTypeList.end(); ++ p) {
    delete *p;
  }
}

// @brief void 型を得る．
const IrType*
IrTypeMgr::void_type()
{
  return mVoidType;
}

// @brief boolean 型を得る．
const IrType*
IrTypeMgr::boolean_type()
{
  return mBooleanType;
}

// @brief int 型を得る．
const IrType*
IrTypeMgr::int_type()
{
  return mIntType;
}

// @brief float 型を得る．
const IrType*
IrTypeMgr::float_type()
{
  return mFloatType;
}

// @brief string 型を得る．
const IrType*
IrTypeMgr::string_type()
{
  return mStringType;
}

// @brief array 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const IrType*
IrTypeMgr::array_type(const IrType* elem_type)
{
}

// @brief set 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const IrType*
IrTypeMgr::set_type(const IrType* elem_type)
{
}

// @brief map 型を得る．
// @param[in] elem_type 要素の型
// @param[in] key_type キーの型
//
// 登録されていなければ新たに登録する．
const IrType*
IrTypeMgr::map_type(const IrType* elem_type,
		      const IrType* key_type)
{
}

// @brief function 型を得る．
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
//
// 登録されていなければ新たに登録する．
const IrType*
IrTypeMgr::function_type(const IrType* output_type,
			   const vector<IrType*>& input_type_list)
{
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
IrType*
IrTypeMgr::enum_type(ShString name,
		       const vector<pair<ShString, int> >& elem_list)
{
  IrType* type = new_EnumType(name, elem_list);
  return type;
}

// @brief 組み込み型を登録する．
void
IrTypeMgr::init()
{
  mVoidType = new_PrimType(kVoidType);
  mBooleanType = new_PrimType(kBooleanType);
  mIntType = new_PrimType(kIntType);
  mFloatType = new_PrimType(kFloatType);
  mStringType = new_PrimType(kStringType);
}

// @brief プリミティブ型を作る．
// @param[in] type_id 型番号
IrType*
IrTypeMgr::new_PrimType(TypeId type_id)
{
  IrType* type = new IrPrimType(type_id);
  reg_type(type);
  return type;
}

// @brief array 型を作る．
// @param[in] elem_type 要素の型
IrType*
IrTypeMgr::new_ArrayType(const IrType* elem_type)
{
  IrType* type = new IrArrayType(elem_type);
  reg_type(type);
  return type;
}

// @brief set 型を作る．
// @param[in] elem_type 要素の型
IrType*
IrTypeMgr::new_SetType(const IrType* elem_type)
{
  IrType* type = new IrSetType(elem_type);
  reg_type(type);
  return type;
}

// @brief map 型を作る．
// @param[in] key_type キーの
// @param[in] elem_type 要素の型
IrType*
IrTypeMgr::new_MapType(const IrType* key_type,
			 const IrType* elem_type)
{
  IrType* type = new IrMapType(key_type, elem_type);
  reg_type(type);
  return type;
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
IrType*
IrTypeMgr::new_EnumType(ShString name,
			  const vector<pair<ShString, int> >& elem_list)
{
  IrType* type = new IrEnumType(name, elem_list);
  reg_type(type);
  return type;
}

// @brief 型を登録する．
// @param[in] type 登録する型
void
IrTypeMgr::reg_type(IrType* type)
{
  type->mId = mTypeList.size();
  mTypeList.push_back(type);
}

END_NAMESPACE_YM_YMSL
