#ifndef SATIMP_H
#define SATIMP_H

/// @file SatImp.h
/// @brief SatImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class SatImp SatImp.h "SatImp.h"
/// @brief SATを用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class SatImp :
  public ImpBase
{
public:

  /// @brief コンストラクタ
  SatImp();

  /// @brief デストラクタ
  virtual
  ~SatImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] network 対象のネットワーク
  /// @param[in] imp_info 間接含意のリスト
  virtual
  void
  learning(const BdnMgr& network,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // SATIMP_H
