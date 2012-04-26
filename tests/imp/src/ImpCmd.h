#ifndef IMPCMD_H
#define IMPCMD_H

/// @file ImpCmd.h
/// @brief ImpCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_tclpp/TclCmd.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpData ImpCmd.h "ImpCmd.h"
/// @brief ImpCmd に共通のデータ
//////////////////////////////////////////////////////////////////////
struct ImpData
{
  // ImpMgr
  ImpMgr mImpMgr;
};


//////////////////////////////////////////////////////////////////////
/// @class ImpCmd ImpCmd.h "ImpCmd.h"
/// @brief 間接含意の計算用のコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class ImpCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_data 共通のデータ
  ImpCmd(ImpData* imp_data);

  /// @brief デストラクタ
  virtual
  ~ImpCmd();


protected:

  /// @brief ImpMgr を得る．
  ImpMgr&
  mgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 共通のデータ
  ImpData* mData;

};


//////////////////////////////////////////////////////////////////////
/// @class ReadBlifCmd ImpCmd.h "ImpCmd.h"
/// @brief blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlifCmd :
  public ImpCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_data 共通のデータ
  ReadBlifCmd(ImpData* imp_data);

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
/// @class ReadBlifCmd ImpCmd.h "ImpCmd.h"
/// @brief iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89Cmd :
  public ImpCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_data 共通のデータ
  ReadIscas89Cmd(ImpData* imp_data);

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
/// @class LearningCmd ImpCmd.h "ImpCmd.h"
/// @brief 間接含意を求めるコマンド
//////////////////////////////////////////////////////////////////////
class LearningCmd :
  public ImpCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_data 共通のデータ
  LearningCmd(ImpData* imp_data);

  /// @brief デストラクタ
  virtual
  ~LearningCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // method オプション
  TclPoptStr* mPoptMethod;

  // level オプション
  TclPoptInt* mPoptLevel;

  // dump オプション
  TclPopt* mPoptDump;

  // verify オプション
  TclPopt* mPoptVerify;

};


//////////////////////////////////////////////////////////////////////
/// @class CheckConstCmd ImpCmd.h "ImpCmd.h"
/// @brief 定数チェックを行うコマンド
//////////////////////////////////////////////////////////////////////
class CheckConstCmd :
  public ImpCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_data 共通のデータ
  CheckConstCmd(ImpData* imp_data);

  /// @brief デストラクタ
  virtual
  ~CheckConstCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class PrintCmd ImpCmd.h "ImpCmd.h"
/// @brief ネットワークを出力するコマンド
//////////////////////////////////////////////////////////////////////
class PrintCmd :
  public ImpCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_data 共通のデータ
  PrintCmd(ImpData* imp_data);

  /// @brief デストラクタ
  virtual
  ~PrintCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPCMD_H
