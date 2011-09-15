
/// @file LcGroupMgr.cc
/// @brief LcGroupMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcGroupMgr.h"
#include "LcClass.h"
#include "LcGroup.h"
#include "LibComp.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcGroupMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] libcomp 親の LibComp
LcGroupMgr::LcGroupMgr(LibComp& libcomp) :
  mLibComp(libcomp)
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
  mGroupMap.clear();
  mClassMap.clear();
}

// @brief セルを追加する．
void
LcGroupMgr::add_cell(const Cell* cell)
{
  if ( !cell->has_logic() || cell->output_num2() == 0 ) {
    // ひとつでも論理式を持たない出力があるセルは独立したグループとなる．
    LcGroup* fgroup = mLibComp.new_group();
    fgroup->add_cell(cell);

    ymuint ni = cell->input_num2();
    ymuint no = cell->output_num2();
    TvFuncM repfunc(ni, no);
    LcClass* fclass = mLibComp.new_class(repfunc);
    NpnMapM xmap;
    xmap.set_identity(ni, no);
    fclass->add_group(fgroup, xmap);
  }
  else {
    // セルのシグネチャ関数を作る．
    TvFuncM f;
    gen_signature(cell, f);

    // f に対するセルグループを求める．
    LcGroup* fgroup = find_group(f);

    // セル(番号)を追加する．
    fgroup->add_cell(cell);
  }
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
    fgroup = mLibComp.new_group();
    mGroupMap.insert(make_pair(f, fgroup->id()));

    // 代表関数を求める．
    TvFuncM repfunc;
    NpnMapM xmap;
    find_repfunc(f, repfunc, xmap);

    LcClass* fclass = NULL;
    hash_map<TvFuncM, ymuint>::iterator q = mClassMap.find(repfunc);
    if ( q == mClassMap.end() ) {
      // まだ登録されていない．
      fclass = mLibComp.new_class(repfunc);
      mClassMap.insert(make_pair(repfunc, fclass->id()));
    }
    else {
      // 登録されていた．
      fclass = mLibComp.npn_class(q->second);
    }

    // グループを追加する．
    fclass->add_group(fgroup, xmap);
  }
  else {
    // 既に登録されていた．
    fgroup = mLibComp.group(p->second);
  }

  return fgroup;
}

#if 0
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
#endif

END_NAMESPACE_YM_CELL_LIBCOMP
