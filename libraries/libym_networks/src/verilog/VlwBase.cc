
/// @file VlwBase.cc
/// @brief VlwBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlwBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] writer 出力用のオブジェクト
// @param[in] name モジュール名
VlwBase::VlwBase(VlWriter& writer) :
  mWriter(writer)
{
}

// @brief デストラクタ
VlwBase::~VlwBase()
{
}

// @brief 出力用のオブジェクトを得る．
VlWriter&
VlwBase::writer()
{
  return mWriter;
}

END_NAMESPACE_YM_NETWORKS
