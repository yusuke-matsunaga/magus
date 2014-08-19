#ifndef NAIMP2_H
#define NAIMP2_H

/// @file NaImp2.h
/// @brief NaImp2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class NaImp2 NaImp2.h "NaImp2.h"
/// @brief 構造を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class NaImp2
{
public:

  /// @brief コンストラクタ
  NaImp2();

  /// @brief デストラクタ
  ~NaImp2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  /// @param[in] direct_imp 直接含意のリスト
  /// @param[out] imp_info 間接含意のリスト
  virtual
  void
  learning(ImpMgr& imp_mgr,
	   const ImpInfo& direct_imp,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // NAIMP2_H
