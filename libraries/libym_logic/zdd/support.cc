
/// @file support.cc
/// @brief サポート変数を求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"
#include "CompTbl.h"
#include "ZddVar.h"


BEGIN_NAMESPACE_YM_ZDD

// eを根とするZDDのサポートに印をつける．
ymuint
ZddMgrImpl::mark_support(ZddEdge e)
{
  clear_varmark();
  sup_step(e);
  clear_mark(e);
  return mVarSet.size();
}

// edge_list に含まれる枝を根とするZDDのサポートに印をつける．
ymuint
ZddMgrImpl::mark_support(const vector<ZddEdge>& edge_list)
{
  clear_varmark();
  // サポート変数にマークをつける．
  for (vector<ZddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    sup_step(*p);
  }
  for (vector<ZddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    clear_mark(*p);
  }
  return mVarSet.size();
}

// サポート変数に印をつける．
void
ZddMgrImpl::sup_step(ZddEdge e)
{
  for ( ; ; ) {
    ZddNode* vp = e.get_node();
    if ( vp == 0 || vp->mark() ) {
      return;
    }

    vp->mark(1);
    ZddVar* v = var_at(vp->level());
    if ( !v->mMark ) {
      v->mMark = 1;
      mVarSet.push_back(v);
    }
    sup_step(vp->edge0());
    e = vp->edge1();
  }
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
ZddMgrImpl::mark_to_vector(VarVector& vars)
{
  ymuint n = mVarSet.size();
  vars.clear();
  vars.reserve(n);
  for (list<ZddVar*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    ZddVar* v = *p;
    VarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  sort(vars.begin(), vars.end());
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
ZddMgrImpl::mark_to_list(VarList& vars)
{
  vars.clear();
  for (list<ZddVar*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    ZddVar* v = *p;
    VarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  vars.sort();
  return vars.size();
}

END_NAMESPACE_YM_ZDD
