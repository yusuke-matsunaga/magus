
/// @file LcFFMgr.cc
/// @brief LcFFMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcFFMgr.h"
#include "LcGroup.h"
#include "LcClass.h"
#include "ym_cell/Cell.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラスLcFFMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] libcomp 親の LibComp
LcFFMgr::LcFFMgr(LibComp& libcomp) :
  LcGroupMgr(libcomp)
{
}

// @brief デストラクタ
LcFFMgr::~LcFFMgr()
{
}

// @brief 初期化する．
void
LcFFMgr::init()
{
  clear();

  { // クリアなし，プリセットなしのFFクラスの登録
    vector<TvFunc> f_list;
    f_list.push_back(TvFunc::posi_literal(4, VarId(0)));
    f_list.push_back(TvFunc::posi_literal(4, VarId(1)));
    f_list.push_back(TvFunc::const_zero(4));
    f_list.push_back(TvFunc::const_zero(4));
    f_list.push_back(TvFunc::posi_literal(4, VarId(2)));
    f_list.push_back(TvFunc::const_zero(4));
    f_list.push_back(TvFunc::posi_literal(4, VarId(3)));
    f_list.push_back(TvFunc::const_zero(4));
    TvFuncM f(f_list);
    LcGroup* group = find_group(f, false);
    LcClass* cclass = group->parent();
    mFFClass[0] = cclass->id();
  }
  { // クリアあり，プリセットなしのFFクラスの登録
    vector<TvFunc> f_list;
    f_list.push_back(TvFunc::posi_literal(5, VarId(0)));
    f_list.push_back(TvFunc::posi_literal(5, VarId(1)));
    f_list.push_back(TvFunc::posi_literal(5, VarId(2)));
    f_list.push_back(TvFunc::const_zero(5));
    f_list.push_back(TvFunc::posi_literal(5, VarId(3)));
    f_list.push_back(TvFunc::const_zero(5));
    f_list.push_back(TvFunc::posi_literal(5, VarId(4)));
    f_list.push_back(TvFunc::const_zero(5));
    TvFuncM f(f_list);
    LcGroup* group = find_group(f, false);
    LcClass* cclass = group->parent();
    mFFClass[1] = cclass->id();
  }
  { // クリアなし，プリセットありのFFクラスの登録
    vector<TvFunc> f_list;
    f_list.push_back(TvFunc::posi_literal(5, VarId(0)));
    f_list.push_back(TvFunc::posi_literal(5, VarId(1)));
    f_list.push_back(TvFunc::const_zero(5));
    f_list.push_back(TvFunc::posi_literal(5, VarId(2)));
    f_list.push_back(TvFunc::posi_literal(5, VarId(3)));
    f_list.push_back(TvFunc::const_zero(5));
    f_list.push_back(TvFunc::posi_literal(5, VarId(4)));
    f_list.push_back(TvFunc::const_zero(5));
    TvFuncM f(f_list);
    LcGroup* group = find_group(f, false);
    LcClass* cclass = group->parent();
    mFFClass[2] = cclass->id();
  }
  { // クリアあり，プリセットありのFFクラスの登録
    vector<TvFunc> f_list;
    f_list.push_back(TvFunc::posi_literal(6, VarId(0)));
    f_list.push_back(TvFunc::posi_literal(6, VarId(1)));
    f_list.push_back(TvFunc::posi_literal(6, VarId(2)));
    f_list.push_back(TvFunc::posi_literal(6, VarId(3)));
    f_list.push_back(TvFunc::posi_literal(6, VarId(4)));
    f_list.push_back(TvFunc::const_zero(6));
    f_list.push_back(TvFunc::posi_literal(6, VarId(5)));
    f_list.push_back(TvFunc::const_zero(6));
    TvFuncM f(f_list);
    LcGroup* group = find_group(f, false);
    LcClass* cclass = group->parent();
    mFFClass[3] = cclass->id();
  }
}

// @brief 定義済みのFFクラス番号を返す．
// @param[in] id 番号
// - 0: クリアなし，プリセットなし
// - 1: クリアあり，プリセットなし
// - 2: クリアなし，プリセットあり
// - 3: クリアあり，プリセットあり
ymuint
LcFFMgr::ff_class(ymuint id) const
{
  assert_cond( id < 4, __FILE__, __LINE__);
  return mFFClass[id];
}

// @brief セルのシグネチャ関数を作る．
// @param[in] cell セル
// @param[out] f シグネチャ関数
void
LcFFMgr::gen_signature(const Cell* cell,
		       TvFuncM& f)
{
  // + 2 は状態変数
  ymuint ni2 = cell->input_num2() + 2;
  ymuint no2 = cell->output_num2();

  vector<TvFunc> f_list(no2 * 2 + 4);
  {
    LogExpr expr = cell->next_state_expr();
    f_list[0] = expr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->clock_expr();
    f_list[1] = expr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->clear_expr();
    f_list[2] = expr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->preset_expr();
    f_list[3] = expr.make_tv(ni2);
  }
  for (ymuint i = 0; i < no2; ++ i) {
    LogExpr lexpr = cell->logic_expr(i);
    f_list[i * 2 + 4] = lexpr.make_tv(ni2);
    LogExpr texpr = cell->tristate_expr(i);
    f_list[i * 2 + 5] = texpr.make_tv(ni2);
  }
  f = TvFuncM(f_list);
}

// @brief 代表関数を求める．
// @param[in] f 関数
// @param[out] repfunc 代表関数
// @param[out] xmap 変換
void
LcFFMgr::find_repfunc(const TvFuncM& f,
		      TvFuncM& repfunc,
		      NpnMapM& xmap)
{
  default_repfunc(f, repfunc, xmap);
}

// @brief 同位体変換リストを求める．
// @param[in] func 対象の関数
// @param[out] idmap_list 同位体変換のリスト
void
LcFFMgr::find_idmap_list(const TvFuncM& func,
			 vector<NpnMapM>& idmap_list)
{
  idmap_list.clear();
  // 今のところなし．
}

END_NAMESPACE_YM_CELL_LIBCOMP
