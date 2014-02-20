
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/Zdd.h"
#include "logic/ZddMgr.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD


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
    ans = mMgr->cap(e1, e2);
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
    ZddEdge e1 = ZddEdge(mRoot);
    ZddEdge e2 = ZddEdge(src2.mRoot);
    ans = mMgr->cup(e1, e2);
  }
  assign(ans);
  return *this;
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
    ans = mMgr->diff(e1, e2);
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

// @brief 指定した変数の0枝と1枝を交換する．
// @param[in] var 交換を行う変数番号
// @return 自分自身を返す．
const Zdd&
Zdd::swap(VarId var)
{
  ZddEdge ans = mMgr->swap(ZddEdge(mRoot), var);
  assign(ans);
  return *this;
}

// @brief コファクター演算
// @param[in] var 変数番号
// @return 変数 var を含まないコファクターを返す．
Zdd
Zdd::cofactor0(VarId var) const
{
  ZddEdge ans = mMgr->cofactor0(ZddEdge(mRoot), var);
  return Zdd(mMgr, ans);
}

// @brief コファクター演算
// @param[in] var 変数番号
// @return 変数 var を含むコファクターを返す．
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
  ZddEdge e0;
  ZddEdge e1;
  ZddEdge e(mRoot);
  ZddNode* node = e.get_node();
  VarId varid;
  if ( node ) {
    e0 = node->edge0();
    e0.add_zattr(e.zattr());
    e1 = node->edge1();
    ymuint level = node->level();
    varid = mMgr->varid(level);
  }
  else {
    e0 = e;
    e1 = ZddEdge::make_zero();
    varid = kVarIdIllegal;
  }
  f0 = Zdd(mMgr, e0);
  f1 = Zdd(mMgr, e1);
  return varid;
}

// @brief 根の変数番号を取り出す．
VarId
Zdd::root_var() const
{
  ZddEdge e(mRoot);
  ZddNode* node = e.get_node();
  if ( node ) {
    ymuint level = node->level();
    return mMgr->varid(level);
  }
  return kVarIdIllegal;
}

// @brief 0枝の取得
Zdd
Zdd::edge0() const
{
  ZddEdge e(mRoot);
  ZddNode* node = e.get_node();
  if ( node ) {
    ZddEdge ans = node->edge0();
    ans.add_zattr(e.zattr());
    return Zdd(mMgr, ans);
  }
  else {
    return *this;
  }
}

// @brief 1枝の取得
Zdd
Zdd::edge1() const
{
  ZddEdge e(mRoot);
  ZddNode* node = e.get_node();
  if ( node ) {
    return Zdd(mMgr, node->edge1());
  }
  else {
    return Zdd(mMgr, ZddEdge::make_zero());
  }
}

// @brief ZDD が使っているノード数を数える．
ymuint64
Zdd::node_count() const
{
  return mMgr->node_count(ZddEdge(mRoot));
}

// @brief 要素数の計算
mpz_class
Zdd::count() const
{
  return mMgr->count(ZddEdge(mRoot));
}

// @brief サポート変数集合の計算 (VarVector)
ymuint
Zdd::support(VarVector& vars) const
{
  mMgr->mark_support(ZddEdge(mRoot));
  return mMgr->mark_to_vector(vars);
}

// @brief サポート変数集合の計算 (VarList)
ymuint
Zdd::support(VarList& vars) const
{
  mMgr->mark_support(ZddEdge(mRoot));
  return mMgr->mark_to_list(vars);
}

// @brief サポート変数集合の要素数の計算
ymuint
Zdd::support_size() const
{
  return mMgr->mark_support(ZddEdge(mRoot));
}

END_NAMESPACE_YM_ZDD
