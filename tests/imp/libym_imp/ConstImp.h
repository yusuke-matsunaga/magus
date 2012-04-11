#ifndef CONSTIMP_H
#define CONSTIMP_H

/// @file ConstImp.h
/// @brief ConstImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;
class ImpInfo;

//////////////////////////////////////////////////////////////////////
/// @class ConstImp ConstImp.h "ConstImp.h"
/// @brief SATを用いた間接含意エンジン
//////////////////////////////////////////////////////////////////////
class ConstImp
{
public:

  /// @brief コンストラクタ
  ConstImp();

  /// @brief デストラクタ
  ~ConstImp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイスの宣言
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワーク中の間接含意を求める．
  /// @param[in] imp_mgr マネージャ
  void
  learning(ImpMgr& imp_mgr);

};

END_NAMESPACE_YM_NETWORKS

#endif // CONSTIMP_H
