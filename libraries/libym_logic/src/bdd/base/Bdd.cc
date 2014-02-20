
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/Bdd.h"
#include "logic/BddVarSet.h"
#include "logic/BddLitSet.h"
#include "logic/BddVector.h"
#include "logic/BddList.h"

#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

// @brief mgr, root をセットする時に呼ばれる関数
void
Bdd::set(BddMgrImpl* mgr,
	 ympuint root)
{
  mMgr = mgr;
  mRoot = root;

  BddEdge e(mRoot);
  mMgr->inc_rootref(e);

  // BDD のリストに追加
  Bdd* top = mMgr->mTopBdd;
  mNext = top;
  if ( top ) {
    top->mPrev = this;
  }
  mMgr->mTopBdd = this;
  mPrev = NULL;
}

// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
void
Bdd::reset()
{
  BddEdge e(mRoot);

  mMgr->dec_rootref(e);

  // BDD のリストから削除
  Bdd* p = mPrev;
  Bdd* n = mNext;
  if ( p ) {
    p->mNext = n;
  }
  else {
    mMgr->mTopBdd = n;
  }
  if ( n ) {
    n->mPrev = p;
  }
}

// @brief 根の枝を new_e に置き換える．
void
Bdd::assign(ympuint new_e)
{
  BddEdge e1(new_e);
  mMgr->inc_rootref(e1);
  BddEdge e2(mRoot);
  mMgr->dec_rootref(e2);
  mRoot = new_e;
}

// @brief デフォルトのコンストラクタ
Bdd::Bdd()
{
  set(BddMgr::default_mgr().mImpl, BddEdge::make_error());
}

// @brief マネージャを指定したコンストラクタ
Bdd::Bdd(BddMgr& mgr)
{
  set(mgr.mImpl, BddEdge::make_error());
}

// @brief コピーコンストラクタ
Bdd::Bdd(const Bdd& src)
{
  set(src.mMgr, src.mRoot);
}

// @brief BDD マネージャと根の枝を引数とするコンストラクタ
Bdd::Bdd(BddMgrImpl* mgr,
	 ympuint e)
{
  set(mgr, e);
}

// @brief デストラクタ
Bdd::~Bdd()
{
  reset();
}

// @brief 定数0 のチェック
// @return 定数0の時 true を返す．
bool
Bdd::is_zero() const
{
  return BddEdge(mRoot).is_zero();
}

// @brief 定数1 のチェック
// @return 定数1 の時 true を返す．
bool
Bdd::is_one() const
{
  return BddEdge(mRoot).is_one();
}

// @brief 定数のチェック
// @return 定数の時 true を返す．
bool
Bdd::is_const() const
{
  return BddEdge(mRoot).is_const();
}

// @brief オーバーフローのチェック
// @return 演算結果がオーバーフローしたとき true を返す．
bool
Bdd::is_overflow() const
{
  return BddEdge(mRoot).is_overflow();
}

// @brief エラーのチェック
// @return 演算結果がエラーになったとき true を返す．
bool
Bdd::is_error() const
{
  return BddEdge(mRoot).is_error();
}

// @brief オーバーフローとエラーのチェック
// @return 演算結果がオーバーフローかエラーのとき true を返す．
bool
Bdd::is_invalid() const
{
  return BddEdge(mRoot).is_invalid();
}

// @brief 終端ノードのチェック
// @return 終端ノードのとき true を返す．
bool
Bdd::is_leaf() const
{
  return BddEdge(mRoot).is_leaf();
}

// @brief 定数0に設定する．
void
Bdd::set_zero()
{
  set(mMgr, BddEdge::make_zero());
}

// @brief 定数1に設定する．
void
Bdd::set_one()
{
  set(mMgr, BddEdge::make_one());
}

// @brief エラー値に設定する．
void
Bdd::set_error()
{
  set(mMgr, BddEdge::make_error());
}

// @brief オーバーフロー値に設定する．
void
Bdd::set_overflow()
{
  set(mMgr, BddEdge::make_overflow());
}

// @brief 等価比較
// @return 2つのBDDが等しいとき true を返す．
bool
Bdd::operator==(const Bdd& src2) const
{
  // 実はただのポインタ（スカラ値）比較でわかる．
  return mRoot == src2.mRoot;
}

// @brief 代入演算子
const Bdd&
Bdd::operator=(const Bdd& src)
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

