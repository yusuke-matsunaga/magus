#ifndef NETWORKS_BNETVERILOGWRITER_H
#define NETWORKS_BNETVERILOGWRITER_H

/// @file networks/BNetVerilogWriter.h
/// @brief BNetVerilogWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetVerilogWriter BNetVerilogWriter.h <networks/BNetVerilogWriter.h>
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

#endif // NETWORKS_BNETVERILOGWRITER_H
