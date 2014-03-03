
/// @file CellDotlibReader.sip
/// @brief CellDotlibReader の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellDotlibReader
{
%TypeHeaderCode
#include "cell/CellDotlibReader.h"
using nsYm::CellDotlibReader;
%End

public:

  /// @brief mislib 形式のファイルを読み込んでライブラリを生成する．
  /// @param[in] filename ファイル名
  /// @return 読み込んで作成したセルライブラリを返す．
  /// @note エラーが起きたら NULL を返す．
  const CellLibrary*
  operator()(const char* filename);

};