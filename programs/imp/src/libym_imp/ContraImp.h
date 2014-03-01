#ifndef CONTRAIMP_H
#define CONTRAIMP_H

/// @file ContraImp.h
/// @brief ContraImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class ContraImp ContraImp.h "ContraImp.h"
/// @brief 対偶用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class ContraImp
{
public:

  /// @brief コンストラクタ
  ContraImp();

  /// @brief デストラクタ
  virtual
  ~ContraImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  /// @param[in] imp_info 間接含意のリスト
  void
  learning(ImpMgr& imp_mgr,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // CONTRAIMP_H
