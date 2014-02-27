
/// @file count.cc
/// @brief 節点数を数える関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

// edge list に登録されたBDDの節点数を数える．
ymuint64
ZddMgrImpl::node_count(const vector<ZddEdge>& edge_list)
{
  mNum = 0;
  for (vector<ZddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    count1(*p);
  }
  for (vector<ZddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    clear_mark(*p);
  }
  return mNum;
}

// node_count() の下請関数
void
ZddMgrImpl::count1(ZddEdge e)
{
  for ( ; ; ) {
    ZddNode* vp = e.get_node();
    if ( vp == 0 || vp->mark() ) {
      return;
    }
    ++ mNum;
    vp->mark(1);
    count1(vp->edge0());
    e = vp->edge1();
  }
}

// ZDD の表す集合の要素数を返す．
// 無限長精度の整数(mpz_class)を用いて計算する．
mpz_class
ZddMgrImpl::count(ZddEdge e)
{
  if ( e.is_overflow() ) {
    return 0;
  }
  if ( e.is_error() ) {
    return 0;
  }

  ZddEdgeMpzMap mc_map;
  mpz_class ans = count_step(e, mc_map);
  return ans;
}

// count の下請関数
mpz_class
ZddMgrImpl::count_step(ZddEdge e,
		       ZddEdgeMpzMap& mc_map)
{
  bool zattr = e.zattr();
  e.normalize();

  mpz_class ans;

  if ( e.is_zero() ) {
    ans = 0;
  }
  else {
    ZddNode* vp = e.get_node();
    ymuint ref = vp->refcount();
    bool found = false;
    if ( ref != 1 ) {
      ZddEdgeMpzMap::iterator p = mc_map.find(e);
      if ( p != mc_map.end() ) {
	found = true;
	ans = p->second;
      }
    }
    if ( !found ) {
      // 子ノードが表す関数の要素数を計算する
      mpz_class n0 = count_step(vp->edge0(), mc_map);
      mpz_class n1 = count_step(vp->edge1(), mc_map);

      // 子ノードが表す関数の要素数を足す．
      ans = n0 + n1;

      if ( ref != 1) {
	mc_map[e] = ans;
      }
    }
  }

  if ( zattr ) {
    ans += 1;
  }

  return ans;
}

END_NAMESPACE_YM_ZDD
