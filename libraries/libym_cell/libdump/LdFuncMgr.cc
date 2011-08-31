
/// @file LdFuncMgr.cc
/// @brief LdFuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdFuncMgr.h"
#include "LdFuncClass.h"
#include "LdFuncGroup.h"
#include "ym_logic/NpnMgr.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラス LdFuncMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LdFuncMgr::LdFuncMgr()
{
}

// @brief デストラクタ
LdFuncMgr::~LdFuncMgr()
{
  init();
}

// @brief 初期化する．
// 定数関数，リテラル関数を登録しておく
// 常に定数0，定数1，肯定リテラル，否定リテラルの関数番号が
// 0, 1, 2, 3 になるようにする．
void
LdFuncMgr::init()
{
  for (vector<LdFuncGroup*>::iterator p = mFuncList.begin();
       p != mFuncList.end(); ++ p) {
    delete *p;
  }
  for (vector<LdFuncClass*>::iterator p = mRepList.begin();
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
    LdFuncGroup* func0 = find_func(const0);
    assert_cond( func0->id() == 0, __FILE__, __LINE__);

    TvFunc const1 = TvFunc::const_one(0);
    LdFuncGroup* func1 = find_func(const1);
    assert_cond( func1->id() == 1, __FILE__, __LINE__);

    TvFunc plit = TvFunc::posi_literal(1, 0);
    LdFuncGroup* func2 = find_func(plit);
    assert_cond( func2->id() == 2, __FILE__, __LINE__);

    TvFunc nlit = TvFunc::nega_literal(1, 0);
    LdFuncGroup* func3 = find_func(nlit);
    assert_cond( func3->id() == 3, __FILE__, __LINE__);
  }
}

// @brief f に対応する LdFunc を求める．
// @param[in] f 関数
// @note なければ新規に作る．
LdFuncGroup*
LdFuncMgr::find_func(const TvFunc& f)
{
  LdFuncGroup* pgfunc = NULL;
  hash_map<TvFunc, LdFuncGroup*>::iterator p = mFuncMap.find(f);
  if ( p == mFuncMap.end() ) {
    // なかったので新たに作る．
    pgfunc = new LdFuncGroup;
    pgfunc->mId = mFuncList.size();
    mFuncList.push_back(pgfunc);
    mFuncMap.insert(make_pair(f, pgfunc));

    // 代表関数を求める．
    NpnMgr npnmgr;
    npnmgr.cannonical(f, pgfunc->mMap);

    TvFunc repfunc = f.xform(pgfunc->mMap);
    LdFuncClass* pgrep = find_repfunc(repfunc);

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

// @brief f に対応する LdFuncClass を求める．
// @param[in] f 関数
// @note なければ新規に作る．
// @note f は NpnMgr によって正規化されている必要がある．
LdFuncClass*
LdFuncMgr::find_repfunc(const TvFunc& f)
{
  LdFuncClass* pgrep = NULL;
  hash_map<TvFunc, LdFuncClass*>::iterator p = mRepMap.find(f);
  if ( p == mRepMap.end() ) {
    // まだ登録されていない．
    pgrep = new LdFuncClass;
    pgrep->mId = mRepList.size();
    pgrep->mFunc = f;
    mRepList.push_back(pgrep);
    mRepMap.insert(make_pair(f, pgrep));
  }
  else {
    // 登録されていた．
    pgrep = p->second;
  }
  return pgrep;
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
LdFuncMgr::dump(ostream& s) const
{
  // 関数の情報をダンプする．
  ymuint nf = func_num();
  BinIO::write_32(s, nf);
  for (ymuint i = 0; i < nf; ++ i) {
    const LdFuncGroup* func = this->func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    // 代表関数に対する変換マップをダンプする．
    dump_map(s, func->map());
    // 属しているセル番号をダンプする．
    const vector<ymuint>& cell_list = func->cell_list();
    ymuint nc = cell_list.size();
    BinIO::write_32(s, nc);
    for (ymuint i = 0; i < nc; ++ i) {
      BinIO::write_32(s, cell_list[i]);
    }
  }

  // 代表関数の情報をダンプする．
  ymuint nr = rep_num();
  BinIO::write_32(s, nr);
  for (ymuint i = 0; i < nr; ++ i) {
    const LdFuncClass* rep = this->rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    ymuint ne = rep->func_num();
    BinIO::write_32(s, ne);
    for (ymuint j = 0; j < ne; ++ j) {
      BinIO::write_32(s, rep->func(j)->id());
    }
  }
}

// @brief 内容を出力する．(デバッグ用)
// @param[in] s 出力先のストリーム
void
LdFuncMgr::display(ostream& s) const
{
  s << "*** LdFuncMgr BEGIN ***" << endl;
  s << "*** FUNCTION SECTION ***" << endl;
  for (ymuint i = 0; i < func_num(); ++ i) {
    const LdFuncGroup* func = this->func(i);
    assert_cond( func->id() == i, __FILE__, __LINE__);
    s << "FUNC#" << i
      << ": REP#" << func->rep()->id()
      << ": " << func->map()
      << endl;
    s << "  CELL#ID" << endl;
    const vector<ymuint>& cell_list = func->cell_list();
    for (vector<ymuint>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      s << "    " << *p << endl;
    }
  }
  s << endl;

  s << "*** REPRESENTATIVE SECTION ***" << endl;
  for (ymuint i = 0; i < rep_num(); ++ i) {
    const LdFuncClass* rep = this->rep(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "REP#" << i << ": ";
    rep->rep_func().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    for (ymuint j = 0; j < rep->func_num(); ++ j) {
      s << " FUNC#" << rep->func(j)->id();
    }
    s << endl;
  }
  s << endl;
  s << "*** LdFuncMgr END ***" << endl;
}

#if 0
void
dump_func(ostream& s,
	  const TvFunc& f)
{
  ymuint ni = f.ni();
  BinIO::write_32(s, ni);
  ymuint nip = (1U << ni);
  ymuint32 v = 0U;
  ymuint base = 0;
  for (ymuint p = 0; p < nip; ++ p) {
    v |= (f.value(p) << (p - base));
    if ( (p % 32) == 31 ) {
      BinIO::write_32(s, v);
      base += 32;
      v = 0U;
    }
  }
  if ( ni <= 4 ) {
    BinIO::write_32(s, v);
  }
}
#endif

void
LdFuncMgr::dump_map(ostream& s,
		    const NpnMap& map)
{
  ymuint ni = map.ni();
  ymuint32 v = (ni << 1);
  if ( map.opol() == kPolNega ) {
    v |= 1U;
  }
  BinIO::write_32(s, v);
  for (ymuint i = 0; i < ni; ++ i) {
    tNpnImap imap = map.imap(i);
    // 手抜きでは imap を ymuint32 にキャストすればよい．
    ymuint j = npnimap_pos(imap);
    ymuint32 v = (j << 1);
    if ( npnimap_pol(imap) ) {
      v |= 1U;
    }
    BinIO::write_32(s, v);
  }
}

END_NAMESPACE_YM_CELL_LIBDUMP
