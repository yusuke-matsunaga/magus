
/// @file CellDotlibReader.cc
/// @brief CellDotlibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellDotlibReader.h"

#include "YmCell/CellLibrary.h"
#include "YmCell/Cell.h"
#include "YmCell/CellPin.h"
#include "YmCell/CellTiming.h"
#include "YmCell/CellArea.h"
#include "YmCell/CellResistance.h"
#include "YmCell/CellCapacitance.h"
#include "YmCell/CellTime.h"
#include "YmCell/CellLut.h"
#include "YmCell/CellLutTemplate.h"

#include "DotlibParser.h"
#include "DotlibMgr.h"
#include "DotlibNode.h"
#include "DotlibLibrary.h"
#include "DotlibCell.h"
#include "DotlibFF.h"
#include "DotlibLatch.h"
#include "DotlibPin.h"
#include "DotlibTiming.h"
#include "DotlibTemplate.h"
#include "DotlibLut.h"

#include "YmLogic/Expr.h"
#include "YmLogic/TvFunc.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

BEGIN_NONAMESPACE

typedef unordered_map<ShString, ymuint> PinMap;

// 文字列を空白で区切る
void
split(const string& src_str,
      vector<string>& str_list)
{
  string tmp_str(src_str);
  for ( ; ; ) {
    string::size_type p = tmp_str.find_first_of(' ');
    if ( p == string::npos ) {
      str_list.push_back(tmp_str);
      return;
    }
    string tmp = tmp_str.substr(0, p);
    tmp_str = tmp_str.substr(p + 1, string::npos);
  }
}

