#ifndef NETWORKS_BNETEQUWRITER_H
#define NETWORKS_BNETEQUWRITER_H

/// @file YmNetworks/BNetEquWriter.h
/// @brief BNetEquWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS_BNET

//////////////////////////////////////////////////////////////////////
/// @class BNetEquWriter BNetEquWriter.h <YmNetworks/BNetEquWriter.h>
/// @ingroup BnetGroup
/// @brief BNetwork の内容を equ 形式で出力するクラス
/// @sa BNetwork
//////////////////////////////////////////////////////////////////////
class BNetEquWriter
{
public:

  /// @brief コンストラクタ
  BNetEquWriter();

  /// @brief デストラクタ
  ~BNetEquWriter();


public:

  /// @brief 出力する．
  void
  dump(ostream& s,
       const BNetwork& network) const;

};

END_NAMESPACE_YM_NETWORKS_BNET

#endif // NETWORKS_BNETEQUWRITER_H
