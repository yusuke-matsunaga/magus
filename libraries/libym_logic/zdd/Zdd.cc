
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"
#include "ym_logic/ZddMgr.h"
#include "ZddMgrImpl.h"

#include "ym_utils/HeapTree.h"


BEGIN_NAMESPACE_YM_ZDD

BEGIN_NONAMESPACE

// 入力数がこの値よりも小さかったらヒープ木は使わない．
const ymuint kNiLimit = 8;

END_NONAMESPACE


// @brief mgr, root をセットする時に呼ばれる関数
void
Zdd::set(ZddMgrImpl* mgr,
	 ympuint root)
{
  mMgr = mgr;
  mRoot = root;

  mMgr->inc_rootref(ZddEdge(mRoot));

  // ZDD のリストに追加
  Zdd* top = mMgr->mTopZdd;
  mNext = top;
  if ( top ) {
    top->mPrev = this;
  }
  mMgr->mTopZdd = this;
  mPrev = NULL;
}

// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
void
Zdd::reset()
{
  mMgr->dec_rootref(ZddEdge(mRoot));

  // ZDD のリストから削除
  Zdd* p = mPrev;
  Zdd* n = mNext;
  if ( p ) {
    p->mNext = n;
  }
  else {
    mMgr->mTopZdd = n;
  }
  if ( n ) {
    n->mPrev = p;
  }
}

// @brief 根の枝を new_e に置き換える．
void
Zdd::assign(ympuint new_e)
{
  mMgr->inc_rootref(ZddEdge(new_e));
  mMgr->dec_rootref(ZddEdge(mRoot));
  mRoot = new_e;
}

// @brief デフォルトのコンストラクタ
Zdd::Zdd()
{
  set(ZddMgr::default_mgr().mImpl, ZddEdge::make_zero());
}

// @brief コピーコンストラクタ
Zdd::Zdd(const Zdd& src)
{
  set(src.mMgr, src.mRoot);
}

// @brief ZDD マネージャと根の枝を引数とするコンストラクタ
Zdd::Zdd(ZddMgrImpl* mgr,
	 ympuint e)
{
  set(mgr, e);
}

// @brief デストラクタ
Zdd::~Zdd()
{
  reset();
}

// @brief 代入演算子
const Zdd&
Zdd::operator=(const Zdd& src)
{
  if ( src.mMgr != mMgr ) {
    // マネージャも切り替える場合
    reset();
    set(src.mMgr, src.mRoot);
  }
  else {
    // 同じマネージャで根の枝だけ切り替える場合
    assign(src.mRoot);
  }
  return *this;
}

// @brief 定数0 のチェック
// @return 定数0の時 true を返す．
bool
Zdd::is_zero() const
{
  return ZddEdge(mRoot).is_zero();
}

// @brief 定数1 のチェック
// @return 定数1 の時 true を返す．
bool
Zdd::is_one() const
{
  return ZddEdge(mRoot).is_one();
}

// @brief 定数のチェック
// @return 定数の時 true を返す．
bool
Zdd::is_const() const
{
  return ZddEdge(mRoot).is_const();
}

// @brief オーバーフローのチェック
// @return 演算結果がオーバーフローしたとき true を返す．
bool
Zdd::is_overflow() const
{
  return ZddEdge(mRoot).is_overflow();
}

// @brief エラーのチェック
// @return 演算結果がエラーになったとき true を返す．
bool
Zdd::is_error() const
{
  return ZddEdge(mRoot).is_error();
}

// @brief オーバーフローとエラーのチェック
// @return 演算結果がオーバーフローかエラーのとき true を返す．
bool
Zdd::is_invalid() const
{
  return ZddEdge(mRoot).is_invalid();
}

// @brief 終端ノードのチェック
// @return 終端ノードのとき true を返す．
bool
Zdd::is_leaf() const
{
  return ZddEdge(mRoot).is_leaf();
}

// @brief 定数0に設定する．
void
Zdd::set_zero()
{
  set(mMgr, ZddEdge::make_zero());
}

// @brief 定数1に設定する．
void
Zdd::set_one()
{
  set(mMgr, ZddEdge::make_one());
}

