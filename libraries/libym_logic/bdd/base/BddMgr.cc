
/// @file libym_logic/bdd/base/BddMgrRef.cc
/// @brief BddMgrRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddMgrRef.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/BddMgr.h"

#include "BddMgrImpl.h"
#include "Dumper.h"

#include "bmc/BddMgrClassic.h"
#include "bmm/BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

BddMgr BddMgr::theDefaultMgr;

//////////////////////////////////////////////////////////////////////
// クラス BddMgr
//////////////////////////////////////////////////////////////////////

// @brief デフォルトマネージャを返す．
BddMgr&
BddMgr::default_mgr()
{
  return theDefaultMgr;
}

// @brief デフォルトマネージャを作るためのコンストラクタ
BddMgr::BddMgr() :
  mImpl(BddMgrImpl::default_mgr())
{
  mImpl->inc_ref();
}

// @brief コンストラクタ
// @param[in] type BddMgr の型を表す文字列
// @param[in] name マネージャの名前
// @param[in] option オプション文字列
BddMgr::BddMgr(const string& type,
	       const string& name,
	       const string& option)
{
  if ( type == "bmc" ) {
    mImpl = new BddMgrClassic(name, option);
  }
  else if ( type == "bmm" ) {
    mImpl = new BddMgrModern(name, option);
  }
  else {
    mImpl = new BddMgrClassic(name, option);
  }
  assert_cond(mImpl, __FILE__, __LINE__);
  mImpl->inc_ref();
}

// デストラクタ
BddMgr::~BddMgr()
{
  assert_cond(mImpl, __FILE__, __LINE__);
  mImpl->dec_ref();
}

// 定数0を表すBDDを作る．
Bdd
BddMgr::make_zero()
{
  return mImpl->make_zero();
}

// 定数1を表すBDDを作る．
Bdd
BddMgr::make_one()
{
  return mImpl->make_one();
}

// エラーを表すBDDを作る．
Bdd
BddMgr::make_error()
{
  return mImpl->make_error();
}

// オーバーフローを表すBDDを作る．
Bdd
BddMgr::make_overflow()
{
  return mImpl->make_overflow();
}

// リテラル関数を表すBDDを作る．
Bdd
BddMgr::make_literal(tVarId index,
		     tPol pol)
{
  return mImpl->make_literal(index, pol);
}

// リテラル関数を表すBDDを作る
Bdd
BddMgr::make_literal(const Literal& lit)
{
  return mImpl->make_literal(lit);
}

// 肯定のリテラル関数を作る
Bdd
BddMgr::make_posiliteral(tVarId varid)
{
  return Bdd(mImpl, mImpl->make_posiliteral(varid));
}

// 否定のリテラル関数を作る．
Bdd
BddMgr::make_negaliteral(tVarId varid)
{
  return mImpl->make_negaliteral(varid);
}

// 指定されたインデックスを持つ中間ノードを一つ持つBDDを作る
// その左右の子のデフォルトはそれぞれ0と1
// メモリ不足によってエラーが起きる可能性がある．
Bdd
BddMgr::make_bdd(tVarId index,
		 const Bdd& chd0,
		 const Bdd& chd1)
{
  return mImpl->make_bdd(index, chd0.root(), chd1.root());
}

// ベクタを真理値表と見なしてBDDを作る．
// 変数は 0 〜 (ni - 1) とみなす．
// ベクタの値は非ゼロを true とみなす．
// v の大きさは 2^ni に等しくなければならない．
Bdd
BddMgr::tvec_to_bdd(const vector<int>& v,
		    tVarSize ni)
{
  VarVector vv(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vv[i] = i;
  }
  return tvec_to_bdd(v, vv);
}

// ベクタを真理値表と見なしてBDDを作る．
// こちらは個々の変数を vars で指定したもの．
// ベクタの値は非ゼロを true とみなす．
// v の大きさは 2^(vars.size()) に等しくなければならない．
Bdd
BddMgr::tvec_to_bdd(const vector<int>& v,
		    const VarVector& vars)
{
  return mImpl->tvec_to_bdd(v, vars);
#if DEBUG_MAKE_BDD
  ymuint ni = vars.size();
  ymuint nv = (1 << ni);
  Bdd ans0(ans, mImpl);
  BddVector varbdds(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    tVarId vid = vars[i];
    varbdds[i] = make_posiliteral(vid);
  }

  Bdd ans1 = make_zero();
  for (ymuint p = 0; p < nv; ++ p) {
    if ( v[p] ) {
      Bdd minterm = make_one();
      for (ymuint i = 0; i < ni; ++ i) {
	if ( p & (1 << (ni - i - 1)) ) {
	  minterm &= varbdds[i];
	}
	else {
	  minterm &= ~varbdds[i];
	}
      }
      ans1 |= minterm;
    }
  }
  assert_cond(ans0 == ans1, __FILE__, __LINE__);
  return ans0;
#endif
}

