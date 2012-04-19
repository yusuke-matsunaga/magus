
/// @file TclObjMsgHandler.cc
/// @brief TclObjMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/TclObjMsgHandler.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// TclObj に出力するメッセージハンドラ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TclObjMsgHandler::TclObjMsgHandler()
{
}

// @brief デストラクタ
TclObjMsgHandler::~TclObjMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
TclObjMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  tMsgType type,
			  const char* label,
			  const char* body)
{
  ostringstream buf;
  buf << loc << type << " [" << label << "]: " << body << endl;
  mMsg << buf.str();
}

// @brief 内容をクリアする．
void
TclObjMsgHandler::clear()
{
  mMsg.clear();
}

// @brief メッセージオブジェクトを取り出す．
TclObj
TclObjMsgHandler::msg_obj() const
{
  return mMsg;
}

END_NAMESPACE_YM_TCLPP
