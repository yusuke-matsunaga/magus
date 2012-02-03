#ifndef CNFIMP2_H
#define CNFIMP2_H

/// @file CnfImp2.h
/// @brief CnfImp2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class CnfImp2 CnfImp2.h "CnfImp2.h"
/// @brief CNFddを用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class CnfImp2 :
  public ImpBase
{
public:

  /// @brief コンストラクタ
  CnfImp2();

  /// @brief デストラクタ
  virtual
  ~CnfImp2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] network 対象のネットワーク
  /// @param[in] imp_list 間接含意のリスト
  virtual
  void
  learning(const BdnMgr& network,
	   vector<ImpInfo>& imp_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_NETWORKS

#endif // CNFIMP_H
