#ifndef NETWORKS_BNETBLIFWRITER_H
#define NETWORKS_BNETBLIFWRITER_H

/// @file YmNetworks/BNetBlifWriter.h
/// @brief BNetBlifWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifWriter BNetBlifWriter.h <YmNetworks/BNetBlifWriter.h>
/// @ingroup BnetGroup
/// @brief BNetwork の内容を blif 形式で出力するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetBlifWriter
{
public:

  /// @brief コンストラクタ
  BNetBlifWriter();

  /// @brief デストラクタ
  ~BNetBlifWriter();


public:

  /// @brief 出力する．
  void
  dump(ostream& s,
       const BNetwork& network) const;

};

END_NAMESPACE_YM_NETWORKS_BNET

#endif // NETWORKS_BNETBLIFWRITER_H
