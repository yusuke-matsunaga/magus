
/// @file RestoreLibrary.cc
/// @brief RestoreLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RestoreLibrary.h"
#include "ym/ClibCellLibrary.h"
#include "ym/BinDec.h"


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
// @param[in] library 設定対象のライブラリ
// @return 読み込みが成功したら true を返す．
bool
RestoreLibrary::read_library(const string& filename,
			     ClibCellLibrary& library)
{
  ifstream fi(filename);
  if ( !fi ) {
    TclObj emsg;
    emsg << "Could not open " << filename;
    set_result(emsg);
    return false;
  }

  BinDec bdec{fi};
  library.restore(bdec);

  return true;
}

END_NAMESPACE_MAGUS
