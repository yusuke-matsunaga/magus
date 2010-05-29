#ifndef SRC_SEAL_TMRCMD_H
#define SRC_SEAL_TMRCMD_H

/// @file src/seal/TMRCmd.h
/// @brief TMRCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TMRCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagBNetCmd.h"
#include "seal.h"


BEGIN_NAMESPACE_MAGUS_SEAL

//////////////////////////////////////////////////////////////////////
// 回路を3重化するコマンド
//////////////////////////////////////////////////////////////////////
class TMRCmd :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  TMRCmd(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~TMRCmd();


protected:
  
  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS_SEAL

#endif // SRC_SEAL_TMRCMD_H
