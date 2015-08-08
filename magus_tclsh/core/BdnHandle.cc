﻿
/// @file src/logbase/BdnHandle.cc
/// @brief BdnHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "BdnHandle.h"
#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BNetwork.h"
#include "YmNetworks/BNetDecomp.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス BdnHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
BdnHandle::BdnHandle(const string& name) :
  NetHandle(name)
{
}

// @brief デストラクタ
BdnHandle::~BdnHandle()
{
}

// @brief 型を返す．
NetHandle::tType
BdnHandle::type() const
{
  return kMagBdn;
}

// @brief BdnMgr を得る．
const BdnMgr*
BdnHandle::bdn() const
{
  return &mNetwork;
}

// @brief BdnMgr を得る．
BdnMgr*
BdnHandle::_bdn()
{
  return &mNetwork;
}

// @brief ネットワークをクリアする．
void
BdnHandle::clear()
{
  mNetwork.clear();
}

// @brief ネットワークをコピーする．
// @param[in] src コピー元のネットハンドル
// @param[in] allow_conv true ならタイプが異なる時に変換する．
// @return コピーが成功したら true を返す．
bool
BdnHandle::copy(const NetHandle* src,
		bool allow_conv)
{
  switch ( src->type() ) {
  case kMagBNet:
    return false;
#if 0
    if ( allow_conv ) {
      BNetDecomp decomp;
      decomp(src->bnetwork());
    }
#endif
    break;

  case kMagBdn:
    mNetwork = *(src->bdn());
    break;

  case kMagMvn:
    return false;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  return true;
}

END_NAMESPACE_MAGUS
