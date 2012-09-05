
/// @file BlifParser.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BlifParserImpl.h"
#include "BlifParser.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

// @brief コンストラクタ
BlifParser::BlifParser()
{
  mRep = new BlifParserImpl;
}

// @brief デストラクタ
BlifParser::~BlifParser()
{
  delete mRep;
}

// @brief 読み込みを行う．
bool
BlifParser::read(const string& filename,
		 const CellLibrary* cell_library)
{
  return mRep->read(filename, cell_library);
}

// @brief イベントハンドラの登録
void
BlifParser::add_handler(BlifHandler* handler)
{
  mRep->add_handler(handler);
}

END_NAMESPACE_YM_NETWORKS_BLIF
