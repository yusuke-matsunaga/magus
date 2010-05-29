#ifndef YM_CELL_CELLMISLIBREADER_H
#define YM_CELL_CELLMISLIBREADER_H

/// @file ym_cell/CellMislibReader.h
/// @brief CellMislibReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellMislibReader.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_type.h"


BEGIN_NAMESPACE_YM_BNET

//////////////////////////////////////////////////////////////////////
/// @class CellMislibReader CellMislibReader.h <ym_cell/CellMislibReader.h>
/// @brief mislib 形式のファイルを読み込んで CellLibrary に設定するクラス
/// @sa CellLibrary Cell
//////////////////////////////////////////////////////////////////////
class CellMislibReader
{
public:

  /// @brief コンストラクタ
  CellMislibReader();

  /// @brief デストラクタ
  ~CellMislibReader();

  
public:

  /// @brief mislib 形式のファイルを読み込む
  /// @param[in] in 入力元のストリーム
  /// @param[in] filename ファイル名
  /// @param[out] library 読み込んだ内容を設定するネットワーク
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  read(istream& in,
       const string& filename,
       CellLibrary& library);

  /// @brief メッセージハンドラを付加する．
  void
  add_msg_handler(MsgHandler* msg_handler);
  
};

END_NAMESPACE_YM_BNET

#endif // YM_CELL_CELLMISLIBREADER_H
