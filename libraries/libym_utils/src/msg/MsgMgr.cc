
/// @file MsgMgr.cc
/// @brief MsgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MsgMgr.h"
#include "MsgMgrImpl.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

MsgMgrImpl gTheMgr;

END_NONAMESPACE


// @brief MsgType のストリーム出力演算子
// @param[in] s 出力先のストリーム
// @param[in] type メッセージの種類
ostream&
operator<<(ostream& s,
	   MsgType type)
{
  switch ( type ) {
  case kMsgError:   s << "(ERROR  )"; break;
  case kMsgWarning: s << "(WARNING)"; break;
  case kMsgInfo:    s << "(INFO   )"; break;
  case kMsgFailure: s << "(FAILURE)"; break;
  case kMsgDebug:   s << "(DEBUG  )"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス MsgMgr
//////////////////////////////////////////////////////////////////////

// @brief ハンドラを登録する．
// @param[in] handler 登録するハンドラ
void
MsgMgr::reg_handler(MsgHandler* handler)
{
  gTheMgr.reg_handler(handler);
}

// @brief ハンドラの登録を解除する．
// @param[in] handler 解除するハンドラ
// @note ハンドラの登録が解除されてもハンドラその物の削除は行わない．
void
MsgMgr::unreg_handler(MsgHandler* handler)
{
  gTheMgr.unreg_handler(handler);
}

// @brief すべてのハンドラの登録を解除する．
void
MsgMgr::unreg_all_handlers()
{
  gTheMgr.unreg_all_handlers();
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgMgr::put_msg(const char* src_file,
		int src_line,
		const FileRegion& file_loc,
		MsgType type,
		const char* label,
		const char* msg)
{
  gTheMgr.put_msg(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgMgr::put_msg(const char* src_file,
		int src_line,
		MsgType type,
		const char* label,
		const char* msg)
{
  gTheMgr.put_msg(src_file, src_line, type, label, msg);
}

// @brief カウント値をクリアする．
void
MsgMgr::clear_count()
{
  gTheMgr.clear_count();
}

// @brief 全メッセージ数を得る．
ymuint32
MsgMgr::msg_num()
{
  return gTheMgr.msg_num();
}

// @brief エラーメッセージ数を得る．
ymuint32
MsgMgr::error_num()
{
  return gTheMgr.error_num();
}

// @brief 警告メッセージ数を得る．
ymuint32
MsgMgr::warning_num()
{
  return gTheMgr.warning_num();
}

// @brief 情報メッセージ数を得る．
ymuint32
MsgMgr::info_num()
{
  return gTheMgr.info_num();
}

// @brief 失敗メッセージ数を得る．
ymuint32
MsgMgr::fail_num()
{
  return gTheMgr.fail_num();
}

// @brief デバッグメッセージ数を得る．
ymuint32
MsgMgr::debug_num()
{
  return gTheMgr.debug_num();
}

END_NAMESPACE_YM
