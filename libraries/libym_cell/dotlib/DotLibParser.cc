
/// @file ym_cell/DotLibParser.cc
/// @brief DotLibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotLibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#if HAVE_CONFIG
#include <ymconfig.h>
#endif

#include "DotLibParser.h"
#include "DotLibLex.h"


BEGIN_NAMESPACE_YM_CELL

// コンストラクタ
DotLibParser::DotLibParser() :
  mLex(new DotLibLex(mMsgMgr))
{
  
}

// デストラクタ
DotLibParser::~DotLibParser()
{
  delete mLex;
}
  
// @brief ファイルを読み込む．
bool
DotLibParser::read_file(const string& filename)
{
  if ( !mLex->open_file(filename) ) {
    return false;
  }


  mLex->close_file();

  return true;
}

END_NAMESPACE_YM_CELL
