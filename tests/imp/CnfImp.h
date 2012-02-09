#ifndef CNFIMP_H
#define CNFIMP_H

/// @file CnfImp.h
/// @brief CnfImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class CnfImp CnfImp.h "CnfImp.h"
/// @brief CNFを用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class CnfImp :
  public ImpBase
{
public:

  /// @brief コンストラクタ
  CnfImp();

  /// @brief デストラクタ
  virtual
  ~CnfImp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_NETWORKS

#endif // CNFIMP_H
