
/// @file TgBlifReader.cc
/// @brief TgBlifReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/TgBlifReader.h"
#include "BlifParser.h"
#include "TgBlifHandler.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgBlifReader::TgBlifReader() :
  mParser(new nsBlif::BlifParser),
  mHandler(new nsBlif::TgBlifHandler)
{
  mParser->add_handler(mHandler);
}

// @brief デストラクタ
TgBlifReader::~TgBlifReader()
{
  delete mParser;
  // mHandler は BlifParser が責任をもって破壊してくれる．
}

// @brief blif ファイルを読み込む．
bool
TgBlifReader::operator()(const string& filename,
			 TgNetwork& network,
			 const CellLibrary* cell_library)
{
  mHandler->set_network(network);
  bool stat = mParser->read(filename, cell_library);
  return stat;
}

END_NAMESPACE_YM_NETWORKS_TGNET
