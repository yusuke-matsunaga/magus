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


//////////////////////////////////////////////////////////////////////
/// @brief TgNode の種類を表す列挙型
///
/// 全部で6ビット長
/// - 上位2ビット [5:4] の値
///   - 0: 未定義
///   - 1: 外部入力
///   - 2: 外部出力
///   - 3: 論理ノード
///   を表す．
/// - 論理ノードの場合は下位4ビット[3:0]の値で論理関数を表す．
//////////////////////////////////////////////////////////////////////
enum tTgNodeType {
  /// @brief 未定義
  kTgUndef  = 0x00U,

  /// @brief 外部入力
  kTgInput  = 0x10U,

  /// @brief 外部出力
  kTgOutput = 0x20U,

  /// @brief 論理ノード
  kTgLogic  = 0x30U,

  /// @brief 定数0
  kTgConst0 = kTgLogic | static_cast<ymuint>(kTgGateConst0),

  /// @brief 定数1
  kTgConst1 = kTgLogic | static_cast<ymuint>(kTgGateConst1),

  /// @brief バッファ
  kTgBuff   = kTgLogic | static_cast<ymuint>(kTgGateBuff),

  /// @brief NOT ゲート
  kTgNot    = kTgLogic | static_cast<ymuint>(kTgGateNot),
  /// @brief AND ゲート
  kTgAnd    = kTgLogic | static_cast<ymuint>(kTgGateAnd),

  /// @brief NAND ゲート
  kTgNand   = kTgLogic | static_cast<ymuint>(kTgGateNand),

  /// @brief OR ゲート
  kTgOr     = kTgLogic | static_cast<ymuint>(kTgGateOr),

  /// @brief NOR ゲート
  kTgNor    = kTgLogic | static_cast<ymuint>(kTgGateNor),

  /// @brief XOR ゲート
  kTgXor    = kTgLogic | static_cast<ymuint>(kTgGateXor),

  /// @brief XNOR ゲート
  kTgXnor   = kTgLogic | static_cast<ymuint>(kTgGateXnor),

  /// @brief 上記以外の複雑なゲート
  kTgCplx   = kTgLogic | static_cast<ymuint>(kTgGateCplx)
};

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

#endif // YM_NETWORKS_TGNET_NSDEF_H
