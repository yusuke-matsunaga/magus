
/// @file YmslEnumType.cc
/// @brief YmslEnumType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslEnumType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslEnumConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslEnumConst::YmslEnumConst()
{
}

// @brief デストラクタ
YmslEnumConst::~YmslEnumConst()
{
}

// @brief enum 型を返す．
YmslType*
YmslEnumConst::parent() const
{
  return mParent;
}

// @brief 名前を返す．
ShString
YmslEnumConst::name() const
{
  return mName;
}

// @brief 値を返す．
ymuint
YmslEnumConst::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス YmslEnumType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] elem_list 要素のリスト
YmslEnumType::YmslEnumType(ShString name,
			   const vector<ShString>& elem_list) :
  mName(name),
  mElemNum(elem_list.size())
{
  mElemArray = new YmslEnumConst[mElemNum];
  for (ymuint i = 0; i < mElemNum; ++ i) {
    YmslEnumConst& enum_const = mElemArray[i];
    enum_const.mParent = this;
    enum_const.mName = elem_list[i];
    enum_const.mVal = i;
  }
}

// @brief デストラクタ
YmslEnumType::~YmslEnumType()
{
  delete [] mElemArray;
}

// @brief 型を得る．
TypeId
YmslEnumType::type_id() const
{
  return kEnumType;
}

// @brief 型名を得る．
//
// enum のみ有効
ShString
YmslEnumType::type_name() const
{
  return mName;
}

// @brief 列挙型の数を得る．
//
// enum のみ有効
ymuint
YmslEnumType::enum_num() const
{
  return mElemNum;
}

// @brief 列挙型の定数を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
const YmslEnumConst*
YmslEnumType::enum_elem(ymuint index) const
{
  ASSERT_COND( index < enum_num() );
  return &mElemArray[index];
}

// @brief 列挙型のインデックスを得る．
// @param[in] name 列挙型の定数の名前
//
// enum のみ有効
// 該当する名前がなければ -1 を返す．
int
YmslEnumType::enum_index(ShString name) const
{
  // とりあえず線形探索
  for (ymuint i = 0; i < mElemNum; ++ i) {
    if ( mElemArray[i].mName == name ) {
      return i;
    }
  }
  return -1;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
YmslEnumType::print(ostream& s) const
{
  s << type_name();
}

END_NAMESPACE_YM_YMSL
