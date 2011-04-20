
/// @file libym_cell/dotlib/DummyGroupHandler.cc
/// @brief DummyGroupHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DummyGroupHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DummyGroupHandler::DummyGroupHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
}

// @brief デストラクタ
DummyGroupHandler::~DummyGroupHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
DummyGroupHandler::begin_group(const string& attr_name,
			       const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
DummyGroupHandler::end_group()
{
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