// @brief 否定を計算する．
Bdd
Bdd::operator~() const
{
  BddEdge e(mRoot);
  BddEdge ne = ~e;
  return Bdd(mMgr, ne);
}

// @brief 否定の論理を計算して代入する．
const Bdd&
Bdd::negate()
{
  mRoot = ~BddEdge(mRoot);
  return *this;
}

// @brief AND 付き代入
const Bdd&
Bdd::operator&=(const Bdd& src2)
{
  BddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(mRoot);
    BddEdge e2(src2.mRoot);
    ans = mMgr->and_op(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief OR 付き代入
const Bdd&
Bdd::operator|=(const Bdd& src2)
{
  BddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(mRoot);
    BddEdge e2(src2.mRoot);
    ans = ~mMgr->and_op(~e1, ~e2);
  }
  assign(ans);
  return *this;
}

// @brief XOR 付き代入
const Bdd&
Bdd::operator^=(const Bdd& src2)
{
  BddEdge ans;
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(mRoot);
    BddEdge e2(src2.mRoot);
    ans = mMgr->xor_op(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief 共通部分のチェック
// @param[in] src2 オペランド
// @return 自分自身と src2 が共通部分を持つとき true を返す．
bool
Bdd::operator&&(const Bdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  BddEdge e1(mRoot);
  BddEdge e2(src2.mRoot);
  return mMgr->check_intersect(e1, e2).is_one();
}

// @brief 包含関係のチェック
// @param[in] src2
// @return 自分自身が minterms の集合として src2 を含むとき true を返す．
bool
Bdd::operator>=(const Bdd& src2) const
{
  if ( mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  BddEdge e1(mRoot);
  BddEdge e2(src2.mRoot);
  return mMgr->check_intersect(~e1, e2).is_zero();
}

// @brief compose 演算
Bdd
Bdd::compose(VarId var,
	     const Bdd& g) const
{
  BddEdge ans;
  if ( mMgr != g.mMgr || g.is_error() ) {
    ans = BddEdge::make_error();
  }
  else if ( g.is_overflow() ) {
    ans = BddEdge::make_overflow();
  }
  else {
    mMgr->compose_start();
    BddEdge e1(g.mRoot);
    mMgr->compose_reg(var, e1);
    BddEdge e2(mRoot);
    ans = mMgr->compose(e2);
  }
  return Bdd(mMgr, ans);
}

// @brief 複数変数の compose 演算
Bdd
Bdd::compose(const VarBddMap& comp_map) const
{
  // 始める前にエラーチェックを行う．
  for (VarBddMap::const_iterator p = comp_map.begin();
       p != comp_map.end(); ++ p) {
    Bdd bdd = p->second;
    if ( mMgr != bdd.mMgr || bdd.is_error() ) {
      return Bdd(mMgr, BddEdge::make_error());
    }
    if ( bdd.is_overflow() ) {
      return Bdd(mMgr, BddEdge::make_overflow());
    }
  }

  mMgr->compose_start();
  for (VarBddMap::const_iterator p = comp_map.begin();
       p != comp_map.end(); ++ p) {
    VarId id = p->first;
    Bdd bdd = p->second;
    BddEdge e1(bdd.mRoot);
    mMgr->compose_reg(id, e1);
  }
  BddEdge e(mRoot);
  BddEdge ans = mMgr->compose(e);
  return Bdd(mMgr, ans);
}

// @brief 変数インデックスの置き換え
Bdd
Bdd::remap_var(const VarVarMap& var_map) const
{
  mMgr->compose_start();
  for (VarVarMap::const_iterator p = var_map.begin();
       p != var_map.end(); ++ p) {
    VarId id = p->first;
    VarId mid = p->second;
    BddEdge e1 = mMgr->make_posiliteral(mid);
    mMgr->compose_reg(id, e1);
  }
  BddEdge e(mRoot);
  BddEdge ans = mMgr->compose(e);
  return Bdd(mMgr, ans);
}

// @brief コファクター演算
Bdd
Bdd::cofactor(VarId var,
	      tPol pol) const
{
  BddEdge e(mRoot);
  BddEdge ans = mMgr->scofactor(e, var, pol);
  return Bdd(mMgr, ans);
}

// @brief 複数変数のコファクター演算付き代入
const Bdd&
Bdd::operator/=(const BddLitSet& lit_set)
{
  return operator/=(lit_set.function());
}

// @brief generalized cofactor 演算
const Bdd&
Bdd::operator/=(const Bdd& c)
{
  BddEdge ans;
  if ( mMgr != c.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(mRoot);
    BddEdge e2(c.mRoot);
    ans = mMgr->gcofactor(e1, e2);
  }
  assign(ans);
  return *this;
}

// @brief Davio 展開のモーメント項を求める処理
Bdd
Bdd::xor_moment(VarId idx) const
{
  BddEdge e(mRoot);
  BddEdge ans = mMgr->xor_moment(e, idx);
  return Bdd(mMgr, ans);
}

// @brief Smallest Cube Containg F を求める．
Bdd
Bdd::SCC() const
{
  BddEdge e(mRoot);
  BddEdge ans = mMgr->SCC(e);
  return Bdd(mMgr, ans);
}

// @brief AND 演算
Bdd
operator&(const Bdd& src1,
	  const Bdd& src2)
{
  return Bdd(src1).operator&=(src2);
}

// @brief OR 演算
Bdd
operator|(const Bdd& src1,
	  const Bdd& src2)
{
  return Bdd(src1).operator|=(src2);
}

// @brief XOR 演算
Bdd
operator^(const Bdd& src1,
	  const Bdd& src2)
{
  return Bdd(src1).operator^=(src2);
}

// @brief 複数変数のコファクター演算
Bdd
operator/(const Bdd& src,
	  const BddLitSet& lit_set)
{
  return Bdd(src).operator/=(lit_set.function());
}

// @brief generalized cofactor 演算
Bdd
operator/(const Bdd& src,
	  const Bdd& c)
{
  return Bdd(src).operator/=(c);
}

// @brief 正リテラルのみのキューブの時，真となる．
bool
Bdd::is_posi_cube() const
{
  BddEdge e(mRoot);
  return mMgr->check_posi_cube(e);
}

// @brief キューブの時，真となる．
bool
Bdd::is_cube() const
{
  BddEdge e(mRoot);
  return mMgr->check_cube(e);
}

// @brief 対称性のチェック
bool
Bdd::check_symmetry(VarId x,
		    VarId y,
		    tPol pol) const
{
  if ( x == y ) {
    return pol == kPolPosi;
  }
#if defined(VERIFY_CHECK_SYMMETRY)
  bool result = mMgr->check_symmetry(mRoot, x, y, pol);
  Bdd f0 = cofactor(x, kPolNega);
  Bdd f1 = cofactor(x, kPolPosi);
  bool result2;
  if ( pol == kPolPosi ) {
    Bdd f2 = f0.cofactor(y, kPolPosi);
    Bdd f3 = f1.cofactor(y, kPolNega);
    result2 = f2 == f3;
  }
  else {
    Bdd f2 = f0.cofactor(y, kPolNega);
    Bdd f3 = f1.cofactor(y, kPolPosi);
    result2 = f2 == f3;
  }
  if ( result != result2 ) {
    cout << "ERROR @ check_symmetry("
	 << x << ", " << y << ", ";
    if ( pol == kPolPosi ) {
      cout << "positive";
    }
    else {
      cout << "negative";
    }
    cout << ")" << endl;
    //display(cout);
    cout << sop() << endl;
    cout << "expected result = ";
    if ( result2 ) {
      cout << "true";
    }
    else {
      cout << "false";
    }
    cout << endl;
  }
  return result2;
#else
  BddEdge e(mRoot);
  return mMgr->check_symmetry(e, x, y, pol);
#endif
}

// @brief existential quntification(smoothing)
Bdd
Bdd::esmooth(const BddVarSet& svars) const
{
  BddEdge e(mRoot);
  BddEdge s(svars.function().mRoot);
  BddEdge ans = mMgr->esmooth(e, s);

  return Bdd(mMgr, ans);
}

// @brief universal quantification
Bdd
Bdd::asmooth(const BddVarSet& svars) const
{
  BddEdge e(mRoot);
  BddEdge s(svars.function().mRoot);
  BddEdge ans = ~mMgr->esmooth(~e, s);

  return Bdd(mMgr, ans);
}

// @brief x_level の変数を y_level まで「押し込む」
Bdd
Bdd::push_down(ymuint x_level,
	       ymuint y_level,
	       tPol pol) const
{
  if ( x_level >= y_level ) {
    return Bdd(mMgr, BddEdge::make_error());
  }
  BddEdge e(mRoot);
  BddEdge ans = mMgr->push_down(e, x_level, y_level, pol);
  return Bdd(mMgr, ans);
}

// @brief Shannon 展開 (Boole 展開) を行なう．
VarId
Bdd::root_decomp(Bdd& f0,
		 Bdd& f1) const
{
  BddEdge e(mRoot);
  tPol pol = e.pol();
  BddNode* node = e.get_node();
  if ( node ) {
    BddEdge e0 = node->edge0(pol);
    f0 = Bdd(mMgr, e0);
    BddEdge e1 = node->edge1(pol);
    f1 = Bdd(mMgr, e1);
    ymuint level = node->level();
    return mMgr->varid(level);
  }
  else {
    f0 = f1 = *this;
    return kVarIdIllegal;
  }
}

// @brief 根の変数番号を取り出す．
VarId
Bdd::root_var() const
{
  BddEdge e(mRoot);
  BddNode* node = e.get_node();
  if ( node ) {
    ymuint level = node->level();
    return mMgr->varid(level);
  }
  else {
    return kVarIdIllegal;
  }
}

// @brief 0枝の取得
Bdd
Bdd::edge0() const
{
  BddEdge e(mRoot);
  tPol pol = e.pol();
  BddNode* node = e.get_node();
  if ( node ) {
    BddEdge e0 = node->edge0(pol);
    return Bdd(mMgr, e0);
  }
  else {
    return *this;
  }
}

// @brief 1枝の取得
Bdd
Bdd::edge1() const
{
  BddEdge e(mRoot);
  tPol pol = e.pol();
  BddNode* node = e.get_node();
  if ( node ) {
    BddEdge e1 = node->edge1(pol);
    return Bdd(mMgr, e1);
  }
  else {
    return *this;
  }
}

// @brief BDD の内容を積和形論理式に変換する．
LogExpr
Bdd::sop() const
{
  BddEdge e(mRoot);
  LogExpr ans_expr;
  mMgr->isop(e, e, ans_expr);
  return ans_expr;
}

// @brief 1パスの探索
BddLitSet
Bdd::onepath() const
{
  BddEdge e(mRoot);
  BddEdge ans = mMgr->onepath(e);
  return BddLitSet(Bdd(mMgr, ans));
}

// @brief 最短の 1パスの探索
BddLitSet
Bdd::shortest_onepath() const
{
  BddEdge e(mRoot);
  BddEdge ans = mMgr->shortest_onepath(e);
  return BddLitSet(Bdd(mMgr, ans));
}

// @brief 最短の 1パスの長さの取得
ymuint
Bdd::shortest_onepath_len() const
{
  BddEdge e(mRoot);
  return mMgr->shortest_onepath_len(e);
}

// @brief BDD が使っているノード数を数える．
ymuint64
Bdd::node_count() const
{
  BddEdge e(mRoot);
  return mMgr->node_count(vector<BddEdge>(1, e));
}

// @brief 真理値表密度の計算
// @param[in] n 入力数
double
Bdd::density(ymuint nvar) const
{
  BddEdge e(mRoot);
  mpz_class mc = mMgr->minterm_count(e, nvar);
  mpf_class mc_f(mc);
  mpz_class w = mpz_class(1) << nvar;
  mpf_class d = mc_f / w;
  return d.get_d();
}

// @brief 最小項の数の計算
mpz_class
Bdd::minterm_count(ymuint nvar) const
{
  BddEdge e(mRoot);
  return mMgr->minterm_count(e, nvar);
}

// @brief Walsh変換の 0次係数の計算
mpz_class
Bdd::walsh0(ymuint nvar) const
{
  BddEdge e(mRoot);
  return mMgr->walsh0(e, nvar);
}

// @brief Walsh変換の 1次係数の計算
mpz_class
Bdd::walsh1(VarId var,
	    ymuint nvar) const
{
  BddEdge e(mRoot);
  return mMgr->walsh1(e, var, nvar);
}

// @brief サポート変数集合の計算 (VarVector)
ymuint
Bdd::support(VarVector& vars) const
{
  BddMgrImpl* mgr = mMgr;
  BddEdge e(mRoot);
  mgr->mark_support(vector<BddEdge>(1, e));
  return mgr->mark_to_vector(vars);
}

// @brief サポート変数集合の計算 (VarList)
ymuint
Bdd::support(VarList& vars) const
{
  BddMgrImpl* mgr = mMgr;
  BddEdge e(mRoot);
  mgr->mark_support(vector<BddEdge>(1, e));
  return mgr->mark_to_list(vars);
}

// @brief サポート変数集合の計算 (BddVarSet)
BddVarSet
Bdd::support() const
{
  BddMgrImpl* mgr = mMgr;
  BddEdge e(mRoot);
  mgr->mark_support(vector<BddEdge>(1, e));
  BddEdge ans = mgr->mark_to_bdd();
  return BddVarSet(Bdd(mgr, ans));
}

// @brief サポート変数集合の要素数の計算
ymuint
Bdd::support_size() const
{
  BddMgrImpl* mgr = mMgr;
  BddEdge e(mRoot);
  return mgr->mark_support(vector<BddEdge>(1, e));
}

// @brief if-then-else 演算．
// @param[in] cond 条件を表す BDD
// @param[in] s 条件が成り立ったときに選ばれる関数
// @param[in] t 条件が成り立たなかったときに選ばれる関数
// @return if-then-else 演算の結果
Bdd
ite_op(const Bdd& cond,
       const Bdd& s,
       const Bdd& t)
{
  BddEdge ans;
  if ( cond.mMgr != s.mMgr || cond.mMgr != t.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(cond.mRoot);
    BddEdge e2(s.mRoot);
    BddEdge e3(t.mRoot);
    ans = cond.mMgr->ite_op(e1, e2, e3);
  }
  return Bdd(cond.mMgr, ans);
}

// @brief AND-EXIST 演算
// @param[in] src1 第1オペランド
// @param[in] src2 第2オペランド
// @param[in] svars スムージング対象の変数の集合
// @return src1 と src2 の論理積を計算してムージングを行なった結果
Bdd
and_exist(const Bdd& src1,
	  const Bdd& src2,
	  const BddVarSet& svars)
{
  Bdd sbdd = svars.function();
  BddEdge ans;
  if ( src1.mMgr != src2.mMgr || src1.mMgr != sbdd.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(src1.mRoot);
    BddEdge e2(src2.mRoot);
    BddEdge e3(sbdd.mRoot);
    ans = src1.mMgr->and_exist(e1, e2, e3);
  }
  return Bdd(src1.mMgr, ans);
}

// @brief 非冗長積和形表現の計算
// @param[in] lower 不完全指定論理関数の下限
// @param[in] upper 不完全指定論理関数の上限
// @param[in] cover 計算された非冗長積和形表現を格納する論理式
// @return 非冗長積和形の表している論理関数
Bdd
isop(const Bdd& lower,
     const Bdd& upper,
     LogExpr& cover)
{
  BddEdge ans;
  if ( lower.mMgr != upper.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(lower.mRoot);
    BddEdge e2(upper.mRoot);
    ans = lower.mMgr->isop(e1, e2, cover);
  }
  return Bdd(lower.mMgr, ans);
}

// @brief prime cover の計算
// @param[in] lower 不完全指定論理関数の下限
// @param[in] upper 不完全指定論理関数の上限
// @return 計算された prime cover
LogExpr
prime_cover(const Bdd& lower,
	    const Bdd& upper)
{
  if ( lower.mMgr != upper.mMgr ) {
    // マネージャが異なる．
    return LogExpr();
  }
  BddEdge e1(lower.mRoot);
  BddEdge e2(upper.mRoot);
  return lower.mMgr->prime_cover(e1, e2);
}

// @brief 極小サポート集合の計算
// @param[in] lower 不完全指定論理関数の下限
// @param[in] upper 不完全指定論理関数の上限
// @return 求まった極小サポートを表す BDD
Bdd
minimal_support(const Bdd& lower,
		const Bdd& upper)
{
  BddEdge ans;
  if ( lower.mMgr != upper.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(lower.mRoot);
    BddEdge e2(upper.mRoot);
    ans = lower.mMgr->minimal_support(e1, e2);
  }
  return Bdd(lower.mMgr, ans);
}

// @brief 変数集合としての共通部分の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return 変数集合としての共通部分を表す BDD
Bdd
vscap(const Bdd& src1,
      const Bdd& src2)
{
  BddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(src1.mRoot);
    BddEdge e2(src2.mRoot);
    ans = src1.mMgr->vscap(e1, e2);
  }
  return Bdd(src1.mMgr, ans);
}

// @brief 変数集合としての差集合の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return 変数集合としての差集合 (src1 - src2) を表す BDD
Bdd
vsdiff(const Bdd& src1,
       const Bdd& src2)
{
  BddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(src1.mRoot);
    BddEdge e2(src2.mRoot);
    ans = src1.mMgr->vsdiff(e1, e2);
  }
  return Bdd(src1.mMgr, ans);
}

// @brief 変数集合としての共通部分チェック
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return 変数集合として共通部分をもつとき true を返す．
bool
vsintersect(const Bdd& src1,
	    const Bdd& src2)
{
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  BddEdge e1(src1.mRoot);
  BddEdge e2(src2.mRoot);
  return src1.mMgr->vsintersect(e1, e2);
}

// @brief リテラル集合としての共通部分の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return リテラル集合としての共通部分を表す BDD
Bdd
lscap(const Bdd& src1,
      const Bdd& src2)
{
  BddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(src1.mRoot);
    BddEdge e2(src2.mRoot);
    ans = src1.mMgr->lscap(e1, e2);
  }
  return Bdd(src1.mMgr, ans);
}

// @brief リテラル集合としての差集合の計算
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return リテラル集合としての差集合 (src1 - src2) を表す BDD
Bdd
lsdiff(const Bdd& src1,
       const Bdd& src2)
{
  BddEdge ans;
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    ans = BddEdge::make_error();
  }
  else {
    BddEdge e1(src1.mRoot);
    BddEdge e2(src2.mRoot);
    ans = src1.mMgr->lsdiff(e1, e2);
  }
  return Bdd(src1.mMgr, ans);
}

