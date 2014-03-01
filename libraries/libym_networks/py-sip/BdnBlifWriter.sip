
/// @file BdnBlifWriter.sip
/// @brief BdnBlifWriter の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnBlifWriter
{
%TypeHeaderCode
#include "networks/BdnBlifWriter.h"
using nsYm::BdnBlifWriter;
%End

public:

  /// @brief コンストラクタ
  BdnBlifWriter();

  /// @brief 出力する．
  void
  __call__(ostream& s,
	   const BdnMgr& network);

};
