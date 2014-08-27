#ifndef NETWORKS_BDNVERILOGWRITER_H
#define NETWORKS_BDNVERILOGWRITER_H

/// @file YmNetworks/BdnVerilogWriter.h
/// @brief BdnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnVerilogWriter BdnVerilogWriter.h <YmNetworks/BdnVerilogWriter.h>
/// @ingroup BnetGroup
/// @brief BdnMgr の内容を blif 形式で出力するクラス
/// @sa Bdnwork
//////////////////////////////////////////////////////////////////////
class BdnVerilogWriter
{
public:

  /// @brief コンストラクタ
  BdnVerilogWriter();

  /// @brief デストラクタ
  ~BdnVerilogWriter();


public:

  /// @brief 出力する．
  void
  operator()(ostream& s,
	     const BdnMgr& network);

};

END_NAMESPACE_YM_NETWORKS_BDN

#endif // NETWORKS_BDNVERILOGWRITER_H
