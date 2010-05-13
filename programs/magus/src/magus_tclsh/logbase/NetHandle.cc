
/// @file magus/logbase/NetHandle.cc
/// @brief NetHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NetHandle.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "NetHandle.h"
#include <ym_bnet/BNetwork.h>
#if 0
#include <ym_bdn/BdNetwork.h>
#endif


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス NetHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr 親のマネージャ
// @param[in] name 名前
NetHandle::NetHandle(NetMgr* mgr,
		     const string& name) :
  mName(name),
  mBNetwork(NULL),
#if 0
  mBdNetwork(NULL),
#endif
  mCount(0)
{
  mBNetwork = new BNetwork;
  mBNetwork->set_model_name(name);
}

// @brief デストラクタ
NetHandle::~NetHandle()
{
  delete mBNetwork;
#if 0
  delete mBdNetwork;
#endif
}
  
// @brief ネットワークを複製する．
void
NetHandle::copy(const NetHandle& src)
{
  *mBNetwork = *src.mBNetwork;
}

END_NAMESPACE_MAGUS
