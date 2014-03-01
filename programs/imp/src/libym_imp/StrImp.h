#ifndef STRIMP_H
#define STRIMP_H

/// @file StrImp.h
/// @brief StrImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "networks/BNetwork.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class StrImp StrImp.h "StrImp.h"
/// @brief 構造を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class StrImp
{
public:

  /// @brief コンストラクタ
  StrImp();

  /// @brief デストラクタ
  virtual
  ~StrImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  /// @param[in] imp_info 間接含意のリスト
  virtual
  void
  learning(ImpMgr& imp_mgr,
	   ImpInfo& imp_info);

};

END_NAMESPACE_YM_NETWORKS

#endif // STRIMP_H
