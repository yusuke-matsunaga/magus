
/// @file LcFFMgr.cc
/// @brief LcFFMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcFFMgr.h"
#include "LcGroup.h"
#include "LcClass.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラスLcFFMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcFFMgr::LcFFMgr()
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
  for (vector<LcGroup*>::iterator p = mFFGroupList.begin();
       p != mFFGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LcClass*>::iterator p = mFFClassList.begin();
       p != mFFClassList.end(); ++ p) {
    delete *p;
  }
  mFFGroupList.clear();
  mFFGroupMap.clear();
  mFFClassList.clear();
  mFFClassMap.clear();
}

// @brief 対応する LcGroup を求める．
// @param[in] f_array 関数の配列
// @note なければ新規に作る．
LcGroup*
LcFFMgr::find_ff_group(const vector<TvFunc>& f_array)
{
  TvFuncM f(f_array);
  LcGroup* pgfunc = NULL;
  hash_map<TvFuncM, ymuint>::iterator p = mFFGroupMap.find(f);
  if ( p == mFFGroupMap.end() ) {
    // なかったので新たに作る．
    ymuint new_id = mFFGroupList.size();
    pgfunc = new LcGroup(new_id);
    mFFGroupList.push_back(pgfunc);
    mFFGroupMap.insert(make_pair(f, new_id));

    // 代表関数を求める．
    // 今は手抜きで多出力はすべてが代表関数となる．
    NpnMapM xmap;
    xmap.set_identity(f.ni(), f.no());
    LcClass* pgrep = NULL;
    hash_map<TvFuncM, ymuint>::iterator p = mFFClassMap.find(f);
    if ( p == mFFClassMap.end() ) {
      // まだ登録されていない．
      ymuint new_id = mFFClassList.size();
      pgrep = new LcClass(new_id, f);
      mFFClassList.push_back(pgrep);
      mFFClassMap.insert(make_pair(f, new_id));
    }
    else {
      // 登録されていた．
      pgrep = mFFClassList[p->second];
    }

    // 関数を追加する．
    pgrep->add_group(pgfunc, xmap);
  }
  else {
    // 既に登録されていた．
    pgfunc = mFFGroupList[p->second];
  }
  return pgfunc;
}

// @brief 内容をバイナリダンプする．
// @param[in] bos 出力先のストリーム
void
LcFFMgr::dump(BinO& bos) const
{
  // FFグループの情報をダンプする．
  ymuint32 ng = mFFGroupList.size();
  bos << ng;
  for (ymuint i = 0; i < ng; ++ i) {
    const LcGroup* ff_group = mFFGroupList[i];
    assert_cond( ff_group->id() == i, __FILE__, __LINE__);
#if 0
    // ピン位置の情報をダンプする．
    BinIO::write_32(s, ff_group->signature());
#endif
    // 属しているセル番号をダンプする．
    const vector<ymuint>& cell_list = ff_group->cell_list();
    ymuint32 nc = cell_list.size();
    bos << nc;
    for (ymuint j = 0; j < nc; ++ j) {
      bos << static_cast<ymuint32>(cell_list[j]);
    }
  }

  // FFクラスの情報をダンプする．
  ymuint32 nc = mFFClassList.size();
  bos << nc;
  for (ymuint i = 0; i < nc; ++ i) {
    const LcClass* ff_class = mFFClassList[i];
    assert_cond( ff_class->id() == i, __FILE__, __LINE__);
#if 0
    // 入力のタイプ情報をダンプする．
    BinIO::write_8(s, ff_class->signature());
#endif
    // 属しているFFグループ番号をダンプする．
    const vector<LcGroup*>& group_list = ff_class->group_list();
    ymuint32 ng = group_list.size();
    bos << ng;
    for (ymuint j = 0; j < ng; ++ j) {
      bos << static_cast<ymuint32>(group_list[j]->id());
    }
  }
}

// @brief 内容を出力する．(デバッグ用)
// @param[in] s 出力先のストリーム
void
LcFFMgr::display(ostream& s) const
{
  s << "*** LcFFMgr BEGIN ***" << endl;
  s << "*** FF Group SECTION ***" << endl;
  for (vector<LcGroup*>::const_iterator p = mFFGroupList.begin();
       p != mFFGroupList.end(); ++ p) {
    const LcGroup* ff_group = *p;
    const LcClass* ff_class = ff_group->parent();
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
  for (vector<LcClass*>::const_iterator p = mFFClassList.begin();
       p != mFFClassList.end(); ++ p) {
    const LcClass* ff_class = *p;
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
    const vector<LcGroup*>& g_list = ff_class->group_list();
    for (vector<LcGroup*>::const_iterator q = g_list.begin();
	 q != g_list.end(); ++ q) {
      const LcGroup* ff_group = *q;
      s << " GROUP#" << ff_group->id();
    }
    s << endl;
    s << endl;
  }
  s << endl;
  s << "*** LcFFMgr END ***" << endl;
}

END_NAMESPACE_YM_CELL_LIBCOMP