// @brief リテラル集合としての共通部分チェック
// @param[in] src1 第オペランド
// @param[in] src2 第オペランド
// @return リテラル集合として共通部分をもつとき true を返す．
bool
lsintersect(const Bdd& src1,
	    const Bdd& src2)
{
  if ( src1.mMgr != src2.mMgr ) {
    // マネージャが異なる．
    return false;
  }
  BddEdge e1(src1.mRoot);
  BddEdge e2(src2.mRoot);
  return src1.mMgr->lsintersect(e1, e2);
}

// @brief BddLitSet を表しているときに内容をベクタに変換する．
// @param[in] dst 結果を格納するベクタ
// @return 要素数
ymuint
Bdd::to_literalvector(LiteralVector& dst) const
{
  BddEdge e(mRoot);
  return mMgr->to_literalvector(e, dst);
}

// @brief BddLitSet を表しているときに内容をリストに変換する．
// @param[in] dst 結果を格納するリスト
// @return 要素数
ymuint
Bdd::to_literallist(LiteralList& dst) const
{
  BddEdge e(mRoot);
  return mMgr->to_literallist(e, dst);
}

// @brief カルノー図を描く
// @param[in] s 出力ストリーム
// 4入力以下した受け付けない
void
Bdd::print_map(ostream& s) const
{
  char val[16];
  bool flag = true;
  Bdd f = *this;
  for (ymuint i = 0; i < 16; i ++) {
    for (ymuint j = 0; j < 4; j ++) {
      if ( f.is_const() ) {
	break;
      }
      Bdd f0;
      Bdd f1;
      VarId vid = f.root_decomp(f0, f1);
      if ( vid.val() > j ) continue;
      if ( i & (1 << j) ) {
	f = f1;
      }
      else {
	f = f0;
      }
    }
    if ( f.is_zero() ) {
      val[i] = '0';
    }
    else if ( f.is_one() ) {
      val[i] = '1';
    }
    else {
      flag = false;
      break;
    }
  }
  if ( !flag ) {
    // 4変数ではなかった．
    s << "DumpMap: can't draw a map" << endl;
  }
  else {
    s << "   00 01 11 10" << endl;
    s << "00  " << val[0] << "  " << val[2] << "  "
      << val[3] << "  " << val[1] << endl;
    s << "01  " << val[8] << "  " << val[10] << "  "
      << val[11] << "  " << val[9] << endl;
    s << "11  " << val[12] << "  " << val[14] << "  "
      << val[15] << "  " << val[15] << endl;
    s << "10  " << val[4] << "  " << val[6] << "  "
      << val[7] << "  " << val[5] << endl;
  }
}

END_NAMESPACE_YM_BDD
