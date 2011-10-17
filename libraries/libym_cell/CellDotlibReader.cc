
/// @file CellDotlibReader.cc
/// @brief CellDotlibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellDotlibReader.h"

#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellArea.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_cell/CellTime.h"

#include "dotlib/DotlibParser.h"
#include "dotlib/DotlibMgr.h"
#include "dotlib/DotlibNode.h"
#include "dotlib/DotlibLibrary.h"
#include "dotlib/DotlibCell.h"
#include "dotlib/DotlibFF.h"
#include "dotlib/DotlibLatch.h"
#include "dotlib/DotlibPin.h"

#include "ym_logic/LogExpr.h"
#include "ym_logic/TvFunc.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB


BEGIN_NONAMESPACE

// DotlibNode から　LogExpr を作る．
LogExpr
dot2expr(const DotlibNode* node,
	 const hash_map<ShString, ymuint>& pin_map)
{
  // 特例
  if ( node == NULL ) {
    return LogExpr::make_zero();
  }

  if ( node->is_int() ) {
    int v = node->int_value();
    if ( v == 0 ) {
      return LogExpr::make_zero();
    }
    if ( v == 1 ) {
      return LogExpr::make_one();
    }
    assert_not_reached(__FILE__, __LINE__);
    return LogExpr();
  }
  if ( node->is_string() ) {
    ShString name = node->string_value();
    hash_map<ShString, ymuint>::const_iterator p = pin_map.find(name);
    if ( p == pin_map.end() ) {
      ostringstream buf;
      buf << name << ": No such pin-name.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      return LogExpr();
    }
    return LogExpr::make_posiliteral(p->second);
  }
  if ( node->is_opr() ) {
    if ( node->type() == DotlibNode::kNot ) {
      LogExpr expr1 = dot2expr(node->opr1(), pin_map);
      return ~expr1;
    }
    else {
      LogExpr expr1 = dot2expr(node->opr1(), pin_map);
      LogExpr expr2 = dot2expr(node->opr2(), pin_map);
      switch ( node->type() ) {
      case DotlibNode::kAnd: return expr1 & expr2;
      case DotlibNode::kOr:  return expr1 | expr2;
      case DotlibNode::kXor: return expr1 ^ expr2;
      default: break;
      }
      assert_not_reached(__FILE__, __LINE__);
      return LogExpr();
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief DotlibNode から CellLibrary を生成する．
// @param[in] dt_library ライブラリを表すパース木のルート
// @return 生成したライブラリを返す．
// @note 生成が失敗したら NULL を返す．
const CellLibrary*
gen_library(const DotlibNode* dt_library)
{
  DotlibLibrary library_info;

  if ( !dt_library->get_library_info(library_info) ) {
    return NULL;
  }

  // ライブラリの生成
  CellLibrary* library = CellLibrary::new_obj();
  library->set_name(library_info.name());

  // セル数の設定
  const list<const DotlibNode*>& dt_cell_list = library_info.cell_list();
  library->set_cell_num(dt_cell_list.size());

  // セルの内容の設定
  ymuint cell_id = 0;
  for (list<const DotlibNode*>::const_iterator p = dt_cell_list.begin();
       p != dt_cell_list.end(); ++ p, ++ cell_id) {
    const DotlibNode* dt_cell = *p;

    // セル情報の読み出し
    DotlibCell cell_info;
    if ( !dt_cell->get_cell_info(cell_info) ) {
      continue;
    }
    ShString cell_name = cell_info.name();
    CellArea area(cell_info.area());
    const list<const DotlibNode*>& dt_pin_list = cell_info.pin_list();
    const list<const DotlibNode*>& dt_bus_list = cell_info.bus_list();
    const list<const DotlibNode*>& dt_bundle_list = cell_info.bundle_list();
    ymuint npin = dt_pin_list.size();
    ymuint nbus = dt_bus_list.size();
    ymuint nbundle = dt_bundle_list.size();

    // ピン情報の配列
    vector<DotlibPin> pin_info_array(npin);

    // ピン名とピン番号の連想配列
    hash_map<ShString, ymuint> pin_map;

    // ピン情報の読み出し
    ymuint ni = 0;
    ymuint no = 0;
    ymuint nio = 0;
    {
      ymuint pin_id = 0;
      bool error = false;
      for (list<const DotlibNode*>::const_iterator q = dt_pin_list.begin();
	   q != dt_pin_list.end(); ++ q, ++ pin_id) {
	const DotlibNode* dt_pin = *q;

	// ピン情報の読み出し
	DotlibPin& pin_info = pin_info_array[pin_id];
	if ( !dt_pin->get_pin_info(pin_info) ) {
	  error = true;
	  continue;
	}

	// 各タイプの個数のカウント
	switch ( pin_info.direction() ) {
	case DotlibPin::kInput:
	  ++ ni;
	  break;

	case DotlibPin::kOutput:
	  ++ no;
	  break;

	case DotlibPin::kInout:
	  ++ nio;
	  break;

	default:
	  break;
	}
      }
      if ( error ) {
	continue;
      }
    }
    ymuint ni2 = ni + nio;

    // ピン名とピン番号の対応づけを行う．
    {
      ymuint ipos = 0;
      for (ymuint pin_id = 0; pin_id < npin; ++ pin_id) {
	DotlibPin& pin_info = pin_info_array[pin_id];
	switch ( pin_info.direction() ) {
	case DotlibPin::kInput:
	case DotlibPin::kInout:
	  pin_map.insert(make_pair(pin_info.name(), ipos));
	  ++ ipos;
	  break;

	default:
	  break;
	}
      }
      assert_cond( ipos == ni2, __FILE__, __LINE__);
    }

    // FF情報の読み出し
    const DotlibNode* dt_ff = cell_info.ff();
    DotlibFF ff_info;
    if ( dt_ff != NULL ) {
      if ( !dt_ff->get_ff_info(ff_info) ) {
	continue;
      }
      ShString var1 = ff_info.var1_name();
      ShString var2 = ff_info.var2_name();
      // pin_map に登録しておく
      pin_map.insert(make_pair(var1, ni2 + 0));
      pin_map.insert(make_pair(var2, ni2 + 1));
    }

    // ラッチ情報の読み出し
    const DotlibNode* dt_latch = cell_info.latch();
    DotlibLatch latch_info;
    if ( dt_latch != NULL) {
      if ( !dt_latch->get_latch_info(latch_info) ) {
	continue;
      }
      ShString var1 = latch_info.var1_name();
      ShString var2 = latch_info.var2_name();
      // pin_map に登録しておく
      pin_map.insert(make_pair(var1, ni2 + 0));
      pin_map.insert(make_pair(var2, ni2 + 1));
    }

    // 出力ピン(入出力ピン)の論理式を作る．
    vector<bool> output_array;
    vector<LogExpr> logic_array;
    vector<LogExpr> tristate_array;
    ymuint no2 = no + nio;
    output_array.reserve(no2);
    logic_array.reserve(no2);
    tristate_array.reserve(no2);
    for (ymuint i = 0; i < npin; ++ i) {
      DotlibPin& pin_info = pin_info_array[i];
      switch ( pin_info.direction() ) {
      case DotlibPin::kOutput:
	{
	  const DotlibNode* func_node = pin_info.function();
	  if ( func_node ) {
	    LogExpr expr = dot2expr(func_node, pin_map);
	    logic_array.push_back(expr);
	    output_array.push_back(true);
	  }
	  else {
	    logic_array.push_back(LogExpr::make_zero());
	    output_array.push_back(false);
	  }
	  const DotlibNode* three_state = pin_info.three_state();
	  if ( three_state ) {
	    LogExpr expr = dot2expr(three_state, pin_map);
	    tristate_array.push_back(expr);
	  }
	  else {
	    tristate_array.push_back(LogExpr::make_zero());
	  }
	}
	break;

      case DotlibPin::kInout:
	{
	  const DotlibNode* func_node = pin_info.function();
	  if ( func_node ) {
	    LogExpr expr = dot2expr(func_node, pin_map);
	    logic_array.push_back(expr);
	    output_array.push_back(true);
	  }
	  else {
	    logic_array.push_back(LogExpr::make_zero());
	    output_array.push_back(false);
	  }
	  const DotlibNode* three_state = pin_info.three_state();
	  if ( three_state ) {
	    LogExpr expr = dot2expr(three_state, pin_map);
	    tristate_array.push_back(expr);
	  }
	  else {
	    tristate_array.push_back(LogExpr::make_zero());
	  }
	}
	break;

      default:
	break;
      }
    }

    // セルの生成
    if ( dt_ff ) {
      LogExpr next_state = dot2expr(ff_info.next_state(), pin_map);
      LogExpr clocked_on = dot2expr(ff_info.clocked_on(), pin_map);
      LogExpr clocked_on_also = dot2expr(ff_info.clocked_on_also(), pin_map);
      LogExpr clear = dot2expr(ff_info.clear(), pin_map);
      LogExpr preset = dot2expr(ff_info.preset(), pin_map);
      ymuint v1 = ff_info.clear_preset_var1();
      ymuint v2 = ff_info.clear_preset_var2();
      library->new_ff_cell(cell_id, cell_name, area,
			   ni, no, nio, nbus, nbundle,
			   output_array,
			   logic_array,
			   tristate_array,
			   next_state,
			   clocked_on, clocked_on_also,
			   clear, preset,
			   v1, v2);

    }
    else if ( dt_latch ) {
      LogExpr data_in = dot2expr(latch_info.data_in(), pin_map);
      LogExpr enable = dot2expr(latch_info.enable(), pin_map);
      LogExpr enable_also = dot2expr(latch_info.enable_also(), pin_map);
      LogExpr clear = dot2expr(latch_info.clear(), pin_map);
      LogExpr preset = dot2expr(latch_info.preset(), pin_map);
      ymuint v1 = latch_info.clear_preset_var1();
      ymuint v2 = latch_info.clear_preset_var2();
      library->new_latch_cell(cell_id, cell_name, area,
			      ni, no, nio, nbus, nbundle,
			      output_array,
			      logic_array,
			      tristate_array,
			      data_in,
			      enable, enable_also,
			      clear, preset,
			      v1, v2);
    }
    else {
      library->new_logic_cell(cell_id, cell_name, area,
			      ni, no, nio, nbus, nbundle,
			      output_array,
			      logic_array,
			      tristate_array);
    }

    // ピンの生成
    ymuint i_pos = 0;
    ymuint o_pos = 0;
    ymuint io_pos = 0;
    for (ymuint i = 0; i < npin; ++ i) {
      const DotlibPin& pin_info = pin_info_array[i];
      switch ( pin_info.direction() ) {
      case DotlibPin::kInput:
	{ // 入力ピンの生成
	  CellCapacitance cap(pin_info.capacitance());
	  CellCapacitance rise_cap(pin_info.rise_capacitance());
	  CellCapacitance fall_cap(pin_info.fall_capacitance());
	  library->new_cell_input(cell_id, i, i_pos, pin_info.name(),
				  cap, rise_cap, fall_cap);
	}
	++ i_pos;
	break;

      case DotlibPin::kOutput:
	{ // 出力の生成
	  CellCapacitance max_fanout(pin_info.max_fanout());
	  CellCapacitance min_fanout (pin_info.min_fanout());
	  CellCapacitance max_capacitance(pin_info.max_capacitance());
	  CellCapacitance min_capacitance(pin_info.min_capacitance());
	  CellTime max_transition(pin_info.max_transition());
	  CellTime min_transition(pin_info.min_transition());
	  library->new_cell_output(cell_id, i, o_pos, pin_info.name(),
				   max_fanout, min_fanout,
				   max_capacitance, min_capacitance,
				   max_transition, min_transition);
#if 0
	  const DotlibNode* func_node = pin_info.function();
	  if ( func_node ) {
	    LogExpr expr = dot2expr(func_node, pin_map);
	    cell->set_logic_expr(o_pos, expr);
#if 0
	    TvFunc tv_function = expr.make_tv(ni);
	    for (ymuint i = 0; i < ni; ++ i) {
	      // タイミング情報の設定
	      const DotlibNode* pt_pin = ipin_array[i];
	      TvFunc p_func = tv_function.cofactor(i, kPolPosi);
	      TvFunc n_func = tv_function.cofactor(i, kPolNega);
	      tCellTimingSense sense_real = kSenseNonUnate;
	      bool redundant = false;
	      if ( ~p_func && n_func ) {
		if ( ~n_func && p_func ) {
		  sense_real = kCellNonUnate;
		}
		else {
		  sense_real = kCellNegaUnate;
		}
	      }
	      else {
		if ( ~n_func && p_func ) {
		  sense_real = kCellPosiUnate;
		}
		else {
		  // つまり p_func == n_func ということ．
		  // つまりこの変数は出力に影響しない．
		  ostringstream buf;
		  buf << "The output function does not depend on the input pin, "
		      << pt_pin->name()->str() << ".";
		  MsgMgr::put_msg(__FILE__, __LINE__,
				  pt_pin->loc(),
				  kMsgWarning,
				  "DOTLIB_PARSER",
				  buf.str());
		  redundant = true;
		}
	      }

	      tCellTimingSense sense = kCellNonUnate;
	      switch ( pt_pin->phase()->type() ) {
	      case DotlibNode::kNoninv:
		sense = kCellPosiUnate;
		break;

	      case DotlibNode::kInv:
		sense = kCellNegaUnate;
		break;

	      case DotlibNode::kUnknown:
		sense = kCellNonUnate;
		break;

	      default:
		assert_not_reached(__FILE__, __LINE__); break;
	      }

	      if ( sense != sense_real ) {
		ostringstream buf;
		buf << "Phase description does not match the logic expression. "
		    << "Ignored.";
		MsgMgr::put_msg(__FILE__, __LINE__,
				pt_pin->phase()->loc(),
				kMsgWarning,
				"DOTLIB_PARSER",
				buf.str());
		sense = sense_real;
	      }
	      CellTime r_i(pt_pin->rise_block_delay()->num());
	      CellResistance r_r(pt_pin->rise_fanout_delay()->num());
	      CellTime f_i(pt_pin->fall_block_delay()->num());
	      CellResistance f_r(pt_pin->fall_fanout_delay()->num());
	      CellTiming* timing = library->new_timing(i, kTimingCombinational,
						       r_i, f_i,
						       CellTime(0.0),
						       CellTime(0.0),
						       r_r, f_r);
	      if ( !redundant ) {
		library->set_cell_timing(cell, i, o_pos, sense, timing);
	      }
	    }
#endif
	  }
	  const DotlibNode* three_state = pin_info.three_state();
	  if ( three_state ) {
	    LogExpr expr = dot2expr(three_state, pin_map);
	    cell->set_tristate_expr(o_pos, expr);
	  }
#endif
	}
	++ o_pos;
	break;

      case DotlibPin::kInout:
	{ // 入出力ピンの生成
	  CellCapacitance cap(pin_info.capacitance());
	  CellCapacitance rise_cap(pin_info.rise_capacitance());
	  CellCapacitance fall_cap(pin_info.fall_capacitance());
	  CellCapacitance max_fanout(pin_info.max_fanout());
	  CellCapacitance min_fanout(pin_info.min_fanout());
	  CellCapacitance max_capacitance(pin_info.max_capacitance());
	  CellCapacitance min_capacitance(pin_info.min_capacitance());
	  CellTime max_transition(pin_info.max_transition());
	  CellTime min_transition(pin_info.min_transition());
	  library->new_cell_inout(cell_id, i, io_pos + ni, io_pos + no, pin_info.name(),
				  cap, rise_cap, fall_cap,
				  max_fanout, min_fanout,
				  max_capacitance, min_capacitance,
				  max_transition, min_transition);
	}
	++ io_pos;
	break;

      case DotlibPin::kInternal:
#if 0
	library->new_cell_internal(cell, i, pin_info.name());
#endif
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
      }
    }

  }

  library->compile();

  return library;
}

END_NONAMESPACE

END_NAMESPACE_YM_DOTLIB


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// Dotlibファイルを読んで CellLibrary を作るクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellDotlibReader::CellDotlibReader()
{
}

// @brief デストラクタ
CellDotlibReader::~CellDotlibReader()
{
}

// @brief dotlib ファイルを読み込む
// @param[in] filename ファイル名
// @return 読み込んで作成したセルライブラリを返す．
// @note エラーが起きたら NULL を返す．
const CellLibrary*
CellDotlibReader::operator()(const string& filename)
{
  using namespace nsDotlib;

  DotlibMgr mgr;
  DotlibParser parser;
  if ( !parser.read_file(filename, mgr, false) ) {
    return NULL;
  }
  return gen_library(mgr.root_node()->attr_value());
}

END_NAMESPACE_YM_CELL
