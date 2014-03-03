
/// @file CellMislibReader.sip
/// @brief CellMislibReader の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellMislibReader
{
%TypeHeaderCode
#include "cell/CellMislibReader.h"
using nsYm::CellMislibReader;
%End

public:

  /// @brief mislib 形式のファイルを読み込んでライブラリを生成する．
  /// @param[in] filename ファイル名
  /// @return 読み込んで作成したセルライブラリを返す．
  /// @note エラーが起きたら NULL を返す．
  const CellLibrary*
  operator()(const char* filename);

};