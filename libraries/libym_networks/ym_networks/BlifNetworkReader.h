#ifndef YM_NETWORKS_BLIFNETWORKREADER_H
#define YM_NETWORKS_BLIFNETWORKREADER_H

/// @file ym_networks/BlifNetworkReader.h
/// @brief BlifNetworkReader のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

class BlifParser;
class BlifNetworkHandler;

//////////////////////////////////////////////////////////////////////
/// @class BlifNetworkReader BlifNetworkReader.h "ym_networks/BlifNetworkReader.h"
/// @ingroup BlifGroup
/// @brief blif 形式のファイルを読み込んで BlifNetwork に設定するクラス
/// @sa BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifNetworkReader
{
public:

  /// @brief コンストラクタ
  BlifNetworkReader();

  /// @brief デストラクタ
  ~BlifNetworkReader();


public:

  /// @brief BLIF 形式のファイルを読み込む
  /// @param[in] filename ファイル名
  /// @param[in] network 読み込んだ内容を設定するネットワーク
  /// @param[in] cell_library セルライブラリ
  /// @retval true 正常に読み込めた
  /// @retval false 読み込み中にエラーが起こった．
  bool
  operator()(const string& filename,
	     BlifNetwork& network,
	     const CellLibrary* cell_library = NULL);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif パーサー
  BlifParser* mParser;

  // ハンドラ
  BlifNetworkHandler* mHandler;

};

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFNETWORKREADER_H
