
/// @file LdFFMgr.cc
/// @brief LdFFMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdFFMgr.h"
#include "LdGroup.h"
#include "LdClass.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラスLdFFMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LdFFMgr::LdFFMgr()
{
}

// @brief デストラクタ
LdFFMgr::~LdFFMgr()
{
}

// @brief 初期化する．
void
LdFFMgr::init()
{
  for (vector<LdGroup*>::iterator p = mFFGroupList.begin();
       p != mFFGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LdClass*>::iterator p = mFFClassList.begin();
       p != mFFClassList.end(); ++ p) {
    delete *p;
  }
  mFFGroupList.clear();
  mFFGroupMap.clear();
  mFFClassList.clear();
  mFFClassMap.clear();
}

// @brief 対応する LdGroup を求める．
// @param[in] f_array 関数の配列
// @note なければ新規に作る．
LdGroup*
LdFFMgr::find_ff_group(const vector<TvFunc>& f_array)
{
  TvFuncM f(f_array);
  LdGroup* pgfunc = NULL;
  hash_map<TvFuncM, ymuint>::iterator p = mFFGroupMap.find(f);
  if ( p == mFFGroupMap.end() ) {
    // なかったので新たに作る．
    ymuint new_id = mFFGroupList.size();
    pgfunc = new LdGroup(new_id);
    mFFGroupList.push_back(pgfunc);
    mFFGroupMap.insert(make_pair(f, new_id));

    // 代表関数を求める．
    // 今は手抜きで多出力はすべてが代表関数となる．
    LdClass* pgrep = NULL;
    hash_map<TvFuncM, ymuint>::iterator p = mFFClassMap.find(f);
    if ( p == mFFClassMap.end() ) {
      // まだ登録されていない．
      ymuint new_id = mFFClassList.size();
      pgrep = new LdClass(new_id, f);
      mFFClassList.push_back(pgrep);
      mFFClassMap.insert(make_pair(f, new_id));
    }
    else {
      // 登録されていた．
      pgrep = mFFClassList[p->second];
    }

    // 関数を追加する．
    pgrep->add_group(pgfunc);
  }
  else {
    // 既に登録されていた．
    pgfunc = mFFGroupList[p->second];
  }
  return pgfunc;
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
LdFFMgr::dump(ostream& s) const
{
  // FFグループの情報をダンプする．
  ymuint ng = mFFGroupList.size();
  BinIO::write_32(s, ng);
  for (ymuint i = 0; i < ng; ++ i) {
    const LdGroup* ff_group = mFFGroupList[i];
    assert_cond( ff_group->id() == i, __FILE__, __LINE__);
#if 0
    // ピン位置の情報をダンプする．
    BinIO::write_32(s, ff_group->signature());
#endif
    // 属しているセル番号をダンプする．
    const vector<ymuint>& cell_list = ff_group->cell_list();
    ymuint nc = cell_list.size();
    BinIO::write_32(s, nc);
    for (ymuint j = 0; j < nc; ++ j) {
      BinIO::write_32(s, cell_list[j]);
    }
  }

  // FFクラスの情報をダンプする．
  ymuint nc = mFFClassList.size();
  BinIO::write_32(s, nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const LdClass* ff_class = mFFClassList[i];
    assert_cond( ff_class->id() == i, __FILE__, __LINE__);
#if 0
    // 入力のタイプ情報をダンプする．
    BinIO::write_8(s, ff_class->signature());
#endif
    // 属しているFFグループ番号をダンプする．
    const vector<LdGroup*>& group_list = ff_class->group_list();
    ymuint ng = group_list.size();
    BinIO::write_32(s, ng);
    for (ymuint j = 0; j < ng; ++ j) {
      const LdGroup* ff_group = group_list[j];
      BinIO::write_32(s, ff_group->id());
    }
  }
}

// @brief 内容を出力する．(デバッグ用)
// @param[in] s 出力先のストリーム
void
LdFFMgr::display(ostream& s) const
{
  s << "*** LdFFMgr BEGIN ***" << endl;
  s << "*** FF Group SECTION ***" << endl;
  for (vector<LdGroup*>::const_iterator p = mFFGroupList.begin();
       p != mFFGroupList.end(); ++ p) {
    const LdGroup* ff_group = *p;
    const LdClass* ff_class = ff_group->parent();
    s << "FF GROUP#" << ff_group->id()
      << ": CLASS#" << ff_class->id()
      << endl;
#if 0
      << ": SIGNATURE = " << hex << ff_group->signature() << dec << endl
      << "  DATA = Pin#" << ff_group->data_pos();
    s << ", CLOCK = ";
    if ( ff_class->clock_sense() == 2 ) {
      s << "!";
    }
    s << "Pin#" << ff_group->clock_pos();
    s << ", Q = " << ff_group->q_pos()
      << ", IQ = " << ff_group->iq_pos();
    if ( ff_class->clear_sense() != 0 ) {
      s << ", CLEAR = ";
      if ( ff_class->clear_sense() == 2 ) {
	s << "!";
      }
      s << "Pin#" << ff_group->clear_pos();
    }
    if ( ff_class->preset_sense() != 0 ) {
      s << ", PRESET = ";
      if ( ff_class->preset_sense() == 2 ) {
	s << "!";
      }
      s << "Pin#" << ff_group->preset_pos();
    }
#endif
    s << endl;
    s << "  CELLS = ";
    const vector<ymuint>& cell_list = ff_group->cell_list();
    for (vector<ymuint>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      s << " CELL#" << *p;
    }
    s << endl;
    s << endl;
  }
  s << endl;

  s << "*** FF Class SECTION ***" << endl;
  for (vector<LdClass*>::const_iterator p = mFFClassList.begin();
       p != mFFClassList.end(); ++ p) {
    const LdClass* ff_class = *p;
    s << "FF CLASS#" << ff_class->id()
      << endl;
#if 0
      << ": SIGNATURE = " << hex << ff_class->signature() << dec << endl;
    s << "  CLOCK: ";
    if ( ff_class->clock_sense() == 1 ) {
      s << "POSITIVE";
    }
    else {
      s << "NEGATIVE";
    }
    s << endl;

    ymuint r0 = ff_class->clear_sense();
    if ( r0 != 0 ) {
      s << "  CLEAR: ";
      if ( r0 == 1 ) {
	s << "HIGH";
      }
      else {
	s << "LOW";
      }
      s << endl;
    }

    ymuint r1 = ff_class->preset_sense();
    if ( r1 != 0 ) {
      s << "  PRESET: ";
      if ( r1 == 1 ) {
	s << "HIGH";
      }
      else {
	s << "LOW";
      }
      s << endl;
    }
#endif
    s << "  GROUPS = ";
    const vector<LdGroup*>& g_list = ff_class->group_list();
    for (vector<LdGroup*>::const_iterator q = g_list.begin();
	 q != g_list.end(); ++ q) {
      const LdGroup* ff_group = *q;
      s << " GROUP#" << ff_group->id();
    }
    s << endl;
    s << endl;
  }
  s << endl;
  s << "*** LdFFMgr END ***" << endl;
}

END_NAMESPACE_YM_CELL_LIBDUMP