// @brief エラー値に設定する．
void
Zdd::set_error()
{
  set(mMgr, ZddEdge::make_error());
}

// @brief オーバーフロー値に設定する．
void
Zdd::set_overflow()
{
  set(mMgr, ZddEdge::make_overflow());
}

// @brief 等価比較
// @return 2つのZDDが等しいとき true を返す．
bool
Zdd::operator==(const Zdd& src2) const
{
  // 実はただのポインタ（スカラ値）比較でわかる．
  return mRoot == src2.mRoot;
}

// @brief intersection 付き代入
const Zdd&
Zdd::operator&=(const Zdd& src2)
{
  ZddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = ZddEdge::make_error();
  }
  else {
    ZddEdge e1 = ZddEdge(mRoot);
    ZddEdge e2 = ZddEdge(src2.mRoot);
    ans = mMgr->cap_op(e1, e2);
  }
  assign(ans);
  return *this;
}

// 複数のZDDのintersectionを求める．
Zdd
intersection(const ZddVector& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return ZddMgr::default_mgr().make_base();
  }
  if ( n == 1 ) {
    return zdds[0];
  }
  if ( n == 2 ) {
    return zdds[0] & zdds[1];
  }
  if ( n == 3 ) {
    return zdds[0] & zdds[1] & zdds[2];
  }
  if ( n == 4 ) {
    return zdds[0] & zdds[1] & zdds[2] & zdds[3];
  }

  Zdd ans;
  if ( n < kNiLimit ) {
    ZddVector::const_iterator p = zdds.begin();
    ans = *p;
    for (++ p; p != zdds.end(); ++ p) {
      ans &= *p;
    }
  }
  else {
    SimpleHeapTree<Zdd> work;
    for (ZddVector::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      Zdd zdd = *p;
      work.put(zdd, size(zdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans &= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

Zdd
intersection(const ZddList& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return ZddMgr::default_mgr().make_base();
  }
  if ( n == 1 ) {
    return zdds.front();
  }
  if ( n == 2 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd ans = *p;
    ++ p;
    ans &= *p;
    return ans;
  }
  if ( n == 3 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd ans = *p;
    ++ p;
    ans &= *p;
    ++ p;
    ans &= *p;
    return ans;
  }
  if ( n == 4 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd ans = *p;
    ++ p;
    ans &= *p;
    ++ p;
    ans &= *p;
    ++ p;
    ans &= *p;
    return ans;
  }

  Zdd ans;
  if ( n < kNiLimit ) {
    ZddList::const_iterator p = zdds.begin();
    ans = *p;
    for (++ p; p != zdds.end(); ++ p) {
      ans &= *p;
    }
  }
  else {
    SimpleHeapTree<Zdd> work;
    for (ZddList::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      Zdd zdd = *p;
      work.put(zdd, size(zdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans &= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

// @brief union 付き代入
const Zdd&
Zdd::operator|=(const Zdd& src2)
{
  ZddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = ZddEdge::make_error();
  }
  else {
    ZddEdge e1 = ZddEdge(mRoot);
    ZddEdge e2 = ZddEdge(src2.mRoot);
    ans = mMgr->cup_op(e1, e2);
  }
  assign(ans);
  return *this;
}

// 複数のZDDのunionを求める．
Zdd
set_union(const ZddVector& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return ZddMgr::default_mgr().make_empty();
  }
  if ( n == 1 ) {
    return zdds[0];
  }
  if ( n == 2 ) {
    return zdds[0] | zdds[1];
  }
  if ( n == 3 ) {
    return zdds[0] | zdds[1] | zdds[2];
  }
  if ( n == 4 ) {
    return zdds[0] | zdds[1] | zdds[2] | zdds[3];
  }

  Zdd ans;
  if ( n < kNiLimit ) {
    ZddVector::const_iterator p = zdds.begin();
    ans = *p;
    for (++ p; p != zdds.end(); ++ p) {
      ans |= *p;
    }
  }
  else {
    SimpleHeapTree<Zdd> work;
    for (ZddVector::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      Zdd zdd = *p;
      work.put(zdd, size(zdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans |= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

Zdd
set_union(const ZddList& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return ZddMgr::default_mgr().make_empty();
  }
  if ( n == 1 ) {
    return zdds.front();
  }
  if ( n == 2 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd ans = *p;
    ++ p;
    ans |= *p;
    return ans;
  }
  if ( n == 3 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd ans = *p;
    ++ p;
    ans |= *p;
    ++ p;
    ans |= *p;
    return ans;
  }
  if ( n == 4 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd ans = *p;
    ++ p;
    ans |= *p;
    ++ p;
    ans |= *p;
    ++ p;
    ans |= *p;
    return ans;
  }

  Zdd ans;
  if ( n < kNiLimit ) {
    ZddList::const_iterator p = zdds.begin();
    ans = *p;
    for (++ p; p != zdds.end(); ++ p) {
      ans |= *p;
    }
  }
  else {
    SimpleHeapTree<Zdd> work;
    for (ZddList::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      Zdd zdd = *p;
      work.put(zdd, size(zdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans |= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

// @brief diff 付き代入
const Zdd&
Zdd::operator-=(const Zdd& src2)
{
  ZddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = ZddEdge::make_error();
  }
  else {
    ZddEdge e1 = ZddEdge(mRoot);
    ZddEdge e2 = ZddEdge(src2.mRoot);
    ans = mMgr->diff_op(e1, e2);
  }
  assign(ans);
  return *this;
}

#if 0
// @brief 共通部分のチェック
// @param[in] src2 オペランド
// @return 自分自身と src2 が共通部分を持つとき true を返す．
bool
Zdd::operator&&(const Zdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  return mMgr->check_intersect(ZddEdge(mRoot), src2.ZddEdge(mRoot)).is_one();
}

// @brief 包含関係のチェック
// @param[in] src2
// @return 自分自身が minterms の集合として src2 を含むとき true を返す．
bool
Zdd::operator>=(const Zdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  ZddEdge e1 = ~ZddEdge(mRoot);
  ZddEdge e2 = src2.ZddEdge(mRoot);
  return mMgr->check_intersect(e1, e2).is_zero();
}
#endif

// @brief 変数を含まないコファクターを返す．
Zdd
Zdd::cofactor0(VarId var) const
{
  ZddEdge ans = mMgr->cofactor0(ZddEdge(mRoot), var);
  return Zdd(mMgr, ans);
}

// @brief 変数を含むコファクターを返す．
Zdd
Zdd::cofactor1(VarId var) const
{
  ZddEdge ans = mMgr->cofactor1(ZddEdge(mRoot), var);
  return Zdd(mMgr, ans);
}

// @brief Shannon 展開 (Boole 展開) を行なう．
VarId
Zdd::root_decomp(Zdd& f0,
		 Zdd& f1) const
{
  ZddEdge e0, e1;
  VarId ans = mMgr->root_decomp(ZddEdge(mRoot), e0, e1);
  f0 = Zdd(mMgr, e0);
  f1 = Zdd(mMgr, e1);
  return ans;
}

// @brief 根の変数番号を取り出す．
VarId
Zdd::root_var() const
{
  return mMgr->root_var(ZddEdge(mRoot));
}

// @brief 0枝の取得
Zdd
Zdd::edge0() const
{
  ZddEdge ans = mMgr->edge0(ZddEdge(mRoot));
  return Zdd(mMgr, ans);
}

// @brief 1枝の取得
Zdd
Zdd::edge1() const
{
  ZddEdge ans = mMgr->edge1(ZddEdge(mRoot));
  return Zdd(mMgr, ans);
}

#if 0
// @brief ZDD の内容を書き出す．
ymuint64
Zdd::print(ostream& s) const
{
  Printer printer(mMgr, s);
  printer.print_root(ZddEdge(mRoot));
  return printer.num();
}

// @brief ZDD ベクタの内容を書き出す
// @param[in] array ZDD ベクタ
// @param[in] s 出力ストリーム
ymuint64
print(const ZddVector& array,
      ostream& s)
{
  if ( array.empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  Printer printer(mgr, s);
  for (ZddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    print.print_root(zdd.root());
  }
  return printer.num();
}

// @brief ZDD リストの内容を書き出す
// @param[in] array ZDD リスト
// @param[in] s 出力ストリーム
ymuint64
print(const ZddList& array,
      ostream& s)
{
  if ( array.empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  Displayer displayer(mgr, s);
  for (ZddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    displayer.display_root(zdd.root());
  }
  return displayer.num();
}
#endif

// @brief ZDD が使っているノード数を数える．
ymuint64
size(const Zdd& zdd)
{
  return zdd.mMgr->size(ZddEdge(zdd.mRoot));
}

// @brief ZDD ベクタが使っているノード数を数える．
// @param[in] array ZDD ベクタ
// @return ZDD ベクタが使っているノード数
ymuint64
size(const ZddVector& array)
{
  if ( array.empty() ) {
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  return mgr->size(edge_list);
}

// @brief ZDD リストが使っているノード数を数える．
// @param[in] array ZDD リスト
// @return ZDD リストが使っているノード数
ymuint64
size(const ZddList& array)
{
  if ( array.empty() ) {
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  return mgr->size(edge_list);
}

// @brief 要素数の計算
mpz_class
count(const Zdd& zdd)
{
  return zdd.mMgr->count(ZddEdge(zdd.mRoot));
}

// @brief サポート変数集合の計算 (VarVector)
ymuint
support(const Zdd& zdd,
	VarVector& vars)
{
  zdd.mMgr->mark_support(ZddEdge(zdd.mRoot));
  return zdd.mMgr->mark_to_vector(vars);
}

// @brief サポート変数集合の計算 (VarList)
ymuint
support(const Zdd& zdd,
	VarList& vars)
{
  zdd.mMgr->mark_support(ZddEdge(zdd.mRoot));
  return zdd.mMgr->mark_to_list(vars);
}

// @brief サポート変数集合の要素数の計算
ymuint
support_size(const Zdd& zdd)
{
  return zdd.mMgr->mark_support(ZddEdge(zdd.mRoot));
}

// @brief ZDD ベクタのサポート変数集合の計算 (VarVector)
// @param[in] zdd_array ZDD のベクタ
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
ymuint
support(const ZddVector& zdd_array,
	VarVector& sup)
{
  if ( zdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddVector::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_vector(sup);
}

// @brief ZDD ベクタのサポート変数集合の計算 (VarList)
// @param[in] zdd_array ZDD のベクタ
// @param[in] sup サポート変数集合を格納するリスト
// @return サポート変数集合の要素数
ymuint
support(const ZddVector& zdd_array,
	VarList& sup)
{
  if ( zdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddVector::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief ZDD ベクタのサポート変数集合の要素数の計算
// @param[in] zdd_array ZDD のベクタ
// @return サポート変数集合の要素数
ymuint
support_size(const ZddVector& zdd_array)
{
  if ( zdd_array.empty() ) {
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddVector::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  return mgr->mark_support(edge_list);
}

// @brief ZDD リストのサポート変数集合の計算 (VarVector)
// @param[in] zdd_array ZDD のリスト
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
ymuint
support(const ZddList& zdd_array,
	VarVector& sup)
{
  if ( zdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddList::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_vector(sup);
}

// @brief ZDD リストのサポート変数集合の計算 (VarList)
// @param[in] zdd_array ZDD のリスト
// @param[in] sup サポート変数集合を格納するリスト
// @return サポート変数集合の要素数
ymuint
support(const ZddList& zdd_array,
	VarList& sup)
{
  if ( zdd_array.empty() ) {
    sup.clear();
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddList::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief ZDD リストのサポート変数集合の要素数の計算
// @param[in] zdd_array ZDD のリスト
// @return サポート変数集合の要素数
ymuint
support_size(const ZddList& zdd_array)
{
  if ( zdd_array.empty() ) {
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddList::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  return mgr->mark_support(edge_list);
}

END_NAMESPACE_YM_ZDD
