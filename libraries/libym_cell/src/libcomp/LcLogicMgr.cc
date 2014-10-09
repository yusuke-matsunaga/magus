
/// @file LcLogicMgr.cc
/// @brief LcLogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LcLogicMgr.h"
#include "LcGroup.h"
#include "YmLogic/Expr.h"
#include "YmLogic/TvFunc.h"
#include "YmLogic/NpnMgr.h"
#include "YmCell/Cell.h"


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
    LcGroup* func0 = find_group(const0, false);
    mLogicGroup[0] = func0->id();
  }
  { // 定数1グループの登録
    TvFuncM const1(TvFunc::const_one(0));
    LcGroup* func1 = find_group(const1, false);
    mLogicGroup[1] = func1->id();
  }
  { // バッファグループの登録
    TvFuncM plit(TvFunc::posi_literal(1, VarId(0)));
    LcGroup* func2 = find_group(plit, false);
    mLogicGroup[2] = func2->id();
  }
  { // インバーターグループの登録
    TvFuncM nlit(TvFunc::nega_literal(1, VarId(0)));
    LcGroup* func3 = find_group(nlit, false);
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
    Expr expr = cell->logic_expr(0);
    f = TvFuncM(expr.make_tv(ni2));
  }
  else {
    vector<TvFunc> f_list(no2 * 2);
    for (ymuint i = 0; i < no2; ++ i) {
      Expr lexpr = cell->logic_expr(i);
      f_list[i * 2 + 0] = lexpr.make_tv(ni2);
      Expr texpr = cell->tristate_expr(i);
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
  ymuint no = f.output_num();

  if ( no == 1 ) {
    TvFunc f1 = f.output(VarId(0));
    NpnMap xmap1;
    mNpnMgr.cannonical(f1, xmap1);
    xmap = NpnMapM(xmap1);
    repfunc = f.xform(xmap);
    { // 一応検証
      TvFunc repfunc1 = f1.xform(xmap1);

      if ( repfunc.output(VarId(0)) != repfunc1 ) {
	cout << "Error in find_repfunc" << endl
	     << "f          = " << f1 << endl
	     << "repfunc    = " << repfunc << endl
	     << "repfunc(0) = " << repfunc.output(VarId(0)) << endl
	     << "repfunc1   = " << repfunc1 << endl;
      }
      ASSERT_COND( repfunc.output(VarId(0)) == repfunc1 );
    }
  }
  else {
    default_repfunc(f, repfunc, xmap);
  }
}

// @brief 同位体変換リストを求める．
// @param[in] func 対象の関数
// @param[out] idmap_list 同位体変換のリスト
void
LcLogicMgr::find_idmap_list(const TvFuncM& func,
			    vector<NpnMapM>& idmap_list)
{
  idmap_list.clear();

  ymuint ni = func.input_num();
  ymuint no = func.output_num();
  if ( no == 1 ) {
    NpnMap xmap1;
    TvFunc f1 = func.output(VarId(0));
    mNpnMgr.cannonical(f1, xmap1);
    { // 検証
      TvFunc f2 = f1.xform(xmap1);
      if ( f1 != f2 ) {
	cerr << "f1   = " << f1 << endl
	     << "f2   = " << f2 << endl
	     << "func = " << func << endl
	     << "xmap1 = " << xmap1 << endl;
      }
      assert_cond( f1 == f2, __FILE__, __LINE__);
    }
    vector<NpnMap> tmp_list;
    mNpnMgr.all_map(tmp_list);
    idmap_list.reserve(tmp_list.size());
    for (vector<NpnMap>::iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      const NpnMap& map = *p;
      // 恒等変換は追加しない．
      bool ident = true;
      for (ymuint i = 0; i < ni; ++ i) {
	VarId src_var(i);
	NpnVmap vmap = map.imap(src_var);
	if ( vmap.var() != src_var || vmap.inv() ) {
	  ident = false;
	  break;
	}
      }
      if ( !ident ) {
	idmap_list.push_back(NpnMapM(map));
      }
    }
  }
  else {
    // 今のところなし．
  }
}

END_NAMESPACE_YM_CELL_LIBCOMP
