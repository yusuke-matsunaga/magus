#ifndef SATIMP2_H
#define SATIMP2_H

/// @file SatImp2.h
/// @brief SatImp2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class SatImp2 SatImp2.h "SatImp2.h"
/// @brief SATを用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class SatImp2
{
public:

  /// @brief コンストラクタ
  SatImp2();

  /// @brief デストラクタ
  virtual
  ~SatImp2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] network 対象のネットワーク
  /// @param[in] imp_info 間接含意のリスト
  void
  learning(const BdnMgr& network,
	   const ImpInfo& direct_imp,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // SATIMP2_H
