
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

BEGIN_NONAMESPACE

// array 型用のハッシュ関数
inline
ymuint
hash_array(const Type* elem_type)
{
  return elem_type->id();
}

// set 型用のハッシュ関数
inline
ymuint
hash_set(const Type* elem_type)
{
  return elem_type->id() + 511;
}

// map 型用のハッシュ関数
inline
ymuint
hash_map(const Type* key_type,
	 const Type* elem_type)
{
  return key_type->id() * 2047 + elem_type->id() + 511;
}

// function 型用のハッシュ関数
inline
ymuint
hash_func(const Type* output_type,
	  const vector<const Type*>& input_type_list)
{
  ymuint h = output_type->id() + 253;
  ymuint n = input_type_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    const Type* t = input_type_list[i];
    h += (t->id() * i);
  }
  return h;
}

// function 型用のハッシュ関数
inline
ymuint
hash_func2(const Type* func_type)
{
  ASSERT_COND( func_type->type_id() == kFuncType );
  ymuint h = func_type->function_output_type()->id() + 253;
  ymuint n = func_type->function_input_num();
  for (ymuint i = 0; i < n; ++ i) {
    const Type* t = func_type->function_input_type(i);
    h += (t->id() * i);
  }
  return h;
}

END_NONAMESPACE

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
  clear();
}

// @brief 内容をクリアする．
void
TypeMgr::clear()
{
  for (vector<Type*>::iterator p = mTypeList.begin();
       p != mTypeList.end(); ++ p) {
    delete *p;
  }

  mVoidType = NULL;
  mBooleanType = NULL;
  mIntType = NULL;
  mFloatType = NULL;
  mStringType = NULL;

  mHashSize = 0;
  mHashTable = NULL;
  mHashNum = 0;
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
  ymuint h = hash_array(elem_type) % mHashSize;
  for (Type* type = mHashTable[h]; type != NULL; type = type->mLink) {
    if ( type->type_id() == kArrayType && type->elem_type() == elem_type ) {
      return type;
    }
  }
  Type* type = new ArrayType(elem_type);
  reg_type(type);
  return type;
}

// @brief set 型を得る．
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const Type*
TypeMgr::set_type(const Type* elem_type)
{
  ymuint h = hash_set(elem_type) % mHashSize;
  for (Type* type = mHashTable[h]; type != NULL; type = type->mLink) {
    if ( type->type_id() == kSetType && type->elem_type() == elem_type ) {
      return type;
    }
  }
  Type* type = new SetType(elem_type);
  reg_type(type);
  return type;
}

// @brief map 型を得る．
// @param[in] key_type キーの型
// @param[in] elem_type 要素の型
//
// 登録されていなければ新たに登録する．
const Type*
TypeMgr::map_type(const Type* key_type,
		  const Type* elem_type)
{
  ymuint h = hash_map(key_type, elem_type) % mHashSize;
  for (Type* type = mHashTable[h]; type != NULL; type = type->mLink) {
    if ( type->type_id() == kMapType &&
	 type->key_type() == key_type &&
	 type->elem_type() == elem_type ) {
      return type;
    }
  }
  Type* type = new MapType(key_type, elem_type);
  reg_type(type);
  return type;
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
  ymuint h = hash_func(output_type, input_type_list) % mHashSize;
  for (Type* type = mHashTable[h]; type != NULL; type = type->mLink) {
    if ( type->type_id() == kFuncType &&
	 type->function_output_type() == output_type ) {
      ymuint n = input_type_list.size();
      if ( type->function_input_num() != n ) {
	continue;
      }
      bool differ = false;
      for (ymuint i = 0; i < n; ++ i ) {
	if ( type->function_input_type(i) != input_type_list[i] ) {
	  differ = true;
	  break;
	}
      }
      if ( differ ) {
	continue;
      }
      return type;
    }
  }
  Type* type = new FuncType(output_type, input_type_list);
  reg_type(type);
  return type;
}

// @brief enum 型を作る．
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
const Type*
TypeMgr::enum_type(ShString name,
		   const vector<pair<ShString, int> >& elem_list)
{
  Type* type = new EnumType(name, elem_list);
  reg_type(type);
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

  mHashSize = 0;
  mHashTable = NULL;
  mHashNum = 0;
  alloc_table(1024);
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

// @brief 型を登録する．
// @param[in] type 登録する型
void
TypeMgr::reg_type(Type* type)
{
  type->mId = mTypeList.size();
  mTypeList.push_back(type);

  ymuint h;
  switch ( type->type_id() ) {
  case kArrayType:
    h = hash_array(type->elem_type());
    break;

  case kSetType:
    h = hash_set(type->elem_type());
    break;

  case kMapType:
    h = hash_map(type->key_type(), type->elem_type());
    break;

  case kFuncType:
    h = hash_func2(type);
    break;

  default:
    return;
  }

  if ( mHashNum >= mNextLimit ) {
    alloc_table(mHashSize * 2);
  }
  h = h % mHashSize;
  type->mLink = mHashTable[h];
  mHashTable[h] = type;
  ++ mHashNum;
}

void
TypeMgr::alloc_table(ymuint req_size)
{
  ymuint old_size = mHashSize;
  Type** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<ymuint>(mHashSize * 1.8);
  mHashTable = new Type*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  for (ymuint i = 0; i < old_size; ++ i) {
    for (Type* type = old_table[i];
	 type != NULL; ) {
      Type* next = type->mLink;
      reg_type(type);
      type = next;
    }
  }
  delete [] old_table;
}

END_NAMESPACE_YM_YMSL
