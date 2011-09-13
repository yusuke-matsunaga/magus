
/// @file LcGroupMgr.cc
/// @brief LcGroupMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcGroupMgr.h"
#include "LcClass.h"
#include "LcGroup.h"
#include "ym_cell/Cell.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcGroupMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcGroupMgr::LcGroupMgr()
{
}

// @brief デストラクタ
LcGroupMgr::~LcGroupMgr()
{
  clear();
}

// @brief 内容を空にする．
void
LcGroupMgr::clear()
{
  for (vector<LcGroup*>::iterator p = mGroupList.begin();
       p != mGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LcClass*>::iterator p = mClassList.begin();
       p != mClassList.end(); ++ p) {
    delete *p;
  }
  mGroupList.clear();
  mGroupMap.clear();
  mClassList.clear();
  mClassMap.clear();
}

// @brief セルを追加する．
void
LcGroupMgr::add_cell(const Cell* cell)
{
  // セルのシグネチャ関数を作る．
  TvFuncM f;
  gen_signature(cell, f);

  // f に対するセルグループを求める．
  LcGroup* fgroup = find_group(f);

  // セル(番号)を追加する．
  fgroup->add_cell(cell);
}

// @brief f に対応する LcGroup を求める．
// @param[in] f 関数
// @note なければ新規に作る．
LcGroup*
LcGroupMgr::find_group(const TvFuncM& f)
{
  LcGroup* fgroup = NULL;
  hash_map<TvFuncM, ymuint>::iterator p = mGroupMap.find(f);
  if ( p == mGroupMap.end() ) {
    // なかったので新たに作る．
    ymuint new_id = mGroupList.size();
    fgroup = new LcGroup(new_id);
    mGroupList.push_back(fgroup);
    mGroupMap.insert(make_pair(f, new_id));

    // 代表関数を求める．
    TvFuncM repfunc;
    NpnMapM xmap;
    find_repfunc(f, repfunc, xmap);

    LcClass* fclass = NULL;
    hash_map<TvFuncM, ymuint>::iterator p = mClassMap.find(repfunc);
    if ( p == mClassMap.end() ) {
      // まだ登録されていない．
      ymuint new_id = mClassList.size();
      fclass = new LcClass(new_id, repfunc);
      mClassList.push_back(fclass);
      mClassMap.insert(make_pair(repfunc, new_id));
    }
    else {
      // 登録されていた．
      fclass = mClassList[p->second];
    }

    // グループを追加する．
    fclass->add_group(fgroup, xmap);
  }
  else {
    // 既に登録されていた．
    fgroup = mGroupList[p->second];
  }

  return fgroup;
}

// @brief セルグループの数を返す．
ymuint
LcGroupMgr::group_num() const
{
  return mGroupList.size();
}

// @brief セルグループを返す．
// @param[in] id グループ番号 ( 0 <= id < group_num() )
const LcGroup*
LcGroupMgr::group(ymuint id) const
{
  return mGroupList[id];
}

// @brief NPN同値クラスの数を返す．
ymuint
LcGroupMgr::npn_class_num() const
{
  return mClassList.size();
}

// @brief NPN同値クラスを返す．
// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
const LcClass*
LcGroupMgr::npn_class(ymuint id) const
{
  return mClassList[id];
}

// @brief 内容をバイナリダンプする．
// @param[in] bos 出力先のストリーム
void
LcGroupMgr::dump(BinO& bos) const
{
  // セルグループの情報をダンプする．
  ymuint32 ng = group_num();
  bos << ng;
  for (ymuint i = 0; i < ng; ++ i) {
    const LcGroup* group = this->group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);

    // 論理クラスに対する変換マップをダンプする．
    const NpnMapM& map = group->map();
    ymuint32 ni = map.ni();
    bos << ni;
    for (ymuint i = 0; i < ni; ++ i) {
      NpnVmap imap = map.imap(i);
      // 手抜きでは imap を ymuint32 にキャストすればよい．
      ymuint j = imap.pos();
      ymuint32 v = (j << 1);
      if ( imap.pol() == kPolNega ) {
	v |= 1U;
      }
      bos << v;
    }
    ymuint32 no = map.no();
    bos << no;
    for (ymuint i = 0; i < no; ++ i) {
      NpnVmap omap = map.omap(i);
      ymuint j = omap.pos();
      ymuint32 v = (j << 1);
      if ( omap.pol() == kPolNega ) {
	v |= 1U;
      }
      bos << v;
    }

    // 属しているセル番号をダンプする．
    const vector<const Cell*>& cell_list = group->cell_list();
    ymuint32 nc = cell_list.size();
    bos << nc;
    for (ymuint i = 0; i < nc; ++ i) {
      const Cell* cell = cell_list[i];
      bos << static_cast<ymuint32>(cell->id());
    }
  }

  // NPN同値クラスの情報をダンプする．
  ymuint32 nc = npn_class_num();
  bos << nc;
  for (ymuint i = 0; i < nc; ++ i) {
    const LcClass* rep = npn_class(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    const vector<LcGroup*>& group_list = rep->group_list();
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
LcGroupMgr::display(ostream& s) const
{
  s << "*** LcGroupMgr BEGIN ***" << endl;
  s << "*** CELL GROUP SECTION ***" << endl;
  for (ymuint i = 0; i < group_num(); ++ i) {
    const LcGroup* group = this->group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);
    s << "GROUP#" << i
      << ": CLASS#" << group->parent()->id()
      << ": " << group->map()
      << endl;
    s << "  CELL#ID" << endl;
    const vector<const Cell*>& cell_list = group->cell_list();
    for (vector<const Cell*>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      const Cell* cell = *p;
      s << "    " << cell->name() << endl;
    }
  }
  s << endl;

  s << "*** NPN CLASS SECTION ***" << endl;
  for (ymuint i = 0; i < npn_class_num(); ++ i) {
    const LcClass* rep = npn_class(i);
    assert_cond( rep->id() == i , __FILE__, __LINE__);
    s << "CLASS#" << i << ": ";
    rep->repfunc().dump(s, 2);
    s << endl;
    s << "  equivalence = ";
    const vector<LcGroup*>& group_list = rep->group_list();
    for (ymuint j = 0; j < group_list.size(); ++ j) {
	s << " GROUP#" << group_list[j]->id();
    }
    s << endl;
  }
  s << endl;
  s << "*** LcGroupMgr END ***" << endl;
}

END_NAMESPACE_YM_CELL_LIBCOMP
