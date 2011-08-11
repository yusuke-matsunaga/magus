
/// @file BlifNetworkReader.cc
/// @brief BlifNetworkReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BlifNetworkReader.h"
#include "BlifNetworkHandler.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
// BlifNetworkReader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifNetworkReader::BlifNetworkReader() :
  mHandler(new BlifNetworkHandler)
{
  mParser.add_handler(mHandler);
}

// @brief デストラクタ
BlifNetworkReader::~BlifNetworkReader()
{
  // mHandler は BlifParser が責任を持って破壊してくれる．
}

// @brief BLIF 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[out] network 読み込んだ内容を設定するネットワーク
// @param[in] msg_handler メッセージハンドラ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BlifNetworkReader::read(const string& filename,
			BlifNetwork& network)
{
  mHandler->set_network(&network);

  bool stat = mParser.read(filename);
  if ( !stat ) {
    return false;
  }

  return true;
}

END_NAMESPACE_YM_BLIF
