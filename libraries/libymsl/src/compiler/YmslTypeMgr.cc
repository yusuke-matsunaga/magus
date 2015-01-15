
/// @file YmslTypeMgr.cc
/// @brief YmslTypeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslTypeMgr.h"
#include "YmslType.h"
#include "YmslSimpleType.h"


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

/// @brief 型を登録する．
/// @param[in] type 登録する型
void
YmslTypeMgr::reg_type(YmslType* type)
{
  type->mId = mTypeList.size();
  mTypeList.push_back(type);
}

// @brief 組み込み型を登録する．
void
YmslTypeMgr::init()
{
  mVoidType = new YmslSimpleType(kVoidType);
  mBooleanType = new YmslSimpleType(kBooleanType);
  mIntType = new YmslSimpleType(kIntType);
  mFloatType = new YmslSimpleType(kFloatType);
  mStringType = new YmslSimpleType(kStringType);

  reg_type(mVoidType);
  reg_type(mBooleanType);
  reg_type(mIntType);
  reg_type(mFloatType);
  reg_type(mStringType);
}

END_NAMESPACE_YM_YMSL
