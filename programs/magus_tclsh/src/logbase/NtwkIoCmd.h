#ifndef MAGUS_LOGBASE_NTWKIOCMD_H
#define MAGUS_LOGBASE_NTWKIOCMD_H

/// @file magus/logbase/NtwkIoCmd.h
/// @brief ネットワーク入出力コマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NtwkIoCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagBNetCmd.h"
#include "ym_utils/MsgHandler.h"
#include "ym_bnet/BNetBlifReader.h"
#include "ym_bnet/BNetIscas89Reader.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class TclObjMsgHandler NtwkIoCmd.h "NtwkIoCmd.h"
/// @brief TclObj に出力するメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class TclObjMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  TclObjMsgHandler();

  /// @brief デストラクタ
  virtual
  ~TclObjMsgHandler();


public:
  
  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body);

  /// @brief 内容をクリアする．
  void
  clear();


public:

  /// @brief メッセージオブジェクトを取り出す．
  TclObj
  msg_obj() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TclObj mMsg;

};


//////////////////////////////////////////////////////////////////////
/// @class ReadBlif NtwkIoCmd.h "NtwkIoCmd.h"
/// @brief blif 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlif :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  ReadBlif(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadBlif();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Blif ファイルを読み込むためのパーサ
  BNetBlifReader mReader;

  // メッセージハンドラ
  TclObjMsgHandler mMsgHandler;
  
};


//////////////////////////////////////////////////////////////////////
/// @class ReadIscas89 NtwkIoCmd.h "NtwkIoCmd.h"
/// @brief iscas89 形式のファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class ReadIscas89 :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  ReadIscas89(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadIscas89();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // iscas89 ファイルを読み込むためのパーサ
  BNetIscas89Reader mReader;

  // メッセージハンドラ
  TclObjMsgHandler mMsgHandler;
  
};


//////////////////////////////////////////////////////////////////////
/// @class WriteBlif NtwkIoCmd.h "NtwkIoCmd.h"
/// @brief ファイルに blif 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////
class WriteBlif :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  WriteBlif(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~WriteBlif();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class WriteEqu NtwkIoCmd.h "NtwkIoCmd.h"
/// @brief ファイルに EQU 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////
class WriteEqu :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  WriteEqu(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~WriteEqu();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};


//////////////////////////////////////////////////////////////////////
/// @class WriteVerilog NtwkIoCmd.h "NtwkIoCmd.h"
/// @brief ファイルに Verilog 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////
class WriteVerilog :
  public MagBNetCmd
{
public:

  /// @brief コンストラクタ
  WriteVerilog(NetMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~WriteVerilog();


protected:
  
  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_LOGBASE_NTWKIOCMD_H
