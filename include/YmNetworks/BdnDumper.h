#ifndef NETWORKS_BDNDUMPER_H
#define NETWORKS_BDNDUMPER_H

/// @file YmNetworks/BdnDumper.h
/// @brief BdnDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnDumper BdnDumper.h "YmNetworks/BdnDumper.h"
/// @brief BdnMgr の内容を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class BdnDumper
{
public:

  /// @brief コンストラクタ
  BdnDumper();

  /// @brief デストラクタ
  ~BdnDumper();


public:

  /// @brief BdnMgr の内容をダンプする関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] network 対象のネットワーク
  void
  operator()(ostream& s,
	     const BdnMgr& network);

};

END_NAMESPACE_YM_NETWORKS_BDN

#endif // NETWORKS_BDNDUMPER_H
