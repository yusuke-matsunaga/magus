
/// @file VlwModule.cc
/// @brief VlwModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwModule.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス VlwModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] name モジュール名
VlwModule::VlwModule(VlWriter& writer,
		     const char* name) :
  VlwBase(writer)
{
  writer.begin_module(name);
}

// @brief コンストラクタ
// @param[in] writer 出力用オブジェクト
// @param[in] name モジュール名
VlwModule::VlwModule(VlWriter& writer,
		     const string& name) :
  VlwBase(writer)
{
  writer.begin_module(name);
}

// @brief デストラクタ
VlwModule::~VlwModule()
{
  writer().begin_endmodule();
}

END_NAMESPACE_YM_NETWORKS
