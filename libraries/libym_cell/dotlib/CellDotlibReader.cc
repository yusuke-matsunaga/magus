
/// @file libym_mislib/CellDotlibReader.cc
/// @brief CellDotlibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellDotlibReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellDotlibReader.h"
#include "DotlibParser.h"
#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL

using nsDotlib::PtNode;

//////////////////////////////////////////////////////////////////////
// MisLibPt オブジェクトを管理するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellDotlibReader::CellDotlibReader()
{
  mParser = new nsDotlib::DotlibParser;
}

// @brief デストラクタ
CellDotlibReader::~CellDotlibReader()
{
  delete mParser;
}

// @brief mislib ファイルを読み込む
// @param[in] filename ファイル名
// @return 読み込んで作成したセルライブラリを返す．
// @note エラーが起きたら NULL を返す．
const CellLibrary*
CellDotlibReader::read(const string& filename)
{
  PtNode* root = mParser->read_file(filename, false, true);
}

// @brief メッセージハンドラの登録
// @param[in] handler 登録するハンドラ
// @note handler はこのインスタンスが破壊される時に同時に破壊される．
void
CellDotlibReader::add_msg_handler(MsgHandler* handler)
{
  mParser->msg_mgr().reg_handler(handler);
}

END_NAMESPACE_YM_CELL