// DotlibNode から　Expr を作る．
Expr
dot2expr(const DotlibNode* node,
	 const PinMap& pin_map)
{
  // 特例
  if ( node == NULL ) {
    return Expr::make_zero();
  }

  if ( node->is_int() ) {
    int v = node->int_value();
    if ( v == 0 ) {
      return Expr::make_zero();
    }
    if ( v == 1 ) {
      return Expr::make_one();
    }
    assert_not_reached(__FILE__, __LINE__);
    return Expr();
  }
  if ( node->is_string() ) {
    ShString name = node->string_value();
    PinMap::const_iterator p = pin_map.find(name);
    if ( p == pin_map.end() ) {
      ostringstream buf;
      buf << name << ": No such pin-name.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      return Expr();
    }
    ymuint id = p->second;
    return Expr::make_posiliteral(VarId(id));
  }
  if ( node->is_opr() ) {
    if ( node->type() == DotlibNode::kNot ) {
      Expr expr1 = dot2expr(node->opr1(), pin_map);
      return ~expr1;
    }
    else {
      Expr expr1 = dot2expr(node->opr1(), pin_map);
      Expr expr2 = dot2expr(node->opr2(), pin_map);
      switch ( node->type() ) {
      case DotlibNode::kAnd: return expr1 & expr2;
      case DotlibNode::kOr:  return expr1 | expr2;
      case DotlibNode::kXor: return expr1 ^ expr2;
      default: break;
      }
      assert_not_reached(__FILE__, __LINE__);
      return Expr();
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return Expr();
}

// LUT を読み込む．
CellLut*
gen_lut(CellLibrary* library,
	const DotlibNode* lut_node)
{
  DotlibLut lut_info;
  if ( !lut_info.set_data(lut_node) ) {
    return NULL;
  }
  const char* name = lut_info.template_name();
  const CellLutTemplate* templ = library->lu_table_template(name);
  if ( templ == NULL ) {
    ostringstream buf;
    buf << lut_info.template_name()
	<< ": No such lu_table template";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    lut_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    buf.str());
    return NULL;
  }

  ymuint d = templ->dimension();

  vector<double> value_array;
  ymuint n = lut_info.value_list()->list_size();
  for (ymuint i = 0; i < n; ++ i) {
    vector<double> tmp_array;
    lut_info.value_list()->list_elem(i)->get_vector(tmp_array);
    value_array.insert(value_array.end(), tmp_array.begin(), tmp_array.end());
  }

  vector<double> index1_array;
  if ( lut_info.index_1() ) {
    lut_info.index_1()->get_vector(index1_array);
  }
  vector<double> index2_array;
  if ( d >= 2 && lut_info.index_2() ) {
    lut_info.index_2()->get_vector(index2_array);
  }
  vector<double> index3_array;
  if ( d >= 3 && lut_info.index_3() ) {
    lut_info.index_3()->get_vector(index3_array);
  }

  CellLut* lut = NULL;
  if ( d == 1 ) {
    lut = library->new_lut1(templ, value_array,
			    index1_array);
  }
  else if ( d == 2 ) {
    lut = library->new_lut2(templ, value_array,
			    index1_array,
			    index2_array);
  }
  else if ( d == 3 ) {
    lut = library->new_lut3(templ, value_array,
			    index1_array,
			    index2_array,
			    index3_array);
  }
  return lut;
}

// 論理式を生成する．
void
gen_expr(const DotlibPin& pin_info,
	 const PinMap& pin_map,
	 vector<bool>& output_array,
	 vector<Expr>& logic_array,
	 vector<Expr>& tristate_array)
{
  for (ymuint i = 0; i < pin_info.num(); ++ i) {
    const DotlibNode* func_node = pin_info.function();
    if ( func_node ) {
      Expr expr = dot2expr(func_node, pin_map);
      logic_array.push_back(expr);
      output_array.push_back(true);
    }
    else {
      logic_array.push_back(Expr::make_zero());
      output_array.push_back(false);
    }
    const DotlibNode* three_state = pin_info.three_state();
    if ( three_state ) {
      Expr expr = dot2expr(three_state, pin_map);
      tristate_array.push_back(expr);
    }
    else {
      tristate_array.push_back(Expr::make_zero());
    }
  }
}

// ピンを生成する．
void
gen_pin(CellLibrary* library,
	const vector<DotlibPin>& pin_info_array,
	ymuint cell_id,
	const vector<bool>& output_array,
	const vector<Expr>& logic_array,
	const vector<Expr>& tristate_array)
{
  ymuint i_pos = 0;
  ymuint o_pos = 0;
  ymuint io_pos = 0;
  ymuint it_pos = 0;
  ymuint pin_pos = 0;

  const Cell* cell = library->cell(cell_id);
  ymuint ni = cell->input_num();
  ymuint no = cell->output_num();

  ymuint npg = pin_info_array.size();
  for (ymuint pg_id = 0; pg_id < npg; ++ pg_id) {
    const DotlibPin& pin_info = pin_info_array[pg_id];

    switch ( pin_info.direction() ) {
    case DotlibPin::kInput:
      // 入力ピンの生成
      for (ymuint i = 0; i < pin_info.num(); ++ i) {
	CellCapacitance cap(pin_info.capacitance());
	CellCapacitance rise_cap(pin_info.rise_capacitance());
	CellCapacitance fall_cap(pin_info.fall_capacitance());
	library->new_cell_input(cell_id, pin_pos, i_pos,
				pin_info.name(i),
				cap, rise_cap, fall_cap);
	++ pin_pos;
	++ i_pos;
      }
      break;

    case DotlibPin::kOutput:
      // 出力の生成
      for (ymuint i = 0; i < pin_info.num(); ++ i) {
	CellCapacitance max_fanout(pin_info.max_fanout());
	CellCapacitance min_fanout (pin_info.min_fanout());
	CellCapacitance max_capacitance(pin_info.max_capacitance());
	CellCapacitance min_capacitance(pin_info.min_capacitance());
	CellTime max_transition(pin_info.max_transition());
	CellTime min_transition(pin_info.min_transition());
	library->new_cell_output(cell_id, pin_pos, o_pos,
				 pin_info.name(i),
				 output_array[i],
				 logic_array[i],
				 tristate_array[i],
				 max_fanout, min_fanout,
				 max_capacitance, min_capacitance,
				 max_transition, min_transition);
	++ pin_pos;
	++ o_pos;
      }
      break;

    case DotlibPin::kInout:
      // 入出力ピンの生成
      for (ymuint i = 0; i < pin_info.num(); ++ i) {
	CellCapacitance cap(pin_info.capacitance());
	CellCapacitance rise_cap(pin_info.rise_capacitance());
	CellCapacitance fall_cap(pin_info.fall_capacitance());
	CellCapacitance max_fanout(pin_info.max_fanout());
	CellCapacitance min_fanout(pin_info.min_fanout());
	CellCapacitance max_capacitance(pin_info.max_capacitance());
	CellCapacitance min_capacitance(pin_info.min_capacitance());
	CellTime max_transition(pin_info.max_transition());
	CellTime min_transition(pin_info.min_transition());
	ymuint i_pos2 = io_pos + ni;
	ymuint o_pos2 = io_pos + no;
	library->new_cell_inout(cell_id, pin_pos, i_pos2, o_pos2,
				pin_info.name(i),
				output_array[i],
				logic_array[i],
				tristate_array[i],
				cap, rise_cap, fall_cap,
				max_fanout, min_fanout,
				max_capacitance, min_capacitance,
				max_transition, min_transition);
	++ pin_pos;
	++ io_pos;
      }
      break;

    case DotlibPin::kInternal:
      // 内部ピンの生成
      for (ymuint i = 0; i < pin_info.num(); ++ i) {
	library->new_cell_internal(cell_id, pin_pos, it_pos,
				   pin_info.name(i));
	++ pin_pos;
	++ it_pos;
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
    }
  }
}

// タイミング情報を生成する．
void
gen_timing(CellLibrary* library,
	   const list<const DotlibNode*>& timing_list,
	   ymuint cell_id,
	   ymuint& timing_id,
	   const PinMap& pin_map,
	   vector<vector<ymuint> >& tid_list)
{
  const Cell* cell = library->cell(cell_id);
  for (list<const DotlibNode*>::const_iterator p = timing_list.begin();
       p != timing_list.end(); ++ p) {
    const DotlibNode* dt_timing = *p;
    DotlibTiming timing_info;
    if ( !timing_info.set_data(dt_timing) ) {
      continue;
    }
    tCellTimingType timing_type = timing_info.timing_type();
    const DotlibNode* when_node = timing_info.when();
    Expr cond;
    if ( when_node ) {
      cond = dot2expr(when_node, pin_map);
    }
    else {
      cond = Expr::make_one();
    }

    switch ( library->delay_model() ) {
    case kCellDelayGenericCmos:
      {
	CellTime intrinsic_rise(timing_info.intrinsic_rise()->float_value());
	CellTime intrinsic_fall(timing_info.intrinsic_fall()->float_value());
	CellTime slope_rise(timing_info.slope_rise()->float_value());
	CellTime slope_fall(timing_info.slope_fall()->float_value());
	CellResistance rise_res(timing_info.rise_resistance()->float_value());
	CellResistance fall_res(timing_info.fall_resistance()->float_value());
	library->new_timing_generic(cell_id, timing_id,
				    timing_type, cond,
				    intrinsic_rise, intrinsic_fall,
				    slope_rise, slope_fall,
				    rise_res, fall_res);
      }
      break;

    case kCellDelayTableLookup:
      {
	const DotlibNode* cr_node = timing_info.cell_rise();
	const DotlibNode* rt_node = timing_info.rise_transition();
	const DotlibNode* rp_node = timing_info.rise_propagation();

	CellLut* cr_lut = NULL;
	CellLut* rt_lut = NULL;
	CellLut* rp_lut = NULL;
	if ( cr_node != NULL ) {
	  if ( rp_node != NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "cell_rise and rise_propagation are mutually exclusive.");
	    continue;
	  }
	  if ( rt_node == NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "rise_transition is missing.");
	    continue;
	  }
	  cr_lut = gen_lut(library, cr_node);
	  rt_lut = gen_lut(library, rt_node);
	}
	else if ( rp_node != NULL ) {
	  if ( rt_node == NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "rise_transition is missing.");
	    continue;
	  }
	  rt_lut = gen_lut(library, rt_node);
	  rp_lut = gen_lut(library, rp_node);
	}
	else if ( rt_node != NULL ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  dt_timing->loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "Either cell_rise or rise_propagation should be present.");
	  continue;
	}

	const DotlibNode* cf_node = timing_info.cell_fall();
	const DotlibNode* ft_node = timing_info.fall_transition();
	const DotlibNode* fp_node = timing_info.fall_propagation();

	CellLut* cf_lut = NULL;
	CellLut* ft_lut = NULL;
	CellLut* fp_lut = NULL;
	if ( cf_node != NULL ) {
	  if ( fp_node != NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "cell_fall and fall_propagation are mutually exclusive.");
	    continue;
	  }
	  if ( ft_node == NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "fall_transition is missing.");
	    continue;
	  }
	  cf_lut = gen_lut(library, cf_node);
	  ft_lut = gen_lut(library, ft_node);
	}
	else if ( fp_node != NULL ) {
	  if ( ft_node == NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "fall_transition is missing.");
	    continue;
	  }
	  ft_lut = gen_lut(library, ft_node);
	  fp_lut = gen_lut(library, fp_node);
	}
	else if ( ft_node != NULL ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  dt_timing->loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  "Either cell_fall or fall_propagation should be present.");
	  continue;
	}

	if ( cr_lut != NULL || cf_lut != NULL ) {
	  if ( fp_lut != NULL ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    dt_timing->loc(),
			    kMsgError,
			    "DOTLIB_PARSER",
			    "cell_rise and fall_propagation are mutually exclusive.");
	    continue;
	  }
	  library->new_timing_lut1(cell_id, timing_id,
				   timing_type, cond,
				   cr_lut, cf_lut,
				   rt_lut, ft_lut);
	}
	else { // cr_lut == NULL && cf_lut == NULL
	  library->new_timing_lut2(cell_id, timing_id,
				   timing_type, cond,
				   rt_lut, ft_lut,
				   rp_lut, fp_lut);
	}
      }
      break;

    case kCellDelayPiecewiseCmos:
      // 未実装
      break;

    case kCellDelayCmos2:
      // 未実装
      break;

    case kCellDelayDcm:
      // 未実装
      break;
    }

    tCellTimingSense timing_sense = timing_info.timing_sense();

    // タイミング情報の設定
    if ( timing_info.related_pin() ) {
      string tmp_str = timing_info.related_pin()->string_value();
      vector<string> pin_name_list;
      split(tmp_str, pin_name_list);
      for (vector<string>::const_iterator p = pin_name_list.begin();
	   p != pin_name_list.end(); ++ p) {
	string pin_name = *p;
	const CellPin* ipin = cell->pin(pin_name);
	if ( ipin == NULL ) {
	  ostringstream buf;
	  buf << pin_name << ": no such pin";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  dt_timing->loc(),
			  kMsgError,
			  "DOTLIB_PARSER",
			  buf.str());
	  continue;
	}
	ymuint iid = ipin->input_id();
	switch ( timing_sense ) {
	case kCellPosiUnate:
	  tid_list[iid * 2 + 0].push_back(timing_id);
	  break;

	case kCellNegaUnate:
	  tid_list[iid * 2 + 1].push_back(timing_id);
	  break;

	case kCellNonUnate:
	  tid_list[iid * 2 + 0].push_back(timing_id);
	  tid_list[iid * 2 + 1].push_back(timing_id);
	  break;

	default:
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      dt_timing->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "No \"related_pin\"");
    }
    ++ timing_id;
  }
}

