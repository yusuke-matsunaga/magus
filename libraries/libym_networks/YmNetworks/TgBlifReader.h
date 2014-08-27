#ifndef NETWORKS_TGBLIFREADER_H
#define NETWORKS_TGBLIFREADER_H

/// @file YmNetworks/TgBlifReader.h
/// @brief TgBlifReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/tgnet.h"
#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS

namespace nsBlif {
  class BlifParser;
  class TgBlifHandler;
}

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
/// @class TgBlifReader TgBlifReader.h "YmNetworks/TgBlifReader.h"
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
  /// @param[in] cell_library セルライブラリ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  operator()(const string& filename,
	     TgNetwork& network,
	     const CellLibrary* cell_library = NULL);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Blif ファイルを読み込むためのパーサ
  nsBlif::BlifParser* mParser;

  // Blif ファイルを TgNetwork に変換するためのハンドラ
  nsBlif::TgBlifHandler* mHandler;

};

END_NAMESPACE_YM_NETWORKS_TGNET

#endif // NETWORKS_TGBLIFREADER_H
