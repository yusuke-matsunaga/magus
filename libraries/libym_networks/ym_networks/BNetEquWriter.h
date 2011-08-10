#ifndef YM_NETWORKS_BNETEQUWRITER_H
#define YM_NETWORKS_BNETEQUWRITER_H

/// @file ym_networks/BNetEquWriter.h
/// @brief BNetEquWriter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bnet.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BNetEquWriter BNetEquWriter.h <ym_networks/BNetEquWriter.h>
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

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_BNETEQUWRITER_H
