
/// @file libym_cell/dotlib/InputVoltageHandler.cc
/// @brief InputVoltageHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "InputVoltageHandler.h"

#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス InputVoltageHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
InputVoltageHandler::InputVoltageHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);

  reg_handler("vil", dummy_simple);
  reg_handler("vih", dummy_simple);
  reg_handler("vimin", dummy_simple);
  reg_handler("vimax", dummy_simple);
}

// @brief デストラクタ
InputVoltageHandler::~InputVoltageHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
InputVoltageHandler::begin_group(const string& attr_name,
				 const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
InputVoltageHandler::end_group()
{
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
