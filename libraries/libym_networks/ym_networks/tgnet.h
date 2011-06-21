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


#include "ym_networks/networks_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS

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

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM

using nsNetworks::tTgGateType;
using nsNetworks::kTgUndef;
using nsNetworks::kTgInput;
using nsNetworks::kTgOutput;
using nsNetworks::kTgConst0;
using nsNetworks::kTgConst1;
using nsNetworks::kTgBuff;
using nsNetworks::kTgNot;
using nsNetworks::kTgAnd;
using nsNetworks::kTgNand;
using nsNetworks::kTgOr;
using nsNetworks::kTgNor;
using nsNetworks::kTgXor;
using nsNetworks::kTgXnor;
using nsNetworks::kTgUsrDef;

using nsNetworks::TgGateTemplate;
using nsNetworks::TgEdge;
using nsNetworks::TgNode;
using nsNetworks::TgNetwork;
using nsNetworks::TgBlifReader;
using nsNetworks::TgIscas89Reader;

END_NAMESPACE_YM

#endif // YM_NETWORKS_TGNET_NSDEF_H
