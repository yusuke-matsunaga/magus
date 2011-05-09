
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


// 1つの文字列からなる value_list から文字列を取り出す．
// 仮定が外れたらアボートする．
ShString
get_string(const DotlibNode* value_list)
{
  assert_cond( value_list->is_list(), __FILE__, __LINE__);
  assert_cond( value_list->list_size() == 1, __FILE__, __LINE__);
  const DotlibNode* value = value_list->top();
  assert_cond( value->is_string(), __FILE__, __LINE__);
  return root_value->string_value();
}

// @brief DotlibNode から CellLibrary を生成する．
// @param[in] dt_library ライブラリを表すパース木のルート
// @return 生成したライブラリを返す．
// @note 生成が失敗したら NULL を返す．
const CellLibrary*
gen_library(const DotlibNode* dt_library)
{
  assert_cond( dt_library->is_attr(), __FILE__, __LINE__);
  assert_cond( dt_library->attr_name() == "library", __FILE__, __LINE__);
  const DotlibNode* root_group = dt_library->attr_value();
  assert_cond( root_group->is_group(), __FILE__, __LINE__);
  ShString library_name = get_string(root_group->group_value());

  // ライブラリの生成
  CiLibrary* library = new CiLibrary(library_name.c_str());

  // セル数の設定
  list<const DotlibNode*> dt_cell_list;
  for (const DotlibNode* attr = dt_library->attr_top();
       attr; attr = attr->next()) {
    if ( attr->attr_name() == "cell" ) {
      dt_cell_list.push_back(attr);
    }
  }
  library->set_cell_num(dt_cell_list.size());

  // セルの内容の設定
  ymuint cell_id = 0;
  for (list<const DotlibNode*>::iterator p = dt_cell_list.begin();
       p != dt_cell_list.end(); ++ p) {
    cosnt DotlibNode* cell_attr = *p;
    const DotlibNode* dt_cell = cell_attr->attr_value();
    assert_cond( dt_cell->is_group(), __FILE__, __LINE__);
    // セル名
    ShString name = get_string(dt_cell->group_value());

    // 属性のリストを作る．
    hash_map<ShString, list<const DotlibNode*> > attr_map;
    for (const DotlibNode* attr = dt_cell->attr_top();
	 attr; attr = attr->next()) {
      ShString attr_name = attr->attr_name();
      const DotlibNode* attr_value = attr->attr_value();
      // なかなか大胆なコード
      attr_map[attr_name].push_back(attr_value);
    }

    // 面積を得る．
    hash_map<ShString, list<const DotlibNode*> >::iterator p;
    p = attr_map.find("area");
    if ( p == attr_map.end() ) {
      ostringstream buf;
      buf << name << ": No area definition.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      dt_cell->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      continue;
    }
    list<const DotlibNode*>& area_list = p->second;
    if ( vlist.size() != 1 ) {
      list<const DotlibNode*>::iterator q = vlist.begin();
      const DotlibNode* first = *q;
      ++ q;
      const DotlibNode* second = *q;
      ostringstream buf;
      buf << name << ": More than one area definition."
	  << " First occurence is " << first->loc() << ".";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      second->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      continue;
    }
    const DotlibNode* area_node = vlist.front();
    if ( !area_node->is_float() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      area_node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Float value is expected.");
      continue;
    }
    CellArea area(area_node->float_value());

    // ピン数を数える．
    const
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
    CiCell* cell = library->new_cell(cell_id, name, area, ni +  1, 0, 0);
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
