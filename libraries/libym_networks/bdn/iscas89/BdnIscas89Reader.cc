
/// @file BdnIscas89Reader.cc
/// @brief BdnIscas89Reader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnIscas89Reader.h"

#include "Iscas89Network.h"
#include "Iscas89NetworkReader.h"
#include "Iscas89BdnConv.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
// BdnIscas89Reader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdnIscas89Reader::BdnIscas89Reader()
{
}

// @brief デストラクタ
BdnIscas89Reader::~BdnIscas89Reader()
{
}

// @brief ISCAS89 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[in] network 読み込んだ内容を設定するネットワーク
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BdnIscas89Reader::read(const string& filename,
		       BdnMgr& network)
{
  using namespace nsIscas89;

  Iscas89NetworkReader reader;
  Iscas89Network iscas89_network;

  if ( !reader.read(filename, iscas89_network) ) {
    cerr << "Error in reading " << filename << endl;
    return false;
  }

  Iscas89BdnConv conv;
  bool stat = conv(iscas89_network, network);
  if ( !stat ) {
    cerr << "Error in converting from Iscas89Network to BdnMgr" << endl;
    return false;
  }

  return true;
}

END_NAMESPACE_YM_NETWORKS_BDN
