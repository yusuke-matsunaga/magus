#ifndef NAIMP_H
#define NAIMP_H

/// @file NaImp.h
/// @brief NaImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

class StrNode;

//////////////////////////////////////////////////////////////////////
/// @class NaImp NaImp.h "NaImp.h"
/// @brief 構造を用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class NaImp :
  public ImpBase
{
public:

  /// @brief コンストラクタ
  NaImp();

  /// @brief デストラクタ
  virtual
  ~NaImp();


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

#endif // NAIMP_H
