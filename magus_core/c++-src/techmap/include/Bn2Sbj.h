#ifndef BN2SBJ_H
#define BN2SBJ_H

/// @file Bn2Sbj.h
/// @brief Bn2Sbj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @class Bn2Sbj Bn2Sbj.h "Bn2Sbj.h"
/// @brief BnNetwork を SbjGraph に変換するクラス
///
/// 実はクラスにする必要はない．
//////////////////////////////////////////////////////////////////////
class Bn2Sbj
{
public:

  /// @brief コンストラクタ
  Bn2Sbj() = default;

  /// @brief デストラクタ
  ~Bn2Sbj() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNetwork を SbjGraph に変換する．
  static
  void
  convert(
    const BnNetwork& src_network, ///< [in] もとのネットワーク
    SbjGraph& dst_network         ///< [in] 変換されたネットワーク
  );

};

END_NAMESPACE_SBJ

#endif // BN2SBJ_H
