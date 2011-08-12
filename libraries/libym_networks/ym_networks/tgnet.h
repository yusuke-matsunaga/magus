#ifndef YM_NETWORKS_TGNET_NSDEF_H
#define YM_NETWORKS_TGNET_NSDEF_H

/// @file ym_networks/tgnet.h
/// @brief TgNetwork 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．


#include "ym_networks/networks_nsdef.h"


/// @brief tgnet 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_TGNET	\
  BEGIN_NAMESPACE_YM_NETWORKS			\
  BEGIN_NAMESPACE(nsTgnet)

/// @brief tgnet 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_TGNET		\
  END_NAMESPACE(nsTgnet)			\
  END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
/// @brief 組み込み型のゲートの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum tTgGateType {
  /// @brief 未定義
  kTgUndef  = 0,

  /// @brief 外部入力
  kTgInput  = 1,

  /// @brief 外部出力
  kTgOutput = 2,

  /// @brief 定数0
  kTgConst0 = 3,
  /// @brief 定数1
  kTgConst1 = 4,

  /// @brief バッファ
  kTgBuff   = 8,
  /// @brief NOT ゲート
  kTgNot    = 9,
  /// @brief AND ゲート
  kTgAnd    = 10,
  /// @brief NAND ゲート
  kTgNand   = 11,
  /// @brief OR ゲート
  kTgOr     = 12,
  /// @brief NOR ゲート
  kTgNor    = 13,
  /// @brief XOR ゲート
  kTgXor    = 14,
  /// @brief XNOR ゲート
  kTgXnor   = 15,

  /// @brief ユーザー定義型の先頭
  kTgUsrDef = 32
};


class TgGateTemplate;
class TgEdge;
class TgNode;
class TgNetwork;
class TgBlifReader;
class TgIscas89Reader;

END_NAMESPACE_YM_NETWORKS_TGNET

BEGIN_NAMESPACE_YM

using nsNetworks::nsTgnet::tTgGateType;
using nsNetworks::nsTgnet::kTgUndef;
using nsNetworks::nsTgnet::kTgInput;
using nsNetworks::nsTgnet::kTgOutput;
using nsNetworks::nsTgnet::kTgConst0;
using nsNetworks::nsTgnet::kTgConst1;
using nsNetworks::nsTgnet::kTgBuff;
using nsNetworks::nsTgnet::kTgNot;
using nsNetworks::nsTgnet::kTgAnd;
using nsNetworks::nsTgnet::kTgNand;
using nsNetworks::nsTgnet::kTgOr;
using nsNetworks::nsTgnet::kTgNor;
using nsNetworks::nsTgnet::kTgXor;
using nsNetworks::nsTgnet::kTgXnor;
using nsNetworks::nsTgnet::kTgUsrDef;

using nsNetworks::nsTgnet::TgGateTemplate;
using nsNetworks::nsTgnet::TgEdge;
using nsNetworks::nsTgnet::TgNode;
using nsNetworks::nsTgnet::TgNetwork;
using nsNetworks::nsTgnet::TgBlifReader;
using nsNetworks::nsTgnet::TgIscas89Reader;

END_NAMESPACE_YM

#endif // YM_NETWORKS_TGNET_NSDEF_H
