#ifndef RECCMD_H
#define RECCMD_H

/// @file RecCmd.h
/// @brief RecCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"
#include "tclpp/TclCmd.h"


BEGIN_NAMESPACE_YM

class FuncMgr;

//////////////////////////////////////////////////////////////////////
/// @class RecCmd RecCmd.h "RecCmd.h"
/// @brief FuncMgr を扱うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class RecCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  RecCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~RecCmd();


protected:

  /// @brief FuncMgr を得る．
  FuncMgr&
  mgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FuncMgr
  FuncMgr& mFuncMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class ReadCmd RecCmd.h "RecCmd.h"
/// @brief ReadBlif と ReadIscas89 に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class ReadCmd :
  public RecCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  ReadCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~ReadCmd();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カット列挙を行い関数の登録を行う．
  /// @param[in] network 対象のネットワーク
  int
  record(const BdnMgr& network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // min_cut_size オプション
  TclPoptInt* mMinCutSize;

  // max_cut_size オプション
  TclPoptInt* mMaxCutSize;

};


//////////////////////////////////////////////////////////////////////
/// @class ReadBlifCmd RecCmd.h "RecCmd.h"
/// @brief blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlifCmd :
  public ReadCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  ReadBlifCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~ReadBlifCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class ReadBlifCmd RecCmd.h "RecCmd.h"
/// @brief iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89Cmd :
  public ReadCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  ReadIscas89Cmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~ReadIscas89Cmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class DumpCmd RecCmd.h "RecCmd.h"
/// @brief ダンプコマンド
//////////////////////////////////////////////////////////////////////
class DumpCmd :
  public RecCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  DumpCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~DumpCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class DumpRepCmd RecCmd.h "RecCmd.h"
/// @brief ダンプコマンド
//////////////////////////////////////////////////////////////////////
class DumpRepCmd :
  public RecCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  DumpRepCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~DumpRepCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class RestoreCmd RecCmd.h "RecCmd.h"
/// @brief リストアコマンド
//////////////////////////////////////////////////////////////////////
class RestoreCmd :
  public RecCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  RestoreCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~RestoreCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class PrintCmd RecCmd.h "RecCmd.h"
/// @brief 内容を出力するコマンド
//////////////////////////////////////////////////////////////////////
class PrintCmd :
  public RecCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  PrintCmd(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~PrintCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM

#endif // RECCMD_H
