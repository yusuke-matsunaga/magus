
/// @file VlwModuleHeader.cc
/// @brief VlwModuleHeader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VlwModuleHeader.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlwModuleHeader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
VlwModuleHeader::VlwModuleHeader(VlWriter& writer) :
  VlwBase(writer)
{
}

// @brief デストラクタ
VlwModuleHeader::~VlwModuleHeader()
{
  writer().end_module();
}

END_NAMESPACE_YM_NETWORKS

