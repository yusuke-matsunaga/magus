
/// @file libym_cell/dotlib/InternalPowerHandler.cc
/// @brief InternalPowerHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "InternalPowerHandler.h"

#include "DummySimpleHandler.h"
#include "DummyComplexHandler.h"
#include "DummyGroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス InternalPowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
InternalPowerHandler::InternalPowerHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // simple attributes
  reg_handler("equal_or_opposite_output", dummy_simple);
  reg_handler("falling_together_group", dummy_simple);
  reg_handler("power_level", dummy_simple);
  reg_handler("related_pin", dummy_simple);
  reg_handler("rising_together_group", dummy_simple);
  reg_handler("switching_interval", dummy_simple);
  reg_handler("switching_together_group", dummy_simple);
  reg_handler("when", dummy_simple);

  reg_handler("related_inputs", dummy_simple);
  reg_handler("related_outputs", dummy_simple);

  // complex attribute

  // group statements
  reg_handler("power", new PowerHandler(parser));
  reg_handler("fall_power", new FallPowerHandler(parser));
  reg_handler("rise_power", new RisePowerHandler(parser));
  reg_handler("domain", dummy_group);
}

// @brief デストラクタ
InternalPowerHandler::~InternalPowerHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
InternalPowerHandler::begin_group(const string& attr_name,
			   const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
InternalPowerHandler::end_group()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CellInternalPowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
CellInternalPowerHandler::CellInternalPowerHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_simple = new DummySimpleHandler(parser);
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);

  // simple attributes
  reg_handler("related_inputs", dummy_simple);
  reg_handler("related_outputs", dummy_simple);

  // complex attribute
  reg_handler("values", dummy_complex);
}

// @brief デストラクタ
CellInternalPowerHandler::~CellInternalPowerHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
CellInternalPowerHandler::begin_group(const string& attr_name,
				      const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
CellInternalPowerHandler::end_group()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
PowerHandler::PowerHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // complex attribute
  reg_handler("index_1", dummy_complex);
  reg_handler("index_2", dummy_complex);
  reg_handler("index_3", dummy_complex);
  reg_handler("values", dummy_complex);
  reg_handler("orders", dummy_complex);
  reg_handler("coefs", dummy_complex);

  // group statements
  reg_handler("domain", dummy_group);
}

// @brief デストラクタ
PowerHandler::~PowerHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
PowerHandler::begin_group(const string& attr_name,
			  const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
PowerHandler::end_group()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス FallPowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
FallPowerHandler::FallPowerHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // complex attribute
  reg_handler("index_1", dummy_complex);
  reg_handler("index_2", dummy_complex);
  reg_handler("index_3", dummy_complex);
  reg_handler("values", dummy_complex);
  reg_handler("orders", dummy_complex);
  reg_handler("coefs", dummy_complex);

  // group statements
  reg_handler("domain", dummy_group);
}

// @brief デストラクタ
FallPowerHandler::~FallPowerHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
FallPowerHandler::begin_group(const string& attr_name,
			      const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
FallPowerHandler::end_group()
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス RisePowerHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
RisePowerHandler::RisePowerHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
  DotLibHandler* dummy_complex = new DummyComplexHandler(parser);
  DotLibHandler* dummy_group = new DummyGroupHandler(parser);

  // complex attribute
  reg_handler("index_1", dummy_complex);
  reg_handler("index_2", dummy_complex);
  reg_handler("index_3", dummy_complex);
  reg_handler("values", dummy_complex);
  reg_handler("orders", dummy_complex);
  reg_handler("coefs", dummy_complex);

  // group statements
  reg_handler("domain", dummy_group);
}

// @brief デストラクタ
RisePowerHandler::~RisePowerHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
RisePowerHandler::begin_group(const string& attr_name,
			      const vector<Token>& token_list)
{
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
RisePowerHandler::end_group()
{
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
