
/// @file AtpgMsgHandler.cc
/// @brief AtpgMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgMsgHandler.h"
#include "YmUtils/FileRegion.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
AtpgMsgHandler::AtpgMsgHandler()
{
}

// @brief デストラクタ
AtpgMsgHandler::~AtpgMsgHandler()
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
AtpgMsgHandler::put_msg(const char* src_file,
			int src_line,
			const FileRegion& loc,
			MsgType type,
			const char* label,
			const char* body)
{
  ostringstream buf;
  buf << loc << type << " [" << label << "]: " << body << endl;
  mMsg << buf.str();
}

// @brief 内容をクリアする．
void
AtpgMsgHandler::clear()
{
  mMsg.clear();
}

// @brief メッセージオブジェクトを取り出す．
TclObj
AtpgMsgHandler::msg_obj() const
{
  return mMsg;
}

END_NAMESPACE_YM_SATPG
