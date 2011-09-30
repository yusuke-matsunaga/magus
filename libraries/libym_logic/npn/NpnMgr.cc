
/// @file NpnMgr.cc
/// @brief NpnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/NpnMgr.h"
#include "NpnMgrImpl.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
// クラス NpnMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnMgr::NpnMgr()
{
  mImpl = new NpnMgrImpl;
}

// デストラクタ
NpnMgr::~NpnMgr()
{
  delete mImpl;
}

// @brief func の正規化を行う．
void
NpnMgr::cannonical(const TvFunc& func,
		   NpnMap& cmap)
{
  mImpl->cannonical(func, cmap);
}

// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
void
NpnMgr::all_map(list<NpnMap>& map_list) const
{
  mImpl->all_map(map_list);
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
ymulong
NpnMgr::w2max_count() const
{
  return mImpl->w2max_count();
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
ymulong
NpnMgr::tvmax_count() const
{
  return mImpl->tvmax_count();
}

END_NAMESPACE_YM_NPN
