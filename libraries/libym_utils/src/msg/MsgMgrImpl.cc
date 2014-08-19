
/// @file MsgMgrImpl.cc
/// @brief MsgMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MsgMgrImpl.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// MsgMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MsgMgrImpl::MsgMgrImpl()
{
  clear_count();
}

// @brief デストラクタ
// @note ここに登録してあるすべてのハンドラは削除される．
MsgMgrImpl::~MsgMgrImpl()
{
}

// @brief ハンドラを登録する．
// @param[in] handler 登録するハンドラ
void
MsgMgrImpl::reg_handler(MsgHandler* handler)
{
  mMgr.reg_binder(handler);
}

// @brief ハンドラの登録を解除する．
// @param[in] handler 解除するハンドラ
void
MsgMgrImpl::unreg_handler(MsgHandler* handler)
{
  mMgr.unreg_binder(handler);
}

// @brief すべてのハンドラの登録を解除する．
void
MsgMgrImpl::unreg_all_handlers()
{
  mMgr.unreg_all_binders();
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgMgrImpl::put_msg(const char* src_file,
		    int src_line,
		    const FileRegion& file_loc,
		    MsgType type,
		    const char* label,
		    const char* msg)
{
  switch ( type ) {
  case kMsgError:    ++ mErrorNum; break;
  case kMsgWarning:  ++ mWarningNum; break;
  case kMsgInfo:     ++ mInfoNum; break;
  case kMsgFailure:  ++ mFailNum; break;
  case kMsgDebug:    ++ mDebugNum; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  mMgr.prop_event(src_file, src_line, file_loc, type, label, msg);
}

// @brief メッセージを出力する．
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgMgrImpl::put_msg(const char* src_file,
		    int src_line,
		    MsgType type,
		    const char* label,
		    const char* msg)
{
  switch ( type ) {
  case kMsgError:    ++ mErrorNum; break;
  case kMsgWarning:  ++ mWarningNum; break;
  case kMsgInfo:     ++ mInfoNum; break;
  case kMsgFailure:  ++ mFailNum; break;
  case kMsgDebug:    ++ mDebugNum; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }

  mMgr.prop_event(src_file, src_line, FileRegion(), type, label, msg);
}

// @brief カウント値をクリアする．
void
MsgMgrImpl::clear_count()
{
  mErrorNum = 0;
  mWarningNum = 0;
  mInfoNum = 0;
  mFailNum = 0;
  mDebugNum = 0;
}

// @brief 全メッセージ数を得る．
ymuint32
MsgMgrImpl::msg_num() const
{
  return mErrorNum + mWarningNum + mInfoNum + mFailNum + mDebugNum;
}

// @brief エラーメッセージ数を得る．
ymuint32
MsgMgrImpl::error_num() const
{
  return mErrorNum;
}

// @brief 警告メッセージ数を得る．
ymuint32
MsgMgrImpl::warning_num() const
{
  return mWarningNum;
}

// @brief 情報メッセージ数を得る．
ymuint32
MsgMgrImpl::info_num() const
{
  return mInfoNum;
}

// @brief 失敗メッセージ数を得る．
ymuint32
MsgMgrImpl::fail_num() const
{
  return mFailNum;
}

// @brief デバッグメッセージ数を得る．
ymuint32
MsgMgrImpl::debug_num() const
{
  return mDebugNum;
}

END_NAMESPACE_YM
