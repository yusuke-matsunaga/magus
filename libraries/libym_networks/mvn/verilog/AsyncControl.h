#ifndef LIBYM_NETWORKS_VERILOG_ASYNCCONTROL_H
#define LIBYM_NETWORKS_VERILOG_ASYNCCONTROL_H

/// @file libym_networks/verilog/AsyncControl.h
/// @brief AsyncControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn.h"
#include "ym_verilog/vl/VlFwd.h"
#include "Env.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AsyncControl AsyncControl.h "AsyncControl.h"
/// @brief 非同期 set/reset 用コントロール
//////////////////////////////////////////////////////////////////////
struct AsyncControl
{

  /// @brief コンストラクタ
  /// @param[in] global_env プロセスの外側の Env
  AsyncControl(const Env& global_env);

  // コントロール信号
  MvnNode* mNode;

  // 極性 ( 1 が正極性, 0 が負極性 )
  ymuint32 mPol;

  // 環境
  ProcEnv mEnv;

};

END_NAMESPACE_YM_NETWORKS_MVN_VERILOG

#endif // LIBYM_NETWORKS_VERILOG_ASYNCCONTROL_H
