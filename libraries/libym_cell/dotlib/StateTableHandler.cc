
/// @file libym_cell/dotlib/StateTableHandler.cc
/// @brief StateTableHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StateTableHandler.h"

#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス StateTableHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
StateTableHandler::StateTableHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);

  // simple attributes
  reg_handler("table", dummy_simple);
}

// @brief デストラクタ
StateTableHandler::~StateTableHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
StateTableHandler::begin_group(const string& attr_name,
			       const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
StateTableHandler::end_group()
{
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
