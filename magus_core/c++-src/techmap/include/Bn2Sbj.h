#ifndef BN2SBJ_H
#define BN2SBJ_H

/// @file Bn2Sbj.h
/// @brief Bn2Sbj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
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
  Bn2Sbj();

  /// @brief デストラクタ
  ~Bn2Sbj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNetwork を SbjGraph に変換する．
  /// @param[in] src_network もとのネットワーク
  /// @param[in] dst_network 変換されたネットワーク
  void
  convert(const BnNetwork& src_network,
	  SbjGraph& dst_network);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_SBJ

#endif // BN2SBJ_H
