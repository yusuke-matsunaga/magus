#ifndef YM_NETWORKS_NETWORKS_NSDEF_H
#define YM_NETWORKS_NETWORKS_NSDEF_H

/// @file ym_networks/networks_nsdef.h
/// @brief networks の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "ymtools.h"


/// @brief ym_networks 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsNetworks)

/// @brief ym_networks 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS \
END_NAMESPACE(nsNetworks) \
END_NAMESPACE_YM

/// @namespace nsYm::nsNetworks
/// @brief ネットワーク関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

#endif // YM_NETWORKS_NETWORKS_NSDEF_H
