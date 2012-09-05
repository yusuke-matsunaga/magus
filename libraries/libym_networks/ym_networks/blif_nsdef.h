#ifndef YM_NETWORKS_BLIF_NSDEF_H
#define YM_NETWORKS_BLIF_NSDEF_H

/// @file ym_networks/blif_nsdef.h
/// @brief nsBlif の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BlifGroup BLIF パーサ
///
/// UCB で開発された mis, sis の標準入出力フォーマット
/// BLIF(Berkeley Logic Interchange Format) 用のパーサ


#include "ym_networks/networks_nsdef.h"


/// @brief blif 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_BLIF \
BEGIN_NAMESPACE_YM_NETWORKS \
BEGIN_NAMESPACE(nsBlif)

/// @brief blif 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_BLIF \
END_NAMESPACE(nsBlif) \
END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

// クラスの前方宣言
class BlifHandler;
class BlifParser;
class BlifParserImpl;
class BlifNetwork;
class BlifNode;
class BlifNetworkReader;
class BlifNetworkImpl;

END_NAMESPACE_YM_NETWORKS_BLIF

BEGIN_NAMESPACE_YM

using nsNetworks::nsBlif::BlifNode;
using nsNetworks::nsBlif::BlifNetwork;
using nsNetworks::nsBlif::BlifNetworkReader;

END_NAMESPACE_YM

#endif // YM_NETWORKS_BLIF_NSDEF_H
