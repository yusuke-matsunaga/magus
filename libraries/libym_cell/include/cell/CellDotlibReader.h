#ifndef CELL_CELLDOTLIBREADER_H
#define CELL_CELLDOTLIBREADER_H

/// @file cell/CellDotlibReader.h
/// @brief CellDotlibReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellDotlibReader CellDotlibReader.h "cell/CellDotlibReader.h"
/// @brief dotlib(liberty) を読み込んで CellLibrary に設定するクラス
/// @sa CellLibrary Cell
//////////////////////////////////////////////////////////////////////
class CellDotlibReader
{
public:

  /// @brief コンストラクタ
  CellDotlibReader();

  /// @brief デストラクタ
  ~CellDotlibReader();


public:

  /// @brief mislib 形式のファイルを読み込んでライブラリを生成する．
  /// @param[in] filename ファイル名
  /// @return 読み込んで作成したセルライブラリを返す．
  /// @note エラーが起きたら NULL を返す．
  const CellLibrary*
  operator()(const string& filename);

};

END_NAMESPACE_YM_CELL

#endif // CELL_CELLDOTLIBREADER_H
