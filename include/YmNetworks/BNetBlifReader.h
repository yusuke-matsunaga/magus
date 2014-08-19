#ifndef NETWORKS_BNETBLIFREADER_H
#define NETWORKS_BNETBLIFREADER_H

/// @file YmNetworks/BNetBlifReader.h
/// @brief BNetBlifReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bnet.h"
#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS

namespace nsBlif {
  class BlifParser;
  class BNetBlifHandler;
}

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifReader BNetBlifReader.h "YmNetworks/BNetBlifReader.h"
/// @ingroup BnetGroup
/// @brief blif 形式のファイルを読み込んで BNetwork に設定するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetBlifReader
{
public:

  /// @brief コンストラクタ
  BNetBlifReader();

  /// @brief デストラクタ
  ~BNetBlifReader();


public:

  /// @brief blif 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @param[in] cell_library セルライブラリ
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  operator()(const string& filename,
	     BNetwork& network,
	     const CellLibrary* cell_library = NULL);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif パーサー
  nsBlif::BlifParser* mParser;

  // ハンドラ
  nsBlif::BNetBlifHandler* mHandler;

};

END_NAMESPACE_YM_NETWORKS_BNET

#endif // NETWORKS_BNETBLIFREADER_H
