
/// @file libym_cell/dotlib/LibSimpleHandler.cc
/// @brief LibSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibSimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB


//////////////////////////////////////////////////////////////////////
// クラス BusNamingStyleHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
BusNamingStyleHandler::BusNamingStyleHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
BusNamingStyleHandler::~BusNamingStyleHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
BusNamingStyleHandler::read_value(const string& attr_name,
				   tTokenType type,
				   const string& value)
{
  cout << "bus_naming_style : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CommentHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
CommentHandler::CommentHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
CommentHandler::~CommentHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
CommentHandler::read_value(const string& attr_name,
			   tTokenType type,
			   const string& value)
{
  cout << "comment : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CurrentUnitHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
CurrentUnitHandler::CurrentUnitHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
CurrentUnitHandler::~CurrentUnitHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
CurrentUnitHandler::read_value(const string& attr_name,
			       tTokenType type,
			       const string& value)
{
  cout << "current_unit : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス DateHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DateHandler::DateHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
DateHandler::~DateHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
DateHandler::read_value(const string& attr_name,
			tTokenType type,
			const string& value)
{
  cout << "date : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス DelayModelHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DelayModelHandler::DelayModelHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
DelayModelHandler::~DelayModelHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
DelayModelHandler::read_value(const string& attr_name,
			      tTokenType type,
			      const string& value)
{
  // delay_model : generic_cmos|table_lookup|cmos2|piecewise_cmos|dcm ;

  assert_cond( attr_name == "delay_model", __FILE__, __LINE__);

  cout << "delay_model : " << value << endl;

  if ( type != STR ) {
#warning "TODO: エラーメッセージ"
    return false;
  }

  if ( value == "generic_cmos" ) {
  }
  else if ( value == "table_lookup" ) {
  }
  else if ( value == "cmos2" ) {
  }
  else if ( value == "piecewise_cmos" ) {
  }
  else if ( value == "dcm" ) {
  }
  else {
#warning "TODO: エラーメッセージ"
    return false;
  }

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス InPlaceSwapModeHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
InPlaceSwapModeHandler::InPlaceSwapModeHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
InPlaceSwapModeHandler::~InPlaceSwapModeHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
InPlaceSwapModeHandler::read_value(const string& attr_name,
				   tTokenType type,
				   const string& value)
{
  cout << "in_place_swap_mode : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス LeakagePowerUnit
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
LeakagePowerUnitHandler::LeakagePowerUnitHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
LeakagePowerUnitHandler::~LeakagePowerUnitHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
LeakagePowerUnitHandler::read_value(const string& attr_name,
				    tTokenType type,
				    const string& value)
{
  cout << "leakage_power_unit : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PieceTypeHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
PieceTypeHandler::PieceTypeHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
PieceTypeHandler::~PieceTypeHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
PieceTypeHandler::read_value(const string& attr_name,
			     tTokenType type,
			     const string& value)
{
  cout << "piece_type : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PullingResistanceUnitHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
PullingResistanceUnitHandler::PullingResistanceUnitHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
PullingResistanceUnitHandler::~PullingResistanceUnitHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
PullingResistanceUnitHandler::read_value(const string& attr_name,
					 tTokenType type,
					 const string& value)
{
  cout << "pulling_resistance_unit : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス RevisionHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
RevisionHandler::RevisionHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
RevisionHandler::~RevisionHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
RevisionHandler::read_value(const string& attr_name,
			    tTokenType type,
			    const string& value)
{
  cout << "revision : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス SimulationHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
SimulationHandler::SimulationHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
SimulationHandler::~SimulationHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
SimulationHandler::read_value(const string& attr_name,
			      tTokenType type,
			      const string& value)
{
  cout << "simulation : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス TimeUnitHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
TimeUnitHandler::TimeUnitHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
TimeUnitHandler::~TimeUnitHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
TimeUnitHandler::read_value(const string& attr_name,
			    tTokenType type,
			    const string& value)
{
  cout << "time_unit : " << value << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス VoltageUnitHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
VoltageUnitHandler::VoltageUnitHandler(DotLibParser& parser) :
  SimpleHandler(parser)
{
}

// @brief デストラクタ
VoltageUnitHandler::~VoltageUnitHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] type 値の型
// @param[in] value 値を表す文字列
// @return エラーが起きたら false を返す．
bool
VoltageUnitHandler::read_value(const string& attr_name,
			       tTokenType type,
			       const string& value)
{
  cout << "time_unit : " << value << endl;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
