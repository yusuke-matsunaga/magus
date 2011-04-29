
/// @file libym_dotlib/DotlibParser.cc
/// @brief DotlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellDotlibReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibParser.h"
#include "DotlibParserImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] msg_mgr メッセージを管理するオブジェクト
DotlibParser::DotlibParser(MsgMgr& msg_mgr) :
  mImpl(new DotlibParserImpl(msg_mgr))
{
}

// @brief デストラクタ
DotlibParser::~DotlibParser()
{
  delete mImpl;
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] debug デバッグモード
// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
// @return パース木の根のノードを返す．
// @note エラーが起きたら NULL を返す．
const PtNode*
DotlibParser::read_file(const string& filename,
			bool debug,
			bool allow_no_semi)
{
  return mImpl->read_file(filename, debug, allow_no_semi);
}

// @brief 直前の read_file() で確保したパース木を解放する．
void
DotlibParser::clear_node()
{
  mImpl->clear_node();
}

// @brief メモリ使用量のサマリを出力する．
// @param[in] s 出力先のストリーム
void
DotlibParser::show_stats(ostream& s)
{
  mImpl->show_stats(s);
}

END_NAMESPACE_YM_DOTLIB
