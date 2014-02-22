
/// @file BNetBlifReader.cc
/// @brief BNetBlifReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BNetBlifReader.h"
#include "BlifParser.h"
#include "BNetBlifHandler.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
// BNetBlifReader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetBlifReader::BNetBlifReader() :
  mParser(new nsBlif::BlifParser),
  mHandler(new nsBlif::BNetBlifHandler)
{
  mParser->add_handler(mHandler);
}

// @brief デストラクタ
BNetBlifReader::~BNetBlifReader()
{
  delete mParser;
  // mHandler は BlifParser が責任を持って破壊してくれる．
}

// @brief BLIF 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[in] network 読み込んだ内容を設定するネットワーク
// @param[in] cell_library セルライブラリ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BNetBlifReader::operator()(const string& filename,
			   BNetwork& network,
			   const CellLibrary* cell_library)
{
  mHandler->set_network(&network);

  bool stat = mParser->read(filename, cell_library);
  if ( !stat ) {
    return false;
  }

  return true;
}

END_NAMESPACE_YM_NETWORKS_BNET
