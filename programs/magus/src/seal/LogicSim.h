#ifndef LIBYM_SEAL_LOGICSIM_H
#define LIBYM_SEAL_LOGICSIM_H

/// @file libym_seal/LogicSim.h
/// @brief LogicSim のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "seal_nsdef.h"
#include "YmNetworks/BNetwork.h"


BEGIN_NAMESPACE_YM_SEAL

typedef string State;

//////////////////////////////////////////////////////////////////////
/// @class LogcSim LogicSim.h "LogicSim.h"
/// @brief 論理シミュレーションを行うクラス
//////////////////////////////////////////////////////////////////////
class LogicSim
{
public:

  /// @brief コンストラクタ
  /// @param[in] bnetwork ネットワーク
  LogicSim(const BNetwork* bnetwork);

  /// @brief デストラクタ
  ~LogicSim();


public:

  /// @brief シミュレーションを行う．
  /// @param[in] input_vector 入力ベクタを整数にしたもの
  /// @param[in] cur_state 現状態
  /// @param[out] output_vector 出力ベクタを整数にしたもの
  /// @param[out] next_state 次状態
  void
  operator()(ymuint input_vector,
	     State cur_state,
	     ymuint& output_vector,
	     State& next_state);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const BNetwork* mNetwork;

  // ノードをトポロジカル順に納めた配列
  BNodeVector mNodeArray;

  // ノードの値を保持しておく作業領域
  vector<ymuint> mValArray;

};

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_LOGICSIM_H
