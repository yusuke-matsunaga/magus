
/// @file libym_logic/bdd/base/BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddMgr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"

#include "ym_logic/Bdd.h"
#include "bmc/BddMgrClassic.h"


BEGIN_NAMESPACE_YM_BDD

// @brief マークを返す．
bool
BddEdge::mark() const
{
  BddNode* node = get_node();
  if ( pol() == kPolPosi ) {
    return node->pmark();
  }
  else {
    return node->nmark();
  }
}

// @brief マークをつける．
void
BddEdge::set_mark()
{
  BddNode* node = get_node();
  if ( pol() == kPolPosi ) {
    node->pmark(1);
  }
  else {
    node->nmark(1);
  }
}

// @brief マークを消す．
void
BddEdge::clear_mark()
{
  BddNode* node = get_node();
  if ( node &&
       ( node->pmark() || node->nmark() ) ) {
    node->rst_mark();
    node->edge0().clear_mark();
    node->edge1().clear_mark();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス BddMgrImpl
//////////////////////////////////////////////////////////////////////

BddMgrImpl* BddMgrImpl::mDefaultMgr = NULL;

// デフォルトマネージャを返すクラスメソッド
BddMgrImpl*
BddMgrImpl::default_mgr()
{
  if ( !mDefaultMgr ) {
    mDefaultMgr = new BddMgrClassic("default manager");
    assert_cond(mDefaultMgr, __FILE__, __LINE__);
  }
  return mDefaultMgr;
}

// コンストラクタ
BddMgrImpl::BddMgrImpl()
{
  mRefCount = 0;

  // ログ出力用ストリームの初期化
  mNullStream = new ofstream("/dev/null", ios::out);
  if ( !mNullStream->is_open() ) {
    cerr << "Panic! : could not open \"/dev/null\"" << endl;
    abort();
  }
  mLogFp = mNullStream;

  // BDD リストの初期化
  mTopBdd = NULL;
}

// デストラクタ
BddMgrImpl::~BddMgrImpl()
{
  assert_cond(this != mDefaultMgr, __FILE__, __LINE__);

  // BDD の解放
  // と言っても Bdd のオブジェクトを削除するわけには行かないので
  // デフォルトマネージャのエラーBDDにすり替える．
  if ( mTopBdd ) {
    Bdd* last = NULL;
    for (Bdd* bdd = mTopBdd; bdd; bdd = bdd->mNext) {
      bdd->mRoot = BddEdge::make_error();
      bdd->mMgr = mDefaultMgr;
      last = bdd;
    }
    Bdd* first = mDefaultMgr->mTopBdd;
    mDefaultMgr->mTopBdd = mTopBdd;
    mTopBdd->mPrev = NULL;
    last->mNext = first;
    if ( first ) {
      first->mPrev = last;
    }
  }

  // /dev/null ストリームの破壊
  delete mNullStream;
}

// log用ストリームを設定する．
void
BddMgrImpl::set_logstream(ostream& s)
{
  mLogFp = &s;
}

// log用ストリームを解除する．
void
BddMgrImpl::unset_logstream()
{
  mLogFp = mNullStream;
}

// log用ファイルポインタを読み出す．
ostream&
BddMgrImpl::logstream() const
{
  return *mLogFp;
}

// ベクタを真理値表と見なしてBDDを作る．
// 個々の変数を vars で指定する．
// ベクタの値は非ゼロを true とみなす．
// v の大きさは 2^(vars.size()) に等しくなければならない．
BddEdge
BddMgrImpl::tvec_to_bdd(const vector<int>& v,
			const VarVector& vars)
{
  ymuint ni = vars.size();
  ymuint nv = (1 << ni);
  if ( nv != v.size() ) {
    return BddEdge::make_error();
  }

  // 変数の生成
  vector<BddEdge> var_vector;
  for (VarVector::const_iterator p = vars.begin(); p != vars.end(); ++ p) {
    VarId varid = *p;
    var_vector.push_back(make_posiliteral(varid));
  }
  ymuint size = v.size();
  return tvec_sub(v, 0, size, var_vector, 0);
}

// 真理値表からBDDを作るためのサブルーティン
BddEdge
BddMgrImpl::tvec_sub(const vector<int>& v,
		     ymuint32 top,
		     ymuint32 size,
		     const vector<BddEdge>& var_vector,
		     ymuint var_idx)
{
  assert_cond(size > 0, __FILE__, __LINE__);
  if ( size == 1 ) {
    if ( v[top] ) {
      return BddEdge::make_one();
    }
    else {
      return BddEdge::make_zero();
    }
  }
  else {
    size_t size1 = size >> 1;
    BddEdge l = tvec_sub(v, top, size1, var_vector, var_idx + 1);
    BddEdge h = tvec_sub(v, top + size1, size1, var_vector, var_idx + 1);
    return ite_op(var_vector[var_idx], h, l);
  }
}

// ノードをロックする．
// もし，子孫のノードがアンロックの状態ならばロックする
// 無駄な関数呼び出しを避けるため，この関数を呼び出す前に，
// このノードがロックされていないことを確認してあるものとする．
void
BddMgrImpl::lockall(BddNode* vp)
{
  lock_hook(vp);
  activate(vp->edge0());
  activate(vp->edge1());
}

// ノードをアンロックする．
// もし，子孫のノードがこのノード以外から参照されていない場合には，
// そのノードもアンロックする
// 無駄な関数呼び出しを避けるため，この関数を呼び出す前に，
// このノードがロックされていたことは確認してあるものとする．
void
BddMgrImpl::unlockall(BddNode* vp)
{
  unlock_hook(vp);
  deactivate(vp->edge0());
  deactivate(vp->edge1());
}

END_NAMESPACE_YM_BDD
