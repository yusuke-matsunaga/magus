#ifndef MAGUS_LOGBASE_NTWKINFOCMD_H
#define MAGUS_LOGBASE_NTWKINFOCMD_H

/// @file magus/logbase/NtwkInfoCmd.h
/// @brief ネットワークの情報を取得するコマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkInfoCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagBNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NtwkInfo NtwkInfoCmd.h "NtwkInfoCmd.h"
/// @brief ネットワークの諸元を取り出すコマンド
//////////////////////////////////////////////////////////////////////
class NtwkInfo :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  NtwkInfo(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NtwkInfo();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class NtwkAllInfo NtwkInfoCmd.h "NtwkInfoCmd.h"
/// @brief ネットワークの諸元を一括して取り出して連想配列に入れるコマンド
//////////////////////////////////////////////////////////////////////
class NtwkAllInfo :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  NtwkAllInfo(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NtwkAllInfo();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class NtwkPrintStats NtwkInfoCmd.h "NtwkInfoCmd.h"
/// @brief ネットワークの緒元を出力するコマンド
//////////////////////////////////////////////////////////////////////
class NtwkPrintStats :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  NtwkPrintStats(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NtwkPrintStats();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LOGBASE_NTWKINFOCMD_H
