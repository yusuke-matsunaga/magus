
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"

#include "ym_logic/Bdd.h"
#include "../bmc/BddMgrClassic.h"
#include "../bmm/BddMgrModern.h"

#include "AndOp.h"
#include "XorOp.h"
#include "IntsecOp.h"
#include "IteOp.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// 1K = 1,024
const ymuint64 K_unit = (1 << 10);
// 1M = 1,024 x 1,024
const ymuint64 M_unit = (1 << 20);

// パラメータのデフォルト値
const double DEFAULT_GC_THRESHOLD    = 0.10;
const ymuint64 DEFAULT_GC_NODE_LIMIT =  64 * K_unit;
const double DEFAULT_NT_LOAD_LIMIT   = 2.0;
const double DEFAULT_RT_LOAD_LIMIT   = 0.8;
const ymuint64 DEFAULT_MEM_LIMIT     = 400 * M_unit;
const ymuint64 DEFAULT_DZONE         =  10 * M_unit;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BddEdge
//////////////////////////////////////////////////////////////////////

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

// @brief 参照されていない時にtrueを返す
bool
BddEdge::noref() const
{
  BddNode* node = get_node();
  return node->noref();
}

// @brief マークを消す．
void
clear_mark(BddEdge e)
{
  for ( ; ; ) {
    BddNode* node = e.get_node();
    if ( !node ||
	 ( !node->pmark() && !node->nmark() ) ) {
      break;
    }
    node->rst_mark();
    clear_mark(node->edge0());
    e = node->edge1();
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

// @brief マネージャを生成する
// @param[in] type マネージャの型を表す文字列
// @param[in] name マネージャの名前
// @param[in] option オプション文字列
BddMgrImpl*
BddMgrImpl::new_mgr(const string& type,
		    const string& name,
		    const string& option)
{
  BddMgrImpl* impl = NULL;

  if ( type == "bmc" ) {
    impl = new BddMgrClassic(name, option);
  }
  else if ( type == "bmm" ) {
    impl = new BddMgrModern(name, option);
  }
  else {
    impl = new BddMgrClassic(name, option);
  }
  return impl;
}

// @brief コンストラクタ
// @param[in] name マネージャの名前
BddMgrImpl::BddMgrImpl(const string& name) :
  mAlloc(4096)
{
  if ( name == string() ) {
    // 適当な名前を付ける．
    static int num = 1;
    ostringstream s;
    s << "bdd_mgr#" << num ++;
    mName = s.str();
  }

  // ユーザー設定可能パラメータのデフォルト値を設定
  mGcThreshold = DEFAULT_GC_THRESHOLD;
  mGcNodeLimit = DEFAULT_GC_NODE_LIMIT;
  mNtLoadLimit = DEFAULT_NT_LOAD_LIMIT;
  mRtLoadLimit = DEFAULT_RT_LOAD_LIMIT;
  mDangerousZone = DEFAULT_DZONE;
  mGcEnable = 0;

  mAlloc.set_mem_limit(DEFAULT_MEM_LIMIT);

  // ログ出力用ストリームの初期化
  mNullStream = new ofstream("/dev/null", ios::out);
  if ( !mNullStream->is_open() ) {
    cerr << "Panic! : could not open \"/dev/null\"" << endl;
    abort();
  }
  mLogFp = mNullStream;

  // BDD リストの初期化
  mTopBdd = NULL;

  mOverflow = false;

  mAndOp = new AndOp(this);
  mXorOp = new XorOp(this);
  mIntsecOp = new IntsecOp(this);
  mIteOp = new IteOp(this, mAndOp, mXorOp);

  mOpList.push_back(mAndOp);
  mOpList.push_back(mXorOp);
  mOpList.push_back(mIntsecOp);
  mOpList.push_back(mIteOp);
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

  // 演算オブジェクトの解放
  for (list<BddOp*>::iterator p = mOpList.begin();
       p != mOpList.end(); ++ p) {
    delete *p;
  }

  // /dev/null ストリームの破壊
  delete mNullStream;
}

// 肯定のリテラル関数を作る
BddEdge
BddMgrImpl::make_posiliteral(VarId varid)
{
  bool stat = new_var(varid);
  if ( !stat ) {
    return BddEdge::make_overflow();
  }
  else {
    return new_node(level(varid), BddEdge::make_zero(), BddEdge::make_one());
  }
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

// @brief e1 & e2 を計算する．
// @param[in] e1, e2 演算対象の枝
// @return 演算結果を返す．
BddEdge
BddMgrImpl::and_op(BddEdge e1,
		   BddEdge e2)
{
  return mAndOp->apply(e1, e2);
}

// @brief src1 ^ src2 を計算する．
// @param[in] e1, e2 演算対象の枝
// @return 演算結果を返す．
BddEdge
BddMgrImpl::xor_op(BddEdge e1,
		   BddEdge e2)
{
  return mXorOp->apply(e1, e2);
}

// @brief e1 と e2 の共通部分があれば kEdge1 を返す．
// @param[in] e1, e2 演算対象の枝
// @return 演算結果を返す．
BddEdge
BddMgrImpl::check_intersect(BddEdge e1,
			    BddEdge e2)
{
  return mIntsecOp->apply(e1, e2);
}

// @brief if-then-else 演算を計算する．
// @param[in] e1, e2, e3 演算対象の枝
// @return 演算結果を返す．
BddEdge
BddMgrImpl::ite_op(BddEdge e1,
		   BddEdge e2,
		   BddEdge e3)
{
  return mIteOp->apply(e1, e2, e3);
}

// @brief パラメータを設定する．
// @param[in] param パラメータ
// @param[in] mask 設定する項目を指定するマスク
void
BddMgrImpl::param(const BddMgrParam& param,
		  ymuint32 mask)
{
  if ( mask & BddMgrParam::GC_THRESHOLD ) {
    mGcThreshold = param.mGcThreshold;
  }
  if ( mask & BddMgrParam::GC_NODE_LIMIT ) {
    mGcNodeLimit = param.mGcNodeLimit;
  }
  if ( mask & BddMgrParam::NT_LOAD_LIMIT ) {
    mNtLoadLimit = param.mNtLoadLimit;
#if 0
    set_next_limit_size();
#endif
  }
  if ( mask & BddMgrParam::RT_LOAD_LIMIT ) {
    mRtLoadLimit = param.mRtLoadLimit;
  }
  if ( mask & BddMgrParam::MEM_LIMIT ) {
    mAlloc.set_mem_limit(param.mMemLimit);
  }
}

// @brief パラメータを取得する．
// @param[out] param 結果を格納する変数
void
BddMgrImpl::param(BddMgrParam& param) const
{
  param.mGcThreshold = gc_threshold();
  param.mGcNodeLimit = gc_node_limit();
  param.mNtLoadLimit = nt_load_limit();
  param.mRtLoadLimit = rt_load_limit();
  param.mMemLimit = mem_limit();
}

// @brief ガーベージコレクションを許可する．
void
BddMgrImpl::enable_gc()
{
  if ( mGcEnable > 0 ) {
    -- mGcEnable;
  }
}

// @brief ガーベージコレクションを禁止する．
void
BddMgrImpl::disable_gc()
{
  ++ mGcEnable;
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

// @brief このマネージャで使用するメモリ領域を確保する．
// @param[in] size サイズ
void*
BddMgrImpl::allocate(ymuint64 size)
{
  return mAlloc.get_memory(size);
}

// @brief このマネージャで確保したメモリを解放する．
// @param[in] ptr 解放するメモリのアドレス
// @param[in] size サイズ
void
BddMgrImpl::deallocate(void* ptr,
		       ymuint64 size)
{
  mAlloc.put_memory(size, ptr);
}

END_NAMESPACE_YM_BDD
