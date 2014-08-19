#ifndef IMPBASE_H
#define IMPBASE_H

/// @file ImpBase.h
/// @brief ImpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class ImpBase ImpBase.h "ImpBase.h"
/// @brief 含意を求めるクラス
//////////////////////////////////////////////////////////////////////
class ImpBase
{
public:

  /// @brief コンストラクタ
  ImpBase();

  /// @brief デストラクタ
  ~ImpBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  /// @param[in] method 手法を表す文字列
  /// @param[out] imp_info 間接含意のリスト
  void
  learning(ImpMgr& imp_mgr,
	   const string& method,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPBASE_H
