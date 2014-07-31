#ifndef VLWMODULE_H
#define VLWMODULE_H

/// @file VlwModule.h
/// @brief VlwModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class VlwModule VlwModule.h "VlwModule.h"
/// @brief module ブロックを出力するためのクラス
//////////////////////////////////////////////////////////////////////
class VlwModule :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] name モジュール名
  VlwModule(VlWriter& writer,
	    const char* name);

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] name モジュール名
  VlwModule(VlWriter& writer,
	    const string& name);

  /// @brief デストラクタ
  ~VlwModule();

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWMODULE_H
