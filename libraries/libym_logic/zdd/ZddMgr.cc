
/// @file ZddMgr.cc
/// @brief ZddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/ZddMgr.h"

#include "ZddMgrImpl.h"
#include "Dumper.h"

#include "ym_utils/HeapTree.h"


BEGIN_NAMESPACE_YM_ZDD

ZddMgr ZddMgr::theDefaultMgr;

BEGIN_NONAMESPACE

// 入力数がこの値よりも小さかったらヒープ木は使わない．
const ymuint kNiLimit = 8;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス ZddMgr
//////////////////////////////////////////////////////////////////////

// @brief デフォルトマネージャを返す．
ZddMgr&
ZddMgr::default_mgr()
{
  return theDefaultMgr;
}

// @brief デフォルトマネージャを作るためのコンストラクタ
ZddMgr::ZddMgr() :
  mImpl(ZddMgrImpl::default_mgr())
{
}

// @brief コンストラクタ
// @param[in] type ZddMgr の型を表す文字列
// @param[in] name マネージャの名前
// @param[in] option オプション文字列
ZddMgr::ZddMgr(const string& type,
	       const string& name,
	       const string& option)
{
  mImpl = new ZddMgrImpl(name, option);
  assert_cond(mImpl, __FILE__, __LINE__);
}

// デストラクタ
ZddMgr::~ZddMgr()
{
  if ( mImpl != ZddMgrImpl::default_mgr() ) {
    // デフォルトマネージャは削除しない．
    delete mImpl;
  }
}

// 定数0を表すZDDを作る．
Zdd
ZddMgr::make_zero()
{
  return Zdd(mImpl, ZddEdge::make_zero());
}

// 定数1を表すZDDを作る．
Zdd
ZddMgr::make_one()
{
  return Zdd(mImpl, ZddEdge::make_one());
}

// エラーを表すZDDを作る．
Zdd
ZddMgr::make_error()
{
  return Zdd(mImpl, ZddEdge::make_error());
}

// オーバーフローを表すZDDを作る．
Zdd
ZddMgr::make_overflow()
{
  return Zdd(mImpl, ZddEdge::make_overflow());
}

#if 0
// singletonを表すZDDを作る．
Zdd
ZddMgr::make_singleton(ymuint32 varid)
{
  ZddEdge ans = mImpl->make_singleton(varid);
  return Zdd(mImpl, ans);
}

// 指定されたインデックスを持つ中間ノードを一つ持つZDDを作る
// その左右の子のデフォルトはそれぞれ0と1
// メモリ不足によってエラーが起きる可能性がある．
Zdd
ZddMgr::make_zdd(ymuint32 index,
		 const Zdd& chd0,
		 const Zdd& chd1)
{
  return Zdd(mImpl, mImpl->make_zdd(index, chd0.root(), chd1.root()));
}
#endif

// 複数のZDDのintersectionを求める．
Zdd
ZddMgr::cap_op(const ZddVector& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return make_one();
  }
  if ( n == 1 ) {
    return zdds[0];
  }
  if ( n == 2 ) {
    return zdds[0] & zdds[1];
  }

  ZddEdge ans_e;
  if ( n == 3 ) {
    ZddEdge e0(zdds[0].mRoot);
    ZddEdge e1(zdds[1].mRoot);
    ZddEdge e2(zdds[2].mRoot);
    ans_e = mImpl->cap_op(e0, e1, e2);
  }
  else if ( n < kNiLimit ) {
    ZddVector::const_iterator p = zdds.begin();
    ans_e = ZddEdge(p->mRoot);
    for (++ p; p != zdds.end(); ++ p) {
      ans_e = mImpl->cap_op(ans_e, ZddEdge(p->mRoot));
    }
  }
  else {
    SimpleHeapTree<ZddEdge> work;
    for (ZddVector::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      ZddEdge e(p->mRoot);
      work.put(e, mImpl->size(e));
    }
    ans_e = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans_e = mImpl->cap_op(ans_e, work.getmin());
      ans_e = work.xchgmin(ans_e, mImpl->size(ans_e));
    }
  }
  return Zdd(mImpl, ans_e);
}

