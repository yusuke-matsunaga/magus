
/// @file IrEnumType.cc
/// @brief IrEnumType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrEnumType.h"
#include "IrEnumConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrEnumConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrEnumConst::IrEnumConst()
{
}

// @brief デストラクタ
IrEnumConst::~IrEnumConst()
{
}

// @brief enum 型を返す．
IrType*
IrEnumConst::parent() const
{
  return mParent;
}

// @brief 名前を返す．
ShString
IrEnumConst::name() const
{
  return mName;
}

// @brief 値を返す．
int
IrEnumConst::val() const
{
  return mVal;
}


//////////////////////////////////////////////////////////////////////
// クラス IrEnumType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] elem_list 要素名と値のリスト
IrEnumType::IrEnumType(ShString name,
		       const vector<pair<ShString, int> >& elem_list) :
  IrNamedType(name),
  mElemNum(elem_list.size())
{
  mElemArray = new IrEnumConst[mElemNum];
  for (ymuint i = 0; i < mElemNum; ++ i) {
    IrEnumConst& enum_const = mElemArray[i];
    enum_const.mParent = this;
    enum_const.mName = elem_list[i].first;
    enum_const.mVal = elem_list[i].second;
  }
}

// @brief デストラクタ
IrEnumType::~IrEnumType()
{
  delete [] mElemArray;
}

// @brief 型を得る．
TypeId
IrEnumType::type_id() const
{
  return kEnumType;
}

// @brief 列挙型の数を得る．
//
// enum のみ有効
ymuint
IrEnumType::enum_num() const
{
  return mElemNum;
}

// @brief 列挙型の定数値を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
int
IrEnumType::enum_elem_val(ymuint index) const
{
  ASSERT_COND( index < enum_num() );
  return mElemArray[index].val();
}

// @brief 列挙型の定数名を得る．
// @param[in] index インデックス ( 0 <= index < enum_num() )
//
// enum のみ有効
ShString
IrEnumType::enum_elem_name(ymuint index) const
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
IrEnumType::enum_index(ShString name) const
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
IrEnumType::print(ostream& s) const
{
  s << type_name();
}

END_NAMESPACE_YM_YMSL
