#ifndef YMUTILS_MSGMGR_H
#define YMUTILS_MSGMGR_H

/// @file YmUtils/MsgMgr.h
/// @brief MsgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/msg_type.h"


BEGIN_NAMESPACE_YM

class FileRegion;
class MsgHandler;

//////////////////////////////////////////////////////////////////////
/// @class MsgMgr MsgMgr.h "YmUtils/MsgMgr.h"
/// @ingroup YmUtils
/// @brief メッセージを管理するクラス
/// このクラスのメンバ関数はすべて static 関数なので
/// このクラスをインスタンス化する必要はない．というかできない．
/// @sa MsgHandler
//////////////////////////////////////////////////////////////////////
class MsgMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // ハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラを登録する．
  /// @param[in] handler 登録するハンドラ
  static
  void
  reg_handler(MsgHandler* handler);

  /// @brief ハンドラの登録を解除する．
  /// @param[in] handler 解除するハンドラ
  /// @note ハンドラの登録が解除されてもハンドラその物の削除は行わない．
  static
  void
  unreg_handler(MsgHandler* handler);

  /// @brief すべてのハンドラの登録を解除する．
  static
  void
  unreg_all_handlers();


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージの出力
  //////////////////////////////////////////////////////////////////////

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
	  MsgType type,
	  const char* label,
	  const char* msg);

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
	  MsgType type,
	  const char* label,
	  const string& msg);

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
	  MsgType type,
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
	  MsgType type,
	  const char* label,
	  const string& msg);


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報の管理
  //////////////////////////////////////////////////////////////////////

  /// @brief カウント値をクリアする．
  static
  void
  clear_count();

  /// @brief 全メッセージ数を得る．
  static
  ymuint32
  msg_num();

  /// @brief エラーメッセージ数を得る．
  static
  ymuint32
  error_num();

  /// @brief 警告メッセージ数を得る．
  static
  ymuint32
  warning_num();

  /// @brief 情報メッセージ数を得る．
  static
  ymuint32
  info_num();

  /// @brief 失敗メッセージ数を得る．
  static
  ymuint32
  fail_num();

  /// @brief デバッグメッセージ数を得る．
  static
  ymuint32
  debug_num();

};


//////////////////////////////////////////////////////////////////////
//　インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセージを出力する(パーサー用)．
// @param[in] src_file この関数を呼んでいるソースファイル名
// @param[in] src_line この関数を呼んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
inline
void
MsgMgr::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		MsgType type,
		const char* label,
		const string& msg)
{
  put_msg(src_file, src_line, file_loc, type, label, msg.c_str());
}

// @brief メッセージを出力する(一般用)．
// @param[in] src_file この関数を呼んでいるソースファイル名
// @param[in] src_line この関数を呼んでいるソースの行番号
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] msg メッセージ本文
inline
void
MsgMgr::put_msg(const char* src_file,
		int src_line,
		MsgType type,
		const char* label,
		const string& msg)
{
  put_msg(src_file, src_line, type, label, msg.c_str());
}

END_NAMESPACE_YM

#endif // YMUTILS_MSGMGR_H
