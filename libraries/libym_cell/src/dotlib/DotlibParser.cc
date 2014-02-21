
/// @file DotlibParser.cc
/// @brief DotlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibParser.h"
#include "DotlibMgr.h"
#include "DotlibParserImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibParser::DotlibParser() :
  mImpl(new DotlibParserImpl)
{
}

// @brief デストラクタ
DotlibParser::~DotlibParser()
{
  delete mImpl;
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] mgr DotlibNode を管理するオブジェクト
// @param[in] debug デバッグモード
// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
// @return 読み込みが成功したら true を返す．
// @note パース木は mgr にセットされる．
bool
DotlibParser::read_file(const string& filename,
			DotlibMgr& mgr,
			bool debug,
			bool allow_no_semi)
{
  return mImpl->read_file(filename, mgr.mImpl, debug, allow_no_semi);
}

END_NAMESPACE_YM_DOTLIB
