
/// @file ReadMislib.cc
/// @brief ReadMislib の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadMislib.h"
#include "cell/CellMislibReader.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス ReadMislib
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ReadMislib::ReadMislib(MagMgr* mgr) :
  ReadLibrary(mgr)
{
  set_usage_string("<filename>");
}

// @brief デストラクタ
ReadMislib::~ReadMislib()
{
}

// @brief セルライブラリを読み込む．
// @param[in] filename ファイル名
const CellLibrary*
ReadMislib::read_library(const string& filename)
{
  CellMislibReader read;
  return read(filename);
}

END_NAMESPACE_MAGUS
