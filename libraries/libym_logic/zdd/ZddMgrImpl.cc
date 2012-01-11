
/// @file ZddMgrImpl.cc
/// @brief ZddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"

#include "ym_logic/Zdd.h"
#include "ym_logic/ZddMgr.h"
#include "ZddVar.h"

#include "CapOp.h"
#include "CupOp.h"
#include "DiffOp.h"
#include "SwapOp.h"
#include "Cof0Op.h"
#include "Cof1Op.h"
#include "SupOp.h"
#include "MergeOp.h"


#if !defined(__SUNPRO_CC) || __SUNPRO_CC >= 0x500
#include <exception>
#include <new>
#else
#include <exception.h>
#endif


BEGIN_NAMESPACE_YM_ZDD

#if !defined(__SUNPRO_CC) || __SUNPRO_CC >= 0x500

typedef bad_alloc alloc_xcpt;

#else

typedef xalloc alloc_xcpt;

#endif


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

// 節点テーブルの初期サイズ
const ymuint64 INIT_SIZE = 1 * K_unit;

// 一度にアロケートするノード数
const ymuint64 NODE_UNIT = 1 * K_unit;

// 変数テーブルの初期サイズ
const ymuint64 VARTABLE_INIT_SIZE = 1 * K_unit;

// 節点テーブルのハッシュ関数
inline
ymuint64
hash_func3(ZddEdge id1,
	   ZddEdge id2,
	   ymuint level)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  ymuint64 id3 = level;
  return static_cast<ymuint64>(v1 + (v2 >> 2) + (id3 << 3) - id3);
}

// VarId 用のハッシュ関数
inline
ymuint64
var_hash(VarId var)
{
  ymuint64 key = var.val();
  return ((key * key) >> 8) + key;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス ZddEdge
//////////////////////////////////////////////////////////////////////

// @brief 参照されていない時にtrueを返す
bool
ZddEdge::noref() const
{
  ZddNode* node = get_node();
  return node->noref();
}


//////////////////////////////////////////////////////////////////////
// クラス ZddMgrImpl
//////////////////////////////////////////////////////////////////////

ZddMgrImpl* ZddMgrImpl::mDefaultMgr = NULL;

// デフォルトマネージャを返すクラスメソッド
ZddMgrImpl*
ZddMgrImpl::default_mgr()
{
  if ( !mDefaultMgr ) {
    mDefaultMgr = new ZddMgrImpl("default manager");
    assert_cond(mDefaultMgr, __FILE__, __LINE__);
  }
  return mDefaultMgr;
}

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] option オプション
ZddMgrImpl::ZddMgrImpl(const string& name,
		       const string& option) :
  mName(name)
{
  if ( name == string() ) {
    // 適当な名前を付ける．
    static int num = 1;
    ostringstream s;
    s << "zdd_mgr#" << num ++;
    mName = s.str();
  }

  mRefCount = 0;

  // ログ出力用ストリームの初期化
  mNullStream = new ofstream("/dev/null", ios::out);
  if ( !mNullStream->is_open() ) {
    cerr << "Panic! : could not open \"/dev/null\"" << endl;
    abort();
  }
  mLogFp = mNullStream;

  // ZDD リストの初期化
  mTopZdd = NULL;

  // ユーザー設定可能パラメータのデフォルト値を設定
  mGcThreshold = DEFAULT_GC_THRESHOLD;
  mGcNodeLimit = DEFAULT_GC_NODE_LIMIT;
  mNtLoadLimit = DEFAULT_NT_LOAD_LIMIT;
  mRtLoadLimit = DEFAULT_RT_LOAD_LIMIT;
  mMemLimit = DEFAULT_MEM_LIMIT;
  mDangerousZone = DEFAULT_DZONE;
  mGcEnable = 0;

  // メモリ管理用のメンバを初期化
  mFreeTop = NULL;
  mFreeNum = 0;
  mTopBlk = NULL;
  mCurBlk = NULL;
  mCurIdx = 1;
  mOverflow = false;

  // 内部情報の初期化
  mUsedMem = 0;
  mNodeNum = 0;
  mGarbageNum = 0;
  mGcCount = 0;

  // 節点テーブルの初期化
  mTableSize = 0;
  mTableSize_1 = 0;
  mNextLimit = 0;
  mNodeTable = NULL;
  resize(INIT_SIZE);

  // 変数テーブルの初期化
  mVarTableSize = VARTABLE_INIT_SIZE;
  mVarHashTable = alloc_vartable(mVarTableSize);
  assert_cond(mVarHashTable, __FILE__, __LINE__);
  mVarNum = 0;
  mVarTop = NULL;

  // 演算クラスの生成
  mCapOp = new CapOp(this);
  mCupOp = new CupOp(this);
  mDiffOp = new DiffOp(this);
  mSwapOp = new SwapOp(this);
  mCof0Op = new Cof0Op(this);
  mCof1Op = new Cof1Op(this);
  mSupOp = new SupOp(this);
  mMergeOp = new MergeOp(this, mCupOp);
}

