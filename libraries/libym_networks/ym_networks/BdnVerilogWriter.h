#ifndef YM_NETWORKS_BDNVERILOGWRITER_H
#define YM_NETWORKS_BDNVERILOGWRITER_H

/// @file ym_networks/BdnVerilogWriter.h
/// @brief BdnVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BdnVerilogWriter BdnVerilogWriter.h <ym_networks/BdnVerilogWriter.h>
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

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_BDNVERILOGWRITER_H
