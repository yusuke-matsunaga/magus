#ifndef YM_NETWORKS_BNETVERILOGWRITER_H
#define YM_NETWORKS_BNETVERILOGWRITER_H

/// @file ym_networks/BNetVerilogWriter.h
/// @brief BNetVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetVerilogWriter BNetVerilogWriter.h <ym_networks/BNetVerilogWriter.h>
/// @ingroup BnetGroup
/// @brief BNetwork の内容を verilog 形式で出力するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetVerilogWriter
{
public:

  /// @brief コンストラクタ
  BNetVerilogWriter();

  /// @brief デストラクタ
  ~BNetVerilogWriter();


public:

  /// @brief 出力する．
  void
  dump(ostream& s,
       const BNetwork& network) const;

};

END_NAMESPACE_YM_NETWORKS_BNET

#endif // YM_NETWORKS_BNETVERILOGWRITER_H
