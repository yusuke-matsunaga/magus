#ifndef AGUS_LOGBASE_NTWKBASECMD_H
#define MAGUS_LOGBASE_NTWKBASECMD_H

/// @file magus/logbase/NtwkBaseCmd.h
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
/// @class NewBNetwork NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief 新しい BNetwork を作成するコマンド
//////////////////////////////////////////////////////////////////////
class NewBNetwork :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  NewBNetwork(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~NewBNetwork();


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
/// @class NewBdn NtwkBaseCmd.h "NtwkBaseCmd.h"
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
/// @class NewMvn NtwkBaseCmd.h "NtwkBaseCmd.h"
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
/// @class DelNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief ネットワークを削除するコマンド
//////////////////////////////////////////////////////////////////////
class DelNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  DelNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DelNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ClrNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief ネットワークの内容をクリアするコマンド
//////////////////////////////////////////////////////////////////////
class ClrNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ClrNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ClrNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class CopyNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief コピーを行うコマンド
//////////////////////////////////////////////////////////////////////
class CopyNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  CopyNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~CopyNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ChgNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief カレントネットワークを変更するコマンド
//////////////////////////////////////////////////////////////////////
class ChgNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  ChgNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ChgNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief -new_bnet, -new_bdn オプションの処理を行う．
  /// @param[in] name ネットワーク名
  int
  new_opt(string name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // new_bnet オプション解析用のオブジェクト
  TclPopt* mPoptNewBNet;

  // new_bdn オプション解析用のオブジェクト
  TclPopt* mPoptNewBdn;

  // new_mvn オプション解析用のオブジェクト
  TclPopt* mPoptNewMvn;

};


//////////////////////////////////////////////////////////////////////
/// @class PushNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
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


//////////////////////////////////////////////////////////////////////
/// @class PopNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
/// @brief カレントネットワークのスタックを元に戻す．
//////////////////////////////////////////////////////////////////////
class PopNtwk :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  PopNtwk(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PopNtwk();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class CurNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
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
/// @class ListNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
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
/// @class ForNtwk NtwkBaseCmd.h "NtwkBaseCmd.h"
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

#endif // MAGUS_LOGBASE_NTWKBASECMD_H
