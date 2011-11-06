
/// @file libym_logic/zdd/base/Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"

#include "ZddMgrImpl.h"
#include "Dumper.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief mgr, root をセットする時に呼ばれる関数
void
Zdd::set(ZddMgrImpl* mgr,
	 ZddEdge root)
{
  mMgr = mgr;
  mRoot = root;

  mMgr->inc_rootref(mRoot);

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
  mMgr->dec_rootref(mRoot);

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
Zdd::assign(ZddEdge new_e)
{
  mMgr->inc_rootref(new_e);
  mMgr->dec_rootref(mRoot);
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
	 ZddEdge e)
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
    ZddEdge e1 = mRoot;
    ZddEdge e2 = src2.mRoot;
    ans = mMgr->cap_op(e1, e2);
  }
  assign(ans);
  return *this;
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
    ZddEdge e1 = mRoot;
    ZddEdge e2 = src2.mRoot;
    ans = ~mMgr->cup_op(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief diff 付き代入
const Zdd&
Zdd::operator^=(const Zdd& src2)
{
  ZddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = ZddEdge::make_error();
  }
  else {
    ZddEdge e1 = mRoot;
    ZddEdge e2 = src2.mRoot;
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
  return mMgr->check_intersect(mRoot, src2.mRoot).is_one();
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
  ZddEdge e1 = ~mRoot;
  ZddEdge e2 = src2.mRoot;
  return mMgr->check_intersect(e1, e2).is_zero();
}
#endif

// @brief 変数インデックスの置き換え
Zdd
Zdd::remap_var(const VarVarMap& var_map) const
{
  mMgr->compose_start();
  for (VarVarMap::const_iterator p = var_map.begin();
       p != var_map.end(); ++ p) {
    tVarId id = p->first;
    tVarId mid = p->second;
    Zdd zdd(mMgr, mMgr->make_singleton(mid));
    mMgr->compose_reg(id, zdd.mRoot);
  }
  ZddEdge ans = mMgr->compose(mRoot);
  return Zdd(mMgr, ans);
}

// @brief コファクター演算
Zdd
Zdd::cofactor(tVarId var,
	      tPol pol) const
{
  ZddEdge ans = mMgr->scofactor(mRoot, var, pol);
  return Zdd(mMgr, ans);
}

// @brief intersectionD 演算
Zdd
operator&(const Zdd& src1,
	  const Zdd& src2)
{
  return Zdd(src1).operator&=(src2);
}

// @brief union 演算
Zdd
operator|(const Zdd& src1,
	  const Zdd& src2)
{
  return Zdd(src1).operator|=(src2);
}

// @brief diff 演算
Zdd
operator^(const Zdd& src1,
	  const Zdd& src2)
{
  return Zdd(src1).operator^=(src2);
}

// @brief 複数変数のコファクター演算
Zdd
operator/(const Zdd& src,
	  const ZddLitSet& lit_set)
{
  return Zdd(src).operator/=(lit_set.function());
}

// @brief Shannon 展開 (Boole 展開) を行なう．
tVarId
Zdd::root_decomp(Zdd& f0,
		 Zdd& f1) const
{
  ZddEdge e0, e1;
  tVarId ans = mMgr->root_decomp(mRoot, e0, e1);
  f0 = Zdd(mMgr, e0);
  f1 = Zdd(mMgr, e1);
  return ans;
}

// @brief 根の変数番号を取り出す．
tVarId
Zdd::root_var() const
{
  return mMgr->root_var(mRoot);
}

// @brief 0枝の取得
Zdd
Zdd::edge0() const
{
  ZddEdge ans = mMgr->edge0(mRoot);
  return Zdd(mMgr, ans);
}

// @brief 1枝の取得
Zdd
Zdd::edge1() const
{
  ZddEdge ans = mMgr->edge1(mRoot);
  return Zdd(mMgr, ans);
}

// @brief ZDD の内容を書き出す．
ymuint64
Zdd::display(ostream& s) const
{
  Displayer displayer(mMgr, s);
  displayer.display_root(mRoot);
  return displayer.num();
}

// @brief ZDD ベクタの内容を書き出す
// @param[in] array ZDD ベクタ
// @param[in] s 出力ストリーム
ymuint64
display(const ZddVector& array,
	ostream& s)
{
  if ( array.empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  Displayer displayer(mgr, s);
  for (ZddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    displayer.display_root(zdd.root());
  }
  return displayer.num();
}

// @brief ZDD リストの内容を書き出す
// @param[in] array ZDD リスト
// @param[in] s 出力ストリーム
ymuint64
display(const ZddList& array,
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

// @brief 内容のダンプ
void
Zdd::dump(ostream& s) const
{
  Dumper dumper(mMgr, s);
  dumper.dump(mRoot);
  dumper.dump_edge(mRoot);
  s << endl;
}

// @brief ZDD ベクタの内容をダンプする．
// @param[in] array ZDD の配列
// @param[in] s 出力ストリーム
void
dump(const ZddVector& array,
     ostream& s)
{
  if ( array.empty() ) {
    return;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  Dumper dumper(mgr, s);
  for (ZddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump(zdd.root());
  }
  for (ZddVector::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump_edge(zdd.root());
    s << endl;
  }
}

// @brief ZDD リストの内容をダンプする．
// @param[in] array ZDD のリスト
// @param[in] s 出力ストリーム
void
dump(const ZddList& array,
     ostream& s)
{
  if ( array.empty() ) {
    return;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  Dumper dumper(mgr, s);
  for (ZddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump(zdd.root());
  }
  for (ZddList::const_iterator p = array.begin();
       p != array.end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump_edge(zdd.root());
    s << endl;
  }
}

// @brief ZDD が使っているノード数を数える．
ymuint64
Zdd::size() const
{
  return mMgr->size(mRoot);
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
    edge_list.push_back(zdd.root());
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
    edge_list.push_back(zdd.root());
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = array.front().mMgr;
  return mgr->size(edge_list);
}

// @brief 要素数の計算
mpz_class
Zdd::count() const
{
  return mMgr->count(mRoot);
}

// @brief サポート変数集合の計算 (VarVector)
tVarSize
Zdd::support(VarVector& vars) const
{
  mMgr->mark_support(mRoot);
  return mMgr->mark_to_vector(vars);
}

// @brief サポート変数集合の計算 (VarList)
tVarSize
Zdd::support(VarList& vars) const
{
  mMgr->mark_support(mRoot);
  return mMgr->mark_to_list(vars);
}

// @brief サポート変数集合の要素数の計算
tVarSize
Zdd::support_size() const
{
  return mMgr->mark_support(mRoot);
}

// @brief ZDD ベクタのサポート変数集合の計算 (VarVector)
// @param[in] zdd_array ZDD のベクタ
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
tVarSize
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
    edge_list.push_back(zdd.root());
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
tVarSize
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
    edge_list.push_back(zdd.root());
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief ZDD ベクタのサポート変数集合の計算 (ZddVarSet)
// @param[in] zdd_array ZDD のベクタ
// @return サポート変数集合
ZddVarSet
support(const ZddVector& zdd_array)
{
  if ( zdd_array.empty() ) {
    return ZddVarSet(ZddMgr::default_mgr());
  }
  list<ZddEdge> edge_list;
  for (ZddVector::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(zdd.root());
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  ZddEdge ans = mgr->mark_to_zdd();
  return ZddVarSet(Zdd(mgr, ans));
}

// @brief ZDD ベクタのサポート変数集合の要素数の計算
// @param[in] zdd_array ZDD のベクタ
// @return サポート変数集合の要素数
tVarSize
support_size(const ZddVector& zdd_array)
{
  if ( zdd_array.empty() ) {
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddVector::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(zdd.root());
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  return mgr->mark_support(edge_list);
}

// @brief ZDD リストのサポート変数集合の計算 (VarVector)
// @param[in] zdd_array ZDD のリスト
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
tVarSize
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
    edge_list.push_back(zdd.root());
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
tVarSize
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
    edge_list.push_back(zdd.root());
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief ZDD リストのサポート変数集合の計算 (ZddVarSet)
// @param[in] zdd_array ZDD のリスト
// @return[in] サポート変数集合
ZddVarSet
support(const ZddList& zdd_array)
{
  if ( zdd_array.empty() ) {
    return ZddVarSet(ZddMgr::default_mgr());
  }
  list<ZddEdge> edge_list;
  for (ZddList::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(zdd.root());
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  mgr->mark_support(edge_list);
  ZddEdge ans = mgr->mark_to_zdd();
  return ZddVarSet(Zdd(mgr, ans));
}

// @brief ZDD リストのサポート変数集合の要素数の計算
// @param[in] zdd_array ZDD のリスト
// @return サポート変数集合の要素数
tVarSize
support_size(const ZddList& zdd_array)
{
  if ( zdd_array.empty() ) {
    return 0;
  }
  list<ZddEdge> edge_list;
  for (ZddList::const_iterator p = zdd_array.begin();
       p != zdd_array.end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(zdd.root());
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = zdd_array.front().mMgr;
  return mgr->mark_support(edge_list);
}

END_NAMESPACE_YM_ZDD
