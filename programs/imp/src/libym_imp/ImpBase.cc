
/// @file ImpBase.cc
/// @brief ImpBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpBase.h"
#include "ImpInfo.h"

#include "StrImp.h"
#include "ContraImp.h"
#include "CnfImp.h"
#include "CnfImp2.h"
#include "SatImp.h"
#include "RlImp.h"
#include "NaImp.h"
#include "NaImp2.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ImpBase::ImpBase()
{
}

// @brief デストラクタ
ImpBase::~ImpBase()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] method 手法を表す文字列
// @param[in] imp_info 間接含意のリスト
void
ImpBase::learning(ImpMgr& imp_mgr,
		  const string& method,
		  ImpInfo& imp_info)
{

  if ( method == "direct" ) {
    StrImp imp;
    imp.learning(imp_mgr, imp_info);
  }
  else if ( method == "contra" ) {
    ContraImp imp;
    imp.learning(imp_mgr, imp_info);
  }
  else if ( method == "recursive" ) {
    RlImp imp;
    imp.set_learning_level(1);
    imp.learning(imp_mgr, imp_info);
  }
  else if ( method == "exact" ) {
    SatImp imp;
    imp.learning(imp_mgr, imp_info);
  }
  else if ( method == "naive" ) {
    NaImp imp;
    imp.learning(imp_mgr, imp_info);
  }
  else {
    // default fall-back
    StrImp imp;
    imp.learning(imp_mgr, imp_info);
  }
}

END_NAMESPACE_YM_NETWORKS
