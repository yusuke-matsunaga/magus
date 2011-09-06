
/// @file LdLogicMgr.cc
/// @brief LdLogicMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdLogicMgr.h"
#include "LdClass.h"
#include "LdGroup.h"
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
  for (vector<LdGroup*>::iterator p = mLogicGroupList.begin();
       p != mLogicGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LdClass*>::iterator p = mLogicClassList.begin();
       p != mLogicClassList.end(); ++ p) {
    delete *p;
  }
  mLogicGroupList.clear();
  mLogicGroupMap1.clear();
  mLogicGroupMap2.clear();
  mLogicClassList.clear();
  mLogicClassMap1.clear();
  mLogicClassMap2.clear();

  // 既定関数の登録
  {
    TvFunc const0 = TvFunc::const_zero(0);
    LdGroup* func0 = find_logic_group(const0);
    assert_cond( func0->id() == 0, __FILE__, __LINE__);

    TvFunc const1 = TvFunc::const_one(0);
    LdGroup* func1 = find_logic_group(const1);
    assert_cond( func1->id() == 1, __FILE__, __LINE__);

    TvFunc plit = TvFunc::posi_literal(1, 0);
    LdGroup* func2 = find_logic_group(plit);
    assert_cond( func2->id() == 2, __FILE__, __LINE__);

    TvFunc nlit = TvFunc::nega_literal(1, 0);
    LdGroup* func3 = find_logic_group(nlit);
    assert_cond( func3->id() == 3, __FILE__, __LINE__);
  }
}

// @brief f に対応する LdLogic を求める．
// @param[in] f 関数
// @note なければ新規に作る．
LdGroup*
LdLogicMgr::find_logic_group(const TvFunc& f)
{
  LdGroup* pgfunc = NULL;
  hash_map<TvFunc, ymuint>::iterator p = mLogicGroupMap1.find(f);
  if ( p == mLogicGroupMap1.end() ) {
    // なかったので新たに作る．
    ymuint new_id = mLogicGroupList.size();
    pgfunc = new LdGroup(new_id);
    mLogicGroupList.push_back(pgfunc);
    mLogicGroupMap1.insert(make_pair(f, new_id));

    // 代表関数を求める．
    NpnMgr npnmgr;
    NpnMap xmap;
    npnmgr.cannonical(f, xmap);

    TvFunc repfunc = f.xform(xmap);
    LdClass* pgrep = NULL;
    hash_map<TvFunc, ymuint>::iterator p = mLogicClassMap1.find(repfunc);
    if ( p == mLogicClassMap1.end() ) {
      // まだ登録されていない．
      ymuint new_id = mLogicClassList.size();
      pgrep = new LdClass(new_id, repfunc);
      mLogicClassList.push_back(pgrep);
      mLogicClassMap1.insert(make_pair(repfunc, new_id));
    }
    else {
      // 登録されていた．
      pgrep = mLogicClassList[p->second];
    }

    // グループを追加する．
    pgrep->add_group(pgfunc, NpnMapM(xmap));
  }
  else {
    // 既に登録されていた．
    pgfunc = mLogicGroupList[p->second];
  }
  return pgfunc;
}

// @brief f_list に対応する LdGroup を求める．
// @param[in] f_list 関数のリスト
// @note なければ新規に作る．
LdGroup*
LdLogicMgr::find_logic_group(const vector<TvFunc>& f_list)
{
  TvFuncM f(f_list);
  LdGroup* pgfunc = NULL;
  hash_map<TvFuncM, ymuint>::iterator p = mLogicGroupMap2.find(f);
  if ( p == mLogicGroupMap2.end() ) {
    // なかったので新たに作る．
    ymuint new_id = mLogicGroupList.size();
    pgfunc = new LdGroup(new_id);
    mLogicGroupList.push_back(pgfunc);
    mLogicGroupMap2.insert(make_pair(f, new_id));

    // 代表関数を求める．
    // 今は手抜きで多出力はすべてが代表関数となる．
    NpnMapM xmap;
    xmap.set_identity(f.ni(), f.no());
    LdClass* pgrep = NULL;
    hash_map<TvFuncM, ymuint>::iterator p = mLogicClassMap2.find(f);
    if ( p == mLogicClassMap2.end() ) {
      // まだ登録されていない．
      ymuint new_id = mLogicClassList.size();
      pgrep = new LdClass(new_id, f);
      mLogicClassList.push_back(pgrep);
      mLogicClassMap2.insert(make_pair(f, new_id));
    }
    else {
      // 登録されていた．
      pgrep = mLogicClassList[p->second];
    }

    // 関数を追加する．
    pgrep->add_group(pgfunc, xmap);
  }
  else {
    // 既に登録されていた．
    pgfunc = mLogicGroupList[p->second];
  }
  return pgfunc;
}

// @brief 論理グループの数を返す．
ymuint
LdLogicMgr::logic_group_num() const
{
  return mLogicGroupList.size();
}

// @brief 論理グループを返す．
// @param[in] id グループ番号 ( 0 <= id < logic_group_num() )
const LdGroup*
LdLogicMgr::logic_group(ymuint id) const
{
  return mLogicGroupList[id];
}

// @brief 論理クラスの数を返す．
ymuint
LdLogicMgr::logic_class_num() const
{
  return mLogicClassList.size();
}

// @brief 論理クラスを返す．
// @param[in] id クラス番号 ( 0 <= id < logic_class_num() )
const LdClass*
LdLogicMgr::logic_class(ymuint id) const
{
  return mLogicClassList[id];
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
    const LdGroup* group = logic_group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);

    // 論理クラスに対する変換マップをダンプする．
    const NpnMapM& map = group->map();
    ymuint ni = map.ni();
    BinIO::write_32(s, ni);
    for (ymuint i = 0; i < ni; ++ i) {
      NpnVmap imap = map.imap(i);
      // 手抜きでは imap を ymuint32 にキャストすればよい．
      ymuint j = imap.pos();
      ymuint32 v = (j << 1);
      if ( imap.pol() == kPolNega ) {
	v |= 1U;
      }
      BinIO::write_32(s, v);
    }
    ymuint no = map.no();
    BinIO::write_32(s, no);
    for (ymuint i = 0; i < no; ++ i) {
      NpnVmap omap = map.omap(i);
      ymuint j = omap.pos();
      ymuint32 v = (j << 1);
      if ( omap.pol() == kPolNega ) {
	v |= 1U;
      }
      BinIO::write_32(s, v);
    }

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
    const LdClass* rep = logic_class(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    const vector<LdGroup*>& group_list = rep->group_list();
    ymuint ng = group_list.size();
    BinIO::write_32(s, ng);
    for (ymuint j = 0; j < ng; ++ j) {
      BinIO::write_32(s, group_list[j]->id());
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
    const LdGroup* group = logic_group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);
    s << "GROUP#" << i
      << ": CLASS#" << group->parent()->id()
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
    const LdClass* rep = logic_class(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "CLASS#" << i << ": ";
    rep->repfunc().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    const vector<LdGroup*>& group_list = rep->group_list();
    for (ymuint j = 0; j < group_list.size(); ++ j) {
	s << " GROUP#" << group_list[j]->id();
    }
    s << endl;
  }
  s << endl;
  s << "*** LdLogicMgr END ***" << endl;
}

END_NAMESPACE_YM_CELL_LIBDUMP
