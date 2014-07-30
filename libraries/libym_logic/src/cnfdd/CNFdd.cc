
/// @file CNFdd.cc
/// @brief CNFdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/CNFdd.h"
#include "YmLogic/CNFddMgr.h"
#include "CNFddMgrImpl.h"


BEGIN_NAMESPACE_YM_CNFDD

// @brief mgr, root をセットする時に呼ばれる関数
void
CNFdd::set(CNFddMgrImpl* mgr,
	   ympuint root)
{
  mMgr = mgr;
  mRoot = root;

  mMgr->inc_rootref(CNFddEdge(mRoot));

  // CNFDD のリストに追加
  CNFdd* top = mMgr->mTopCNFdd;
  mNext = top;
  if ( top ) {
    top->mPrev = this;
  }
  mMgr->mTopCNFdd = this;
  mPrev = NULL;
}

// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
void
CNFdd::reset()
{
  mMgr->dec_rootref(CNFddEdge(mRoot));

  // CNFDD のリストから削除
  CNFdd* p = mPrev;
  CNFdd* n = mNext;
  if ( p ) {
    p->mNext = n;
  }
  else {
    mMgr->mTopCNFdd = n;
  }
  if ( n ) {
    n->mPrev = p;
  }
}

// @brief 根の枝を new_e に置き換える．
void
CNFdd::assign(ympuint new_e)
{
  mMgr->inc_rootref(CNFddEdge(new_e));
  mMgr->dec_rootref(CNFddEdge(mRoot));
  mRoot = new_e;
}

// @brief デフォルトのコンストラクタ
CNFdd::CNFdd()
{
  set(CNFddMgr::default_mgr().mImpl, CNFddEdge::make_zero());
}

// @brief コピーコンストラクタ
CNFdd::CNFdd(const CNFdd& src)
{
  set(src.mMgr, src.mRoot);
}

// @brief CNFDD マネージャと根の枝を引数とするコンストラクタ
CNFdd::CNFdd(CNFddMgrImpl* mgr,
	 ympuint e)
{
  set(mgr, e);
}

// @brief デストラクタ
CNFdd::~CNFdd()
{
  reset();
}

// @brief 代入演算子
const CNFdd&
CNFdd::operator=(const CNFdd& src)
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
CNFdd::is_zero() const
{
  return CNFddEdge(mRoot).is_zero();
}

// @brief 定数1 のチェック
// @return 定数1 の時 true を返す．
bool
CNFdd::is_one() const
{
  return CNFddEdge(mRoot).is_one();
}

// @brief 定数のチェック
// @return 定数の時 true を返す．
bool
CNFdd::is_const() const
{
  return CNFddEdge(mRoot).is_const();
}

// @brief オーバーフローのチェック
// @return 演算結果がオーバーフローしたとき true を返す．
bool
CNFdd::is_overflow() const
{
  return CNFddEdge(mRoot).is_overflow();
}

// @brief エラーのチェック
// @return 演算結果がエラーになったとき true を返す．
bool
CNFdd::is_error() const
{
  return CNFddEdge(mRoot).is_error();
}

// @brief オーバーフローとエラーのチェック
// @return 演算結果がオーバーフローかエラーのとき true を返す．
bool
CNFdd::is_invalid() const
{
  return CNFddEdge(mRoot).is_invalid();
}

// @brief 終端ノードのチェック
// @return 終端ノードのとき true を返す．
bool
CNFdd::is_leaf() const
{
  return CNFddEdge(mRoot).is_leaf();
}

// @brief 定数0に設定する．
void
CNFdd::set_zero()
{
  set(mMgr, CNFddEdge::make_zero());
}

// @brief 定数1に設定する．
void
CNFdd::set_one()
{
  set(mMgr, CNFddEdge::make_one());
}

// @brief エラー値に設定する．
void
CNFdd::set_error()
{
  set(mMgr, CNFddEdge::make_error());
}

// @brief オーバーフロー値に設定する．
void
CNFdd::set_overflow()
{
  set(mMgr, CNFddEdge::make_overflow());
}

