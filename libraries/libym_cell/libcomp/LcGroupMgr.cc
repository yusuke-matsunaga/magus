
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
#include "ym_utils/MFSet.h"
#include "ym_utils/PermGen.h"


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
    LcClass* fclass = mLibComp.new_class(repfunc, false);
    NpnMapM xmap;
    xmap.set_identity(ni, no);
    fclass->add_group(fgroup, xmap);
  }
  else {
    // セルのシグネチャ関数を作る．
    TvFuncM f;
    gen_signature(cell, f);

    // f に対するセルグループを求める．
    LcGroup* fgroup = find_group(f, false);

    // セル(番号)を追加する．
    fgroup->add_cell(cell);
  }
}

// @brief f に対応する LcGroup を求める．
// @param[in] f 関数
// @param[in] builtin 組み込みクラスの時 true にするフラグ
// @note なければ新規に作る．
LcGroup*
LcGroupMgr::find_group(const TvFuncM& f,
		       bool builtin)
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
      fclass = mLibComp.new_class(repfunc, builtin);
      mClassMap.insert(make_pair(repfunc, fclass->id()));
      find_idmap_list(repfunc, fclass->mIdmapList);
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


BEGIN_NONAMESPACE

// サポートを表すビットベクタを作る．
ymuint32
gen_support(const TvFunc& f)
{
  ymuint ni = f.ni();
  ymuint32 ans = 0U;
  for (ymuint i = 0; i < ni; ++ i) {
    VarId var(i);
    if ( f.check_sup(var) ) {
      ans |= (1U << i);
    }
  }
  return ans;
}

// f を最大化する変換を求める．
ymuint
gen_maxmap(const TvFuncM& f,
	   ymuint offset,
	   NpnMapM& xmap)
{
  ymuint ni = f.ni();
  ymuint no = f.no();

  vector<VarId> i_list;
  i_list.reserve(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    VarId var(i);
    if ( f.check_sup(var) ) {
      i_list.push_back(var);
    }
  }
  ymuint ni1 = i_list.size();
  if ( ni1 == 0 ) {
    return 0;
  }

  NpnMapM map;
  bool first = true;
  TvFuncM repfunc;
  PermGen pg(ni1, ni1);
  ymuint nip = 1U << ni1;
  for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
    NpnMapM map1(ni, no);
    for (ymuint i = 0; i < no; ++ i) {
      map1.set_omap(VarId(i), VarId(i), kPolPosi);
    }
    for (ymuint x = 0U; x < nip; ++ x) {
      for (ymuint i = 0; i < ni1; ++ i) {
	tPol pol = (x & (1U << i)) ? kPolPosi : kPolNega;
	VarId src_var = i_list[i];
	VarId dst_var(p(i) + offset);
	map1.set_imap(src_var, dst_var, pol);
      }
      TvFuncM f1 = f.xform(map1);
      if ( first || repfunc < f1 ) {
	first = false;
	repfunc = f1;
	map = map1;
      }
    }
  }

  for (vector<VarId>::iterator p = i_list.begin();
       p != i_list.end(); ++ p) {
    VarId src_var = *p;
    NpnVmap vmap = map.imap(src_var);
    xmap.set_imap(src_var, vmap);
  }

  return ni1;
}

END_NONAMESPACE

// @brief 代表関数を求める．
void
LcGroupMgr::default_repfunc(const TvFuncM& f,
			    TvFuncM& repfunc,
			    NpnMapM& xmap)
{
  ymuint ni = f.ni();
  ymuint no = f.no();

  // 各出力のサポートをビットベクタの形で求める．
  vector<ymuint32> sup_array(no);
  for (ymuint o = 0; o < no; ++ o) {
    VarId ovar(o);
    TvFunc func1 = f.output(ovar);
    ymuint32 supbits = gen_support(func1);
    sup_array[o] = supbits;
  }

  // サポートが共通な出力を同じグループにマージする．
  MFSet mfset(no);
  for (ymuint o = 0; o < no; ++ o) {
    ymuint supbits0 = sup_array[o];
    for (ymuint o1 = o + 1; o1 < no; ++o1) {
      ymuint supbits1 = sup_array[o1];
      if ( supbits0 & supbits1 ) {
	mfset.merge(o, o1);
      }
    }
  }

  // 一つ一つのグループを独立に処理する．
  xmap.set_identity(ni, no);
  ymuint offset = 0;
  for (ymuint first = 0; first < no; ++ first) {
    if ( mfset.find(first) != first ) continue;
    vector<ymuint> o_list;
    o_list.reserve(no);
    vector<TvFunc> f_list;
    f_list.reserve(no);
    for (ymuint o = 0; o < no; ++ o) {
      VarId ovar(o);
      if ( mfset.find(o) == first ) {
	o_list.push_back(o);
	f_list.push_back(f.output(ovar));
      }
    }
    TvFuncM f1(f_list);
    NpnMapM map1;
    ymuint ni1 = gen_maxmap(f1, offset, xmap);
    offset += ni1;
  }

  repfunc = f.xform(xmap);
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
