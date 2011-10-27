#ifndef LSIMLCC_H
#define LSIMLCC_H

/// @file LsimLcc.h
/// @brief LsimLcc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimLcc LsimLcc.h "LsimLcc.h"
/// @brief 単純な Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimLcc :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimLcc();

  /// @brief デストラクタ
  virtual
  ~LsimLcc();


public:
  //////////////////////////////////////////////////////////////////////
  // Lsim の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] bdn 対象のネットワーク
  virtual
  void
  set_network(const BdnMgr& bdn);

  /// @brief 論理シミュレーションを行う．
  /// @param[in] iv 入力ベクタ
  /// @param[out] ov 出力ベクタ
  virtual
  void
  eval(const vector<ymuint64>& iv,
       vector<ymuint64>& ov);

};

END_NAMESPACE_YM

#endif // LSIMLCC_H
