
/// @file libym_techmap/TmCell.cc
/// @brief TmCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "TmCell.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
// クラス TmCell
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
// @param[in] input_num 入力ピン数
TmCell::TmCell(ymuint input_num) :
  mInputNum(input_num),
  mInputArray(new TmCellInput[input_num])
{
}

// @brief デストラクタ
TmCell::~TmCell()
{
  delete [] mInputArray;
}


//////////////////////////////////////////////////////////////////////
// クラス TmCellMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TmCellMgr::TmCellMgr()
{
}

// @brief デストラクタ
TmCellMgr::~TmCellMgr()
{
}

// @brief データを読み込む．
// @brief s 入力ストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
TmCellMgr::load(istream& s)
{
  //
}

END_NAMESPACE_YM_TECHMAP
