#ifndef NTWKIOCMD_H
#define NTWKIOCMD_H

/// @file NtwkIoCmd.h
/// @brief NtwkIoCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"
#include "YmTclpp/TclPopt.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlif :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  ReadBlif(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadBlif();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
// iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89 :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  ReadIscas89(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadIscas89();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
// TgNetwork の内容を出力するコマンド
//////////////////////////////////////////////////////////////////////
class WriteNetwork :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  WriteNetwork(AtpgMgr* mgr);


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_SATPG

#endif // NTWKIOCMD_H
