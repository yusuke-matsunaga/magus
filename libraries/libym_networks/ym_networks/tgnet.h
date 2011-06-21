#ifndef YM_NETWORKS_TGNET_NSDEF_H
#define YM_NETWORKS_TGNET_NSDEF_H

/// @file ym_networks/tgnet.h
/// @brief TgNetwork 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: tgnet.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．


#include "ymtools.h"

#define BEGIN_NAMESPACE_YM_NETWORKS \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsTgnet)

#define END_NAMESPACE_YM_NETWORKS \
END_NAMESPACE(nsTgnet) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_NETWORKS

class TgGateTemplate;
class TgEdge;
class TgNode;
class TgNetwork;
class TgBlifReader;
class TgIscas89Reader;

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM

using nsTgnet::TgGateTemplate;
using nsTgnet::TgEdge;
using nsTgnet::TgNode;
using nsTgnet::TgNetwork;
using nsTgnet::TgBlifReader;
using nsTgnet::TgIscas89Reader;

END_NAMESPACE_YM

#endif // YM_NETWORKS_TGNET_NSDEF_H
