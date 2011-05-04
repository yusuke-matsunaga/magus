
/// @file libym_utils/MsgHandler.cc
/// @brief MsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MsgHandler.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MsgHandler.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MsgHandler::MsgHandler(ymuint32 mask) :
  mMask(mask)
{
}

// @brief デストラクタ
MsgHandler::~MsgHandler()
{
}

// @brief メッセージマスクの設定
void
MsgHandler::set_mask(ymuint32 mask)
{
  mMask = mask;
}

// @brief メッセージマスクの取得
ymuint32
MsgHandler::mask() const
{
  return mMask;
}

// @brief マスクの付加
// @param[in] type 付加するメッセージタイプ
// @note type はビットマスクではない．
void
MsgHandler::add_mask(tMsgType type)
{
  mMask |= conv2bitmask(type);
}

// @brief マスクの削除
// @param[in] type 削除するメッセージタイプ
// @note type はビットマスクではない．
void
MsgHandler::delete_mask(tMsgType type)
{
  mMask &= ~(conv2bitmask(type));
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
MsgHandler::event_proc(const char* src_file,
		       int src_line,
		       const FileRegion& loc,
		       tMsgType type,
		       const char* label,
		       const char* body)
{
  ymuint32 bit = conv2bitmask(type);
  if ( mMask & bit ) {
    put_msg(src_file, src_line, loc, type, label, body);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス StreamMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StreamMsgHandler::StreamMsgHandler(ostream* stream_ptr) :
  mStreamPtr(stream_ptr)
{
}

// @brief デストラクタ
StreamMsgHandler::~StreamMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
StreamMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  tMsgType type,
			  const char* label,
			  const char* body)
{
  (*mStreamPtr) << loc << type << " [" << label << "]: " << body << endl;
}

END_NAMESPACE_YM
