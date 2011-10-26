#ifndef YM_CELL_CELLRESTORER_H
#define YM_CELL_CELLRESTORER_H

/// @file ym_cell/CellRestorer.h
/// @brief CellRestorer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellRestorer CellRestorer.h "ym_cell/CellRestorer.h"
/// @brief バイナリファイルを読み込んでセルライブラリを作るクラス
//////////////////////////////////////////////////////////////////////
class CellRestorer
{
public:

  /// @brief コンストラクタ
  CellRestorer();

  /// @brief デストラクタ
  ~CellRestorer();


public:
  //////////////////////////////////////////////////////////////////////
  // 唯一の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルを読み込んでセルライブラリを作る．
  /// @param[in] s 入力元のストリーム
  /// @return 生成されたセルライブラリを返す．
  const CellLibrary*
  operator()(istream& s);

};

END_NAMESPACE_YM_CELL


#endif // YM_CELL_CELLRESTORER_H