// Expr から対応するBDDを作り出す．
// 論理式中に現れるリテラルを置き換えるBDDを varmap に入れる．
// varmap に登録されていないリテラルはそのまま使う．
Bdd
BddMgr::expr_to_bdd(const LogExpr& expr,
		    const VarBddMap& varmap)
{
  hash_map<tVarId, tBddEdge> vemap;
  for (VarBddMap::const_iterator p = varmap.begin();
       p != varmap.end(); ++ p) {
    tVarId id = p->first;
    Bdd bdd = p->second;
    if ( mImpl != bdd.mMgr || bdd.is_error() ) {
      return make_error();
    }
    if ( bdd.is_overflow() ) {
      return make_overflow();
    }
    vemap.insert(make_pair(id, bdd.root()));
  }
  return mImpl->expr_to_bdd(expr, vemap);
}

// LogExpr から対応するBDDを作り出す．
// 論理式中に現れるリテラルを置き換えるリテラル番号を varmap に入れる．
// varmap に登録されていないリテラルはそのまま使う．
Bdd
BddMgr::expr_to_bdd(const LogExpr& expr,
		    const VarVarMap& varmap)
{
  hash_map<tVarId, tBddEdge> vemap;
  for (VarVarMap::const_iterator p = varmap.begin();
       p != varmap.end(); ++ p) {
    tVarId id = p->first;
    tVarId id2 = p->second;
    tBddEdge tmp = mImpl->make_posiliteral(id2);
    vemap.insert(make_pair(id, tmp));
  }
  return mImpl->expr_to_bdd(expr, vemap);
}

// ストリームを読んで論理式を作り,そこからBDDを作る．
// エラーが起きたら err_msg をセットしてエラーBDDを返す．
Bdd
BddMgr::expr_to_bdd(istream& in,
		    string& err_msg)
{
  LogExpr expr = LogExpr::stream_to_expr(in, err_msg);
  if ( err_msg == string() ) {
    return expr_to_bdd(expr);
  }
  return make_error();
}

// 文字列を読んで論理式を作り,そこからBDDを作る．
// エラーが起きたら err_msg をセットしてエラーBDDを返す．
Bdd
BddMgr::expr_to_bdd(const char* s,
		    string& err_msg)
{
  istringstream in(s);
  return expr_to_bdd(in, err_msg);
}

// 文字列を読んで論理式を作り,そこからBDDを作る．
// エラーが起きたら err_msg をセットしてエラーBDDを返す．
Bdd
BddMgr::expr_to_bdd(const string& s,
		    string& err_msg)
{
  istringstream in(s.c_str());
  return expr_to_bdd(in, err_msg);
}

BEGIN_NONAMESPACE
// make_thfunc() 中で使われるサブルーティン
inline
ymuint
elem(ymuint i,
     ymuint j,
     ymuint th)
{
  return i * (th + 1) + j;
}
END_NONAMESPACE

// 閾値関数を作る．
Bdd
BddMgr::make_thfunc(tVarSize n,
		    tVarSize th)
{
  vector<tBddEdge> table((n + 1) * (th + 1));
  table[elem(n, 0, th)] = kEdge1;
  for (ymuint i = 1; i <= th; i ++) {
    table[elem(n, i, th)] = kEdge0;
  }
  for (ymuint i = n; i -- > 0; ) {
    table[elem(i, 0, th)] = kEdge1;
    for (ymuint j = 1; j <= th; j ++) {
      if ( j > n - i ) {
	table[elem(i, j, th)] = kEdge0;
      }
      else {
	tBddEdge l = table[elem(i + 1, j, th)];
	tBddEdge h = table[elem(i + 1, j - 1, th)];
	tBddEdge tmp = mImpl->make_bddedge(i, l, h);
	if ( check_overflow(tmp) ) {
	  return make_overflow();
	}
	table[elem(i, j, th)] = tmp;
      }
    }
  }
  tBddEdge ans = table[elem(0, th, th)];
  return Bdd(mImpl, ans);
}

// 複数のBDDの論理積を求める．
Bdd
BddMgr::and_op(const BddVector& bdds)
{
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mImpl->and_op(edge_list);
  return Bdd(mImpl, ans);
}

Bdd
BddMgr::and_op(const BddList& bdds)
{
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mImpl->and_op(edge_list);
  return Bdd(mImpl, ans);
}

// 複数のBDDの論理和を求める．
Bdd
BddMgr::or_op(const BddVector& bdds)
{
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mImpl->or_op(edge_list);
  return Bdd(mImpl, ans);
}

Bdd
BddMgr::or_op(const BddList& bdds)
{
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mImpl->or_op(edge_list);
  return Bdd(mImpl, ans);
}

