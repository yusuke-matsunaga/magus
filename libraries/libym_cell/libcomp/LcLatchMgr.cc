
/// @file LcLatchMgr.cc
/// @brief LcLatchMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcLatchMgr.h"
#include "ym_cell/Cell.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMapM.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラスLcLatchMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcLatchMgr::LcLatchMgr()
{
}

// @brief デストラクタ
LcLatchMgr::~LcLatchMgr()
{
}

// @brief 初期化する．
void
LcLatchMgr::init()
{
  clear();
}

// @brief セルのシグネチャ関数を作る．
// @param[in] cell セル
// @param[out] f シグネチャ関数
void
LcLatchMgr::gen_signature(const Cell* cell,
		       TvFuncM& f)
{
  ymuint ni = cell->input_num();
  ymuint no = cell->output_num();
  ymuint nio = cell->inout_num();

  // + 2 は状態変数
  ymuint ni2 = ni + nio + 2;
  ymuint no2 = no + nio;

  vector<TvFunc> f_list(no2 * 2 + 4);
  for (ymuint i = 0; i < no2; ++ i) {
    LogExpr lexpr = cell->logic_expr(i);
    f_list[i * 2 + 0] = lexpr.make_tv(ni2);
    LogExpr texpr = cell->tristate_expr(i);
    f_list[i * 2 + 1] = texpr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->data_in_expr();
    f_list[no2 * 2 + 0] = expr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->enable_expr();
    f_list[no2 * 2 + 1] = expr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->clear_expr();
    f_list[no2 * 2 + 2] = expr.make_tv(ni2);
  }
  {
    LogExpr expr = cell->preset_expr();
    f_list[no2 * 2 + 3] = expr.make_tv(ni2);
  }
}

// @brief 代表関数を求める．
// @param[in] f 関数
// @param[out] repfunc 代表関数
// @param[out] xmap 変換
void
LcLatchMgr::find_repfunc(const TvFuncM& f,
		      TvFuncM& repfunc,
		      NpnMapM& xmap)
{
  ymuint ni = f.ni();
  ymuint no = f.no();
  // 今は手抜きで自分自身を代表関数とする．
  xmap.set_identity(ni, no);
  repfunc = f;
}

END_NAMESPACE_YM_CELL_LIBCOMP
