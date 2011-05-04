#ifndef LIBYM_UTILS_MSGMGRIMPL_H
#define LIBYM_UTILS_MSGMGRIMPL_H

/// @file ym_utils/MsgMgrImpl.h
/// @brief MsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MsgMgrImpl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MsgMgrImpl MsgMgrImpl.h <ym_utils/MsgMgrImpl.h>
//////////////////////////////////////////////////////////////////////
class MsgMgrImpl
{
public:

  /// @brief コンストラクタ
  MsgMgrImpl();

  /// @brief デストラクタ
  /// @note ここに登録してあるすべてのハンドラは削除される．
  ~MsgMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // ハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラを登録する．
  /// @param[in] handler 登録するハンドラ
  void
  reg_handler(MsgHandler* handler);

  /// @brief ハンドラの登録を解除する．
  /// @param[in] handler 解除するハンドラ
  void
  unreg_handler(MsgHandler* handler);

  /// @brief すべてのハンドラの登録を解除する．
  void
  unreg_all_handlers();


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージの出力
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);

  /// @brief メッセージを出力する．
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const string& msg);


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報の管理
  //////////////////////////////////////////////////////////////////////

  /// @brief カウント値をクリアする．
  void
  clear_count();

  /// @brief 全メッセージ数を得る．
  ymuint32
  msg_num() const;

  /// @brief エラーメッセージ数を得る．
  ymuint32
  error_num() const;

  /// @brief 警告メッセージ数を得る．
  ymuint32
  warning_num() const;

  /// @brief 情報メッセージ数を得る．
  ymuint32
  info_num() const;

  /// @brief 失敗メッセージ数を得る．
  ymuint32
  fail_num() const;

  /// @brief デバッグメッセージ数を得る．
  ymuint32
  debug_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージを各ハンドラに送るクラス
  T6BindMgr<const char*,
	    int,
	    const FileRegion&,
	    tMsgType,
	    const char*,
	    const char*> mMgr;

  // エラーメッセージ数
  ymuint32 mErrorNum;

  // 警告メッセージ数
  ymuint32 mWarningNum;

  // 情報メッセージ数
  ymuint32 mInfoNum;

  // 失敗メッセージ数
  ymuint32 mFailNum;

  // デバッグメッセージ数
  ymuint32 mDebugNum;

};

END_NAMESPACE_YM

#endif // LIBYM_UTILS_MSGMGRIMPL_H
