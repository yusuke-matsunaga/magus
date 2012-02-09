#ifndef IMPBASE_H
#define IMPBASE_H

/// @file ImpBase.h
/// @brief ImpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class ImpBase ImpBase.h "ImpBase.h"
/// @brief 含意を求める基底クラス
//////////////////////////////////////////////////////////////////////
class ImpBase
{
public:

  /// @brief デストラクタ
  virtual
  ~ImpBase() { }


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
	   ImpInfo& imp_info) = 0;

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPBASE_H
