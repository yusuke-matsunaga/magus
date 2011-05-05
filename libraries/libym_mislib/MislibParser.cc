
/// @file libym_cell/mislib/MislibParser.cc
/// @brief MislibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mislib/MislibParser.h"
#include "MislibParserImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibParser
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibParser::MislibParser() :
  mImpl(new MislibParserImpl)
{
}

// デストラクタ
MislibParser::~MislibParser()
{
  delete mImpl;
}

// @brief mislib ファイルを読み込んでライブラリを生成する．
// @param[in] filename ファイル名
// @return パース木の根のノードを返す．
// @note 読み込みが失敗したら NULL を返す．
const MislibNode*
MislibParser::read(const string& filename)
{
  return mImpl->read(filename);
}

END_NAMESPACE_YM_MISLIB
