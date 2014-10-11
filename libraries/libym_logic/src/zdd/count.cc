
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
// 無限長精度の整数(MpInt)を用いて計算する．
MpInt
ZddMgrImpl::count(ZddEdge e)
{
  if ( e.is_overflow() ) {
    return MpInt(0);
  }
  if ( e.is_error() ) {
    return MpInt(0);
  }

  HashMap<ZddEdge, MpInt> mc_map;
  MpInt ans = count_step(e, mc_map);
  return ans;
}

// count の下請関数
MpInt
ZddMgrImpl::count_step(ZddEdge e,
		       HashMap<ZddEdge, MpInt>& mc_map)
{
  bool zattr = e.zattr();
  e.normalize();

  MpInt ans;

  if ( e.is_zero() ) {
    ans = MpInt(0);
  }
  else {
    ZddNode* vp = e.get_node();
    ymuint ref = vp->refcount();
    bool found = false;
    if ( ref != 1 ) {
      found = mc_map.find(e, ans);
    }
    if ( !found ) {
      // 子ノードが表す関数の要素数を計算する
      MpInt n0 = count_step(vp->edge0(), mc_map);
      MpInt n1 = count_step(vp->edge1(), mc_map);

      // 子ノードが表す関数の要素数を足す．
      ans = n0 + n1;

      if ( ref != 1) {
	mc_map.add(e, ans);
      }
    }
  }

  if ( zattr ) {
    ans += MpInt(1);
  }

  return ans;
}

END_NAMESPACE_YM_ZDD
