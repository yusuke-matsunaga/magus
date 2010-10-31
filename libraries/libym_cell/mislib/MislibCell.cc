
/// @file libym_cell/mislib/MislibCell.cc
/// @brief MislibCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MislibCell.h"
#include "MislibPin.h"
#include "MislibTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス MislibCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MislibCell::MislibCell()
{
}

// @brief デストラクタ
MislibCell::~MislibCell()
{
}

// @brief 名前の取得
string
MislibCell::name() const
{
  return mName;
}

// @brief 面積の取得
CellArea
MislibCell::area() const
{
  return mArea;
}

// @brief ピン数の取得
ymuint
MislibCell::pin_num() const
{
  return mInputPinNum + 1;
}

// @brief ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const CellPin*
MislibCell::pin(ymuint pos) const
{
  if ( pos == 0 ) {
    return &mOutputPin;
  }
  else {
    return &mInputPinArray[pos - 1];
  }
}

// @brief 名前からピンの取得
// @param[in] name ピン名
// @return name という名前をピンを返す．
// @note なければ NULL を返す．
const CellPin*
MislibCell::pin(const string& name) const
{
  if ( mOutputPin.name() == name ) {
    return &mOutputPin;
  }

  // とりあえずナイーブなリニアサーチ
  for (ymuint i = 0; i < mInputPinNum; ++ i) {
    if ( mInputPinArray[i].name() == name ) {
      return &mInputPinArray[i];
    }
  }
  return NULL;
}

// @brief バス数の取得
ymuint
MislibCell::bus_num() const
{
  return 0;
}

// @brief バスの取得
// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
const CellBus*
MislibCell::bus(ymuint pos) const
{
  return NULL;
}

// @brief 名前からバスの取得
// @param[in] name バス名
// @return name という名前のバスを返す．
// @note なければ NULL を返す．
const CellBus*
MislibCell::bus(const string& name) const
{
  return NULL;
}

// @brief バンドル数の取得
ymuint
MislibCell::bundle_num() const
{
  return 0;
}

// @brief バンドルの取得
// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
const CellBundle*
MislibCell::bundle(ymuint pos) const
{
  return NULL;
}

// @brief 名前からバンドルの取得
const CellBundle*
MislibCell::bundle(const string& name) const
{
  return NULL;
}

END_NAMESPACE_YM_CELL
