
/// @file SupOp.cc
/// @brief SupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SupOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス SupOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
SupOp::SupOp(BddMgrImpl* mgr) :
  BddOp(mgr)
{
}

// @brief デストラクタ
SupOp::~SupOp()
{
}

// @brief 演算を行う関数
// @param[in] edge_list 根の枝のリスト
// @return edge_list の根からたどれる BDD のノード数を返す．
ymuint
SupOp::apply(const vector<BddEdge>& edge_list)
{
  ymuint n = mgr()->max_level();
  mMarks.clear();
  mMarks.resize(n, 0);
  mSupList.clear();
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    count_sub(e);
  }
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    clear_mark(e);
  }
  sort(mSupList.begin(), mSupList.end());
  return mSupList.size();
}

// @brief サポート変数のリストを得る．
// @param[out] var_list 結果を格納する変数
ymuint
SupOp::to_list(vector<VarId>& var_list)
{
  ymuint n = mSupList.size();
  var_list.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint level = mSupList[i];
    var_list[i] = mgr()->varid(level);
  }
  return n;
}

// @brief サポート変数のリストを得る．
// @param[out] var_list 結果を格納する変数
ymuint
SupOp::to_list(list<VarId>& var_list)
{
  ymuint n = mSupList.size();
  for (ymuint i = 0; i < n; ++ i) {
    ymuint level = mSupList[i];
    var_list.push_back(mgr()->varid(level));
  }
  return n;
}

// @brief サポート変数のリストを BDD の形で得る．
BddEdge
SupOp::to_bdd()
{
  ymuint n = mSupList.size();
  BddEdge ans = BddEdge::make_one();
  for (ymuint i = 0; i < n; ++ i) {
    ymuint level = mSupList[n - i - 1];
    ans = new_node(level, BddEdge::make_zero(), ans);
  }
  return ans;
}

// @brief SCC を求める．
BddEdge
SupOp::scc(BddEdge e)
{
  apply(vector<BddEdge>(1, e));

  ymuint n = mSupList.size();
  for(ymuint i = 0; i < n; ++ i) {
    ymuint level = mSupList[i];
    mMarks[level] = 0;
  }
  scc_step(e, 0);
  clear_mark(e);

  BddEdge ans = BddEdge::make_one();
  for (ymuint i = 0; i < n; ++ i) {
    ymuint level = mSupList[n - i - 1];
    ymuint m = mMarks[level];
    if ( m == 1 ) {
      ans = new_node(level, ans, BddEdge::make_zero());
    }
    else if ( m == 2 ) {
      ans = new_node(level, BddEdge::make_zero(), ans);
    }
  }
  return ans;
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
SupOp::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数
void
SupOp::count_sub(BddEdge e)
{
  // ちょっと分かりにくいけど node->edge1() への再帰を
  // for-loop に変換している．
  for ( ; ; ) {
    BddNode* node = e.get_node();
    if ( node == 0 || node->pmark() ) {
      return;
    }
    node->pmark(1);

    ymuint level = node->level();
    if ( mMarks[level] == 0 ) {
      mSupList.push_back(level);
    }
    mMarks[level] = 1;

    count_sub(node->edge0());
    e = node->edge1();
  }
}

// @brief scc() の下請け関数
void
SupOp::scc_step(BddEdge e,
		ymuint spos)
{
  if ( e.is_one() ) {
    for (ymuint i = spos; i < mSupList.size(); ++ i) {
      ymuint level = mSupList[i];
      mMarks[level] = 3;
    }
    return;
  }
  assert_cond( !e.is_zero(), __FILE__, __LINE__);

  if ( !e.mark() ) {
    e.set_mark();
    BddNode* node = e.get_node();
    ymuint level = node->level();
    ymuint slevel = mSupList[spos];
    ++ spos;
    if ( slevel < level ) {
      mMarks[slevel] = 3;
      scc_step(e, spos);
    }
    else {
      tPol pol = e.pol();
      BddEdge e0 = node->edge0(pol);
      if ( !e0.is_zero() ) {
	mMarks[level] |= 1;
	scc_step(e0, spos);
      }
      BddEdge e1 = node->edge1(pol);
      if ( !e1.is_zero() ) {
	mMarks[level] |= 2;
	scc_step(e1, spos);
      }
    }
  }
}

END_NAMESPACE_YM_BDD
