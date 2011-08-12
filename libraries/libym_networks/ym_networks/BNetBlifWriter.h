#ifndef YM_NETWORKS_BNETBLIFWRITER_H
#define YM_NETWORKS_BNETBLIFWRITER_H

/// @file ym_networks/BNetBlifWriter.h
/// @brief BNetBlifWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifWriter BNetBlifWriter.h <ym_networks/BNetBlifWriter.h>
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

#endif // YM_NETWORKS_BNETBLIFWRITER_H
