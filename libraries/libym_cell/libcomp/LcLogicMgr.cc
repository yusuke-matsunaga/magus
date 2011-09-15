
/// @file LcLogicMgr.cc
/// @brief LcLogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcLogicMgr.h"
#include "LcGroup.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/NpnMgr.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcLogicMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] libcomp 親の LibComp
LcLogicMgr::LcLogicMgr(LibComp& libcomp) :
  LcGroupMgr(libcomp)
{
}

// @brief デストラクタ
LcLogicMgr::~LcLogicMgr()
{
}

// @brief 初期化する．
// 定数関数，リテラル関数を登録しておく
// 常に定数0，定数1，肯定リテラル，否定リテラルの関数番号が
// 0, 1, 2, 3 になるようにする．
void
LcLogicMgr::init()
{
  clear();

  { // 定数0グループの登録
    TvFuncM const0(TvFunc::const_zero(0));
    LcGroup* func0 = find_group(const0);
    mLogicGroup[0] = func0->id();
  }
  { // 定数1グループの登録
    TvFuncM const1(TvFunc::const_one(0));
    LcGroup* func1 = find_group(const1);
    mLogicGroup[1] = func1->id();
  }
  { // バッファグループの登録
    TvFuncM plit(TvFunc::posi_literal(1, 0));
    LcGroup* func2 = find_group(plit);
    mLogicGroup[2] = func2->id();
  }
  { // インバーターグループの登録
    TvFuncM nlit(TvFunc::nega_literal(1, 0));
    LcGroup* func3 = find_group(nlit);
    mLogicGroup[3] = func3->id();
  }
}

// @brief 定義済みの論理グループ番号を返す．
// @param[in] id 番号
// - 0: 定数0
// - 1: 定数1
// - 2: バッファ
// - 3: インバーター
ymuint
LcLogicMgr::logic_group(ymuint id) const
{
  assert_cond( id < 4, __FILE__, __LINE__);
  return mLogicGroup[id];
}

// @brief セルのシグネチャ関数を作る．
// @param[in] cell セル
// @param[out] f シグネチャ関数
void
LcLogicMgr::gen_signature(const Cell* cell,
			  TvFuncM& f)
{
  ymuint ni2 = cell->input_num2();
  ymuint no2 = cell->output_num2();
  bool has_tristate = false;
  for (ymuint i = 0; i < no2; ++ i) {
    if ( cell->has_tristate(i) ) {
      has_tristate = true;
      break;
    }
  }
  if ( no2 == 1 && !has_tristate ) {
    LogExpr expr = cell->logic_expr(0);
    f = TvFuncM(expr.make_tv(ni2));
  }
  else {
    vector<TvFunc> f_list(no2 * 2);
    for (ymuint i = 0; i < no2; ++ i) {
      LogExpr lexpr = cell->logic_expr(i);
      f_list[i * 2 + 0] = lexpr.make_tv(ni2);
      LogExpr texpr = cell->tristate_expr(i);
      f_list[i * 2 + 1] = texpr.make_tv(ni2);
    }
    f = TvFuncM(f_list);
  }
}

// @brief 代表関数を求める．
// @param[in] f 関数
// @param[out] repfunc 代表関数
// @param[out] xmap 変換
void
LcLogicMgr::find_repfunc(const TvFuncM& f,
			 TvFuncM& repfunc,
			 NpnMapM& xmap)
{
  ymuint ni = f.ni();
  ymuint no = f.no();

  if ( no == 1 ) {
    NpnMgr npnmgr;
    NpnMap xmap1;
    npnmgr.cannonical(f.output(0), xmap1);
    xmap = NpnMapM(xmap1);
    repfunc = f.xform(xmap);
  }
  else {
    // 今は手抜きで多出力はすべてが代表関数となる．
    xmap.set_identity(ni, no);
    repfunc = f;
  }
}

END_NAMESPACE_YM_CELL_LIBCOMP
