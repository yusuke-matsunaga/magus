
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"

#include "ym_logic/Bdd.h"
#include "../bmc/BddMgrClassic.h"
#include "../bmm/BddMgrModern.h"

#include "AndOp.h"
#include "XorOp.h"
#include "IntsecOp.h"
#include "IteOp.h"
#include "CofOp.h"
#include "XcOp.h"
#include "GcOp.h"
#include "SymOp.h"
#include "CountOp.h"
#include "McOp.h"
#include "W0Op.h"
#include "W1Op.h"
#include "SupOp.h"
#include "SmoothOp.h"
#include "AeOp.h"


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
  if ( is_const() ) {
    return false;
  }
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
  else {
    mName = name;
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

  // 演算オブジェクトの生成
  mAndOp = new AndOp(this);
  mXorOp = new XorOp(this);
  mIntsecOp = new IntsecOp(this);
  mIteOp = new IteOp(this, mAndOp, mXorOp);
  mCofOp = new CofOp(this);
  mXcOp = new XcOp(this, mXorOp);
  mGcOp = new GcOp(this);
  mSymOp = new SymOp(this);
  mCountOp = new CountOp(this);
  mMcOp = new McOp(this);
  mW0Op = new W0Op(this);
  mW1Op = new W1Op(this);
  mSupOp = new SupOp(this);
  mSmoothOp = new SmoothOp(this, mAndOp);
  mAeOp = new AeOp(this, mAndOp, mSmoothOp);
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

// @brief 一つの変数に対する cofactor を計算する．
// @param[in] e 演算対象の枝
// @param[in] id 展開対象の変数番号
// @param[in] pol 極性
// @return 演算結果を返す．
BddEdge
BddMgrImpl::scofactor(BddEdge e,
		      VarId id,
		      tPol pol)
{
  return mCofOp->apply(e, id, pol);
}

// @brief Davio展開のモーメント項を求める処理
// @param[in] e 演算対象の枝
// @param[in] idx 展開を行う変数番号
// @return 演算結果を返す．
// @note モーメント項とは $f_{\overline{x}} \oplus f_x$ のこと．
BddEdge
BddMgrImpl::xor_moment(BddEdge e,
		       VarId idx)
{
  return mXcOp->apply(e, idx);
}

// @brief generalized cofactor を計算する．
// @param[in] e1, e2 演算対象の枝
// @return 演算結果を返す．
BddEdge
BddMgrImpl::gcofactor(BddEdge e1,
		      BddEdge e2)
{
  return mGcOp->apply(e1, e2);
}

// @brief smoothing(elimination)
// @param[in] e 演算対象の枝
// @param[in] s 消去対象の変数リストを表す枝
// @return 演算結果を返す．
// @note s に含まれる変数を消去する．
BddEdge
BddMgrImpl::esmooth(BddEdge e,
		    BddEdge s)
{
  return mSmoothOp->apply(e, s);
}

// @brief src1 と src2 の論理積を計算して src3 の変数を消去する．
// @param[in] e1, e2, e3 演算対象の枝
// @return 演算結果を返す．
BddEdge
BddMgrImpl::and_exist(BddEdge e1,
		      BddEdge e2,
		      BddEdge e3)
{
  return mAeOp->apply(e1, e2, e3);
}

// bdd が正リテラルのみのキューブの時，真となる．
bool
BddMgrImpl::check_posi_cube(BddEdge e)
{
  // エラーやオーバーフローの時は false を返す．
  if ( e.is_invalid() ) {
    return false;
  }

  // 定数0の場合も false かな？
  if ( e.is_zero() ) {
    return false;
  }

  // 定数1の場合は true
  if ( e.is_one() ) {
    return true;
  }

  for ( ; ; ) {
    BddNode* vp = e.get_node();
    tPol pol = e.pol();
    BddEdge e0 = vp->edge0(pol);
    BddEdge e1 = vp->edge1(pol);
    if ( !e0.is_zero() || e1.is_zero() ) {
      return false;
    }
    if ( e1.is_one() ) {
      return true;
    }
    e = e1;
  }
}

// bdd がキューブの時, true となる．
bool
BddMgrImpl::check_cube(BddEdge e)
{
  // エラーやオーバーフローの時は false を返す．
  if ( e.is_invalid() ) {
    return false;
  }

  // 定数0の場合も false かな？
  if ( e.is_zero() ) {
    return false;
  }

  // 定数1の場合は true
  if ( e.is_one() ) {
    return true;
  }

  for ( ; ; ) {
    BddNode* vp = e.get_node();
    tPol pol = e.pol();
    BddEdge e0 = vp->edge0(pol);
    BddEdge e1 = vp->edge1(pol);
    if ( e0.is_zero() ) {
      e = e1;
    }
    else if ( e1.is_zero() ) {
      e = e0;
    }
    else {
      return false;
    }
    if ( e.is_one() ) {
      return true;
    }
  }
}

// @brief 変数 xと y が対称(交換可能)な時にtrueを返す．
// @param[in] e 演算対象の枝
// @param[in] x, y 変数番号
// @param[in] pol 極性
bool
BddMgrImpl::check_symmetry(BddEdge e,
			   VarId x,
			   VarId y,
			   tPol pol)
{
  return mSymOp->apply(e, x, y, pol);
}

// @brief edge_list に登録されたBDDのノード数を数える．
ymuint64
BddMgrImpl::node_count(const vector<BddEdge>& edge_list)
{
  return mCountOp->apply(edge_list);
}

// @brief BDD の表す論理関数の minterm の数を返す．
// @param[in] e 根の枝
// @param[in] nvar 論理関数の変数の数
// 無限長精度の整数(mpz_class)を用いて計算する．
mpz_class
BddMgrImpl::minterm_count(BddEdge e,
			  ymuint nvar)
{
  return mMcOp->apply(e, nvar);
}

// @brief Walsh 変換の0次の係数を計算する．
// @param[in] e 根の枝
// @param[in] nvar 論理関数の変数の数
mpz_class
BddMgrImpl::walsh0(BddEdge e,
		   ymuint nvar)
{
  return mW0Op->apply(e, nvar);
}

// @brief Walsh 変換の1次の係数を計算する．
// @param[in] e 根の枝
// @param[in] var 変数番号
// @param[in] nvar 論理関数の変数の数
mpz_class
BddMgrImpl::walsh1(BddEdge e,
		   VarId var,
		   ymuint nvar)
{
  return mW1Op->apply(e, level(var), nvar);
}

// @brief edge_list に登録されたBDDのサポートに印をつける．
// @param[in] edge_list 根の枝のリスト
// @return サポート数を返す．
ymuint
BddMgrImpl::mark_support(const vector<BddEdge>& edge_list)
{
  return mSupOp->apply(edge_list);
}

// @brief 印のついた変数をベクタに変換する．
// @param[out] support 結果を格納する変数
// @return サポート数を返す．
ymuint
BddMgrImpl::mark_to_vector(VarVector& support)
{
  return mSupOp->to_list(support);
}

// @brief 印のついた変数をリストに変換する．
// @param[out] support 結果を格納する変数
// @return サポート数を返す．
ymuint
BddMgrImpl::mark_to_list(VarList& support)
{
  return mSupOp->to_list(support);
}

// @brief 印のついた変数をBDD(キューブ)に変換する．
BddEdge
BddMgrImpl::mark_to_bdd()
{
  return mSupOp->to_bdd();
}

// @brief smallest cube containing F 演算
BddEdge
BddMgrImpl::SCC(BddEdge e)
{
  return mSupOp->scc(e);
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

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
BddMgrImpl::gc(bool shrink_nodetable)
{
  for (list<BddOp*>::iterator p = mOpList.begin();
       p != mOpList.end(); ++ p) {
    BddOp* op = *p;
    op->sweep();
  }

  _gc(shrink_nodetable);

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