// デストラクタ
ZddMgrImpl::~ZddMgrImpl()
{
  assert_cond(this != mDefaultMgr, __FILE__, __LINE__);

  // ZDD の解放
  // と言っても Zdd のオブジェクトを削除するわけには行かないので
  // デフォルトマネージャのエラーZDDにすり替える．
  if ( mTopZdd ) {
    Zdd* last = NULL;
    for (Zdd* bdd = mTopZdd; bdd; bdd = bdd->mNext) {
      bdd->mRoot = ZddEdge::make_error();
      bdd->mMgr = mDefaultMgr;
      last = bdd;
    }
    Zdd* first = mDefaultMgr->mTopZdd;
    mDefaultMgr->mTopZdd = mTopZdd;
    mTopZdd->mPrev = NULL;
    last->mNext = first;
    if ( first ) {
      first->mPrev = last;
    }
  }

  // /dev/null ストリームの破壊
  delete mNullStream;

  // 節点テーブルの解放
  dealloc_nodetable(mNodeTable, mTableSize);

  // 節点用のメモリブロックの解放
  for (ZddNode* blk = mTopBlk; blk; ) {
    ZddNode* temp = blk;
    blk = temp->mLink;
    dealloc_nodechunk(temp);
  }
  if ( mCurBlk ) {
    dealloc_nodechunk(mCurBlk);
  }

  // 変数の解放
  for (ZddVar* var = mVarTop; var; ) {
    ZddVar* next = var->mNext;
    delete var;
    var = next;
  }

  // 変数テーブルの解放
  dealloc_vartable(mVarHashTable, mVarTableSize);

  // 演算クラスの解放
  delete mCapOp;
  delete mCupOp;
  delete mDiffOp;
  delete mSwapOp;
  delete mCof0Op;
  delete mCof1Op;

  // このマネージャに関わるメモリはすべて解放したはず．
  assert_cond( mUsedMem == 0, __FILE__, __LINE__);

}

// log用ストリームを設定する．
void
ZddMgrImpl::set_logstream(ostream& s)
{
  mLogFp = &s;
}

// log用ストリームを解除する．
void
ZddMgrImpl::unset_logstream()
{
  mLogFp = mNullStream;
}

// log用ファイルポインタを読み出す．
ostream&
ZddMgrImpl::logstream() const
{
  return *mLogFp;
}

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
ZddMgrImpl::new_var(VarId varid)
{
  return alloc_var(varid) != NULL;
}

// 変数を確保する．
ZddVar*
ZddMgrImpl::alloc_var(VarId varid)
{
  if ( mVarTableSize == mVarNum ) {
    ZddVar** old_table = mVarHashTable;
    ymuint64 old_size = mVarTableSize;
    ymuint64 new_size = mVarTableSize << 1;
    ZddVar** new_hash = alloc_vartable(new_size);
    if ( !new_hash ) {
      return NULL;
    }
    mVarTableSize = new_size;
    mVarHashTable = new_hash;
    dealloc_vartable(old_table, old_size);
    for (ZddVar* var = mVarTop; var; var = var->mNext) {
      reg_var(var);
    }
  }
  ZddVar* var = new ZddVar(varid);
  if ( var ) {
    reg_var(var);
    ++ mVarNum;
    // varid の昇順になるような位置に挿入する．
    ZddVar** pprev = &mVarTop;
    ZddVar* temp;
    while ( (temp = *pprev) && temp->varid() < varid ) {
      pprev = &(temp->mNext);
      temp = *pprev;
    }
    *pprev = var;
    var->mNext = temp;
  }
  return var;
}

