#ifndef SRC_CORE_PUSHNETWORK_H
#define SRC_CORE_PUSHNETWORK_H

/// @file src/core/PushNetwork.h
/// @brief PushNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class PushNtwk BNetBaseCmd.h "BNetBaseCmd.h"
/// @brief カレントネットワークスタックにプッシュするコマンド
//////////////////////////////////////////////////////////////////////
class PushNtwk :
  public ChgNtwk
{
public:

  /// @brief コンストラクタ
  PushNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PushNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_PUSHNETWORK_H
