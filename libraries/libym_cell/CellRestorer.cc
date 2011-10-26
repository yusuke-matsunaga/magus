
/// @file restore.cc
/// @brief バイナリファイルを読み込む関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellRestorer.h"
#include "ym_cell/CellLibrary.h"

#if 0
#include "CiLibrary.h"
#include "CiClass.h"
#include "CiGroup.h"

#include "ym_cell/CellArea.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellResistance.h"
#include "ym_cell/CellTime.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/BinIO.h"
#endif

BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellRestorer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellRestorer::CellRestorer()
{
}

// @brief デストラクタ
CellRestorer::~CellRestorer()
{
}

// @brief バイナリファイルを読み込んでセルライブラリを作る．
// @param[in] s 入力元のストリーム
// @return 生成されたセルライブラリを返す．
const CellLibrary*
CellRestorer::operator()(istream& s)
{
  CellLibrary* lib = CellLibrary::new_obj();
  lib->restore(s);
  return lib;
}


END_NAMESPACE_YM_CELL
