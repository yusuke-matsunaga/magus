
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
MislibCell::MislibCell() :
  mPinNum(0U),
  mPinArray(NULL),
  mTimingArray(NULL)
{
}

// @brief デストラクタ
MislibCell::~MislibCell()
{
  delete [] mPinArray;
  delete [] mTimingArray;
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
  return mPinNum;
}

// @brief ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const CellPin*
MislibCell::pin(ymuint pos) const
{
  return mPinArray[pos];
}

// @brief 名前からピンの取得
// @param[in] name ピン名
// @return name という名前をピンを返す．
// @note なければ NULL を返す．
const CellPin*
MislibCell::pin(const string& name) const
{
  // とりあえずナイーブなリニアサーチ
  for (ymuint i = 0; i < mPinNum; ++ i) {
    if ( mPinArray[i]->name() == name ) {
      return mPinArray[i];
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

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[out] timing_list タイミング情報を納めるベクタ
// @return 条件に合致するタイミング情報の数を返す．
ymuint
MislibCell::timing(ymuint ipos,
		   ymuint opos,
		   vector<const CellTiming*>& timing_list) const
{
  timing_list.clear();
  if ( opos != 0 || ipos == 0 ) {
    return 0;
  }
  timing_list.push_back(mTimingArray[ipos - 1]);
  return 1;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @param[in] timing_type タイミング情報の種類
// @param[out] timing_list タイミング情報を納めるベクタ
// @return 条件に合致するタイミング情報の数を返す．
ymuint
MislibCell::timing(ymuint ipos,
		   ymuint opos,
		   tCellTimingSense timing_sense,
		   tCellTimingType timing_type,
		   vector<const CellTiming*>& timing_list) const
{
  timing_list.clear();
  if ( opos != 0 || ipos == 0 ) {
    return 0;
  }
  CellTiming* timing = mTimingArray[ipos - 1];
  if ( timing->timing_sense() == timing_sense &&
       timing->timing_type() == timing_type ) {
    timing_list.push_back(timing);
  }
  return timing_list.size();
}

END_NAMESPACE_YM_CELL
