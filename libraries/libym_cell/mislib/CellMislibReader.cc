
/// @file libym_mislib/CellMislibReader.cc
/// @brief CellMislibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellMislibReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellMislibReader.h"
#include "ym_mislib/MislibParser.h"
#include "ym_mislib/MislibMgr.h"
#include "ym_mislib/MislibNode.h"
#include "ym_lexp/LogExpr.h"
#if 0
#include "ym_npn/TvFunc.h"
#endif
#include "../ci/CiLibrary.h"


BEGIN_NAMESPACE_YM_CELL


BEGIN_NONAMESPACE

// 論理式を表すパース木をスキャンして端子名のリストを作る．
// param[in] node パース木の根のノード
// param[out] name_list 名前が現れた順に格納されたリスト
// param[out] name_map 名前をキーにして name_list 中の位置を返す連想配列
void
dfs(const MislibNode* node,
    vector<ShString>& name_list,
    hash_map<ShString, ymuint>& name_map)
{
  switch ( node->type() ) {
  case MislibNode::kConst0:
  case MislibNode::kConst1:
    return;

  case MislibNode::kStr:
    {
      ShString name = node->str();
      if ( name_map.count(name) == 0 ) {
	// 登録する．
	name_map.insert(make_pair(name, name_list.size()));
	name_list.push_back(name);
      }
    }
    break;

  case MislibNode::kNot:
    dfs(node->child1(), name_list, name_map);
    break;

  case MislibNode::kAnd:
  case MislibNode::kOr:
  case MislibNode::kXor:
    dfs(node->child1(), name_list, name_map);
    dfs(node->child2(), name_list, name_map);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

#if 0
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
#endif

// @brief MislibNode から CellLibrary を生成する．
// @param[in] lib_name ライブラリ名
// @param[in] gate_list パース木のルート
// @return 生成したライブラリを返す．
// @note 生成が失敗したら NULL を返す．
const CellLibrary*
gen_library(const string& lib_name,
	    const MislibNode* gate_list)
{
  // ライブラリの生成
  CiLibrary* library = new CiLibrary(lib_name);

  // セル数の設定
  ymuint cell_num = 0;
  for (const MislibNode* gate = gate_list->top(); gate; gate = gate->next()) {
    ++ cell_num;
  }
  library->set_cell_num(cell_num);

  // セルの内容の設定
  ymuint cell_id = 0;
  for (const MislibNode* gate = gate_list->top(); gate;
       gate = gate->next(), ++ cell_id) {
    ShString name = gate->name()->str();
    CellArea area(gate->area()->num());
    ShString opin_name = gate->opin_name()->str();
    const MislibNode* opin_expr = gate->opin_expr();
    const MislibNode* ipin_list = gate->ipin_list();
    vector<const MislibNode*> ipin_array;
    vector<ShString> ipin_name_list;
    hash_map<ShString, ymuint> ipin_name_map;
    if ( ipin_list->type() == MislibNode::kList ) {
      // 通常の入力ピン定義がある場合
      // ipin_list の順に入力ピンを作る．
      for (const MislibNode* pin = ipin_list->top(); pin; pin = pin->next()) {
	assert_cond( pin->type() == MislibNode::kPin, __FILE__, __LINE__);
	ShString name = pin->name()->str();
	assert_cond( ipin_name_map.count(name) == 0, __FILE__, __LINE__);
	ipin_name_map.insert(make_pair(name, ipin_array.size()));
	ipin_array.push_back(pin);
	ipin_name_list.push_back(name);
      }
    }
    else if ( ipin_list->type() == MislibNode::kPin ) {
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
      const MislibNode* pin = ipin_array[i];
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
#if 0
    TvFunc tv_function = expr_to_tvfunc(function, ni);
#endif
    for (ymuint i = 0; i < ni; ++ i) {
      // タイミング情報の設定
      const MislibNode* pt_pin = ipin_array[i];
#if 0
      int unate = tv_function.check_unateness(i);
#else
      tCellTimingSense sense;
      switch ( pt_pin->phase()->type() ) {
      case MislibNode::kNoninv:  sense = kSensePosiUnate; break;
      case MislibNode::kInv:     sense = kSenseNegaUnate; break;
      case MislibNode::kUnknown: sense = kSenseNonUnate; break;
      default: assert_not_reached(__FILE__, __LINE__); break;
      }
#endif
      CellTime r_i(pt_pin->rise_block_delay()->num());
      CellResistance r_r(pt_pin->rise_fanout_delay()->num());
      CellTime f_i(pt_pin->fall_block_delay()->num());
      CellResistance f_r(pt_pin->fall_fanout_delay()->num());
      CellTiming* timing = library->new_timing(i, kTimingCombinational,
					       r_i, f_i,
					       CellTime(0.0), CellTime(0.0),
					       r_r, f_r);
      library->set_opin_timing(opin, i, sense, timing);
    }
  }

  return library;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// Mislibファイルを読んで CellLibrary を作るクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellMislibReader::CellMislibReader()
{
}

// @brief デストラクタ
CellMislibReader::~CellMislibReader()
{
}

// @brief mislib ファイルを読み込む
// @param[in] filename ファイル名
// @return 読み込んで作成したセルライブラリを返す．
// @note エラーが起きたら NULL を返す．
const CellLibrary*
CellMislibReader::read(const string& filename)
{
  MislibMgr mgr;
  MislibParser parser;
  if ( !parser.read_file(filename, mgr) ) {
    return NULL;
  }
  return gen_library(filename, mgr.gate_list());
}

END_NAMESPACE_YM_CELL
