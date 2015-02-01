
/// @file EnumType.cc
/// @brief EnumType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "EnumType.h"
#include "EnumConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス EnumConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EnumConst::EnumConst()
{
}

// @brief デストラクタ
EnumConst::~EnumConst()
{
}

// @brief enum 型を返す．
const Type*
EnumConst::parent_type() const
{
  return mParentType;
}

// @brief 名前を返す．
ShString
EnumConst::name() const
{
  return mName;
}

// @brief 値を返す．
int
EnumConst::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス EnumType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
EnumType::EnumType(ShString name,
		   const vector<pair<ShString, int> >& elem_list) :
  NamedType(kEnumType, name),
  mElemNum(elem_list.size())
{
  mElemArray = new EnumConst[mElemNum];
  for (ymuint i = 0; i < mElemNum; ++ i) {
    EnumConst& enum_const = mElemArray[i];
    enum_const.mParentType = this;
    enum_const.mName = elem_list[i].first;
    enum_const.mVal = elem_list[i].second;
  }
}

// @brief デストラクタ
EnumType::~EnumType()
{
  delete [] mElemArray;
}

// @brief 指定された型にキャスト可能な場合に true を返す．
// @param[in] type 指定された型
bool
EnumType::castable_to(const Type* type) const
{
  return type == this;
}

// @brief 列挙型の数を得る．
//
// enum のみ有効
ymuint
EnumType::enum_num() const
{
  return mElemNum;
}

// @brief 列挙型の定数値を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
int
EnumType::enum_elem_val(ymuint index) const
{
  ASSERT_COND( index < enum_num() );
  return mElemArray[index].val();
}

// @brief 列挙型の定数名を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
ShString
EnumType::enum_elem_name(ymuint index) const
{
  ASSERT_COND( index < enum_num() );
  return mElemArray[index].name();
}

// @brief 列挙型のインデックスを得る．
// @param[in] name 列挙型の定数の名前
//
// enum のみ有効
// 該当する名前がなければ -1 を返す．
int
EnumType::enum_index(ShString name) const
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
EnumType::print(ostream& s) const
{
  s << type_name();
}

END_NAMESPACE_YM_YMSL