// @brief DotlibNode から CellLibrary を生成する．
// @param[in] dt_library ライブラリを表すパース木のルート
// @return 生成したライブラリを返す．
// @note 生成が失敗したら NULL を返す．
const CellLibrary*
gen_library(const DotlibNode* dt_library)
{
  DotlibLibrary library_info;

  if ( !library_info.set_data(dt_library) ) {
    return NULL;
  }

  // ライブラリの生成
  CellLibrary* library = CellLibrary::new_obj();
  library->set_name(library_info.name());

  // 'technology' の設定
  library->set_technology(library_info.technology());

  // 'delay_model' の設定
  library->set_delay_model(library_info.delay_model());

  // 'bus_naming_style' の設定
  if ( library_info.bus_naming_style() ) {
    ShString value = library_info.bus_naming_style()->string_value();
    library->set_attr("bus_naming_style", value);
  }

  // 'comment' の設定
  if ( library_info.comment() ) {
    ShString value = library_info.comment()->string_value();
    library->set_attr("comment", value);
  }

  // 'date' の設定
  if ( library_info.date() ) {
    ShString value = library_info.date()->string_value();
    library->set_attr("date", value);
  }

  // 'revision' の設定
  if ( library_info.revision() ) {
    ShString value = library_info.revision()->string_value();
    library->set_attr("revision", value);
  }

  // 'time_unit' の設定
  if ( library_info.time_unit() ) {
    ShString value = library_info.time_unit()->string_value();
    library->set_attr("time_unit", value);
  }

  // 'voltage_unit' の設定
  if ( library_info.voltage_unit() ) {
    ShString value = library_info.voltage_unit()->string_value();
    library->set_attr("voltage_unit", value);
  }

  // 'current_unit' の設定
  if ( library_info.current_unit() ) {
    ShString value = library_info.current_unit()->string_value();
    library->set_attr("current_unit", value);
  }

  // 'pulling_resistance_unit' の設定
  if ( library_info.pulling_resistance_unit() ) {
    ShString value = library_info.pulling_resistance_unit()->string_value();
    library->set_attr("pulling_resistance_unit", value);
  }

  // 'capacitive_load_unit' の設定
  if ( library_info.capacitive_load_unit() ) {
    double u = library_info.capacitive_load_unit();
    string ustr = library_info.capacitive_load_unit_str();
    library->set_capacitive_load_unit(u, ustr);
  }

  // 'leakage_power_unit' の設定
  if ( library_info.leakage_power_unit() ) {
    ShString value = library_info.leakage_power_unit()->string_value();
    library->set_attr("leakage_power_unit", value);
  }

  // 'lu_table_template' の設定
  const list<const DotlibNode*>& dt_lut_template_list =
    library_info.lut_template_list();
  library->set_lu_table_template_num(dt_lut_template_list.size());
  ymuint templ_id = 0;
  for (list<const DotlibNode*>::const_iterator p = dt_lut_template_list.begin();
       p != dt_lut_template_list.end(); ++ p, ++ templ_id) {
    DotlibTemplate templ_info;
    if ( !templ_info.set_data(*p) ) {
      return false;
    }
    ymuint d = templ_info.dimension();
    switch ( d ) {
    case 1:
      {
	vector<double> index_1;
	templ_info.index_1()->get_vector(index_1);
	library->new_lut_template1(templ_id, templ_info.name(),
				   templ_info.variable_1(), index_1);
      }
      break;

    case 2:
      {
	vector<double> index_1;
	templ_info.index_1()->get_vector(index_1);
	vector<double> index_2;
	templ_info.index_2()->get_vector(index_2);
	library->new_lut_template2(templ_id, templ_info.name(),
				   templ_info.variable_1(), index_1,
				   templ_info.variable_2(), index_2);
      }
      break;

    case 3:
      {
	vector<double> index_1;
	templ_info.index_1()->get_vector(index_1);
	vector<double> index_2;
	templ_info.index_2()->get_vector(index_2);
	vector<double> index_3;
	templ_info.index_3()->get_vector(index_3);
	library->new_lut_template3(templ_id, templ_info.name(),
				   templ_info.variable_1(), index_1,
				   templ_info.variable_2(), index_2,
				   templ_info.variable_3(), index_3);
      }
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
    }
  }

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
    if ( !cell_info.set_data(dt_cell) ) {
      continue;
    }

    ShString cell_name = cell_info.name();
    CellArea area(cell_info.area());
    const list<const DotlibNode*>& dt_pin_list = cell_info.pin_list();
    const list<const DotlibNode*>& dt_bus_list = cell_info.bus_list();
    const list<const DotlibNode*>& dt_bundle_list = cell_info.bundle_list();
    ymuint npg = dt_pin_list.size();
    ymuint nbus = dt_bus_list.size();
    ymuint nbundle = dt_bundle_list.size();

    // ピン情報の配列
    vector<DotlibPin> pin_info_array(npg);

    // ピン名とピン番号の連想配列
    PinMap pin_map;

    // ピン情報の読み出し
    ymuint ni = 0;
    ymuint no = 0;
    ymuint nio = 0;
    ymuint nit = 0;
    {
      ymuint pg_id = 0;
      bool error = false;
      for (list<const DotlibNode*>::const_iterator q = dt_pin_list.begin();
	   q != dt_pin_list.end(); ++ q, ++ pg_id) {
	const DotlibNode* dt_pin = *q;

	// ピン情報の読み出し
	DotlibPin& pin_info = pin_info_array[pg_id];
	if ( !pin_info.set_data(dt_pin) ) {
	  error = true;
	  continue;
	}

	// 各タイプの個数のカウント
	ymuint nn = pin_info.num();
	switch ( pin_info.direction() ) {
	case DotlibPin::kInput:
	  ni += nn;
	  break;

	case DotlibPin::kOutput:
	  no += nn;
	  break;

	case DotlibPin::kInout:
	  nio += nn;
	  break;

	case DotlibPin::kInternal:
	  nit += nn;
	  break;

	default:
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
      if ( error ) {
	continue;
      }
      assert_cond( pg_id == npg, __FILE__, __LINE__);
    }
    ymuint ni2 = ni + nio;

    // ピン名とピン番号の対応づけを行う．
    {
      ymuint ipos = 0;
      ymuint itpos = 0;
      for (ymuint pg_id = 0; pg_id < npg; ++ pg_id) {
	DotlibPin& pin_info = pin_info_array[pg_id];
	switch ( pin_info.direction() ) {
	case DotlibPin::kInput:
	case DotlibPin::kInout:
	  for (ymuint i = 0; i < pin_info.num(); ++ i) {
	    pin_map.insert(make_pair(pin_info.name(i), ipos));
	    ++ ipos;
	  }
	  break;

	case DotlibPin::kInternal:
	  for (ymuint i = 0; i < pin_info.num(); ++ i) {
	    pin_map.insert(make_pair(pin_info.name(i), itpos + ni2));
	    ++ itpos;
	  }
	  break;

	default:
	  break;
	}
      }
      assert_cond( ipos == ni2, __FILE__, __LINE__);
      assert_cond( itpos == nit, __FILE__, __LINE__);
    }

    // FF情報の読み出し
    const DotlibNode* dt_ff = cell_info.ff();
    DotlibFF ff_info;
    if ( dt_ff != NULL ) {
      if ( !ff_info.set_data(dt_ff) ) {
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
      if ( !latch_info.set_data(dt_latch) ) {
	continue;
      }
      ShString var1 = latch_info.var1_name();
      ShString var2 = latch_info.var2_name();
      // pin_map に登録しておく
      pin_map.insert(make_pair(var1, ni2 + 0));
      pin_map.insert(make_pair(var2, ni2 + 1));
    }

    // 遷移表情報の読み出し
    const DotlibNode* dt_fsm = cell_info.statetable();

    // 出力ピン(入出力ピン)の論理式を作る．
    vector<bool> output_array;
    vector<Expr> logic_array;
    vector<Expr> tristate_array;
    ymuint no2 = no + nio;
    output_array.reserve(no2);
    logic_array.reserve(no2);
    tristate_array.reserve(no2);

    // 0 - (no - 1) に出力ピンの論理式
    // no - (no2 - 1) に入出力ピンの論理式を格納する．
    for (ymuint pg_id = 0; pg_id < npg; ++ pg_id) {
      const DotlibPin& pin_info = pin_info_array[pg_id];
      if ( pin_info.direction() == DotlibPin::kOutput ) {
	gen_expr(pin_info, pin_map,
		 output_array,
		 logic_array,
		 tristate_array);
      }
    }
    for (ymuint pg_id = 0; pg_id < npg; ++ pg_id) {
      const DotlibPin& pin_info = pin_info_array[pg_id];
      if ( pin_info.direction() == DotlibPin::kInout ) {
	gen_expr(pin_info, pin_map,
		 output_array,
		 logic_array,
		 tristate_array);
      }
    }

    // セルの生成
    if ( dt_ff ) {
      Expr next_state = dot2expr(ff_info.next_state(), pin_map);
      Expr clocked_on = dot2expr(ff_info.clocked_on(), pin_map);
      Expr clocked_on_also = dot2expr(ff_info.clocked_on_also(), pin_map);
      Expr clear = dot2expr(ff_info.clear(), pin_map);
      Expr preset = dot2expr(ff_info.preset(), pin_map);
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
      Expr data_in = dot2expr(latch_info.data_in(), pin_map);
      Expr enable = dot2expr(latch_info.enable(), pin_map);
      Expr enable_also = dot2expr(latch_info.enable_also(), pin_map);
      Expr clear = dot2expr(latch_info.clear(), pin_map);
      Expr preset = dot2expr(latch_info.preset(), pin_map);
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
    else if ( dt_fsm ) {
      library->new_fsm_cell(cell_id, cell_name, area,
			    ni, no, nio, nit, nbus, nbundle,
			    output_array,
			    logic_array,
			    tristate_array);
    }
    else {
      library->new_logic_cell(cell_id, cell_name, area,
			      ni, no, nio, nbus, nbundle,
			      output_array,
			      logic_array,
			      tristate_array);
    }

    // ピンの生成
    gen_pin(library, pin_info_array, cell_id,
	    output_array,
	    logic_array,
	    tristate_array);

    // タイミング情報の生成
    ymuint nt = 0;
    for (ymuint pg_id = 0; pg_id < npg; ++ pg_id) {
      const DotlibPin& pin_info = pin_info_array[pg_id];
      const list<const DotlibNode*>& timing_list = pin_info.timing_list();
      nt += timing_list.size();
    }
    library->set_timing_num(cell_id, nt);

    const Cell* cell = library->cell(cell_id);

    ymuint timing_id = 0;
    for (ymuint pg_id = 0; pg_id < npg; ++ pg_id) {
      const DotlibPin& pin_info = pin_info_array[pg_id];
      vector<vector<ymuint> > tid_list(ni2 * 2);
      const list<const DotlibNode*>& timing_list = pin_info.timing_list();

      gen_timing(library, timing_list, cell_id, timing_id, pin_map, tid_list);

      ymuint nop = pin_info.num();
      for (ymuint i = 0; i < nop; ++ i) {
	ShString oname = pin_info.name(i);
	const CellPin* opin = cell->pin((const char*)oname);
	assert_cond( opin != NULL, __FILE__, __LINE__);
	ymuint oid = opin->output_id();
	bool has_logic = cell->has_logic(oid);
	TvFunc tv_function;
	if ( has_logic ) {
	  Expr expr = cell->logic_expr(oid);
	  tv_function = expr.make_tv(ni2);
	}
	for (ymuint iid = 0; iid < ni2; ++ iid) {
	  TvFunc p_func = tv_function.cofactor(VarId(iid), false);
	  TvFunc n_func = tv_function.cofactor(VarId(iid), true);

	  const vector<ymuint>& tid_list_p = tid_list[iid * 2 + 0];
	  if ( !tid_list_p.empty() ) {
	    ymuint tid = tid_list_p[0];
	    const CellTiming* timing = cell->timing(tid);
	    bool depend = true;
	    if ( timing->type() == kCellTimingCombinational ) {
	      if ( has_logic && !(~n_func && p_func) ) {
		depend = false;
	      }
	    }
	    if ( depend ) {
	      library->set_timing(cell_id, iid, oid, kCellPosiUnate, tid_list_p);
	    }
	  }

	  const vector<ymuint>& tid_list_n = tid_list[iid * 2 + 1];
	  if ( !tid_list_n.empty() ) {
	    ymuint tid = tid_list_n[0];
	    const CellTiming* timing = cell->timing(tid);
	    bool depend = true;
	    if ( timing->type() == kCellTimingCombinational ) {
	      if ( has_logic && !(~p_func && n_func) ) {
		depend = false;
	      }
	    }
	    if ( depend ) {
	      library->set_timing(cell_id, iid, oid, kCellNegaUnate, tid_list_n);
	    }
	  }
	}
      }
    }
    assert_cond( timing_id == nt, __FILE__, __LINE__);
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
  return gen_library(mgr.root_node());
}

// @brief dotlib ファイルを読み込む
// @param[in] filename ファイル名
// @return 読み込んで作成したセルライブラリを返す．
// @note エラーが起きたら NULL を返す．
const CellLibrary*
CellDotlibReader::operator()(const char* filename)
{
  using namespace nsDotlib;

  DotlibMgr mgr;
  DotlibParser parser;
  if ( !parser.read_file(filename, mgr, false) ) {
    return NULL;
  }
  return gen_library(mgr.root_node());
}

END_NAMESPACE_YM_CELL
