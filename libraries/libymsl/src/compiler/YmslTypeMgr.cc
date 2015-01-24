
/// @file YmslTypeMgr.cc
/// @brief YmslTypeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslTypeMgr.h"
#include "YmslType.h"
#include "YmslPrimType.h"
#include "YmslArrayType.h"
#include "YmslSetType.h"
#include "YmslMapType.h"
#include "YmslEnumType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslTypeMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
//
// この時点で組込型だけは登録されている．
YmslTypeMgr::YmslTypeMgr()
{
  init();
}

// @brief デストラクタ
YmslTypeMgr::~YmslTypeMgr()
{
  for (vector<YmslType*>::iterator p = mTypeList.begin();
       p != mTypeList.end(); ++ p) {
    delete *p;
  }
}

// @brief void 型を得る．
const YmslType*
YmslTypeMgr::void_type()
{
  return mVoidType;
}

// @brief boolean 型を得る．
const YmslType*
YmslTypeMgr::boolean_type()
{
  return mBooleanType;
}

// @brief int 型を得る．
const YmslType*
YmslTypeMgr::int_type()
{
  return mIntType;
}

// @brief float 型を得る．
const YmslType*
YmslTypeMgr::float_type()
{
  return mFloatType;
}

// @brief string 型を得る．
const YmslType*
YmslTypeMgr::string_type()
{
  return mStringType;
}

// @brief array 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const YmslType*
YmslTypeMgr::array_type(const YmslType* elem_type)
{
}

// @brief set 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const YmslType*
YmslTypeMgr::set_type(const YmslType* elem_type)
{
}

// @brief map 型を得る．
// @param[in] elem_type 要素の型
// @param[in] key_type キーの型
//
// 登録されていなければ新たに登録する．
const YmslType*
YmslTypeMgr::map_type(const YmslType* elem_type,
		      const YmslType* key_type)
{
}

// @brief function 型を得る．
// @param[in] output_type 出力の型
// @param[in] input_type_list 入力の型のリスト
//
// 登録されていなければ新たに登録する．
const YmslType*
YmslTypeMgr::function_type(const YmslType* output_type,
			   const vector<YmslType*>& input_type_list)
{
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
YmslType*
YmslTypeMgr::enum_type(ShString name,
		       const vector<pair<ShString, int> >& elem_list)
{
  YmslType* type = new_EnumType(name, elem_list);
  return type;
}

// @brief 組み込み型を登録する．
void
YmslTypeMgr::init()
{
  mVoidType = new_PrimType(kVoidType);
  mBooleanType = new_PrimType(kBooleanType);
  mIntType = new_PrimType(kIntType);
  mFloatType = new_PrimType(kFloatType);
  mStringType = new_PrimType(kStringType);
}

// @brief プリミティブ型を作る．
// @param[in] type_id 型番号
YmslType*
YmslTypeMgr::new_PrimType(TypeId type_id)
{
  YmslType* type = new YmslPrimType(type_id);
  reg_type(type);
  return type;
}

// @brief array 型を作る．
// @param[in] elem_type 要素の型
YmslType*
YmslTypeMgr::new_ArrayType(const YmslType* elem_type)
{
  YmslType* type = new YmslArrayType(elem_type);
  reg_type(type);
  return type;
}

// @brief set 型を作る．
// @param[in] elem_type 要素の型
YmslType*
YmslTypeMgr::new_SetType(const YmslType* elem_type)
{
  YmslType* type = new YmslSetType(elem_type);
  reg_type(type);
  return type;
}

// @brief map 型を作る．
// @param[in] key_type キーの
// @param[in] elem_type 要素の型
YmslType*
YmslTypeMgr::new_MapType(const YmslType* key_type,
			 const YmslType* elem_type)
{
  YmslType* type = new YmslMapType(key_type, elem_type);
  reg_type(type);
  return type;
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
YmslType*
YmslTypeMgr::new_EnumType(ShString name,
			  const vector<pair<ShString, int> >& elem_list)
{
  YmslType* type = new YmslEnumType(name, elem_list);
  reg_type(type);
  return type;
}

// @brief 型を登録する．
// @param[in] type 登録する型
void
YmslTypeMgr::reg_type(YmslType* type)
{
  type->mId = mTypeList.size();
  mTypeList.push_back(type);
}

END_NAMESPACE_YM_YMSL
