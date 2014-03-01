#ifndef SATIMP_H
#define SATIMP_H

/// @file SatImp.h
/// @brief SatImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class SatImp SatImp.h "SatImp.h"
/// @brief SATを用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class SatImp
{
public:

  /// @brief コンストラクタ
  SatImp();

  /// @brief デストラクタ
  ~SatImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  /// @param[out] imp_info 間接含意のリスト
  void
  learning(ImpMgr& imp_mgr,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // SATIMP_H
