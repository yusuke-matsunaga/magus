#ifndef LIBYM_CELL_DOTLIB_LIBSIMPLEHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBSIMPLEHANDLER_H

/// @file libym_cell/dotlib/LibSimpleHandler.h
/// @brief library グループの simple attribute のハンドラのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

// 参考文献:
//  Library Compiler Reference Manual, Version Z-2007.03
//  1-3 Example 1-1 Attributes, Groups, and Values in a Technology Library
//  /* Library Description: Simple Attributes */


#include "SimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class BusNamingStyleHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief bus_naming_style 属性
//////////////////////////////////////////////////////////////////////
class BusNamingStyleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  BusNamingStyleHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~BusNamingStyleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class CommentHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief comment 属性
//////////////////////////////////////////////////////////////////////
class CommentHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CommentHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~CommentHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class CurrentUnitHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief current_unit 属性
//////////////////////////////////////////////////////////////////////
class CurrentUnitHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CurrentUnitHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~CurrentUnitHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class DateHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief date 属性
//////////////////////////////////////////////////////////////////////
class DateHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  DateHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~DateHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class DelayModelHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief delay_model 属性
//////////////////////////////////////////////////////////////////////
class DelayModelHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  DelayModelHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~DelayModelHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};

// em_temp_degradation_factor

// fpga_technology


//////////////////////////////////////////////////////////////////////
/// @class InPlaceSwapModeHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief in_place_swap_mode 属性
//////////////////////////////////////////////////////////////////////
class InPlaceSwapModeHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  InPlaceSwapModeHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~InPlaceSwapModeHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};

// input_threshold_pct_fail

// input_threshold_pct_rise


//////////////////////////////////////////////////////////////////////
/// @class LeakagePowerUnitHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief leakage_power_unit 属性
//////////////////////////////////////////////////////////////////////
class LeakagePowerUnitHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LeakagePowerUnitHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~LeakagePowerUnitHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};

// nom_calc_mode

// nom_process

// nom_temperature

// nom_voltage

// output_threshold_pct_fall

// output_threshold_pct_rise


//////////////////////////////////////////////////////////////////////
/// @class PieceTypeHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief piece_type 属性
//////////////////////////////////////////////////////////////////////
class PieceTypeHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  PieceTypeHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~PieceTypeHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};

// power_model

// preferred_output_pad_slew_rate_control

// preferred_input_pad_voltage

// preferred_output_pad_voltage


//////////////////////////////////////////////////////////////////////
/// @class PullingResistanceUnitHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief pulling_resistance_unit 属性
//////////////////////////////////////////////////////////////////////
class PullingResistanceUnitHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  PullingResistanceUnitHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~PullingResistanceUnitHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class RevisionHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief revision 属性
//////////////////////////////////////////////////////////////////////
class RevisionHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  RevisionHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~RevisionHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class SimulationHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief simulation 属性
//////////////////////////////////////////////////////////////////////
class SimulationHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  SimulationHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~SimulationHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};

// slew_derate_from_library

// slew_lower_threshold_pct_fall

// slew_lower_threshold_pct_rise

// slew_upper_threshold_pct_fall

// slew_upper_threshold_pct_rise


//////////////////////////////////////////////////////////////////////
/// @class TimeUnitHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief time_unit 属性
//////////////////////////////////////////////////////////////////////
class TimeUnitHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  TimeUnitHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~TimeUnitHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};


//////////////////////////////////////////////////////////////////////
/// @class VoltageUnitHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief voltage_unit 属性
//////////////////////////////////////////////////////////////////////
class VoltageUnitHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  VoltageUnitHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~VoltageUnitHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBSIMPLEHANDLER_H
