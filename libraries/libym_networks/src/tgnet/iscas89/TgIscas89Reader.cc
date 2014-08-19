
/// @file TgIscas89Reader
/// @brief TgIscas89Reaer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/TgIscas89Reader.h"
#include "Iscas89Parser.h"
#include "TgIscas89Handler.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
// ISCAS89 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgIscas89Reader::TgIscas89Reader() :
  mParser(new nsIscas89::Iscas89Parser),
  mHandler(new nsIscas89::TgIscas89Handler)
{
  mParser->add_handler(mHandler);
}

// @brief デストラクタ
TgIscas89Reader::~TgIscas89Reader()
{
  delete mParser;
  // mHandler は Iscas89Parser が責任をもって破壊してくれる．
}

// @brief blif ファイルを読み込む．
bool
TgIscas89Reader::operator()(const string& filename,
			    TgNetwork& network)
{
  mHandler->set_network(network);
  bool stat = mParser->read(filename);
  return stat;
}

END_NAMESPACE_YM_NETWORKS_TGNET
