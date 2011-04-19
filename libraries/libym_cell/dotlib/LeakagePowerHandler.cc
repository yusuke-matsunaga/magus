
/// @file libym_cell/dotlib/LeakagePowerHandler.cc
/// @brief LeakagePowerHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LeakagePowerHandler.h"

#include "DummySimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス LeakagePowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
LeakagePowerHandler::LeakagePowerHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);

  reg_handler("power_level", dummy_simple);
  reg_handler("related_pg_pin", dummy_simple);
  reg_handler("when", dummy_simple);
  reg_handler("value", dummy_simple);
}

// @brief デストラクタ
LeakagePowerHandler::~LeakagePowerHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
LeakagePowerHandler::begin_group(const string& attr_name,
				 const vector<Token>& token_list)
{
  cout << attr_name << "( ";
  ymuint n = token_list.size();
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    cout << comma << token_list[i].value();
    comma = ", ";
  }
  cout << " ) {" << endl;
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
LeakagePowerHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
