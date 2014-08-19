
/// @file ZddMgr.cc
/// @brief ZddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/ZddMgr.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

ZddMgr ZddMgr::theDefaultMgr;


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
ZddMgr::make_empty()
{
  return Zdd(mImpl, ZddEdge::make_zero());
}

// 定数1を表すZDDを作る．
Zdd
ZddMgr::make_base()
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

// @brief 2つの集合をマージする．
Zdd
ZddMgr::merge(const Zdd& left,
	      const Zdd& right)
{
  ZddEdge e1(left.mRoot);
  ZddEdge e2(right.mRoot);
  ZddEdge ans = mImpl->merge(e1, e2);
  return Zdd(mImpl, ans);
}

// @brief 2つの集合をマージする．
Zdd
ZddMgr::merge(const Zdd& left,
	      const Zdd& right,
	      ymuint limit)
{
  ZddEdge e1(left.mRoot);
  ZddEdge e2(right.mRoot);
  ZddEdge ans = mImpl->merge(e1, e2, limit);
  return Zdd(mImpl, ans);
}

// @brief 要素数を制限する．
Zdd
ZddMgr::n_element(const Zdd& left,
		  ymuint limit)
{
  ZddEdge e(left.mRoot);
  ZddEdge ans = mImpl->n_element(e, limit);
  return Zdd(mImpl, ans);
}

// @brief 極小集合のみにする．
Zdd
ZddMgr::make_minimal(const Zdd& left)
{
  ZddEdge e(left.mRoot);
  ZddEdge ans = mImpl->make_minimal(e);
  return Zdd(mImpl, ans);
}

// @brief 要素数最小の集合を一つ返す．
Zdd
ZddMgr::minimum_set(const Zdd& left)
{
  ZddEdge e(left.mRoot);
  ZddEdge ans = mImpl->minimum_set(e);
  return Zdd(mImpl, ans);
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

END_NAMESPACE_YM_ZDD
