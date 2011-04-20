
/// @file libym_cell/dotlib/PowerLutTemplateHandler.cc
/// @brief PowerLutTemplateHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PowerLutTemplateHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PowerLutTemplateHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
PowerLutTemplateHandler::PowerLutTemplateHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // simple attributes
  reg_handler("variable_1", dummy_simple);
  reg_handler("variable_2", dummy_simple);
  reg_handler("variable_3", dummy_simple);

  // complex attributes
  reg_handler("index_1", dummy_complex);
  reg_handler("index_2", dummy_complex);
  reg_handler("index_3", dummy_complex);

  // group statements
  reg_handler("domain", dummy_group);
}

// @brief デストラクタ
PowerLutTemplateHandler::~PowerLutTemplateHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
PowerLutTemplateHandler::begin_group(const string& attr_name,
				     const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
PowerLutTemplateHandler::end_group()
{
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