// 現在登録されている変数をそのレベルの昇順で返す．
ymuint
ZddMgrImpl::var_list(list<VarId>& vlist) const
{
  vlist.clear();
  for (ZddVar* var = mVarTop; var; var = var->mNext) {
    vlist.push_back(var->varid());
  }
  return mVarNum;
}

// 変数番号からレベルを得る．
ymuint
ZddMgrImpl::level(VarId varid) const
{
  // 実は同じ
  return varid.val();
}

// レベルから変数番号を得る．
VarId
ZddMgrImpl::varid(ymuint level) const
{
  // 実は同じ
  return VarId(level);
}

// @brief 現在の最大レベル + 1を返す．
ymuint
ZddMgrImpl::max_level() const
{
  return mVarNum + 1;
}

// @brief e1 $\cap$ e2 を計算する．
ZddEdge
ZddMgrImpl::cap(ZddEdge e1,
		ZddEdge e2)
{
  return mCapOp->apply(e1, e2);
}

// @brief e1 $\cup$ e2 を計算する．
ZddEdge
ZddMgrImpl::cup(ZddEdge e1,
		ZddEdge e2)
{
  return mCupOp->apply(e1, e2);
}

// @brief src1 $\setdiff$ src2 を計算する．
ZddEdge
ZddMgrImpl::diff(ZddEdge e1,
		 ZddEdge e2)
{
  return mDiffOp->apply(e1, e2);
}

// @brief 指定した変数の0枝と1枝を交換する．
// @param[in] e 枝
// @param[in] var 交換を行う変数番号
ZddEdge
ZddMgrImpl::swap(ZddEdge e,
		 VarId var)
{
  return mSwapOp->apply(e, level(var));
}

// @brief 指定された変数を含まないコファクターを返す．
ZddEdge
ZddMgrImpl::cofactor0(ZddEdge e,
		      VarId var)
{
  return mCof0Op->apply(e, level(var));
}

// @brief 指定された変数を含むコファクターを返す．
ZddEdge
ZddMgrImpl::cofactor1(ZddEdge e,
		      VarId var)
{
  return mCof1Op->apply(e, level(var));
}

// @brief 要素ごとのユニオンを計算する．
ZddEdge
ZddMgrImpl::merge(ZddEdge e1,
		  ZddEdge e2)
{
  return mMergeOp->apply(e1, e2);
}

