
/// @file libym_cell/dotlib/DummySimpleHandler.cc
/// @brief DummySimpleHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DummySimpleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DummySimpleHandler::DummySimpleHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
DummySimpleHandler::~DummySimpleHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] value 値を表すトークン
// @return エラーが起きたら false を返す．
bool
DummySimpleHandler::read_value(const string& attr_name,
			       Token token)
{
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
