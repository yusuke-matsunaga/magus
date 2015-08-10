
/// @file magus/logbase/NetHandle.cc
/// @brief NetHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NetHandle.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス NetHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
NetHandle::NetHandle(const string& name) :
  mName(name),
  mCount(0)
{
}

// @brief デストラクタ
NetHandle::~NetHandle()
{
}

// @brief BNetwork を得る．
const BNetwork*
NetHandle::bnetwork() const
{
  return nullptr;
}

// @brief BNetwork を得る．
BNetwork*
NetHandle::_bnetwork()
{
  return nullptr;
}

// @brief BdnMgr を得る．
const BdnMgr*
NetHandle::bdn() const
{
  return nullptr;
}

// @brief BdnMgr を得る．
BdnMgr*
NetHandle::_bdn()
{
  return nullptr;
}

// @brief MvNetwork を得る．
const MvnMgr*
NetHandle::mvn() const
{
  return nullptr;
}

// @brief MvNetwork を得る．
MvnMgr*
NetHandle::_mvn()
{
  return nullptr;
}

END_NAMESPACE_MAGUS
