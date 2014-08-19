
/// @file CNFddMgrImpl.cc
/// @brief CNFddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddMgrImpl.h"

#include "YmLogic/CNFdd.h"
#include "YmLogic/CNFddMgr.h"
#include "CNFddVar.h"

#include "ConOp.h"
#include "DisOp.h"
#include "DiffOp.h"
#include "DiffOp2.h"
#include "MergeOp.h"
#include "MinOp.h"
#include "CutOp.h"
#include "LitPOp.h"
#include "LitNOp.h"
#include "Cof0Op.h"
#include "CofPOp.h"
#include "CofNOp.h"
#include "SupOp.h"


#if !defined(__SUNPRO_CC) || __SUNPRO_CC >= 0x500
#include <exception>
#include <new>
#else
#include <exception.h>
#endif


BEGIN_NAMESPACE_YM_CNFDD

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
hash_func3(CNFddEdge id1,
	   CNFddEdge id2,
	   CNFddEdge id3,
	   ymuint level)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  ymuint64 v3 = id3.hash();
  ymuint64 v4 = level;
  return static_cast<ymuint64>((v1 >> 2) + (v2 >> 1) + v3 + v4 * 7);
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
// クラス CNFddMgrImpl
//////////////////////////////////////////////////////////////////////

CNFddMgrImpl* CNFddMgrImpl::mDefaultMgr = NULL;

