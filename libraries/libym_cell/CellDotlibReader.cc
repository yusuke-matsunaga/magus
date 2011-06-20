
/// @file libym_mislib/CellDotlibReader.cc
/// @brief CellDotlibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellDotlibReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellDotlibReader.h"

#include "CiLibrary.h"

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

// @brief LogExpr を TvFunc に変換する．
// @param[in] expr 対象の論理式
// @param[in] ni 全入力数
TvFunc
expr_to_tvfunc(const LogExpr& expr,
	       ymuint ni)
{
  if ( expr.is_zero() ) {
    return TvFunc::const_zero(ni);
  }
  if ( expr.is_one() ) {
    return TvFunc::const_one(ni);
  }
  if ( expr.is_posiliteral() ) {
    return TvFunc::posi_literal(ni, expr.varid());
  }
  if ( expr.is_negaliteral() ) {
    return TvFunc::nega_literal(ni, expr.varid());
  }
  // あとは AND/OR/XOR のみ
  ymuint n = expr.child_num();
  vector<TvFunc> child_func(n);
  for (ymuint i = 0; i < n; ++ i) {
    child_func[i] = expr_to_tvfunc(expr.child(i), ni);
  }
  TvFunc func = child_func[0];
  if ( expr.is_and() ) {
    for (ymuint i = 1; i < n; ++ i) {
      func &= child_func[i];
    }
  }
  else if ( expr.is_or() ) {
    for (ymuint i = 1; i < n; ++ i) {
      func |= child_func[i];
    }
  }
  else if ( expr.is_xor() ) {
    for (ymuint i = 1; i < n; ++ i) {
      func ^= child_func[i];
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  return func;
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
  CiLibrary* library = new CiLibrary(library_info.name());

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

    // ピン名とピン番号の連想配列
    hash_map<ShString, ymuint> pin_map;

    // ピン情報の配列
    vector<DotlibPin> pin_info_array(npin);

    // ピン情報の読み出し
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

	ShString pin_name = pin_info.name();
	pin_map.insert(make_pair(pin_name, pin_id));
      }
      if ( error ) {
	continue;
      }
    }

    const DotlibNode* dt_ff = cell_info.ff();
    const DotlibNode* dt_latch = cell_info.latch();

    // セルの生成
    CiCell* cell = NULL;
    if ( dt_ff ) {
      DotlibFF ff_info;
      if ( !dt_ff->get_ff_info(ff_info) ) {
	continue;
      }
      ShString var1 = ff_info.var1_name();
      ShString var2 = ff_info.var2_name();
      LogExpr next_state = dot2expr(ff_info.next_state(), pin_map);
      LogExpr clocked_on = dot2expr(ff_info.clocked_on(), pin_map);
      LogExpr clocked_on_also = dot2expr(ff_info.clocked_on_also(), pin_map);
      LogExpr clear = dot2expr(ff_info.clear(), pin_map);
      LogExpr preset = dot2expr(ff_info.preset(), pin_map);
      ymuint v1 = ff_info.clear_preset_var1();
      ymuint v2 = ff_info.clear_preset_var2();

      cell = library->new_ff_cell(cell_id, cell_name, area,
				  var1, var2,
				  next_state, clocked_on, clocked_on_also,
				  clear, preset, v1, v2,
				  npin, nbus, nbundle);

      // pin_map に登録しておく
      pin_map.insert(make_pair(var1, npin));
      pin_map.insert(make_pair(var2, npin + 1));
    }
    else if ( dt_latch ) {
      DotlibLatch latch_info;
      if ( !dt_latch->get_latch_info(latch_info) ) {
	continue;
      }
      ShString var1 = latch_info.var1_name();
      ShString var2 = latch_info.var2_name();
      LogExpr data_in = dot2expr(latch_info.data_in(), pin_map);
      LogExpr enable = dot2expr(latch_info.enable(), pin_map);
      LogExpr enable_also = dot2expr(latch_info.enable_also(), pin_map);
      LogExpr clear = dot2expr(latch_info.clear(), pin_map);
      LogExpr preset = dot2expr(latch_info.preset(), pin_map);
      ymuint v1 = latch_info.clear_preset_var1();
      ymuint v2 = latch_info.clear_preset_var2();

      cell = library->new_latch_cell(cell_id, cell_name, area,
				     var1, var2,
				     data_in, enable, enable_also,
				     clear, preset, v1, v2,
				     npin, nbus, nbundle);

      // pin_map に登録しておく
      pin_map.insert(make_pair(var1, npin));
      pin_map.insert(make_pair(var2, npin + 1));
    }
    else {
      cell = library->new_logic_cell(cell_id, cell_name, area,
				     npin, nbus, nbundle);
    }

    // ピンの生成
    for (ymuint i = 0; i < npin; ++ i) {
      const DotlibPin& pin_info = pin_info_array[i];
      switch ( pin_info.direction() ) {
      case DotlibPin::kInput:
	{
	  CellCapacitance cap(pin_info.capacitance());
	  CellCapacitance rise_cap(pin_info.rise_capacitance());
	  CellCapacitance fall_cap(pin_info.fall_capacitance());
	  library->new_cell_input(cell, i, pin_info.name(),
				  cap, rise_cap, fall_cap);
	}
	break;

      case DotlibPin::kOutput:
	{
	  CellCapacitance max_fanout(pin_info.max_fanout());
	  CellCapacitance min_fanout (pin_info.min_fanout());
	  CellCapacitance max_capacitance(pin_info.max_capacitance());
	  CellCapacitance min_capacitance(pin_info.min_capacitance());
	  CellTime max_transition(pin_info.max_transition());
	  CellTime min_transition(pin_info.min_transition());
	  library->new_cell_output(cell, i, pin_info.name(),
				   max_fanout, min_fanout,
				   max_capacitance, min_capacitance,
				   max_transition, min_transition);
	  const DotlibNode* func_node = pin_info.function();
	  if ( func_node ) {
	    LogExpr expr = dot2expr(func_node, pin_map);
	    library->set_opin_function(cell, i, expr);

#if 0
	    TvFunc tv_function = expr_to_tvfunc(expr, ni);
	    for (ymuint i = 0; i < ni; ++ i) {
	      // タイミング情報の設定
	      const DotlibNode* pt_pin = ipin_array[i];
	      TvFunc p_func = tv_function.cofactor(i, kPolPosi);
	      TvFunc n_func = tv_function.cofactor(i, kPolNega);
	      tCellTimingSense sense_real = kSenseNonUnate;
	      bool redundant = false;
	      if ( ~p_func && n_func ) {
		if ( ~n_func && p_func ) {
		  sense_real = kSenseNonUnate;
		}
		else {
		  sense_real = kSenseNegaUnate;
		}
	      }
	      else {
		if ( ~n_func && p_func ) {
		  sense_real = kSensePosiUnate;
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
				  "MISLIB_PARSER",
				  buf.str());
		  redundant = true;
		}
	      }

	      tCellTimingSense sense = kSenseNonUnate;
	      switch ( pt_pin->phase()->type() ) {
	      case DotlibNode::kNoninv:
		sense = kSensePosiUnate;
		break;

	      case DotlibNode::kInv:
		sense = kSenseNegaUnate;
		break;

	      case DotlibNode::kUnknown:
		sense = kSenseNonUnate;
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
				"MISLIB_PARSER",
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
		library->set_opin_timing(opin, i, sense, timing);
	      }
	    }
#endif
	  }
	  const DotlibNode* three_state = pin_info.three_state();
	  if ( three_state ) {
	    LogExpr expr = dot2expr(three_state, pin_map);
	    library->set_opin_three_state(cell, i, expr);
	  }
	}
	break;

      case DotlibPin::kInout:
	{
	  CellCapacitance cap(pin_info.capacitance());
	  CellCapacitance rise_cap(pin_info.rise_capacitance());
	  CellCapacitance fall_cap(pin_info.fall_capacitance());
	  CellCapacitance max_fanout(pin_info.max_fanout());
	  CellCapacitance min_fanout(pin_info.min_fanout());
	  CellCapacitance max_capacitance(pin_info.max_capacitance());
	  CellCapacitance min_capacitance(pin_info.min_capacitance());
	  CellTime max_transition(pin_info.max_transition());
	  CellTime min_transition(pin_info.min_transition());
	  library->new_cell_inout(cell, i, pin_info.name(),
				  cap, rise_cap, fall_cap,
				  max_fanout, min_fanout,
				  max_capacitance, min_capacitance,
				  max_transition, min_transition);
	}
	break;

      case DotlibPin::kInternal:
	library->new_cell_internal(cell, i, pin_info.name());
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
      }
    }

  }

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
CellDotlibReader::read(const string& filename)
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
