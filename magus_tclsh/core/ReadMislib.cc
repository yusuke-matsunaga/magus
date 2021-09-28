
/// @file ReadMislib.cc
/// @brief ReadMislib の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadMislib.h"
#include "ym/ClibCellLibrary.h"


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
// @param[in] library 設定対象のライブラリ
// @return 読み込みが成功したら true を返す．
bool
ReadMislib::read_library(const string& filename,
			 ClibCellLibrary& library)
{
  library = ClibCellLibrary::read_mislib(filename);
  return library.is_valid();
}

END_NAMESPACE_MAGUS
