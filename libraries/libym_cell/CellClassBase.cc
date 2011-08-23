
/// @file CellClassBase.cc
/// @brief CellClassBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellClassBase.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

// ダミーの定数0関数
TvFunc lDummy0Func;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CellClassBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellClassBase::CellClassBase() :
  mGroupNum(0),
  mGroupList(NULL)
{
}

// @brief デストラクタ
CellClassBase::~CellClassBase()
{
  // mGroupList は CellMgr が管理している．
}

// @brief 論理セルの場合に true を返す．
bool
CellClassBase::is_logic() const
{
  return false;
}

// @brief トライステートセルの場合に true を返す．
// @note もちろん論理セルでもある．
// @note 複数出力のうち1つでもトライステートなら true を返す．
bool
CellClassBase::is_tristate() const
{
  return false;
}

// @brief FFセルの場合に true を返す．
bool
CellClassBase::is_ff() const
{
  return false;
}

// @brief ラッチセルの場合に true を返す．
bool
CellClassBase::is_latch() const
{
  return false;
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CellClassBase::is_seq() const
{
  return false;
}

// @brief 論理セルの場合に出力ピン数を返す．
ymuint
CellClassBase::output_num() const
{
  return 0;
}

// @brief 論理セルの場合に関数を返す．
// @param[in] pos 出力番号
// @note 非論理セルの場合の返り値は不定
const TvFunc&
CellClassBase::logic_function(ymuint pos) const
{
  return lDummy0Func;
}

// @brief トライステートセルの場合にトライステート条件関数を返す．
// @param[in] pos 出力番号
// @note 通常の論理セルの場合には定数0関数を返す．
const TvFunc&
CellClassBase::tristate_function(ymuint pos) const
{
  return lDummy0Func;
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CellClassBase::has_clear() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CellClassBase::has_preset() const
{
  return false;
}

// @brief セルグループの数を返す．
ymuint
CellClassBase::group_num() const
{
  return mGroupNum;
}

// @brief セルグループを返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
const CellGroup*
CellClassBase::cell_group(ymuint pos) const
{
  return mGroupList[pos];
}

END_NAMESPACE_YM_CELL
