#ifndef LIBYM_CELL_DOTLIB_LIBSIMPLE_H
#define LIBYM_CELL_DOTLIB_LIBSIMPLE_H

/// @file libym_cell/dotlib/LibSimple.h
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
/// @class BusNamingStyleHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class CommentHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class CurrentUnitHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class DateHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class DelayModelHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};

// em_temp_degradation_factor

// fpga_technology


//////////////////////////////////////////////////////////////////////
/// @class InPlaceSwapModeHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};

// input_threshold_pct_fail

// input_threshold_pct_rise


//////////////////////////////////////////////////////////////////////
/// @class LeakagePowerUnitHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};

// nom_calc_mode

// nom_process

// nom_temperature

// nom_voltage

// output_threshold_pct_fall

// output_threshold_pct_rise


//////////////////////////////////////////////////////////////////////
/// @class PieceTypeHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};

// power_model

// preferred_output_pad_slew_rate_control

// preferred_input_pad_voltage

// preferred_output_pad_voltage


//////////////////////////////////////////////////////////////////////
/// @class PullingResistanceUnitHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class RevisionHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class SimulationHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};

// slew_derate_from_library

// slew_lower_threshold_pct_fall

// slew_lower_threshold_pct_rise

// slew_upper_threshold_pct_fall

// slew_upper_threshold_pct_rise


//////////////////////////////////////////////////////////////////////
/// @class TimeUnitHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};


//////////////////////////////////////////////////////////////////////
/// @class VoltageUnitHandler LibSimple.h "LibSimple.h"
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
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     tTokenType type,
	     const string& value);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBSIMPLE_H
