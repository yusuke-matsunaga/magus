
/// @file LdFFMgr.cc
/// @brief LdFFMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdFFMgr.h"
#include "LdFFGroup.h"
#include "LdFFClass.h"
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
  for (vector<LdFFGroup*>::iterator p = mFFGroupList.begin();
       p != mFFGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LdFFClass*>::iterator p = mFFClassList.begin();
       p != mFFClassList.end(); ++ p) {
    delete *p;
  }
  mFFGroupList.clear();
  mFFClassList.clear();
}

// @brief 対応する LdFFGroup を求める．
// @param[in] f 関数
// @note なければ新規に作る．
LdFFGroup*
LdFFMgr::find_group(ymuint clock_sense,
		    ymuint clear_sense,
		    ymuint preset_sense,
		    ymuint data_pos,
		    ymuint clock_pos,
		    ymuint clear_pos,
		    ymuint preset_pos,
		    ymuint q_pos,
		    ymuint iq_pos)
{
  ymuint sig = (clock_sense << 0) | (clear_sense << 2) | (preset_sense << 4);
  ymuint n = mFFClassList.size();
  LdFFClass* ff_class = NULL;
  for (ymuint i = 0; i < n; ++ i) {
    LdFFClass* ff_class1 = mFFClassList[i];
    if ( ff_class1->signature() == sig ) {
      ff_class = ff_class1;
      break;
    }
  }
  if ( ff_class == NULL ) {
    ff_class = new LdFFClass;
    ff_class->mId = mFFClassList.size();
    mFFClassList.push_back(ff_class);
    ff_class->mBits = sig;
  }

  CellFFPosArray sig2;
  sig2.set(data_pos,
	   clock_pos, clock_sense,
	   clear_pos, clear_sense,
	   preset_pos, preset_sense,
	   q_pos, iq_pos);

  LdFFGroup* ff_group = NULL;
  for (vector<LdFFGroup*>::iterator p = ff_class->mGroupList.begin();
       p != ff_class->mGroupList.end(); ++ p) {
    LdFFGroup* ff_group1 = *p;
    if ( ff_group1->signature() == sig2.signature() ) {
      ff_group = ff_group1;
      break;
    }
  }
  if ( ff_group == NULL ) {
    ff_group = new LdFFGroup;
    ff_group->mId = mFFGroupList.size();
    mFFGroupList.push_back(ff_group);
    ff_class->mGroupList.push_back(ff_group);
    ff_group->mParent = ff_class;
    ff_group->mPosArray = sig2;
  }

  return ff_group;
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
    const LdFFGroup* ff_group = mFFGroupList[i];
    assert_cond( ff_group->id() == i, __FILE__, __LINE__);
    // ピン位置の情報をダンプする．
    BinIO::write_32(s, ff_group->signature());
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
    const LdFFClass* ff_class = mFFClassList[i];
    assert_cond( ff_class->id() == i, __FILE__, __LINE__);
    // 入力のタイプ情報をダンプする．
    BinIO::write_8(s, ff_class->signature());
    // 属しているFFグループ番号をダンプする．
    const vector<LdFFGroup*>& group_list = ff_class->group_list();
    ymuint ng = group_list.size();
    BinIO::write_32(s, ng);
    for (ymuint j = 0; j < ng; ++ j) {
      const LdFFGroup* ff_group = group_list[j];
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
  for (vector<LdFFGroup*>::const_iterator p = mFFGroupList.begin();
       p != mFFGroupList.end(); ++ p) {
    const LdFFGroup* ff_group = *p;
    const LdFFClass* ff_class = ff_group->parent();
    s << "FF GROUP#" << ff_group->id()
      << ": CLASS#" << ff_class->id()
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
  for (vector<LdFFClass*>::const_iterator p = mFFClassList.begin();
       p != mFFClassList.end(); ++ p) {
    const LdFFClass* ff_class = *p;
    s << "FF CLASS#" << ff_class->id()
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

    s << "  GROUPS = ";
    const vector<LdFFGroup*>& g_list = ff_class->group_list();
    for (vector<LdFFGroup*>::const_iterator q = g_list.begin();
	 q != g_list.end(); ++ q) {
      const LdFFGroup* ff_group = *q;
      s << " GROUP#" << ff_group->id();
    }
    s << endl;
    s << endl;
  }
  s << endl;
  s << "*** LdFFMgr END ***" << endl;
}

END_NAMESPACE_YM_CELL_LIBDUMP
