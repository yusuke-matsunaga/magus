
/// @file libym_techmap/patgen/PgFuncMgr.cc
/// @brief PgFuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PgFuncMgr.h"
#include "PgFuncRep.h"
#include "PgFunc.h"
#include "ym_npn/NpnMgr.h"


BEGIN_NAMESPACE_YM_CELLMAP_PATGEN

//////////////////////////////////////////////////////////////////////
// クラス PgFuncMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PgFuncMgr::PgFuncMgr()
{
}

// @brief デストラクタ
PgFuncMgr::~PgFuncMgr()
{
  init();
}

// @brief 初期化する．
// 定数関数，リテラル関数を登録しておく
// 常に定数0，定数1，肯定リテラル，否定リテラルの関数番号が
// 0, 1, 2, 3, 4 になるようにする．
void
PgFuncMgr::init()
{
  for (vector<PgFunc*>::iterator p = mFuncList.begin();
       p != mFuncList.end(); ++ p) {
    delete *p;
  }
  for (vector<PgFuncRep*>::iterator p = mRepList.begin();
       p != mRepList.end(); ++ p) {
    delete *p;
  }
  mFuncList.clear();
  mFuncMap.clear();
  mRepList.clear();
  mRepMap.clear();

  // 既定関数の登録
  {
    TvFunc const0 = TvFunc::const_zero(0);
    PgFunc* func0 = find_func(const0);
    assert_cond( func0->id() == 0, __FILE__, __LINE__);

    TvFunc const1 = TvFunc::const_one(0);
    PgFunc* func1 = find_func(const1);
    assert_cond( func1->id() == 1, __FILE__, __LINE__);

    TvFunc plit = TvFunc::posi_literal(1, 0);
    PgFunc* func2 = find_func(plit);
    assert_cond( func2->id() == 2, __FILE__, __LINE__);

    TvFunc nlit = TvFunc::nega_literal(1, 0);
    PgFunc* func3 = find_func(nlit);
    assert_cond( func3->id() == 3, __FILE__, __LINE__);
  }
}

// @brief 関数と対応するセル番号を登録する．
// @param[in] tvfunc 論理関数関数
// @param[in] cell_id セル番号
// @return 関数情報のオブジェクトを返す．
PgFunc*
PgFuncMgr::reg_func(const TvFunc& tvfunc,
		    ymuint cell_id)
{
  PgFunc* pgfunc = find_func(tvfunc);
  pgfunc->mCellList.push_back(cell_id);
  return pgfunc;
}

// @brief f に対応する PgFunc を求める．
// @param[in] f 関数
// @note なければ新規に作る．
PgFunc*
PgFuncMgr::find_func(const TvFunc& f)
{
  PgFunc* pgfunc = NULL;
  hash_map<TvFunc, PgFunc*>::iterator p = mFuncMap.find(f);
  if ( p == mFuncMap.end() ) {
    // なかったので新たに作る．
    pgfunc = new PgFunc;
    pgfunc->mId = mFuncList.size();
    mFuncList.push_back(pgfunc);
    mFuncMap.insert(make_pair(f, pgfunc));

    // 代表関数を求める．
    NpnMgr npnmgr;
    npnmgr.cannonical(f, pgfunc->mMap);

    TvFunc repfunc = f.xform(pgfunc->mMap);
    PgFuncRep* pgrep = find_repfunc(repfunc);

    // 関数を追加する．
    pgrep->mFuncList.push_back(pgfunc);
    pgfunc->mRep = pgrep;
  }
  else {
    // 既に登録されていた．
    pgfunc = p->second;
  }
  return pgfunc;
}

// @brief f に対応する PgFuncRep を求める．
// @param[in] f 関数
// @note なければ新規に作る．
// @note f は NpnMgr によって正規化されている必要がある．
PgFuncRep*
PgFuncMgr::find_repfunc(const TvFunc& f)
{
  PgFuncRep* pgrep = NULL;
  hash_map<TvFunc, PgFuncRep*>::iterator p = mRepMap.find(f);
  if ( p == mRepMap.end() ) {
    // まだ登録されていない．
    pgrep = new PgFuncRep;
    pgrep->mId = mRepList.size();
    mRepList.push_back(pgrep);
    mRepMap.insert(make_pair(f, pgrep));
    pgrep->mFunc = f;
  }
  else {
    // 登録されていた．
    pgrep = p->second;
  }
  return pgrep;
}

END_NAMESPACE_YM_CELLMAP_PATGEN
