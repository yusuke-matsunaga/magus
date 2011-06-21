
/// @file libym_network/bdn/blif/BdnBlifReader.cc
/// @brief BdnBlifReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnBlifReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnBlifReader.h"

#include "BlifNetwork.h"
#include "BlifNetworkReader.h"
#include "BlifBdnConv.h"


BEGIN_NAMESPACE_YM_NETWORKS

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
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BdnBlifReader::read(const string& filename,
		    BdnMgr& network)
{
  using namespace nsBlif;

  BlifNetworkReader reader;
  BlifNetwork blif_network;

  if ( !reader.read(filename, blif_network) ) {
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

END_NAMESPACE_YM_NETWORKS