// @brief 等価比較
// @return 2つのCNFDDが等しいとき true を返す．
bool
CNFdd::operator==(const CNFdd& src2) const
{
  // 実はただのポインタ（スカラ値）比較でわかる．
  return mRoot == src2.mRoot;
}

// @brief conjunction 付き代入
const CNFdd&
CNFdd::operator&=(const CNFdd& src2)
{
  CNFddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = CNFddEdge::make_error();
  }
  else {
    CNFddEdge e1(mRoot);
    CNFddEdge e2(src2.mRoot);
    ans = mMgr->conjunction(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief disjunction 付き代入
const CNFdd&
CNFdd::operator|=(const CNFdd& src2)
{
  CNFddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = CNFddEdge::make_error();
  }
  else {
    CNFddEdge e1(mRoot);
    CNFddEdge e2(src2.mRoot);
    ans = mMgr->disjunction(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief diff 付き代入
const CNFdd&
CNFdd::operator-=(const CNFdd& src2)
{
  CNFddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = CNFddEdge::make_error();
  }
  else {
    CNFddEdge e1(mRoot);
    CNFddEdge e2(src2.mRoot);
    ans = mMgr->diff(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief 要素ごとのユニオン付き代入
// @param[in] src オペランド
// @return 自分自身
const CNFdd&
CNFdd::operator*=(const CNFdd& src2)
{
  CNFddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = CNFddEdge::make_error();
  }
  else {
    CNFddEdge e1(mRoot);
    CNFddEdge e2(src2.mRoot);
    ans = mMgr->merge(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief 他の節に支配されている節を取り除く
// @return 自分自身
const CNFdd&
CNFdd::make_minimal()
{
  CNFddEdge e(mRoot);
  CNFddEdge ans = mMgr->make_minimal(e);
  assign(ans);
  return *this;
}

// @brief 要素数が limit 以下の要素のみを残す．
// @param[in] limit 要素数の制限値
// @return 結果の CNFdd
CNFdd
CNFdd::cut_off(ymuint limit) const
{
  CNFddEdge e(mRoot);
  CNFddEdge ans = mMgr->cut_off(e, limit);
  return CNFdd(mMgr, ans);
}

#if 0
// @brief 共通部分のチェック
// @param[in] src2 オペランド
// @return 自分自身と src2 が共通部分を持つとき true を返す．
bool
CNFdd::operator&&(const CNFdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  return mMgr->check_intersect(CNFddEdge(mRoot), src2.CNFddEdge(mRoot)).is_one();
}

// @brief 包含関係のチェック
// @param[in] src2
// @return 自分自身が minterms の集合として src2 を含むとき true を返す．
bool
CNFdd::operator>=(const CNFdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  CNFddEdge e1 = ~CNFddEdge(mRoot);
  CNFddEdge e2 = src2.CNFddEdge(mRoot);
  return mMgr->check_intersect(e1, e2).is_zero();
}
#endif

// @brief 指定した変数の肯定のリテラルを加える．
// @param[in] var 変数番号
// @return 自分自身を返す．
const CNFdd&
CNFdd::add_posiliteral(VarId var)
{
  CNFddEdge e(mRoot);
  CNFddEdge ans = mMgr->add_posiliteral(e, var);
  assign(ans);
  return *this;
}

// @brief 指定した変数の否定のリテラルを加える．
// @param[in] var 変数番号
// @return 自分自身を返す．
const CNFdd&
CNFdd::add_negaliteral(VarId var)
{
  CNFddEdge e(mRoot);
  CNFddEdge ans = mMgr->add_negaliteral(e, var);
  assign(ans);
  return *this;
}

// @brief コファクター演算
// @param[in] var 変数番号
// @return 変数を含まないコファクターを返す．
CNFdd
CNFdd::cofactor_0(VarId var) const
{
  CNFddEdge ans = mMgr->cofactor_0(CNFddEdge(mRoot), var);
  return CNFdd(mMgr, ans);
}

// @brief コファクター演算
// @param[in] var 変数番号
// @return 変数の肯定リテラルを含むコファクターを返す．
CNFdd
CNFdd::cofactor_p(VarId var) const
{
  CNFddEdge ans = mMgr->cofactor_p(CNFddEdge(mRoot), var);
  return CNFdd(mMgr, ans);
}

// @brief コファクター演算
// @param[in] var 変数番号
// @return 変数の否定リテラルを含むコファクターを返す．
CNFdd
CNFdd::cofactor_n(VarId var) const
{
  CNFddEdge ans = mMgr->cofactor_n(CNFddEdge(mRoot), var);
  return CNFdd(mMgr, ans);
}

// @brief Shannon 展開 (Boole 展開) を行なう．
VarId
CNFdd::root_decomp(CNFdd& f_0,
		   CNFdd& f_p,
		   CNFdd& f_n) const
{

  CNFddEdge e(mRoot);
  CNFddNode* node = e.get_node();

  CNFddEdge e_0;
  CNFddEdge e_p;
  CNFddEdge e_n;
  VarId varid;
  if ( node ) {
    e_0 = node->edge_0();
    e_0.add_zattr(e.zattr());
    e_p = node->edge_p();
    e_n = node->edge_n();
    ymuint level = node->level();
    varid = mMgr->varid(level);
  }
  else {
    e_0 = e;
    e_p = e_n = CNFddEdge::make_zero();
    varid = kVarIdIllegal;
  }
  f_0 = CNFdd(mMgr, e_0);
  f_p = CNFdd(mMgr, e_p);
  f_n = CNFdd(mMgr, e_n);
  return varid;
}

// @brief 根の変数番号を取り出す．
VarId
CNFdd::root_var() const
{
  CNFddEdge e(mRoot);
  CNFddNode* node = e.get_node();
  if ( node ) {
    ymuint level = node->level();
    return mMgr->varid(level);
  }
  return kVarIdIllegal;
}

// @brief 0枝の取得
CNFdd
CNFdd::edge_0() const
{
  CNFddEdge e(mRoot);
  CNFddNode* node = e.get_node();
  if ( node ) {
    CNFddEdge ans = node->edge_0();
    ans.add_zattr(e.zattr());
    return CNFdd(mMgr, ans);
  }
  else {
    return *this;
  }
}

// @brief p枝の取得
CNFdd
CNFdd::edge_p() const
{
  CNFddEdge e(mRoot);
  CNFddNode* node = e.get_node();
  if ( node ) {
    return CNFdd(mMgr, node->edge_p());
  }
  else {
    return CNFdd(mMgr, CNFddEdge::make_zero());
  }
}

// @brief n枝の取得
CNFdd
CNFdd::edge_n() const
{
  CNFddEdge e(mRoot);
  CNFddNode* node = e.get_node();
  if ( node ) {
    return CNFdd(mMgr, node->edge_n());
  }
  else {
    return CNFdd(mMgr, CNFddEdge::make_zero());
  }
}

// @brief CNFDD が使っているノード数を数える．
ymuint64
CNFdd::node_count() const
{
  return mMgr->node_count(CNFddEdge(mRoot));
}

// @brief 項数の計算
mpz_class
CNFdd::count() const
{
  return mMgr->count(CNFddEdge(mRoot));
}

// @brief サポート変数集合の計算 (VarVector)
ymuint
CNFdd::support(VarVector& vars) const
{
  mMgr->mark_support(CNFddEdge(mRoot));
  return mMgr->mark_to_vector(vars);
}

// @brief サポート変数集合の計算 (VarList)
ymuint
CNFdd::support(VarList& vars) const
{
  mMgr->mark_support(CNFddEdge(mRoot));
  return mMgr->mark_to_list(vars);
}

// @brief サポート変数集合の要素数の計算
ymuint
CNFdd::support_size() const
{
  return mMgr->mark_support(CNFddEdge(mRoot));
}

END_NAMESPACE_YM_CNFDD