// デフォルトマネージャを返すクラスメソッド
CNFddMgrImpl*
CNFddMgrImpl::default_mgr()
{
  if ( !mDefaultMgr ) {
    mDefaultMgr = new CNFddMgrImpl("default manager");
    assert_cond(mDefaultMgr, __FILE__, __LINE__);
  }
  return mDefaultMgr;
}

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] option オプション
CNFddMgrImpl::CNFddMgrImpl(const string& name,
			   const string& option) :
  mName(name)
{
  if ( name == string() ) {
    // 適当な名前を付ける．
    static int num = 1;
    ostringstream s;
    s << "cnfdd_mgr#" << num ++;
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

  // CNFDD リストの初期化
  mTopCNFdd = NULL;

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

  // 演算結果テーブルの初期化
  mOpList.clear();

  // 演算クラスの生成
  mConOp = new ConOp(*this);
  mDisOp = new DisOp(*this);
  mDiffOp = new DiffOp(*this);
  mMergeOp = new MergeOp(*this, mDisOp);
  BinOp* diff_op2 = new DiffOp2(*this, mDisOp);
  mMinOp = new MinOp(*this, diff_op2);
  mCutOp = new CutOp(*this);
  mLitPOp = new LitPOp(*this);
  mLitNOp = new LitNOp(*this);
  mCof0Op = new Cof0Op(*this);
  mCofPOp = new CofPOp(*this);
  mCofNOp = new CofNOp(*this);
  mSupOp = new SupOp(*this);
}

// デストラクタ
CNFddMgrImpl::~CNFddMgrImpl()
{
  assert_cond(this != mDefaultMgr, __FILE__, __LINE__);

  // CNFDD の解放
  // と言っても CNFdd のオブジェクトを削除するわけには行かないので
  // デフォルトマネージャのエラーCNFDDにすり替える．
  if ( mTopCNFdd ) {
    CNFdd* last = NULL;
    for (CNFdd* bdd = mTopCNFdd; bdd; bdd = bdd->mNext) {
      bdd->mRoot = CNFddEdge::make_error();
      bdd->mMgr = mDefaultMgr;
      last = bdd;
    }
    CNFdd* first = mDefaultMgr->mTopCNFdd;
    mDefaultMgr->mTopCNFdd = mTopCNFdd;
    mTopCNFdd->mPrev = NULL;
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
  for (CNFddNode* blk = mTopBlk; blk; ) {
    CNFddNode* temp = blk;
    blk = temp->mLink;
    dealloc_nodechunk(temp);
  }
  if ( mCurBlk ) {
    dealloc_nodechunk(mCurBlk);
  }

  // 変数の解放
  for (CNFddVar* var = mVarTop; var; ) {
    CNFddVar* next = var->mNext;
    delete var;
    var = next;
  }

  // 変数テーブルの解放
  dealloc_vartable(mVarHashTable, mVarTableSize);

  // 演算クラスの解放
  for (list<Op*>::iterator p = mOpList.begin();
       p != mOpList.end(); ++ p) {
    delete *p;
  }

  // このマネージャに関わるメモリはすべて解放したはず．
  assert_cond( mUsedMem == 0, __FILE__, __LINE__);

}

// log用ストリームを設定する．
void
CNFddMgrImpl::set_logstream(ostream& s)
{
  mLogFp = &s;
}

// log用ストリームを解除する．
void
CNFddMgrImpl::unset_logstream()
{
  mLogFp = mNullStream;
}

// log用ファイルポインタを読み出す．
ostream&
CNFddMgrImpl::logstream() const
{
  return *mLogFp;
}

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
CNFddMgrImpl::new_var(VarId varid)
{
  return alloc_var(varid) != NULL;
}

// 変数を確保する．
CNFddVar*
CNFddMgrImpl::alloc_var(VarId varid)
{
  if ( mVarTableSize == mVarNum ) {
    CNFddVar** old_table = mVarHashTable;
    ymuint64 old_size = mVarTableSize;
    ymuint64 new_size = mVarTableSize << 1;
    CNFddVar** new_hash = alloc_vartable(new_size);
    if ( !new_hash ) {
      return NULL;
    }
    mVarTableSize = new_size;
    mVarHashTable = new_hash;
    dealloc_vartable(old_table, old_size);
    for (CNFddVar* var = mVarTop; var; var = var->mNext) {
      reg_var(var);
    }
  }
  CNFddVar* var = new CNFddVar(varid);
  if ( var ) {
    reg_var(var);
    ++ mVarNum;
    // varid の昇順になるような位置に挿入する．
    CNFddVar** pprev = &mVarTop;
    CNFddVar* temp;
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
CNFddMgrImpl::var_list(list<VarId>& vlist) const
{
  vlist.clear();
  for (CNFddVar* var = mVarTop; var; var = var->mNext) {
    vlist.push_back(var->varid());
  }
  return mVarNum;
}

// 変数番号からレベルを得る．
ymuint
CNFddMgrImpl::level(VarId varid) const
{
  // 実は同じ
  return varid.val();
}

// レベルから変数番号を得る．
VarId
CNFddMgrImpl::varid(ymuint level) const
{
  // 実は同じ
  return VarId(level);
}

// @brief 現在の最大レベル + 1を返す．
ymuint
CNFddMgrImpl::max_level() const
{
  return mVarNum + 1;
}

// @brief e1 $\wedge$ e2 を計算する．
CNFddEdge
CNFddMgrImpl::conjunction(CNFddEdge e1,
			  CNFddEdge e2)
{
  return mConOp->apply(e1, e2);
}

// @brief e1 $\vee$ e2 を計算する．
CNFddEdge
CNFddMgrImpl::disjunction(CNFddEdge e1,
			  CNFddEdge e2)
{
  return mDisOp->apply(e1, e2);
}

// @brief src1 $\setdiff$ src2 を計算する．
CNFddEdge
CNFddMgrImpl::diff(CNFddEdge e1,
		 CNFddEdge e2)
{
  return mDiffOp->apply(e1, e2);
}

// @brief 要素ごとのユニオンを計算する．
CNFddEdge
CNFddMgrImpl::merge(CNFddEdge e1,
		    CNFddEdge e2)
{
  return mMergeOp->apply(e1, e2);
}

// @brief 他の節に支配されている節を取り除く
CNFddEdge
CNFddMgrImpl::make_minimal(CNFddEdge e1)
{
  return mMinOp->apply(e1);
}

// @brief 要素数が limit 以下の要素のみを残す．
CNFddEdge
CNFddMgrImpl::cut_off(CNFddEdge e1,
		      ymuint limit)
{
  return mCutOp->apply(e1, limit);
}

// @brief 指定した変数の肯定のリテラルを加える．
// @param[in] e 枝
// @param[in] var 交換を行う変数番号
CNFddEdge
CNFddMgrImpl::add_posiliteral(CNFddEdge e,
			      VarId var)
{
  return mLitPOp->apply(e, level(var));
}

// @brief 指定した変数の否定のリテラルを加える．
// @param[in] e 枝
// @param[in] var 交換を行う変数番号
CNFddEdge
CNFddMgrImpl::add_negaliteral(CNFddEdge e,
			      VarId var)
{
  return mLitNOp->apply(e, level(var));
}

// @brief 指定された変数を含まないコファクターを返す．
// @param[in] e 枝
// @param[in] var 変数番号
CNFddEdge
CNFddMgrImpl::cofactor_0(CNFddEdge e,
			 VarId var)
{
  return mCof0Op->apply(e, level(var));
}

// @brief 指定された変数を含むコファクターを返す．
// @param[in] e 枝
// @param[in] var 変数番号
CNFddEdge
CNFddMgrImpl::cofactor_p(CNFddEdge e,
			 VarId var)
{
  return mCofPOp->apply(e, level(var));
}

// @brief 指定された変数を含むコファクターを返す．
// @param[in] e 枝
// @param[in] var 変数番号
CNFddEdge
CNFddMgrImpl::cofactor_n(CNFddEdge e,
			 VarId var)
{
  return mCofNOp->apply(e, level(var));
}

// @brief edge_list に登録されたCNFDDのサポートに印をつける．
ymuint
CNFddMgrImpl::mark_support(const vector<CNFddEdge>& edge_list)
{
  return mSupOp->apply(edge_list);
}

// @brief 印のついた変数をベクタに変換する．
ymuint
CNFddMgrImpl::mark_to_vector(VarVector& support)
{
  return mSupOp->to_list(support);
}

// @brief 印のついた変数をリストに変換する．
ymuint
CNFddMgrImpl::mark_to_list(VarList& support)
{
  return mSupOp->to_list(support);
}

// 節点テーブルを次に拡大する時の基準値を計算する．
void
CNFddMgrImpl::set_next_limit_size()
{
  // 制限値はロード値のリミットと現在のテーブルサイズから計算される
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * mNtLoadLimit);
}

// @brief ガーベージコレクションを許可する．
void
CNFddMgrImpl::enable_gc()
{
  if ( mGcEnable > 0 ) {
    -- mGcEnable;
  }
}

// @brief ガーベージコレクションを禁止する．
void
CNFddMgrImpl::disable_gc()
{
  ++ mGcEnable;
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
CNFddMgrImpl::gc(bool shrink_nodetable)
{
  logstream() << "CNFddMgrImpl::GC() begin...." << endl;

  // 演算結果テーブルをスキャンしておかなければならない．
  for (list<Op*>::iterator p = mOpList.begin();
       p != mOpList.end(); ++ p) {
    Op* op = *p;
    op->sweep();
  }

  // その他のテーブルの sweep を行う．
  mSweepMgr.prop_event();

  // 節点テーブルをスキャンしてゴミを取り除く
  CNFddNode** ptr = mNodeTable;
  CNFddNode** end = mNodeTable + mTableSize;
  do {
    CNFddNode* temp;
    CNFddNode** prev = ptr;
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
  CNFddNode** prev = &mFreeTop;
  CNFddNode** prev_blk = &mTopBlk;
  ymuint64 delete_num = 0;
  for (CNFddNode* blk; (blk = *prev_blk);  ) {
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

  logstream() << "CNFddMgrImpl::GC() end." << endl
	      << "  " << free_num
	      << " nodes are deleted from the node-table." << endl
	      << "  " << delete_num << " nodes are (really) freed."
	      << endl;
}

// 節点テーブルを拡張する
// メモリアロケーションに失敗したら false を返す．
bool
CNFddMgrImpl::resize(ymuint64 new_size)
{
  logstream() << "CNFddMgrImpl::resize(" << new_size << ")" << endl;

  CNFddNode** new_table = alloc_nodetable(new_size);
  if ( !new_table ) {
    // アロケーションに失敗した．
    return false;
  }

  ymuint64 old_size = mTableSize;
  CNFddNode** old_table = mNodeTable;
  mNodeTable = new_table;
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  set_next_limit_size();
  CNFddNode** tbl = old_table;
  if ( tbl ) {
    CNFddNode** end = tbl + old_size;
    do {
      CNFddNode* next;
      CNFddNode* temp;
      for (temp = *tbl; temp; temp = next) {
	next = temp->mLink;
	ymuint pos = hash_func3(temp->edge_0(), temp->edge_p(), temp->edge_n(),
				temp->level());
	CNFddNode*& entry = mNodeTable[pos & mTableSize_1];
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
CNFddMgrImpl::reg_sweep_binder(EventBinder* binder)
{
  mSweepMgr.reg_binder(binder);
}

// さまざまなパラメータを設定する．
// 設定方法はX-windowのライブラリ関数に良くあるやり方で，まず，全ての
// パラメータを格納する構造体bdd_param_tに必要なパラメータを設定し，
// 設定したいパラメータに対応するビットのみをmaskにセットする．
void
CNFddMgrImpl::param(const CNFddMgrParam& param,
		  ymuint32 mask)
{
  if ( mask & CNFddMgrParam::GC_THRESHOLD ) {
    mGcThreshold = param.mGcThreshold;
  }
  if ( mask & CNFddMgrParam::GC_NODE_LIMIT ) {
    mGcNodeLimit = param.mGcNodeLimit;
  }
  if ( mask & CNFddMgrParam::NT_LOAD_LIMIT ) {
    mNtLoadLimit = param.mNtLoadLimit;
    set_next_limit_size();
  }
  if ( mask & CNFddMgrParam::RT_LOAD_LIMIT ) {
    mRtLoadLimit = param.mRtLoadLimit;
  }
  if ( mask & CNFddMgrParam::MEM_LIMIT ) {
    mMemLimit = param.mMemLimit;
  }
}

// パラメータを読み出す
void
CNFddMgrImpl::param(CNFddMgrParam& param) const
{
  param.mGcThreshold = mGcThreshold;
  param.mGcNodeLimit = mGcNodeLimit;
  param.mNtLoadLimit = mNtLoadLimit;
  param.mRtLoadLimit = mRtLoadLimit;
  param.mMemLimit = mMemLimit;
}

// 名前を得る．
const string&
CNFddMgrImpl::name() const
{
  return mName;
}

// さまざまな内部情報を得る
// これらの変数はread-onlyなので，変数自体を外部で宣言せずに，
// それを読み出す関数を定義している．
ymuint64
CNFddMgrImpl::used_mem() const
{
  return mUsedMem;
}

ymuint64
CNFddMgrImpl::node_num() const
{
  return mNodeNum;
}

ymuint64
CNFddMgrImpl::garbage_num() const
{
  return mGarbageNum;
}

ymuint64
CNFddMgrImpl::avail_num() const
{
  return mFreeNum;
}

ymuint64
CNFddMgrImpl::gc_count() const
{
  return mGcCount;
}

// 同一の節点が存在するか調べ，ない時にのみ新たなノードを確保する
// 使用メモリ量が上限を越えたら kEdgeInvalid を返す．
CNFddEdge
CNFddMgrImpl::new_node(ymuint level,
		       CNFddEdge e_0,
		       CNFddEdge e_p,
		       CNFddEdge e_n)
{
  if ( e_0.is_invalid() ) {
    return e_0;
  }
  if ( e_p.is_invalid() ) {
    return e_p;
  }
  if ( e_n.is_invalid() ) {
    return e_n;
  }

  // 0 element 属性は0枝のみを考える．
  bool zattr = e_0.zattr();
  e_0.normalize();

  CNFddEdge ans;
  if ( e_p.is_zero() && e_n.is_zero() ) {
    ans = e_0;
  }
  else {
    // 節点テーブルを探す．
    ymuint64 pos = hash_func3(e_0, e_p, e_n, level);
    bool found = false;
    for (CNFddNode* temp = mNodeTable[pos & mTableSize_1];
	 temp; temp = temp->mLink) {
      if ( temp->edge_0() == e_0 &&
	   temp->edge_p() == e_p &&
	   temp->edge_n() == e_n &&
	   temp->level() == level ) {
	// 同一の節点がすでに登録されている
	ans = CNFddEdge(temp, false);
	found = true;
	break;
      }
    }
    if ( !found ) {
      // 節点テーブルには登録されていなかったので新しい節点を取ってきて
      // 内容を設定する．
      CNFddNode* temp = alloc_node();
      if ( !temp ) {
	// メモリアロケーションに失敗した
	return CNFddEdge::make_overflow();
      }
      temp->mEdge0 = e_0;
      temp->mEdgeP = e_p;
      temp->mEdgeN = e_n;
      temp->mLevel = level;
      temp->mRefMark = 0UL;  // mark = none, link = 0

      // 新たなノードを登録する．
      if ( mNodeNum > mNextLimit ) {
	// ノード数が制限値を越えたのでテーブルを2倍に拡張する
	if ( !resize(mTableSize << 1) ) {
	  // リサイズが失敗した．
	  return CNFddEdge::make_overflow();
	}
      }
      {
	CNFddNode*& entry = mNodeTable[pos & mTableSize_1];
	temp->mLink = entry;
	entry = temp;
      }
      ans = CNFddEdge(temp);
    }
  }
  ans.add_zattr(zattr);

  return ans;
}

// e の参照回数を増やす．
void
CNFddMgrImpl::inc_rootref(CNFddEdge e)
{
  activate(e);
}

// e の参照回数を減らす．
void
CNFddMgrImpl::dec_rootref(CNFddEdge e)
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
CNFddMgrImpl::clear_varmark()
{
  for (CNFddVar* var = mVarTop; var; var = var->mNext) {
    var->mMark = 0;
  }
  mVarSet.clear();
}

// level の変数を取り出す．
CNFddVar*
CNFddMgrImpl::var_at(ymuint level) const
{
  return var_of(VarId(level));
}

// varid の変数を取出す．
CNFddVar*
CNFddMgrImpl::var_of(VarId varid) const
{
  ymuint64 pos = var_hash(varid) & (mVarTableSize - 1);
  for (CNFddVar* var = mVarHashTable[pos]; var; var = var->mLink) {
    if ( var->varid() == varid ) {
      return var;
    }
  }
  // 見つからなかった．
  return NULL;
}

// Var を登録する．
void
CNFddMgrImpl::reg_var(CNFddVar* var)
{
  ymuint64 pos = var_hash(var->varid()) & (mVarTableSize - 1);
  CNFddVar*& entry = mVarHashTable[pos];
  var->mLink = entry;
  entry = var;
}

// ノードをロックする．
// もし，子孫のノードがアンロックの状態ならばロックする
// 無駄な関数呼び出しを避けるため，この関数を呼び出す前に，
// このノードがロックされていないことを確認してあるものとする．
void
CNFddMgrImpl::lockall(CNFddNode* vp)
{
  -- mGarbageNum;
  activate(vp->edge_0());
  activate(vp->edge_p());
  activate(vp->edge_n());
}

// ノードをアンロックする．
// もし，子孫のノードがこのノード以外から参照されていない場合には，
// そのノードもアンロックする
// 無駄な関数呼び出しを避けるため，この関数を呼び出す前に，
// このノードがロックされていたことは確認してあるものとする．
void
CNFddMgrImpl::unlockall(CNFddNode* vp)
{
  ++ mGarbageNum;
  deactivate(vp->edge_0());
  deactivate(vp->edge_p());
  deactivate(vp->edge_n());
}

// 節点を確保する．
CNFddNode*
CNFddMgrImpl::alloc_node()
{
  CNFddNode* temp;

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
CNFddMgrImpl::scan_nodechunk(CNFddNode* blk,
			   ymuint blk_size,
			   CNFddNode**& prev)
{
  CNFddNode** prev_orig = prev;
  bool can_delete = true;
  // メモリブロックの先頭のノードは次のブロックを指すポインタとして
  // 使っているのでスキャンから除外する．
  CNFddNode* temp = &blk[1];
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
CNFddVar**
CNFddMgrImpl::alloc_vartable(ymuint size)
{
  ymuint64 byte_size = sizeof(CNFddVar*) * size;
  void* ptr = allocate(byte_size);
  if ( ptr ) {
    memset(ptr, 0, byte_size);
  }
  return static_cast<CNFddVar**>(ptr);
}

// 変数テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
CNFddMgrImpl::dealloc_vartable(CNFddVar** table,
			     ymuint size)
{
  ymuint64 byte_size = sizeof(CNFddVar*) * size;
  deallocate(table, byte_size);
}

// 節点テーブル用のメモリを確保する．
// size はバイト単位ではなくエントリ数
CNFddNode**
CNFddMgrImpl::alloc_nodetable(ymuint64 size)
{
  ymuint64 byte_size = sizeof(CNFddNode*) * size;
  void* ptr = allocate(byte_size);
  if ( ptr ) {
    memset(ptr, 0, byte_size);
  }
  return static_cast<CNFddNode**>(ptr);
}

// 節点テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
CNFddMgrImpl::dealloc_nodetable(CNFddNode** table,
			      ymuint64 size)
{
  ymuint64 byte_size = sizeof(CNFddNode*) * size;
  deallocate(table, byte_size);
}

// 節点チャンク用のメモリを確保する．
CNFddNode*
CNFddMgrImpl::alloc_nodechunk()
{
  const ymuint64 byte_size = sizeof(CNFddNode) * NODE_UNIT;
  void* ptr = allocate(byte_size);
  return static_cast<CNFddNode*>(ptr);
}

// 節点チャンク用のメモリを解放する．
void
CNFddMgrImpl::dealloc_nodechunk(CNFddNode* chunk)
{
  const ymuint64 byte_size = sizeof(CNFddNode) * NODE_UNIT;
  deallocate(chunk, byte_size);
}

// BDD パッケージ用のメモリ確保ルーティン
void*
CNFddMgrImpl::allocate(ymuint64 size)
{
  if ( mOverflow || (mMemLimit > 0 && (mUsedMem + size) > mMemLimit) ) {
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
    logstream() << "CNFddMgrImpl::allocate(" << size << ") --> "
		<< setw(8) << hex << reinterpret_cast<ympuint>(ans) << dec
		<< endl;
    logstream().flags(save);
  }
#endif
  return ans;
}

// BDD パッケージ用のメモリ解放ルーティン
void
CNFddMgrImpl::deallocate(void* ptr,
		       ymuint64 size)
{
#if defined(BDD_DEBUG_MEMALLOC)
  {
    ios::fmtflags save = logstream().flags();
    logstream() << "CNFddMgrImpl::deallocate("
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
// CNFddEdge に関連する関数
//////////////////////////////////////////////////////////////////////

// @brief 参照されていない時にtrueを返す
bool
CNFddEdge::noref() const
{
  CNFddNode* node = get_node();
  if ( node != NULL ) {
    return node->noref();
  }
  else {
    return false;
  }
}

// 節点のマークを消す．
void
clear_mark(CNFddEdge e)
{
  CNFddNode* vp = e.get_node();
  if ( !vp || !vp->mark() ) {
    return;
  }
  vp->mark(0);
  clear_mark(vp->edge_0());
  clear_mark(vp->edge_p());
  clear_mark(vp->edge_n());
}

END_NAMESPACE_YM_CNFDD
