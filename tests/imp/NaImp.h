#ifndef NAIMP_H
#define NAIMP_H

/// @file NaImp.h
/// @brief NaImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class NaImp NaImp.h "NaImp.h"
/// @brief 構造を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class NaImp
{
public:

  /// @brief コンストラクタ
  NaImp();

  /// @brief デストラクタ
  ~NaImp();


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

#endif // NAIMP_H
