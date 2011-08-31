
/// @file CiLogicClass.cc
/// @brief CiLogicClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiLogicClass.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLogicClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLogicClass::CiLogicClass() :
  mNo(0),
  mLogicArray(NULL)
{
}

// @brief デストラクタ
CiLogicClass::~CiLogicClass()
{
  // mLogicArray は CellMgr が管理する．
}

// @brief 論理セルの場合に true を返す．
bool
CiLogicClass::is_logic() const
{
  return true;
}

// @brief 論理セルの場合に出力ピン数を返す．
ymuint
CiLogicClass::output_num() const
{
  return mNo;
}

// @brief 論理セルの場合に関数を返す．
// @param[in] pos 出力番号
// @note 非論理セルの場合の返り値は不定
TvFunc
CiLogicClass::logic_function(ymuint pos) const
{
  return mLogicArray[pos];
}

END_NAMESPACE_YM_CELL
