
/// @file RestoreLibrary.cc
/// @brief RestoreLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RestoreLibrary.h"
#include "YmCell/CellLibrary.h"
#include "YmUtils/FileIDO.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス RestoreLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RestoreLibrary::RestoreLibrary(MagMgr* mgr) :
  ReadLibrary(mgr)
{
  set_usage_string("<filename>");
}

// @brief デストラクタ
RestoreLibrary::~RestoreLibrary()
{
}

// @brief セルライブラリを読み込む．
// @param[in] filename ファイル名
const CellLibrary*
RestoreLibrary::read_library(const string& filename)
{
  FileIDO bi;
  if ( !bi.open(filename) ) {
    TclObj emsg;
    emsg << "Could not open " << filename;
    set_result(emsg);
    return NULL;
  }

  CellLibrary* library = CellLibrary::new_obj();
  library->restore(bi);
  return library;
}

END_NAMESPACE_MAGUS
