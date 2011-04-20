
/// @file libym_cell/dotlib/WireLoadHandler.cc
/// @brief WireLoadHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "WireLoadHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス WireLoadHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
WireLoadHandler::WireLoadHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);

  // simple attribute
  reg_handler("area", dummy_simple);
  reg_handler("capacitance", dummy_simple);
  reg_handler("resistance", dummy_simple);
  reg_handler("slope", dummy_simple);

  // complex attribute
  reg_handler("fanout_length", dummy_complex);
}

// @brief デストラクタ
WireLoadHandler::~WireLoadHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
WireLoadHandler::begin_group(const string& attr_name,
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
WireLoadHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス WireLoadSelectionHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
WireLoadSelectionHandler::WireLoadSelectionHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);

  // simple attribute

  // complex attribute
  reg_handler("wire_load_from_area", dummy_complex);
}

// @brief デストラクタ
WireLoadSelectionHandler::~WireLoadSelectionHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
WireLoadSelectionHandler::begin_group(const string& attr_name,
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
WireLoadSelectionHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス WireLoadTableHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
WireLoadTableHandler::WireLoadTableHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);

  // simple attribute

  // complex attribute
  reg_handler("fanout_area", dummy_complex);
  reg_handler("fanout_capacitance", dummy_complex);
  reg_handler("fanout_length", dummy_complex);
  reg_handler("fanout_resistance", dummy_complex);
}

// @brief デストラクタ
WireLoadTableHandler::~WireLoadTableHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
WireLoadTableHandler::begin_group(const string& attr_name,
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
WireLoadTableHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
