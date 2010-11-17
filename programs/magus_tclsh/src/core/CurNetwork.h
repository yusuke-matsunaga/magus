#ifndef SRC_CORE_CURNETWORK_H
#define SRC_CoRE_CURNETWORK_H

/// @file src/core/CurNetwork.h
/// @brief CurNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class CurNtwk BNetBaseCmd.h "BNetBaseCmd.h"
/// @brief カレントネットワーク名を返すコマンド
//////////////////////////////////////////////////////////////////////
class CurNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  CurNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CurNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_CORE_CURNETWORK_H
