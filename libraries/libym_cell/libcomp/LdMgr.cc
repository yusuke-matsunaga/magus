
/// @file LdLogicMgr.cc
/// @brief LdLogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdLogicMgr.h"
#include "LdLogicClass.h"
#include "LdLogicGroup.h"
#include "ym_logic/NpnMgr.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラス LdLogicMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LdLogicMgr::LdLogicMgr()
{
}

// @brief デストラクタ
LdLogicMgr::~LdLogicMgr()
{
  init();
}

// @brief 初期化する．
// 定数関数，リテラル関数を登録しておく
// 常に定数0，定数1，肯定リテラル，否定リテラルの関数番号が
// 0, 1, 2, 3 になるようにする．
void
LdLogicMgr::init()
{
  for (vector<LdLogicGroup*>::iterator p = mGroupList.begin();
       p != mGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LdLogicClass*>::iterator p = mClassList.begin();
       p != mClassList.end(); ++ p) {
    delete *p;
  }
  mGroupList.clear();
  mLogicMap.clear();
  mClassList.clear();
  mClassMap.clear();

  // 既定関数の登録
  {
    TvFunc const0 = TvFunc::const_zero(0);
    LdLogicGroup* func0 = find_group(const0);
    assert_cond( func0->id() == 0, __FILE__, __LINE__);

    TvFunc const1 = TvFunc::const_one(0);
    LdLogicGroup* func1 = find_group(const1);
    assert_cond( func1->id() == 1, __FILE__, __LINE__);

    TvFunc plit = TvFunc::posi_literal(1, 0);
    LdLogicGroup* func2 = find_group(plit);
    assert_cond( func2->id() == 2, __FILE__, __LINE__);

    TvFunc nlit = TvFunc::nega_literal(1, 0);
    LdLogicGroup* func3 = find_group(nlit);
    assert_cond( func3->id() == 3, __FILE__, __LINE__);
  }
}

// @brief f に対応する LdLogic を求める．
// @param[in] f 関数
// @note なければ新規に作る．
LdLogicGroup*
LdLogicMgr::find_group(const TvFunc& f)
{
  LdLogicGroup* pgfunc = NULL;
  hash_map<TvFunc, LdLogicGroup*>::iterator p = mLogicMap.find(f);
  if ( p == mLogicMap.end() ) {
    // なかったので新たに作る．
    pgfunc = new LdLogicGroup;
    pgfunc->mId = mGroupList.size();
    mGroupList.push_back(pgfunc);
    mLogicMap.insert(make_pair(f, pgfunc));

    // 代表関数を求める．
    NpnMgr npnmgr;
    npnmgr.cannonical(f, pgfunc->mMap);

    TvFunc repfunc = f.xform(pgfunc->mMap);
    LdLogicClass* pgrep = find_class(repfunc);

    // 関数を追加する．
    pgrep->mGroupList.push_back(pgfunc);
    pgfunc->mRep = pgrep;
  }
  else {
    // 既に登録されていた．
    pgfunc = p->second;
  }
  return pgfunc;
}

// @brief f に対応する LdLogicClass を求める．
// @param[in] f 関数
// @note なければ新規に作る．
// @note f は NpnMgr によって正規化されている必要がある．
LdLogicClass*
LdLogicMgr::find_class(const TvFunc& f)
{
  LdLogicClass* pgrep = NULL;
  hash_map<TvFunc, LdLogicClass*>::iterator p = mClassMap.find(f);
  if ( p == mClassMap.end() ) {
    // まだ登録されていない．
    pgrep = new LdLogicClass;
    pgrep->mId = mClassList.size();
    pgrep->mFunc = f;
    mClassList.push_back(pgrep);
    mClassMap.insert(make_pair(f, pgrep));
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
LdLogicMgr::dump(ostream& s) const
{
  // 論理グループの情報をダンプする．
  ymuint ng = logic_group_num();
  BinIO::write_32(s, ng);
  for (ymuint i = 0; i < ng; ++ i) {
    const LdLogicGroup* group = logic_group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);
    // 論理クラスに対する変換マップをダンプする．
    dump_map(s, group->map());
    // 属しているセル番号をダンプする．
    const vector<ymuint>& cell_list = group->cell_list();
    ymuint nc = cell_list.size();
    BinIO::write_32(s, nc);
    for (ymuint i = 0; i < nc; ++ i) {
      BinIO::write_32(s, cell_list[i]);
    }
  }

  // 論理クラスの情報をダンプする．
  ymuint nc = logic_class_num();
  BinIO::write_32(s, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const LdLogicClass* rep = logic_class(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    ymuint ng = rep->group_num();
    BinIO::write_32(s, ng);
    for (ymuint j = 0; j < ng; ++ j) {
      BinIO::write_32(s, rep->group(j)->id());
    }
  }
}

// @brief 内容を出力する．(デバッグ用)
// @param[in] s 出力先のストリーム
void
LdLogicMgr::display(ostream& s) const
{
  s << "*** LdLogicMgr BEGIN ***" << endl;
  s << "*** LOGIC GROUP SECTION ***" << endl;
  for (ymuint i = 0; i < logic_group_num(); ++ i) {
    const LdLogicGroup* group = logic_group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);
    s << "GROUP#" << i
      << ": CLASS#" << group->rep()->id()
      << ": " << group->map()
      << endl;
    s << "  CELL#ID" << endl;
    const vector<ymuint>& cell_list = group->cell_list();
    for (vector<ymuint>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      s << "    " << *p << endl;
    }
  }
  s << endl;

  s << "*** LOGIC CLASS SECTION ***" << endl;
  for (ymuint i = 0; i < logic_class_num(); ++ i) {
    const LdLogicClass* rep = logic_class(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "CLASS#" << i << ": ";
    rep->rep_func().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    for (ymuint j = 0; j < rep->group_num(); ++ j) {
      s << " GROUP#" << rep->group(j)->id();
    }
    s << endl;
  }
  s << endl;
  s << "*** LdLogicMgr END ***" << endl;
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
LdLogicMgr::dump_map(ostream& s,
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
