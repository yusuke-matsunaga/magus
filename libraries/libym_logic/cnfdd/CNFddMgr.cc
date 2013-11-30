
/// @file CNFddMgr.cc
/// @brief CNFddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/CNFddMgr.h"
#include "CNFddMgrImpl.h"


BEGIN_NAMESPACE_YM_CNFDD

CNFddMgr CNFddMgr::theDefaultMgr;


//////////////////////////////////////////////////////////////////////
// クラス CNFddMgr
//////////////////////////////////////////////////////////////////////

// @brief デフォルトマネージャを返す．
CNFddMgr&
CNFddMgr::default_mgr()
{
  return theDefaultMgr;
}

// @brief デフォルトマネージャを作るためのコンストラクタ
CNFddMgr::CNFddMgr() :
  mImpl(CNFddMgrImpl::default_mgr())
{
}

// @brief コンストラクタ
// @param[in] type CNFddMgr の型を表す文字列
// @param[in] name マネージャの名前
// @param[in] option オプション文字列
CNFddMgr::CNFddMgr(const string& type,
	       const string& name,
	       const string& option)
{
  mImpl = new CNFddMgrImpl(name, option);
  assert_cond(mImpl, __FILE__, __LINE__);
}

// デストラクタ
CNFddMgr::~CNFddMgr()
{
  if ( mImpl != CNFddMgrImpl::default_mgr() ) {
    // デフォルトマネージャは削除しない．
    delete mImpl;
  }
}

// 定数0を表すCNFDDを作る．
CNFdd
CNFddMgr::make_empty()
{
  return CNFdd(mImpl, CNFddEdge::make_zero());
}

// 定数1を表すCNFDDを作る．
CNFdd
CNFddMgr::make_base()
{
  return CNFdd(mImpl, CNFddEdge::make_one());
}

// エラーを表すCNFDDを作る．
CNFdd
CNFddMgr::make_error()
{
  return CNFdd(mImpl, CNFddEdge::make_error());
}

// オーバーフローを表すCNFDDを作る．
CNFdd
CNFddMgr::make_overflow()
{
  return CNFdd(mImpl, CNFddEdge::make_overflow());
}

#if 0
// singletonを表すCNFDDを作る．
CNFdd
CNFddMgr::make_singleton(ymuint32 varid)
{
  CNFddEdge ans = mImpl->make_singleton(varid);
  return CNFdd(mImpl, ans);
}

// 指定されたインデックスを持つ中間ノードを一つ持つCNFDDを作る
// その左右の子のデフォルトはそれぞれ0と1
// メモリ不足によってエラーが起きる可能性がある．
CNFdd
CNFddMgr::make_zdd(ymuint32 index,
		 const CNFdd& chd0,
		 const CNFdd& chd1)
{
  return CNFdd(mImpl, mImpl->make_zdd(index, chd0.root(), chd1.root()));
}
#endif

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
CNFddMgr::new_var(VarId varid)
{
  return mImpl->new_var(varid);
}

// 現在登録されている変数をそのレベルの昇順で返す．
ymuint
CNFddMgr::var_list(list<VarId>& vlist) const
{
  return mImpl->var_list(vlist);
}

// 変数番号からレベルを得る．
// もしもレベルが割り当てられていない場合にはエラーとなる．
ymuint
CNFddMgr::level(VarId varid) const
{
  return mImpl->level(varid);
}

// レベルから変数番号を得る．
VarId
CNFddMgr::varid(ymuint level) const
{
  return mImpl->varid(level);
}

// @brief ガーベージコレクションを許可する．
void
CNFddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief ガーベージコレクションを禁止する．
void
CNFddMgr::disable_gc()
{
  mImpl->disable_gc();
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
CNFddMgr::gc(bool shrink_nodetable)
{
  mImpl->gc(shrink_nodetable);
}

// GC 前の sweep 処理を行うためのバインダーを登録する．
void
CNFddMgr::reg_sweep_binder(EventBinder* binder)
{
  mImpl->reg_sweep_binder(binder);
}

// ログ出力用のストリームを設定する．
void
CNFddMgr::set_logstream(ostream& s)
{
  mImpl->set_logstream(s);
}

// ログ出力用のストリームを解除する．
void
CNFddMgr::unset_logstream()
{
  mImpl->unset_logstream();
}

// ログ出力用のストリームを得る．
ostream&
CNFddMgr::logstream() const
{
  return mImpl->logstream();
}

// パラメータを設定する．設定したい項目のマスクビットを1とする．
void
CNFddMgr::param(const CNFddMgrParam& param,
	      ymuint32 mask)
{
  mImpl->param(param, mask);
}

// パラメータを取得する．
void
CNFddMgr::param(CNFddMgrParam& param) const
{
  mImpl->param(param);
}

// 名前を得る．
const string&
CNFddMgr::name() const
{
  return mImpl->name();
}

// 使用メモリ量(in bytes)を得る．
ymuint64
CNFddMgr::used_mem() const
{
  return mImpl->used_mem();
}

// 節点テーブルに登録されているノードの数を得る．
ymuint64
CNFddMgr::node_num() const
{
  return mImpl->node_num();
}

// GC で回収される(フリーになる)ノード数を得る．
ymuint64
CNFddMgr::garbage_num() const
{
  return mImpl->garbage_num();
}

// 利用可能なフリーノード数を得る．
ymuint64
CNFddMgr::avail_num() const
{
  return mImpl->avail_num();
}

// GC の起動された回数を得る．
ymuint64
CNFddMgr::gc_count() const
{
  return mImpl->gc_count();
}

END_NAMESPACE_YM_CNFDD
