
/// @file MsgMgr.sip
/// @brief MsgMgr の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class MsgMgr
{
%TypeHeaderCode
#include "utils/MsgMgr.h"
using nsYm::MsgMgr;
%End

public:

  /// @brief ハンドラを登録する．
  /// @param[in] handler 登録するハンドラ
  static
  void
  reg_handler(SIP_PYCALLABLE handler);
  %MethodCode

  %End

  /// @brief ハンドラの登録を解除する．
  /// @param[in] handler 解除するハンドラ
  /// @note ハンドラの登録が解除されてもハンドラその物の削除は行わない．
  static
  void
  unreg_handler(SIP_PYCALLABLE handler);
  %MethodCode

  %End

  /// @brief すべてのハンドラの登録を解除する．
  static
  void
  unreg_all_handlers();

  /// @brief メッセージを出力する(パーサー用)．
  /// @param[in] src_file この関数を呼んでいるソースファイル名
  /// @param[in] src_line この関数を呼んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] msg メッセージ本文
  static
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& file_loc,
	  tMsgType type,
	  const char* label,
	  const char* msg);

  /// @brief メッセージを出力する(一般用)．
  /// @param[in] src_file この関数を呼んでいるソースファイル名
  /// @param[in] src_line この関数を呼んでいるソースの行番号
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] msg メッセージ本文
  static
  void
  put_msg(const char* src_file,
	  int src_line,
	  tMsgType type,
	  const char* label,
	  const char* msg);

  /// @brief カウント値をクリアする．
  static
  void
  clear_count();

  /// @brief 全メッセージ数を得る．
  static
  unsigned int
  msg_num();

  /// @brief エラーメッセージ数を得る．
  static
  unsigned int
  error_num();

  /// @brief 警告メッセージ数を得る．
  static
  unsigned int
  warning_num();

  /// @brief 情報メッセージ数を得る．
  static
  unsigned int
  info_num();

  /// @brief 失敗メッセージ数を得る．
  static
  unsigned int
  fail_num();

  /// @brief デバッグメッセージ数を得る．
  static
  unsigned int
  debug_num();

};
