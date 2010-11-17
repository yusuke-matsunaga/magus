#ifndef SRC_CORE_BNETBASECMD_H
#define SRC_CoRE_BNETBASECMD_H

/// @file src/core/NtwkBaseCmd.h
/// @brief ネットワーク操作関連の主要なコマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkBaseCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ネットワークを操作するサブパッケージ

#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NewBdn BNetBaseCmd.h "BNetBaseCmd.h"
/// @brief 新しい BdNetwork を作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewBdn :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  NewBdn(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewBdn();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // trace オプション解析用オブジェクト
  TclPoptBool* mPoptTrace;

};


//////////////////////////////////////////////////////////////////////
/// @class NewMvn BNetBaseCmd.h "BNetBaseCmd.h"
/// @brief 新しい BdNetwork を作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewMvn :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  NewMvn(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewMvn();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // trace オプション解析用オブジェクト
  TclPoptBool* mPoptTrace;

};


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


//////////////////////////////////////////////////////////////////////
/// @class ListNtwk BNetBaseCmd.h "BNetBaseCmd.h"
/// @brief ネットワークのリストを作るコマンド
//////////////////////////////////////////////////////////////////////
class ListNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ListNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ListNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // all オプション解析用のオブジェクト
  TclPopt* mPoptAll;

};


//////////////////////////////////////////////////////////////////////
/// @class ForNtwk BNetBaseCmd.h "BNetBaseCmd.h"
/// @brief ネットワークを一つづつ取り出して処理するコマンド
/// その都度，カレントネットワークを切替える．
//////////////////////////////////////////////////////////////////////
class ForNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ForNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ForNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_LOGBASE_BNETBASECMD_H