// edge_list に含まれる枝を根とするZDDのサポートに印をつける．
ymuint
ZddMgrImpl::mark_support(const vector<ZddEdge>& edge_list)
{
  return mSupOp->apply(edge_list);
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
ZddMgrImpl::mark_to_vector(VarVector& vars)
{
  return mSupOp->to_list(vars);
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
ZddMgrImpl::mark_to_list(VarList& vars)
{
  return mSupOp->to_list(vars);
}

// 節点テーブルを次に拡大する時の基準値を計算する．
void
ZddMgrImpl::set_next_limit_size()
{
  // 制限値はロード値のリミットと現在のテーブルサイズから計算される
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * mNtLoadLimit);
}

// @brief ガーベージコレクションを許可する．
void
ZddMgrImpl::enable_gc()
{
  if ( mGcEnable > 0 ) {
    -- mGcEnable;
  }
}

// @brief ガーベージコレクションを禁止する．
void
ZddMgrImpl::disable_gc()
{
  ++ mGcEnable;
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
ZddMgrImpl::gc(bool shrink_nodetable)
{
  logstream() << "ZddMgrImpl::GC() begin...." << endl;

  // 演算結果テーブルをスキャンしておかなければならない．
  for (list<ZddOp*>::iterator p = mOpList.begin();
       p != mOpList.end(); ++ p) {
    ZddOp* op = *p;
    op->sweep();
  }

  // その他のテーブルの sweep を行う．
  mSweepMgr.prop_event();

  // 節点テーブルをスキャンしてゴミを取り除く
  ZddNode** ptr = mNodeTable;
  ZddNode** end = mNodeTable + mTableSize;
  do {
    ZddNode* temp;
    ZddNode** prev = ptr;
    while ( (temp = *prev) ) {
      if ( temp->noref() ) {
	// どこからも参照されていないノードは節点テーブルから除く
	// この時点ではフリーリストを作る必要は無い．
	*prev = temp->mLink;
	temp->mLink = NULL;
      }
      else {
	prev = &(temp->mLink);
      }
    }
  } while ( ++ ptr != end );

  // 確保したメモリブロック順にフリーリストを整列させる．
  // 整列といってもO(n)で実行できる．
  // メモリブロックの中身が全てフリーリストに含まれるのならば
  // そのメモリブロックを本当に解放する．
  // その時には，このブロックに含まれるノードはフリーリストから除かなければ
  // ならない．
  mFreeTop = NULL;
  ZddNode** prev = &mFreeTop;
  ZddNode** prev_blk = &mTopBlk;
  ymuint64 delete_num = 0;
  for (ZddNode* blk; (blk = *prev_blk);  ) {
    if ( scan_nodechunk(blk, NODE_UNIT, prev) ) {
      *prev_blk = blk->mLink;
      dealloc_nodechunk(blk);
      delete_num += NODE_UNIT - 1;
    }
    else {
      prev_blk = &(blk->mLink);
    }
  }

  // カレントブロックを調べる．
  if ( mCurBlk ) {
    if ( scan_nodechunk(mCurBlk, mCurIdx, prev) ) {
      dealloc_nodechunk(mCurBlk);
      delete_num += mCurIdx - 1;
      mCurBlk = NULL;
    }
  }
  *prev_blk = NULL;
  *prev = NULL;

  // 内部 statistics の更新
  ymuint64 free_num = mGarbageNum;
  mFreeNum += free_num - delete_num;
  mNodeNum -= free_num;
  mGarbageNum = 0;
  ++ mGcCount;

  if ( shrink_nodetable ) {
    // ノードテーブルが縮小可能ならば縮小する
    ymuint64 nn = static_cast<ymuint64>(mNodeNum * 2.0 / mNtLoadLimit);
    ymuint64 new_size = mTableSize;
    while ( new_size > INIT_SIZE ) {
      if ( nn < new_size ) {
	new_size >>= 1;
      }
      else {
	break;
      }
    }
    if ( new_size != mTableSize ) {
      resize(new_size);
    }
  }

  logstream() << "ZddMgrImpl::GC() end." << endl
	      << "  " << free_num
	      << " nodes are deleted from the node-table." << endl
	      << "  " << delete_num << " nodes are (really) freed."
	      << endl;
}

// 節点テーブルを拡張する
// メモリアロケーションに失敗したら false を返す．
bool
ZddMgrImpl::resize(ymuint64 new_size)
{
  logstream() << "ZddMgrImpl::resize(" << new_size << ")" << endl;

  ZddNode** new_table = alloc_nodetable(new_size);
  if ( !new_table ) {
    // アロケーションに失敗した．
    return false;
  }

  ymuint64 old_size = mTableSize;
  ZddNode** old_table = mNodeTable;
  mNodeTable = new_table;
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  set_next_limit_size();
  ZddNode** tbl = old_table;
  if ( tbl ) {
    ZddNode** end = tbl + old_size;
    do {
      ZddNode* next;
      ZddNode* temp;
      for (temp = *tbl; temp; temp = next) {
	next = temp->mLink;
	ymuint pos = hash_func3(temp->edge0(), temp->edge1(), temp->level());
	ZddNode*& entry = mNodeTable[pos & mTableSize_1];
	temp->mLink = entry;
	entry = temp;
      }
    } while ( ++ tbl != end );
    dealloc_nodetable(old_table, old_size);
  }
  return true;
}

// GC 前の sweep 処理を行うためのバインダーを登録する．
void
ZddMgrImpl::reg_sweep_binder(EventBinder* binder)
{
  mSweepMgr.reg_binder(binder);
}

// さまざまなパラメータを設定する．
// 設定方法はX-windowのライブラリ関数に良くあるやり方で，まず，全ての
// パラメータを格納する構造体bdd_param_tに必要なパラメータを設定し，
// 設定したいパラメータに対応するビットのみをmaskにセットする．
void
ZddMgrImpl::param(const ZddMgrParam& param,
		  ymuint32 mask)
{
  if ( mask & ZddMgrParam::GC_THRESHOLD ) {
    mGcThreshold = param.mGcThreshold;
  }
  if ( mask & ZddMgrParam::GC_NODE_LIMIT ) {
    mGcNodeLimit = param.mGcNodeLimit;
  }
  if ( mask & ZddMgrParam::NT_LOAD_LIMIT ) {
    mNtLoadLimit = param.mNtLoadLimit;
    set_next_limit_size();
  }
  if ( mask & ZddMgrParam::RT_LOAD_LIMIT ) {
    mRtLoadLimit = param.mRtLoadLimit;
  }
  if ( mask & ZddMgrParam::MEM_LIMIT ) {
    mMemLimit = param.mMemLimit;
  }
}

// パラメータを読み出す
void
ZddMgrImpl::param(ZddMgrParam& param) const
{
  param.mGcThreshold = mGcThreshold;
  param.mGcNodeLimit = mGcNodeLimit;
  param.mNtLoadLimit = mNtLoadLimit;
  param.mRtLoadLimit = mRtLoadLimit;
  param.mMemLimit = mMemLimit;
}

// 名前を得る．
const string&
ZddMgrImpl::name() const
{
  return mName;
}

// さまざまな内部情報を得る
// これらの変数はread-onlyなので，変数自体を外部で宣言せずに，
// それを読み出す関数を定義している．
ymuint64
ZddMgrImpl::used_mem() const
{
  return mUsedMem;
}

ymuint64
ZddMgrImpl::node_num() const
{
  return mNodeNum;
}

ymuint64
ZddMgrImpl::garbage_num() const
{
  return mGarbageNum;
}

ymuint64
ZddMgrImpl::avail_num() const
{
  return mFreeNum;
}

ymuint64
ZddMgrImpl::gc_count() const
{
  return mGcCount;
}

// 同一の節点が存在するか調べ，ない時にのみ新たなノードを確保する
// 使用メモリ量が上限を越えたら kEdgeInvalid を返す．
ZddEdge
ZddMgrImpl::new_node(ymuint level,
		     ZddEdge e0,
		     ZddEdge e1)
{
  if ( e0.is_error() || e1.is_error() ) {
    return ZddEdge::make_error();
  }
  if ( e0.is_overflow() || e1.is_overflow() ) {
    return ZddEdge::make_overflow();
  }

  // 0 element 属性は0枝のみを考える．
  bool zattr = e0.zattr();
  e0.normalize();

  ZddEdge ans;
  if ( e1.is_zero() ) {
    ans = e0;
  }
  else {
    // 節点テーブルを探す．
    ymuint64 pos = hash_func3(e0, e1, level);
    bool found = false;
    for (ZddNode* temp = mNodeTable[pos & mTableSize_1]; temp; temp = temp->mLink) {
      if ( temp->edge0() == e0 && temp->edge1() == e1 && temp->level() == level ) {
	// 同一の節点がすでに登録されている
	ans = ZddEdge(temp, false);
	found = true;
	break;
      }
    }
    if ( !found ) {
      // 節点テーブルには登録されていなかったので新しい節点を取ってきて
      // 内容を設定する．
      ZddNode* temp = alloc_node();
      if ( !temp ) {
	// メモリアロケーションに失敗した
	return ZddEdge::make_overflow();
      }
      temp->mEdge0 = e0;
      temp->mEdge1 = e1;
      temp->mLevel = level;
      temp->mRefMark = 0UL;  // mark = none, link = 0

      // 新たなノードを登録する．
      if ( mNodeNum > mNextLimit ) {
	// ノード数が制限値を越えたのでテーブルを2倍に拡張する
	if ( !resize(mTableSize << 1) ) {
	  // リサイズが失敗した．
	  return ZddEdge::make_overflow();
	}
      }
      {
	ZddNode*& entry = mNodeTable[pos & mTableSize_1];
	temp->mLink = entry;
	entry = temp;
      }
      ans = ZddEdge(temp);
    }
  }
  ans.add_zattr(zattr);

  return ans;
}

// e の参照回数を増やす．
void
ZddMgrImpl::inc_rootref(ZddEdge e)
{
  activate(e);
}

// e の参照回数を減らす．
void
ZddMgrImpl::dec_rootref(ZddEdge e)
{
  deactivate(e);

  // ゴミがたまっていたら回収する．
  // ただし頻繁に gc() を呼びたくないので条件をもうけている．
  if ( mGcEnable == 0 &&
       mNodeNum > mGcNodeLimit	&&
       mGarbageNum > static_cast<ymuint64>(double(mNodeNum) * mGcThreshold) ) {
    gc(false);
  }
}

// mVarTable 中のマークを消す．
void
ZddMgrImpl::clear_varmark()
{
  for (ZddVar* var = mVarTop; var; var = var->mNext) {
    var->mMark = 0;
  }
  mVarSet.clear();
}

// level の変数を取り出す．
ZddVar*
ZddMgrImpl::var_at(ymuint level) const
{
  return var_of(VarId(level));
}

// varid の変数を取出す．
ZddVar*
ZddMgrImpl::var_of(VarId varid) const
{
  ymuint64 pos = var_hash(varid) & (mVarTableSize - 1);
  for (ZddVar* var = mVarHashTable[pos]; var; var = var->mLink) {
    if ( var->varid() == varid ) {
      return var;
    }
  }
  // 見つからなかった．
  return NULL;
}

// Var を登録する．
void
ZddMgrImpl::reg_var(ZddVar* var)
{
  ymuint64 pos = var_hash(var->varid()) & (mVarTableSize - 1);
  ZddVar*& entry = mVarHashTable[pos];
  var->mLink = entry;
  entry = var;
}

// ノードをロックする．
// もし，子孫のノードがアンロックの状態ならばロックする
// 無駄な関数呼び出しを避けるため，この関数を呼び出す前に，
// このノードがロックされていないことを確認してあるものとする．
void
ZddMgrImpl::lockall(ZddNode* vp)
{
  -- mGarbageNum;
  activate(vp->edge0());
  activate(vp->edge1());
}

// ノードをアンロックする．
// もし，子孫のノードがこのノード以外から参照されていない場合には，
// そのノードもアンロックする
// 無駄な関数呼び出しを避けるため，この関数を呼び出す前に，
// このノードがロックされていたことは確認してあるものとする．
void
ZddMgrImpl::unlockall(ZddNode* vp)
{
  ++ mGarbageNum;
  deactivate(vp->edge0());
  deactivate(vp->edge1());
}

// 節点を確保する．
ZddNode*
ZddMgrImpl::alloc_node()
{
  ZddNode* temp;

  if ( (temp = mFreeTop) ) {
    // フリーリストに節点があったのでそれを取り出す．
    mFreeTop = temp->mLink;
    -- mFreeNum;
  }
  else {
    // ないので新たに確保する．
    if ( !mCurBlk ) {
      // 使用可能なメモリブロックもない．
      mCurBlk = alloc_nodechunk();
      if ( !mCurBlk ) {
	// メモリアロケーションに失敗した
	return NULL;
      }
      mCurIdx = 1;
    }

    temp = &mCurBlk[mCurIdx];
    ++ mCurIdx;
    if ( mCurIdx == NODE_UNIT ) {
      // ブロック内のノードを使い切った．
      // メモリブロックのリストへ繋げる．
      mCurBlk->mLink = mTopBlk;
      mTopBlk = mCurBlk;
      mCurBlk = NULL;
    }
  }
  ++ mGarbageNum;  // この時点では誰も指していない．
  ++ mNodeNum;

  return temp;
}

// 節点チャンクをスキャンして参照されていない節点をフリーリストにつなぐ
// ただし，チャンク全体が参照されていなかった場合にはフリーリストには
// つながない．その場合には true を返す．
bool
ZddMgrImpl::scan_nodechunk(ZddNode* blk,
			   ymuint blk_size,
			   ZddNode**& prev)
{
  ZddNode** prev_orig = prev;
  bool can_delete = true;
  // メモリブロックの先頭のノードは次のブロックを指すポインタとして
  // 使っているのでスキャンから除外する．
  ZddNode* temp = &blk[1];
  for (ymuint i = 1; i < blk_size; ++ i, ++ temp) {
    if ( temp->noref() ) {
      *prev = temp;
      prev = &(temp->mLink);
    }
    else {
      can_delete = false;
    }
  }
  if ( can_delete ) {
    prev = prev_orig;
  }
  return can_delete;
}

// 変数テーブル用のメモリを確保する．
// size はバイト単位ではなくエントリ数．
ZddVar**
ZddMgrImpl::alloc_vartable(ymuint size)
{
  ymuint64 byte_size = sizeof(ZddVar*) * size;
  void* ptr = allocate(byte_size);
  if ( ptr ) {
    memset(ptr, 0, byte_size);
  }
  return static_cast<ZddVar**>(ptr);
}

// 変数テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
ZddMgrImpl::dealloc_vartable(ZddVar** table,
			     ymuint size)
{
  ymuint64 byte_size = sizeof(ZddVar*) * size;
  deallocate(table, byte_size);
}

// 節点テーブル用のメモリを確保する．
// size はバイト単位ではなくエントリ数
ZddNode**
ZddMgrImpl::alloc_nodetable(ymuint64 size)
{
  ymuint64 byte_size = sizeof(ZddNode*) * size;
  void* ptr = allocate(byte_size);
  if ( ptr ) {
    memset(ptr, 0, byte_size);
  }
  return static_cast<ZddNode**>(ptr);
}

// 節点テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
ZddMgrImpl::dealloc_nodetable(ZddNode** table,
			      ymuint64 size)
{
  ymuint64 byte_size = sizeof(ZddNode*) * size;
  deallocate(table, byte_size);
}

// 節点チャンク用のメモリを確保する．
ZddNode*
ZddMgrImpl::alloc_nodechunk()
{
  const ymuint64 byte_size = sizeof(ZddNode) * NODE_UNIT;
  void* ptr = allocate(byte_size);
  return static_cast<ZddNode*>(ptr);
}

// 節点チャンク用のメモリを解放する．
void
ZddMgrImpl::dealloc_nodechunk(ZddNode* chunk)
{
  const ymuint64 byte_size = sizeof(ZddNode) * NODE_UNIT;
  deallocate(chunk, byte_size);
}

// BDD パッケージ用のメモリ確保ルーティン
void*
ZddMgrImpl::allocate(ymuint64 size)
{
  if ( mOverflow || mMemLimit > 0 && mUsedMem + size > mMemLimit ) {
    // メモリ制限をオーバーしたので 0 を返す．
    mOverflow = true;
    return 0;
  }
  mUsedMem += size;
  void* ans;

#ifdef BDDMGR_USE_MALLOC
  ans = malloc(size);
  if ( !ans ) {
    return 0;
  }
#else
  try {
    ans = new char[size];
  }
  catch (alloc_xcpt x) {
    // new が失敗した．
    mOverflow = true;
    return 0;
  }
#endif

#if defined(BDD_DEBUG_MEMALLOC)
  {
    ios::fmtflags save = logstream().flags();
    logstream() << "ZddMgrImpl::allocate(" << size << ") --> "
		<< setw(8) << hex << reinterpret_cast<ympuint>(ans) << dec
		<< endl;
    logstream().flags(save);
  }
#endif
  return ans;
}

// BDD パッケージ用のメモリ解放ルーティン
void
ZddMgrImpl::deallocate(void* ptr,
		       ymuint64 size)
{
#if defined(BDD_DEBUG_MEMALLOC)
  {
    ios::fmtflags save = logstream().flags();
    logstream() << "ZddMgrImpl::deallocate("
		<< setw(8) << hex << reinterpret_cast<ympuint>(ptr) << dec
		<< ", " << size << ")" << endl;
    logstream().flags(save);
  }
#endif

  mUsedMem -= size;

#ifdef BDDMGR_USE_MALLOC
  free(ptr);
#else
  memset(ptr, 0xa5, size);
  delete [] (char*)ptr;
#endif
}


//////////////////////////////////////////////////////////////////////
// ZddEdge に関連する関数
//////////////////////////////////////////////////////////////////////

// 節点のマークを消す．
void
clear_mark(ZddEdge e)
{
  for ( ; ; ) {
    ZddNode* vp = e.get_node();
    if ( !vp || !vp->mark() ) {
      break;
    }
    vp->mark(0);
    clear_mark(vp->edge0());
    e = vp->edge1();
  }
}

END_NAMESPACE_YM_ZDD
