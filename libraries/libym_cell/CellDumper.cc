
/// @file CellDumper.cc
/// @brief CellDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellDumper.h"
#include "ym_cell/CellLibrary.h"

#if 0
#include "CiClass.h"
#include "CiGroup.h"

#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_cell/CellTiming.h"

#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/BinIO.h"
#endif

BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellDumper
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellDumper::CellDumper()
{
}

// @brief デストラクタ
CellDumper::~CellDumper()
{
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
// @param[in] library ダンプ対象のライブラリ
void
CellDumper::operator()(ostream& s,
		       const CellLibrary& library)
{
  library.dump(s);
}

END_NAMESPACE_YM_CELL
