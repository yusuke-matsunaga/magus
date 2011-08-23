
/// @file CellGroupBase.cc
/// @brief CellGroupBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroupBase.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellGroupBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellGroupBase::CellGroupBase() :
  mCellClass(NULL),
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
CellGroupBase::~CellGroupBase()
{
  // mCellList は CellMgr が管理する．
}

// @brief 属している CellClass を返す．
const CellClass*
CellGroupBase::cell_class() const
{
  return mCellClass;
}

// @brief 論理セルの場合に関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
// @note 非論理セルの場合の返り値は不定
const TvFunc&
CellGroupBase::logic_function(ymuint pos) const
{
}

// @brief トライステートセルの場合にトライステート条件関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
// @note 通常の論理セルの場合には定数0関数を返す．
const TvFunc&
CellGroupBase::tristate_function(ymuint pos) const
{
}

// @brief 論理セルの場合に入力ピン番号を返す．
// @param[in] pos CellClass の関数における入力番号
ymuint
CellGroupBase::input_pos(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 論理セルの場合に入力ピンの反転属性を返す．
// @param[in] pos CellClass の関数における入力番号
bool
CellGroupBase::input_inv(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief 論理セルの場合に出力ピン番号を返す．
// @param[in] pos CellClass の関数における出力番号
ymuint
CellGroupBase::output_pos(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 論理セルの場合に出力ピンの反転属性を返す．
// @param[in] pos CellClass の関数における出力番号
bool
CellGroupBase::output_inv(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief FF/ラッチセルの場合にデータ入力ピン番号を返す．
ymuint
CellGroupBase::data_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief FF/ラッチセルの場合にデータ入力ピンの反転属性を返す．
bool
CellGroupBase::data_inv() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief FFセルの場合にクロック入力ピン番号を返す．
ymuint
CelLGroupBase::clock_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief FFセルの場合にクロック入力ピンの反転属性を返す．
// @note ポジティブエッジが正極性だと思う．
bool
CellGroupBase::clock_inv() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ラッチセルの場合にイネーブル入力ピン番号を返す．
ymuint
CellGroupBase::enable_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief ラッチセルの場合にイネーブル入力ピンの反転属性を返す．
bool
CellGroupBase::enable_inv() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief FF/ラッチセルの場合にクリア入力ピン番号を返す．
ymuint
CellGroupBase::clear_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief FF/ラッチセルの場合にクリア入力ピンの反転属性を返す．
bool
CellGroupBase::clear_inv() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief FF/ラッチセルの場合にプリセット入力ピン番号を返す．
ymuint
CellGroupBase::clear_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief FF/ラッチセルの場合にプリセット入力ピンの反転属性を返す．
bool
CelLGroupBase::clear_inv() const
{
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief FF/ラッチセルの場合に Q出力ピン番号を返す．
ymuint
CelLGroupBase::q_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief FF/ラッチセルの場合に XQ出力ピン番号を返す．
ymuint
CellGroupBase::xq_pos() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief セル数を返す．
ymuint
CellGroupBase::cell_num() const
{
  return mCellNum;
}

// @brief セルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
const Cell*
CellGroupBase::cell(ymuint pos) const
{
  assert_cond( pos < mCellNum, __FILE__, __LINE__);
  return mCellList[pos];
}

END_NAMESPACE_YM_CELL
