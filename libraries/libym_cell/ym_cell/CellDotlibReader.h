#ifndef YM_CELL_CELLDOTLIBREADER_H
#define YM_CELL_CELLDOTLIBREADER_H

/// @file ym_cell/CellDotlibReader.h
/// @brief CellDotlibReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellDotlibReader.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_type.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_CELL

namespace nsDotlib {
  class DotlibParser;
}

//////////////////////////////////////////////////////////////////////
/// @class CellDotlibReader CellDotlibReader.h "ym_cell/CellDotlibReader.h"
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
  read(const string& filename);

  /// @brief メッセージハンドラを付加する．
  void
  add_msg_handler(MsgHandler* msg_handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // dotlib のパーサー
  nsDotlib::DotlibParser* mParser;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLDOTLIBREADER_H
