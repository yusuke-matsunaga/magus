#ifndef LSIM_H
#define LSIM_H

/// @file Lsim.h
/// @brief Lsim のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Lsim Lsim.h "Lsim.h"
/// @brief 論理シミュレータの基底クラス
//////////////////////////////////////////////////////////////////////
class Lsim
{
public:

  /// @brief デストラクタ
  virtual
  ~Lsim() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] bdn 対象のネットワーク
  /// @param[in] order_map 順序マップ
  virtual
  void
  set_network(const BdnMgr& bdn,
	      const unordered_map<string, ymuint>& order_map) = 0;

  /// @brief 論理シミュレーションを行う．
  /// @param[in] iv 入力ベクタ
  /// @param[out] ov 出力ベクタ
  virtual
  void
  eval(const vector<ymuint64>& iv,
       vector<ymuint64>& ov) = 0;

};

END_NAMESPACE_YM

#endif // LSIM_H
