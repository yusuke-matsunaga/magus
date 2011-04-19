
/// @file libym_cell/dotlib/FfHandler.cc
/// @brief FfHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "FfHandler.h"

#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FfHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
FfHandler::FfHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);

  // simple attributes
  reg_handler("clear", dummy_simple);
  reg_handler("clear_preset_var1", dummy_simple);
  reg_handler("clear_preset_var2", dummy_simple);
  reg_handler("clocked_on", dummy_simple);
  reg_handler("clocked_on_also", dummy_simple);
  reg_handler("next_state", dummy_simple);
  reg_handler("preset", dummy_simple);
}

// @brief デストラクタ
FfHandler::~FfHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] group_name グループ名
bool
FfHandler::read_group_name(const string& attr_name,
				   const string& group_name)
{
  cout << attr_name << "( " << group_name << " ) {" << endl;
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
FfHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
