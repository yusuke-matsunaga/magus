#ifndef LIBYM_DOTLIB_HANDLERFACTORY_H
#define LIBYM_DOTLIB_HANDLERFACTORY_H

/// @file libym_dotlib/HandlerFactory.h
/// @brief HandlerFactory のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_int.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class HandlerFactory HandlerFactory.h "HandlerFactory.h"
/// @brief Handler を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class  HandlerFactory
{
public:

  /// @brief コンストラクタ
  HandlerFactory();

  /// @brief デストラクタ
  ~HandlerFactory();


public:
  //////////////////////////////////////////////////////////////////////
  // group handler の生成
  //////////////////////////////////////////////////////////////////////

  /// @brief library group 用のハンドラを作る．
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  static
  DotlibHandler*
  new_library(DotlibParserImpl& parser,
	      PtMgr& ptmgr);

  /// @brief input_voltage group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_input_voltage(GroupHandler* parent);

  /// @brief output_voltage group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_output_voltage(GroupHandler* parent);

  /// @brief operating_conditions group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_operating_conditions(GroupHandler* parent);

  /// @brief template group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_template(GroupHandler* parent);

  /// @brief wire_load group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_wire_load(GroupHandler* parent);

  /// @brief wire_load_selection group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_wire_load_selection(GroupHandler* parent);

  /// @brief wire_load_table group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_wire_load_table(GroupHandler* parent);

  /// @brief cell group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_cell(GroupHandler* parent);

  /// @brief leakage_power group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_leakage_power(GroupHandler* parent);

  /// @brief test_cell group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_test_cell(GroupHandler* parent);

  /// @brief ff group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_ff(GroupHandler* parent);

  /// @brief ff_bank group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_ff_bank(GroupHandler* parent);

  /// @brief latch group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_latch(GroupHandler* parent);

  /// @brief latch_bank group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_latch_bank(GroupHandler* parent);

  /// @brief cell 内の internal_power group 用のハンドラを作る．
  static
  DotlibHandler*
  new_cell_internal_power(GroupHandler* parent);

  /// @brief power group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_power(GroupHandler* parent);

  /// @brief statetable group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_statetable(GroupHandler* parent);

  /// @brief bus group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_bus(GroupHandler* parent);

  /// @brief bundle group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_bundle(GroupHandler* parent);

  /// @brief pin group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_pin(GroupHandler* parent);

  /// @brief internal_power group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_internal_power(GroupHandler* parent);

  /// @brief timing group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_timing(GroupHandler* parent);

  /// @brief table group 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_table(GroupHandler* parent);

  /// @brief 汎用の group 用のハンドラを作る．
  static
  GroupHandler*
  new_group(GroupHandler* parent);


public:
  //////////////////////////////////////////////////////////////////////
  // complex attribute 用のハンドラの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 汎用の complex attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_complex(GroupHandler* parent);

  /// @brief define complex attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_define(GroupHandler* parent);


public:
  //////////////////////////////////////////////////////////////////////
  // simple attribute 用のハンドラの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 汎用の simple attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_simple(GroupHandler* parent);

  /// @brief シンボルモードの simple attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_sym_simple(GroupHandler* parent);

  /// @brief 整数の simple attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_int_simple(GroupHandler* parent);

  /// @brief 実数の simple attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_float_simple(GroupHandler* parent);

  /// @brief 文字列の simple attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_string_simple(GroupHandler* parent);

  /// @brief 式の simple attribute 用のハンドラを作る．
  /// @param[in] parent 親のハンドラ
  static
  DotlibHandler*
  new_expr_simple(GroupHandler* parent);

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_HANDLERFACTORY_H