Zdd
ZddMgr::cap_op(const ZddList& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return make_one();
  }
  if ( n == 1 ) {
    return zdds.front();
  }
  if ( n == 2 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd zdd0 = *p;
    ++ p;
    Zdd zdd1 = *p;
    return zdd0 & zdd1;
  }

  ZddEdge ans_e;
  if ( n == 3 ) {
    ZddList::const_iterator p = zdds.begin();
    ZddEdge e0(p->mRoot);
    ++ p;
    ZddEdge e1(p->mRoot);
    ++ p;
    ZddEdge e2(p->mRoot);
    ans_e = mImpl->cap_op(e0, e1, e2);
  }
  else if ( n < kNiLimit ) {
    ZddList::const_iterator p = zdds.begin();
    ans_e = ZddEdge(p->mRoot);
    for (++ p; p != zdds.end(); ++ p) {
      ans_e = mImpl->cap_op(ans_e, ZddEdge(p->mRoot));
    }
  }
  else {
    SimpleHeapTree<ZddEdge> work;
    for (ZddList::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      ZddEdge e(p->mRoot);
      work.put(e, mImpl->size(e));
    }
    ans_e = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans_e = mImpl->cap_op(ans_e, work.getmin());
      ans_e = work.xchgmin(ans_e, mImpl->size(ans_e));
    }
  }
  return Zdd(mImpl, ans_e);
}

// 複数のZDDのunionを求める．
Zdd
ZddMgr::union_op(const ZddVector& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return make_zero();
  }
  if ( n == 1 ) {
    return zdds[0];
  }
  if ( n == 2 ) {
    return zdds[0] | zdds[1];
  }

  ZddEdge ans_e;
  if ( n == 3 ) {
    ZddEdge e0(zdds[0].mRoot);
    ZddEdge e1(zdds[1].mRoot);
    ZddEdge e2(zdds[2].mRoot);
    ans_e = mImpl->cup_op(e0, e1, e2);
  }
  else if ( n < kNiLimit ) {
    ZddVector::const_iterator p = zdds.begin();
    ans_e = ZddEdge(p->mRoot);
    for (++ p; p != zdds.end(); ++ p) {
      ans_e = mImpl->cup_op(ans_e, ZddEdge(p->mRoot));
    }
  }
  else {
    SimpleHeapTree<ZddEdge> work;
    for (ZddVector::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      ZddEdge e(p->mRoot);
      work.put(e, mImpl->size(e));
    }
    ans_e = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans_e = mImpl->cup_op(ans_e, work.getmin());
      ans_e = work.xchgmin(ans_e, mImpl->size(ans_e));
    }
  }
  return Zdd(mImpl, ans_e);
}

Zdd
ZddMgr::union_op(const ZddList& zdds)
{
  ymuint n = zdds.size();
  if ( n == 0 ) {
    return make_zero();
  }
  if ( n == 1 ) {
    return zdds.front();
  }
  if ( n == 2 ) {
    ZddList::const_iterator p = zdds.begin();
    Zdd zdd0 = *p;
    ++ p;
    Zdd zdd1 = *p;
    return zdd0 | zdd1;
  }

  ZddEdge ans_e;
  if ( n == 3 ) {
    ZddList::const_iterator p = zdds.begin();
    ZddEdge e0(p->mRoot);
    ++ p;
    ZddEdge e1(p->mRoot);
    ++ p;
    ZddEdge e2(p->mRoot);
    ans_e = mImpl->cup_op(e0, e1, e2);
  }
  else if ( n < kNiLimit ) {
    ZddList::const_iterator p = zdds.begin();
    ans_e = ZddEdge(p->mRoot);
    for (++ p; p != zdds.end(); ++ p) {
      ans_e = mImpl->cup_op(ans_e, ZddEdge(p->mRoot));
    }
  }
  else {
    SimpleHeapTree<ZddEdge> work;
    for (ZddList::const_iterator p = zdds.begin();
	 p != zdds.end(); ++p) {
      ZddEdge e(p->mRoot);
      work.put(e, mImpl->size(e));
    }
    ans_e = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans_e = mImpl->cup_op(ans_e, work.getmin());
      ans_e = work.xchgmin(ans_e, mImpl->size(ans_e));
    }
  }
  return Zdd(mImpl, ans_e);
}

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
ZddMgr::new_var(VarId varid)
{
  return mImpl->new_var(varid);
}

