
/// @file BNetHandle.cc
/// @brief BNetHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "BNetHandle.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス BNetHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
BNetHandle::BNetHandle(const string& name) :
  NetHandle(name)
{
}

// @brief デストラクタ
BNetHandle::~BNetHandle()
{
}

// @brief 型を返す．
NetHandle::Type
BNetHandle::type() const
{
  return kMagBn;
}

// @brief BnNetwork を得る．
const BnNetwork*
BNetHandle::bnetwork() const
{
  return &mNetwork;
}

// @brief BnNetwork を得る．
BnNetwork*
BNetHandle::_bnetwork()
{
  return &mNetwork;
}

// @brief ネットワークをクリアする．
void
BNetHandle::clear()
{
  mNetwork.clear();
}

// @brief ネットワークをコピーする．
// @param[in] src コピー元のネットハンドル
// @param[in] allow_conv true ならタイプが異なる時に変換する．
// @return コピーが成功したら true を返す．
bool
BNetHandle::copy(const NetHandle* src,
		 bool allow_conv)
{
  switch ( src->type() ) {
  case kMagBn:
    mNetwork.copy(*(src->bnetwork()));
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
