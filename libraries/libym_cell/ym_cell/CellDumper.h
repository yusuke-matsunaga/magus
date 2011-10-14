#ifndef YM_CELL_CELLDUMPER_H
#define YM_CELL_CELLDUMPER_H

/// @file ym_cell/CellDumper.h
/// @brief CellDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellDumper CellDumper.h "ym_cell/CelLDumper.h"
/// @brief CellLibrary の内容をバイナリダンプするクラス
//////////////////////////////////////////////////////////////////////
class CellDumper
{
public:

  /// @brief コンストラクタ
  CellDumper();

  /// @brief デストラクタ
  ~CellDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // 唯一の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  /// @param[in] library ダンプ対象のライブラリ
  void
  operator()(ostream& s,
	     const CellLibrary& library);

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLDUMPER_H
