#ifndef YM_NETWORKS_TGBLIFREADER_H
#define YM_NETWORKS_TGBLIFREADER_H

/// @file ym_networks/TgBlifReader.h
/// @brief TgBlifReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/tgnet.h"


BEGIN_NAMESPACE_YM_NETWORKS

namespace nsBlif {
  class BlifParser;
  class TgBlifHandler;
}


//////////////////////////////////////////////////////////////////////
/// @class TgBlifReader TgBlifReader.h "ym_networks/TgBlifReader.h"
/// @brief blif 形式のファイルを読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class TgBlifReader
{
public:

  /// @brief コンストラクタ
  TgBlifReader();

  /// @brief デストラクタ
  ~TgBlifReader();


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

  // Blif ファイルを読み込むためのパーサ
  nsBlif::BlifParser* mParser;

  // Blif ファイルを TgNetwork に変換するためのハンドラ
  nsBlif::TgBlifHandler* mHandler;

};

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_TGBLIFREADER_H
