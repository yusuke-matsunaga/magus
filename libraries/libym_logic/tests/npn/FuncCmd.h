#ifndef FUNCCMD_H
#define FUNCCMD_H

/// @file FuncCmd.h
/// @brief FuncCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "tclpp/TclCmd.h"
#include "YmLogic/common.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// 全ての関数を列挙するコマンド
//////////////////////////////////////////////////////////////////////
class ForallFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();

  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// ランダムに関数を生成するコマンド
//////////////////////////////////////////////////////////////////////
class RandomFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();

  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ形式のファイルから関数を生成するコマンド
//////////////////////////////////////////////////////////////////////
class ReadFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();

  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// 関数のパタンを出力するコマンド
//////////////////////////////////////////////////////////////////////
class DumpFuncCmd :
  public TclCmd
{
protected:

  /// @brief usage を出力する関数
  virtual
  void usage();

  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_NPN

#endif // FUNCCMD_H
