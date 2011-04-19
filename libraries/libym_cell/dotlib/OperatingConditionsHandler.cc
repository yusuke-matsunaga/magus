
/// @file libym_cell/dotlib/OperatingConditionsHandler.cc
/// @brief OperatingConditionsHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "OperatingConditionsHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス OperatingConditionsHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
OperatingConditionsHandler::OperatingConditionsHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);

  // simple attributes
  reg_handler("calc_mode", dummy_simple);
  reg_handler("parameter1", dummy_simple);
  reg_handler("parameter2", dummy_simple);
  reg_handler("parameter3", dummy_simple);
  reg_handler("parameter4", dummy_simple);
  reg_handler("parameter5", dummy_simple);
  reg_handler("process", dummy_simple);
  reg_handler("temperature", dummy_simple);
  reg_handler("tree_type", dummy_simple);
  reg_handler("voltage", dummy_simple);

  // complex attributes
  reg_handler("power_rail", dummy_complex);
}

// @brief デストラクタ
OperatingConditionsHandler::~OperatingConditionsHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
OperatingConditionsHandler::begin_group(const string& attr_name,
					const vector<Token>& token_list)
{
  cout << attr_name << "( ";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << " ) {" << endl;
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
OperatingConditionsHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
