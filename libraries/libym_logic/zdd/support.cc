
/// @file bmc_sup.cc
/// @brief サポート変数を求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

// eを根とするBDDのサポートに印をつける．
ymuint
BddMgrClassic::mark_support(BddEdge e)
{
  clear_varmark();
  sup_step(e);
  clear_pnmark(e);
  return mVarSet.size();
}

// edge_list に含まれる枝を根とするBDDのサポートに印をつける．
ymuint
BddMgrClassic::mark_support(const list<BddEdge>& edge_list)
{
  clear_varmark();
  // サポート変数にマークをつける．
  for (list<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    sup_step(*p);
  }
  for (list<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    clear_pnmark(*p);
  }
  return mVarSet.size();
}

// サポート変数に印をつける．
void
BddMgrClassic::sup_step(BddEdge e)
{
  for ( ; ; ) {
    Node* vp = get_node(e);
    if ( vp == 0 || vp->pmark() ) {
      return;
    }

    vp->pmark(1);
    Var* v = vp->var();
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
BddMgrClassic::mark_to_vector(VarVector& vars)
{
  tVarSize n = mVarSet.size();
  vars.clear();
  vars.reserve(n);
  for (list<Var*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    Var* v = *p;
    tVarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  sort(vars.begin(), vars.end());
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
BddMgrClassic::mark_to_list(VarList& vars)
{
  vars.clear();
  for (list<Var*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    Var* v = *p;
    tVarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  vars.sort();
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
BddEdge
BddMgrClassic::mark_to_bdd()
{
  if ( mVarSet.empty() ) {
    return BddEdge::make_one();
  }
  list<Var*>::iterator p = mVarSet.begin();
  BddEdge tmp = make_posiliteral((*p)->varid());
  for (++ p; p != mVarSet.end(); ++ p) {
    tmp = and_op(tmp, make_posiliteral((*p)->varid()));
  }
  clear_varmark();
  return tmp;
}

END_NAMESPACE_YM_BDD
