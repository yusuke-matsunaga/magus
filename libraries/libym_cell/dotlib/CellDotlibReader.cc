
/// @file libym_mislib/CellDotlibReader.cc
/// @brief CellDotlibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellDotlibReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellDotlibReader.h"
#include "ym_dotlib/DotlibParser.h"
#include "ym_dotlib/DotlibMgr.h"
#include "ym_dotlib/DotlibNode.h"
#include "ym_dotlib/DotlibLibrary.h"
#include "ym_dotlib/DotlibCell.h"
#include "ym_dotlib/DotlibPin.h"

#include "ym_lexp/LogExpr.h"
#include "ym_npn/TvFunc.h"
#include "../ci/CiLibrary.h"
#include "ym_utils/MsgMgr.h"

BEGIN_NAMESPACE_YM_CELL


BEGIN_NONAMESPACE

// 論理式を表すパース木をスキャンして端子名のリストを作る．
// param[in] node パース木の根のノード
// param[out] name_list 名前が現れた順に格納されたリスト
// param[out] name_map 名前をキーにして name_list 中の位置を返す連想配列
void
dfs(const DotlibNode* node,
    vector<ShString>& name_list,
    hash_map<ShString, ymuint>& name_map)
{
  switch ( node->type() ) {
  case DotlibNode::kConst0:
  case DotlibNode::kConst1:
    return;

  case DotlibNode::kStr:
    {
      ShString name = node->str();
      if ( name_map.count(name) == 0 ) {
	// 登録する．
	name_map.insert(make_pair(name, name_list.size()));
	name_list.push_back(name);
      }
    }
    break;

  case DotlibNode::kNot:
    dfs(node->child1(), name_list, name_map);
    break;

  case DotlibNode::kAnd:
  case DotlibNode::kOr:
  case DotlibNode::kXor:
    dfs(node->child1(), name_list, name_map);
    dfs(node->child2(), name_list, name_map);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
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
  CiLibrary* library = new CiLibrary(library_info.name().c_str());

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
    const list<const DotlibNode*>& dt_pin_list = cell_info.pin_list();
    const list<const DotlibNode*>& dt_bus_list = cell_info.bus_list();
    const list<const DotlibNode*>& dot_bundle_list = cell_info.bundle_list();

    // セルの生成
    ShString cell_name = cell_info.name();
    double area = cell_info.area();
    ymuint npin = dt_pin_list.size();
    ymuint nbus = dt_bus_list.size();
    ymuint nbundle = dt_bundle_list.size();
    CiCell* cell = library->new_cell(cell_id, cell_name, area,
				     npin, nbus, nbundle);

    // ピンの生成
    ymuint pin_id = 0;
    for (list<const DotlibNode*>::const_iterator q = dt_pin_list.begin();
	 q != dt_pin_list.end(); ++ q, ++ pin_id) {
      const DotlibNode* dt_pin = *q;

      // ピン情報の読み出し
      DotlibCell pin_info;
      if ( !dt_pin->get_pin_info(pin_info) ) {
	continue;
      }

      ShString pin_name = pin_info.name();
      switch ( pin_info.direction() ) {
      case DotlibPin::kInput:
	{
	  CellCapacitance cap = pin_info.capacitance();
	  CellCapacitance rise_cap = pin_info.rise_capacitance();
	  CellCapacitance fall_cap = pin_info.fall_capacitance();
	  library->new_cell_input(cell, pin_id, pin_name,
				  cap, rise_cap, fall_cap);
	}
	break;

      case DotlibPin::kOutput:
	{
	  CellCapacitance max_fanout = pin_info.max_fanout();
	  CellCapacitance min_fanout = pin_info.min_fanout();
	  CellCapacitance max_capacitance = pin_info.max_capacitance();
	  CellCapacitance min_capacitance = pin_info.min_capacitance();
	  CellTime max_transition = pin_info.max_transition();
	  CellTime min_transition = pin_info.min_transition();
	  library->new_cell_output(cell, pin_id, pin_name,
				   max_fanout, min_fanout,
				   max_capacitance, min_capacitance,
				   max_transition, min_transition);
	}
	break;

      case DotlibPin::kInout:
	{
	  CellCapacitance cap = pin_info.capacitance();
	  CellCapacitance rise_cap = pin_info.rise_capacitance();
	  CellCapacitance fall_cap = pin_info.fall_capacitance();
	  CellCapacitance max_fanout = pin_info.max_fanout();
	  CellCapacitance min_fanout = pin_info.min_fanout();
	  CellCapacitance max_capacitance = pin_info.max_capacitance();
	  CellCapacitance min_capacitance = pin_info.min_capacitance();
	  CellTime max_transition = pin_info.max_transition();
	  CellTime min_transition = pin_info.min_transition();
	  library->new_cell_output(cell, pin_id, pin_name,
				   max_fanout, min_fanout,
				   max_capacitance, min_capacitance,
				   max_transition, min_transition);
	}
	break;

      case DotlibPin::kInternal:
#warning "TODO: 未完"
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
      }
    }

    ShString opin_name = gate->opin_name()->str();
    const DotlibNode* opin_expr = gate->opin_expr();
    const DotlibNode* ipin_list = gate->ipin_list();
    vector<const DotlibNode*> ipin_array;
    vector<ShString> ipin_name_list;
    hash_map<ShString, ymuint> ipin_name_map;
    if ( ipin_list->type() == DotlibNode::kList ) {
      // 通常の入力ピン定義がある場合
      // ipin_list の順に入力ピンを作る．
      for (const DotlibNode* pin = ipin_list->top(); pin; pin = pin->next()) {
	assert_cond( pin->type() == DotlibNode::kPin, __FILE__, __LINE__);
	ShString name = pin->name()->str();
	assert_cond( ipin_name_map.count(name) == 0, __FILE__, __LINE__);
	ipin_name_map.insert(make_pair(name, ipin_array.size()));
	ipin_array.push_back(pin);
	ipin_name_list.push_back(name);
      }
    }
    else if ( ipin_list->type() == DotlibNode::kPin ) {
      // ワイルドカードの場合
      // 論理式に現れる順に入力ピンを作る．
      dfs(opin_expr, ipin_name_list, ipin_name_map);
      for (ymuint i = 0; i < ipin_name_list.size(); ++ i) {
	ipin_array.push_back(ipin_list);
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }

    ymuint ni = ipin_name_list.size();
    for (ymuint i = 0; i < ni; ++ i) {
      // 入力ピンの設定
      ShString name = ipin_name_list[i];
      const DotlibNode* pin = ipin_array[i];
      CellCapacitance load(pin->input_load()->num());
      library->new_cell_input(cell, i, name, load, load, load);
    }
    // 出力ピンの設定
    CiOutputPin* opin = library->new_cell_output(cell, ni, opin_name,
						 CellCapacitance::infty(),
						 CellCapacitance(0.0),
						 CellCapacitance::infty(),
						 CellCapacitance(0.0),
						 CellTime::infty(),
						 CellTime(0.0));
    LogExpr function = opin_expr->to_expr(ipin_name_map);
    library->set_opin_function(opin, function);

    TvFunc tv_function = expr_to_tvfunc(function, ni);
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
					       CellTime(0.0), CellTime(0.0),
					       r_r, f_r);
      if ( !redundant ) {
	library->set_opin_timing(opin, i, sense, timing);
      }
    }
  }

  return library;
}

END_NONAMESPACE


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
  DotlibMgr mgr;
  DotlibParser parser;
  if ( !parser.read_file(filename, mgr) ) {
    return NULL;
  }
  return gen_library(filename, mgr.root_node());
}

END_NAMESPACE_YM_CELL
