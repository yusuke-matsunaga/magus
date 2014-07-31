#ifndef NETWORKS_TGNET_NSDEF_H
#define NETWORKS_TGNET_NSDEF_H

/// @file YmNetworks/tgnet.h
/// @brief TgNetwork 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．


#include "YmNetworks/networks_nsdef.h"


/// @brief tgnet 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_TGNET	\
  BEGIN_NAMESPACE_YM_NETWORKS			\
  BEGIN_NAMESPACE(nsTgnet)

/// @brief tgnet 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_TGNET		\
  END_NAMESPACE(nsTgnet)			\
  END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief 論理ゲートの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tTgGateType {
  /// @brief 定数0
  kTgGateConst0 = 0,

  /// @brief 定数1
  kTgGateConst1 = 1,

  /// @brief バッファ
  kTgGateBuff   = 2,

  /// @brief NOT
  kTgGateNot    = 3,

  /// @brief AND
  kTgGateAnd    = 4,

  /// @brief NAND
  kTgGateNand   = 5,

  /// @brief OR
  kTgGateOr     = 6,

  /// @brief NOR
  kTgGateNor    = 7,

  /// @brief XOR
  kTgGateXor    = 8,

  /// @brief XNOR
  kTgGateXnor   = 9,

  /// @brief 上記以外
  kTgGateCplx   = 10
};

/// @brief tTgGateType のストリーム出力
ostream&
operator<<(ostream& s,
	   tTgGateType gate_type);

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_NETWORKS_TGNET

class TgEdge;
class TgNode;
class TgNetwork;
class TgBlifReader;
class TgIscas89Reader;

END_NAMESPACE_YM_NETWORKS_TGNET

BEGIN_NAMESPACE_YM

using nsNetworks::nsTgnet::TgEdge;
using nsNetworks::nsTgnet::TgNode;
using nsNetworks::nsTgnet::TgNetwork;
using nsNetworks::nsTgnet::TgBlifReader;
using nsNetworks::nsTgnet::TgIscas89Reader;

END_NAMESPACE_YM

#endif // NETWORKS_TGNET_NSDEF_H
