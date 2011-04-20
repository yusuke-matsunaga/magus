
/// @file libym_cell/dotlib/OutputVoltageHandler.cc
/// @brief OutputVoltageHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "OutputVoltageHandler.h"

#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス OutputVoltageHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
OutputVoltageHandler::OutputVoltageHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);

  reg_handler("vol", dummy_simple);
  reg_handler("voh", dummy_simple);
  reg_handler("vomin", dummy_simple);
  reg_handler("vomax", dummy_simple);
}

// @brief デストラクタ
OutputVoltageHandler::~OutputVoltageHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
OutputVoltageHandler::begin_group(const string& attr_name,
				  const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
OutputVoltageHandler::end_group()
{
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