// 複数のBDDの排他的論理和を求める．
Bdd
BddMgr::xor_op(const BddVector& bdds)
{
  list<tBddEdge> edge_list;
  for (BddVector::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mImpl->xor_op(edge_list);
  return Bdd(mImpl, ans);
}

Bdd
BddMgr::xor_op(const BddList& bdds)
{
  list<tBddEdge> edge_list;
  for (BddList::const_iterator p = bdds.begin(); p != bdds.end(); ++p) {
    edge_list.push_back((*p).root());
  }
  tBddEdge ans = mImpl->xor_op(edge_list);
  return Bdd(mImpl, ans);
}

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
BddMgr::new_var(tVarId varid)
{
  return mImpl->new_var(varid);
}

// 現在登録されている変数をそのレベルの昇順で返す．
tVarSize
BddMgr::var_list(list<tVarId>& vlist) const
{
  return mImpl->var_list(vlist);
}

// 変数番号からレベルを得る．
// もしもレベルが割り当てられていない場合にはエラーとなる．
tLevel
BddMgr::level(tVarId varid) const
{
  return mImpl->level(varid);
}

// レベルから変数番号を得る．
tVarId
BddMgr::varid(tLevel level) const
{
  return mImpl->varid(level);
}

// 動的変数順変更を許可する．
void
BddMgr::enable_DVO()
{
  mImpl->enable_DVO();
}

// 動的変数順変更を禁止する．
void
BddMgr::disable_DVO()
{
  mImpl->disable_DVO();
}

// @brief ガーベージコレクションを許可する．
void
BddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief ガーベージコレクションを禁止する．
void
BddMgr::disable_gc()
{
  mImpl->disable_gc();
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
BddMgr::gc(bool shrink_nodetable)
{
  mImpl->gc(shrink_nodetable);
}

// GC 前の sweep 処理を行うためのバインダーを登録する．
void
BddMgr::reg_sweep_binder(EventBinder* binder)
{
  mImpl->reg_sweep_binder(binder);
}

// ログ出力用のストリームを設定する．
void
BddMgr::set_logstream(ostream& s)
{
  mImpl->set_logstream(s);
}

// ログ出力用のストリームを解除する．
void
BddMgr::unset_logstream()
{
  mImpl->unset_logstream();
}

// ログ出力用のストリームを得る．
ostream&
BddMgr::logstream() const
{
  return mImpl->logstream();
}

// パラメータを設定する．設定したい項目のマスクビットを1とする．
void
BddMgr::param(const BddMgrParam& param,
	      ymuint32 mask)
{
  mImpl->param(param, mask);
}

// パラメータを取得する．
void
BddMgr::param(BddMgrParam& param) const
{
  mImpl->param(param);
}

// 名前を得る．
const string&
BddMgr::name() const
{
  return mImpl->name();
}

// 使用メモリ量(in bytes)を得る．
ymuint64
BddMgr::used_mem() const
{
  return mImpl->used_mem();
}

// 節点テーブルに登録されているノードの数を得る．
ymuint64
BddMgr::node_num() const
{
  return mImpl->node_num();
}

// GC で回収される(フリーになる)ノード数を得る．
ymuint64
BddMgr::garbage_num() const
{
  return mImpl->garbage_num();
}

// 利用可能なフリーノード数を得る．
ymuint64
BddMgr::avail_num() const
{
  return mImpl->avail_num();
}

// GC の起動された回数を得る．
ymuint64
BddMgr::gc_count() const
{
  return mImpl->gc_count();
}

// @brief ダンプされた情報を BDD を読み込む．
// @param[in] s 入力ストリーム
// @return 読み込まれた BDD
Bdd
BddMgr::restore(istream& s)
{
  Restorer restorer(mImpl, s);
  ymuint n = restorer.read();
  if ( n != 1 ) {
    // エラーもしくは複数の BDD データだった．
    return Bdd(mImpl, kEdgeError);
  }
  else {
    return Bdd(mImpl, restorer.root(0));
  }
}

// @brief ダンプされた情報を BDD ベクタに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の BDD ベクタ
void
BddMgr::restore(istream& s,
		BddVector& array)
{
  Restorer restorer(mImpl, s);
  ymuint n = restorer.read();
  array.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    array[i] = Bdd(mImpl, restorer.root(i));
  }
}

// @brief ダンプされた情報を BDD リストに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の BDD リスト
void
BddMgr::restore(istream& s,
		BddList& array)
{
  Restorer restorer(mImpl, s);
  ymuint n = restorer.read();
  for (ymuint i = 0; i < n; ++ i) {
    array.push_back(Bdd(mImpl, restorer.root(i)));
  }
}

END_NAMESPACE_YM_BDD