// 現在登録されている変数をそのレベルの昇順で返す．
ymuint
ZddMgr::var_list(list<VarId>& vlist) const
{
  return mImpl->var_list(vlist);
}

// 変数番号からレベルを得る．
// もしもレベルが割り当てられていない場合にはエラーとなる．
ymuint
ZddMgr::level(VarId varid) const
{
  return mImpl->level(varid);
}

// レベルから変数番号を得る．
VarId
ZddMgr::varid(ymuint level) const
{
  return mImpl->varid(level);
}

// @brief ガーベージコレクションを許可する．
void
ZddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief ガーベージコレクションを禁止する．
void
ZddMgr::disable_gc()
{
  mImpl->disable_gc();
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
ZddMgr::gc(bool shrink_nodetable)
{
  mImpl->gc(shrink_nodetable);
}

// GC 前の sweep 処理を行うためのバインダーを登録する．
void
ZddMgr::reg_sweep_binder(EventBinder* binder)
{
  mImpl->reg_sweep_binder(binder);
}

// ログ出力用のストリームを設定する．
void
ZddMgr::set_logstream(ostream& s)
{
  mImpl->set_logstream(s);
}

// ログ出力用のストリームを解除する．
void
ZddMgr::unset_logstream()
{
  mImpl->unset_logstream();
}

// ログ出力用のストリームを得る．
ostream&
ZddMgr::logstream() const
{
  return mImpl->logstream();
}

// パラメータを設定する．設定したい項目のマスクビットを1とする．
void
ZddMgr::param(const ZddMgrParam& param,
	      ymuint32 mask)
{
  mImpl->param(param, mask);
}

// パラメータを取得する．
void
ZddMgr::param(ZddMgrParam& param) const
{
  mImpl->param(param);
}

// 名前を得る．
const string&
ZddMgr::name() const
{
  return mImpl->name();
}

// 使用メモリ量(in bytes)を得る．
ymuint64
ZddMgr::used_mem() const
{
  return mImpl->used_mem();
}

// 節点テーブルに登録されているノードの数を得る．
ymuint64
ZddMgr::node_num() const
{
  return mImpl->node_num();
}

// GC で回収される(フリーになる)ノード数を得る．
ymuint64
ZddMgr::garbage_num() const
{
  return mImpl->garbage_num();
}

// 利用可能なフリーノード数を得る．
ymuint64
ZddMgr::avail_num() const
{
  return mImpl->avail_num();
}

// GC の起動された回数を得る．
ymuint64
ZddMgr::gc_count() const
{
  return mImpl->gc_count();
}

// @brief ダンプされた情報を ZDD を読み込む．
// @param[in] s 入力ストリーム
// @return 読み込まれた ZDD
Zdd
ZddMgr::restore(BinI& s)
{
  Restorer restorer(mImpl, s);
  ymuint n = restorer.read();
  if ( n != 1 ) {
    // エラーもしくは複数の ZDD データだった．
    return make_error();
  }
  else {
    return Zdd(mImpl, restorer.root(0));
  }
}

// @brief ダンプされた情報を ZDD ベクタに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の ZDD ベクタ
void
ZddMgr::restore(BinI& s,
		ZddVector& array)
{
  Restorer restorer(mImpl, s);
  ymuint n = restorer.read();
  array.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    array[i] = Zdd(mImpl, restorer.root(i));
  }
}

// @brief ダンプされた情報を ZDD リストに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の ZDD リスト
void
ZddMgr::restore(BinI& s,
		ZddList& array)
{
  Restorer restorer(mImpl, s);
  ymuint n = restorer.read();
  for (ymuint i = 0; i < n; ++ i) {
    array.push_back(Zdd(mImpl, restorer.root(i)));
  }
}

END_NAMESPACE_YM_ZDD
