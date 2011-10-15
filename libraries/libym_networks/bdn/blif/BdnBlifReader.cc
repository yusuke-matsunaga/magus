
/// @file BdnBlifReader.cc
/// @brief BdnBlifReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnBlifReader.h"

#include "BlifNetwork.h"
#include "BlifNetworkReader.h"
#include "BlifBdnConv.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
// BdnBlifReader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdnBlifReader::BdnBlifReader()
{
}

// @brief デストラクタ
BdnBlifReader::~BdnBlifReader()
{
}

// @brief BLIF 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[in] network 読み込んだ内容を設定するネットワーク
// @param[in] cell_library セルライブラリ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BdnBlifReader::operator()(const string& filename,
			  BdnMgr& network,
			  const CellLibrary* cell_library)
{
  using namespace nsBlif;

  BlifNetworkReader read;
  BlifNetwork blif_network;

  if ( !read(filename, blif_network, cell_library) ) {
    cerr << "Error in reading " << filename << endl;
    return false;
  }

  BlifBdnConv conv;
  bool stat = conv(blif_network, network);
  if ( !stat ) {
    cerr << "Error in converting from BlifNetwork to BdnMgr" << endl;
    return false;
  }

  return true;
}

END_NAMESPACE_YM_NETWORKS_BDN
