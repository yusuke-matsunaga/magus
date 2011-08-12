#ifndef YM_NETWORKS_TGISCAS89READER_H
#define YM_NETWORKS_TGISCAS89READER_H

/// @file ym_networks/TgIscas89Reader.h
/// @brief TgIscas89Reader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/tgnet.h"


BEGIN_NAMESPACE_YM_NETWORKS

namespace nsIscas89 {
  class Iscas89Parser;
  class TgIscas89Handler;
}

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
/// @class TgIscas89Reader TgIscas89Reader.h "ym_networks/TgIscas89Reader.h"
/// @brief iscas89 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class TgIscas89Reader
{
public:

  /// @brief コンストラクタ
  TgIscas89Reader();

  /// @brief デストラクタ
  ~TgIscas89Reader();


public:

  /// @brief blif ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] network 設定対象のネットワーク
  bool
  operator()(const string& filename,
	     TgNetwork& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ISCAS89 ファイルを読み込むためのパーサ
  nsIscas89::Iscas89Parser* mParser;

  // ISCAS89 ファイルを TgNetwork に変換するためのハンドラ
  nsIscas89::TgIscas89Handler* mHandler;

};

END_NAMESPACE_YM_NETWORKS_TGNET

#endif // YM_NETWORKS_TGISCAS89READER_H
