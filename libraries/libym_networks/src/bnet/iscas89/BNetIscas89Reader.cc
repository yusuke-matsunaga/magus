
/// @file BNetIscas89Reader.cc
/// @brief BNetIscas89Reader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BNetIscas89Reader.h"
#include "Iscas89Parser.h"
#include "BNetIscas89Handler.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// BNetIscas89Reader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BNetIscas89Reader::BNetIscas89Reader() :
  mParser(new nsIscas89::Iscas89Parser),
  mHandler(new nsIscas89::BNetIscas89Handler)
{
  mParser->add_handler(mHandler);
}

// @brief デストラクタ
BNetIscas89Reader::~BNetIscas89Reader()
{
  delete mParser;
  // mHandler は Iscas89Parser が責任を持って破壊してくれる．
}

// @brief BLIF 形式のファイルを読み込む
// @param[in] filename ファイル名
// @param[in] network 読み込んだ内容を設定するネットワーク
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BNetIscas89Reader::operator()(const string& filename,
			      BNetwork& network)
{
  mHandler->set_network(&network);

  bool stat = mParser->read(filename);
  if ( !stat ) {
    return false;
  }

  return true;
}

END_NAMESPACE_YM_NETWORKS
