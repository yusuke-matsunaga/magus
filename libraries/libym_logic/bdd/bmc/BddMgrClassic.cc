
/// @file BddMgrClassic.cc
/// @brief BddMgrClassic の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcCompTbl.h"
#include "BmcVar.h"


#if !defined(__SUNPRO_CC) || __SUNPRO_CC >= 0x500
#include <exception>
#include <new>

BEGIN_NAMESPACE_YM_BDD

typedef bad_alloc alloc_xcpt;

#else
#include <exception.h>

BEGIN_NAMESPACE_YM_BDD

typedef xalloc alloc_xcpt;

#endif

//#define BDDMGR_USE_MALLOC
#define BDD_DEBUG_MEMALLOC 1

BEGIN_NONAMESPACE

// 1K = 1,024
const ymuint64 K_unit = (1 << 10);
// 1M = 1,024 x 1,024
const ymuint64 M_unit = (1 << 20);

// パラメータのデフォルト値
const double DEFAULT_GC_THRESHOLD  = 0.10;
const ymuint64 DEFAULT_GC_NODE_LIMIT =  64 * K_unit;
const double DEFAULT_NT_LOAD_LIMIT = 2.0;
const double DEFAULT_RT_LOAD_LIMIT = 0.8;
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
hash_func2(BddEdge id1,
	   BddEdge id2)
{
  ymuint64 v1 = id1.hash();
  ymuint64 v2 = id2.hash();
  return static_cast<ymuint64>(v1 + (v2 >> 2));
}

// 節点テーブルのハッシュ関数
inline
ymuint64
hash_func3(BddEdge id1,
	   BddEdge id2,
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
var_hash(ymuint level)
{
  ymuint64 key = level;
  return ((key * key) >> 8) + key;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// BddMgrClassic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] option オプション
BddMgrClassic::BddMgrClassic(const string& name,
			     const string& option) :
  BddMgrImpl(name)
{
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
  mMaxLevel = 0;

  // 演算結果テーブルの初期化
  mTblTop = NULL;

  mCmpTable = new CompTbl1(this, "compose_table");
  assert_cond(mCmpTable, __FILE__, __LINE__);

  mSmTable = new CompTbl1(this, "sm_table");
  assert_cond(mSmTable, __FILE__, __LINE__);
  mAeTable = new CompTbl2(this, "ae_table");
  assert_cond(mAeTable, __FILE__, __LINE__);
  mCubedivTable = new CompTbl1(this, "cubediv_table");
  assert_cond(mCubedivTable, __FILE__, __LINE__);

  mMinsupTable = new CompTbl2(this, "minsup_table");
  assert_cond(mMinsupTable, __FILE__, __LINE__);
  mIsopTable = new IsopTbl(this, "isop_table");
  assert_cond(mIsopTable, __FILE__, __LINE__);
  mPcTable = new IsopTbl(this, "pc_table");
  assert_cond(mPcTable, __FILE__, __LINE__);

  mCsTable = new CompTbl2(this, "cs_table");
  assert_cond(mCsTable, __FILE__, __LINE__);
  mCs1Table = new CompTbl3(this, "cs1_table");
  assert_cond(mCs1Table, __FILE__, __LINE__);
  mCs2Table = new CompTbl2(this, "cs2_table");
  assert_cond(mCs2Table, __FILE__, __LINE__);
}

// デストラクタ
BddMgrClassic::~BddMgrClassic()
{
  // 節点テーブルの解放
  dealloc_nodetable(mNodeTable, mTableSize);

  // 節点用のメモリブロックの解放
  for (BddNode* blk = mTopBlk; blk; ) {
    BddNode* temp = blk;
    blk = temp->mLink;
    dealloc_nodechunk(temp);
  }
  if ( mCurBlk ) {
    dealloc_nodechunk(mCurBlk);
  }

  // 演算結果テーブルの解放
  for (CompTbl* tbl = mTblTop; tbl; ) {
    CompTbl* tmp = tbl;
    tbl = tbl->mNext;
    delete tmp;
  }

  // 変数の解放
  for (Var* var = mVarTop; var; ) {
    Var* next = var->mNext;
    delete var;
    var = next;
  }

  // 変数テーブルの解放
  dealloc_vartable(mVarHashTable, mVarTableSize);

  // このマネージャに関わるメモリはすべて解放したはず．
  assert_cond( mUsedMem == 0, __FILE__, __LINE__);
}

#if 0
// bdd が正リテラルのみのキューブの時，真となる．
bool
BddMgrClassic::check_posi_cube(BddEdge e)
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
BddMgrClassic::check_cube(BddEdge e)
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
#endif

// 変数を確保する．
// 確保に失敗したら false を返す．
// 最後の変数の後ろに挿入される．
bool
BddMgrClassic::new_var(VarId varid)
{
  Var* var = var_of(varid);
  if ( var == NULL ) {
    return alloc_var(varid) != NULL;
  }
  else {
    return true;
  }
}

// 変数を確保する．
BmcVar*
BddMgrClassic::alloc_var(VarId varid)
{
  if ( mVarTableSize == mVarNum ) {
    Var** old_table = mVarHashTable;
    ymuint64 old_size = mVarTableSize;
    ymuint64 new_size = mVarTableSize << 1;
    Var** new_hash = alloc_vartable(new_size);
    if ( !new_hash ) {
      return NULL;
    }
    mVarTableSize = new_size;
    mVarHashTable = new_hash;
    dealloc_vartable(old_table, old_size);
    for (Var* var = mVarTop; var; var = var->mNext) {
      reg_var(var);
    }
  }
  ymuint level = varid.val();
  Var* var = new Var(level);
  if ( var ) {
    reg_var(var);
    ++ mVarNum;
    // varid の昇順になるような位置に挿入する．
    Var** pprev = &mVarTop;
    Var* temp;
    while ( (temp = *pprev) && temp->level() < level ) {
      pprev = &(temp->mNext);
      temp = *pprev;
    }
    *pprev = var;
    var->mNext = temp;
  }
  if ( mMaxLevel < level ) {
    mMaxLevel = level;
  }
  return var;
}

// 現在登録されている変数をそのレベルの昇順で返す．
ymuint
BddMgrClassic::var_list(list<VarId>& vlist) const
{
  vlist.clear();
  for (Var* var = mVarTop; var; var = var->mNext) {
    vlist.push_back(VarId(var->level()));
  }
  return mVarNum;
}

// 変数番号からレベルを得る．
ymuint
BddMgrClassic::level(VarId varid) const
{
  // 実は同じ
  return varid.val();
}

// レベルから変数番号を得る．
VarId
BddMgrClassic::varid(ymuint level) const
{
  // 実は同じ
  return VarId(level);
}

// @brief 現在の最大レベル + 1を返す．
ymuint
BddMgrClassic::max_level() const
{
  return mMaxLevel + 1;
}

// 動的変数順変更を許可する．
void
BddMgrClassic::enable_DVO()
{
}

// 動的変数順変更を禁止する．
void
BddMgrClassic::disable_DVO()
{
}

// 節点テーブルを次に拡大する時の基準値を計算する．
void
BddMgrClassic::set_next_limit_size()
{
  // 制限値はロード値のリミットと現在のテーブルサイズから計算される
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * nt_load_limit());
}

// ガーベージコレクションを行なう．
// 具体的には各ノードの参照回数が0のノードをフリーリストに回収し
// 再利用できるよ うにする．
// その際，それらのノードに関係した演算結果テーブルの内容はクリアされる．
// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
void
BddMgrClassic::gc(bool shrink_nodetable)
{
  logstream() << "BddMgrClassic::GC() begin...." << endl;

  // 演算結果テーブルをスキャンしておかなければならない．
  for (CompTbl* tbl = mTblTop; tbl; tbl = tbl->mNext) {
    if ( tbl->used_num() > 0 ) tbl->sweep();
  }

  // その他のテーブルの sweep を行う．
  mSweepMgr.prop_event();

  // 節点テーブルをスキャンしてゴミを取り除く
  BddNode** ptr = mNodeTable;
  BddNode** end = mNodeTable + mTableSize;
  do {
    BddNode* temp;
    BddNode** prev = ptr;
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
  BddNode** prev = &mFreeTop;
  BddNode** prev_blk = &mTopBlk;
  ymuint64 delete_num = 0;
  for (BddNode* blk; (blk = *prev_blk);  ) {
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
    ymuint64 nn = static_cast<ymuint64>(mNodeNum * 2.0 / nt_load_limit());
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

  logstream() << "BddMgrClassic::GC() end." << endl
	      << "  " << free_num
	      << " nodes are deleted from the node-table." << endl
	      << "  " << delete_num << " nodes are (really) freed."
	      << endl;
}

// 節点テーブルを拡張する
// メモリアロケーションに失敗したら false を返す．
bool
BddMgrClassic::resize(ymuint64 new_size)
{
  logstream() << "BddMgrClassic::resize(" << new_size << ")" << endl;

  BddNode** new_table = alloc_nodetable(new_size);
  if ( !new_table ) {
    // アロケーションに失敗した．
    return false;
  }

  ymuint64 old_size = mTableSize;
  BddNode** old_table = mNodeTable;
  mNodeTable = new_table;
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  set_next_limit_size();
  BddNode** tbl = old_table;
  if ( tbl ) {
    BddNode** end = tbl + old_size;
    do {
      BddNode* next;
      BddNode* temp;
      for (temp = *tbl; temp; temp = next) {
	next = temp->mLink;
	ymuint pos = hash_func3(temp->edge0(), temp->edge1(), temp->level());
	BddNode*& entry = mNodeTable[pos & mTableSize_1];
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
BddMgrClassic::reg_sweep_binder(EventBinder* binder)
{
  mSweepMgr.reg_binder(binder);
}

// さまざまな内部情報を得る
// これらの変数はread-onlyなので，変数自体を外部で宣言せずに，
// それを読み出す関数を定義している．
ymuint64
BddMgrClassic::used_mem() const
{
  return mUsedMem;
}

ymuint64
BddMgrClassic::node_num() const
{
  return mNodeNum;
}

ymuint64
BddMgrClassic::garbage_num() const
{
  return mGarbageNum;
}

ymuint64
BddMgrClassic::avail_num() const
{
  return mFreeNum;
}

ymuint64
BddMgrClassic::gc_count() const
{
  return mGcCount;
}

// 同一の節点が存在するか調べ，ない時にのみ新たなノードを確保する
// 使用メモリ量が上限を越えたら kEdgeInvalid を返す．
BddEdge
BddMgrClassic::new_node(ymuint level,
			BddEdge e0,
			BddEdge e1)
{
  if ( e0 == e1 ) {
    return e0;
  }

  if ( e0.is_error() || e1.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e0.is_overflow() || e1.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  // 否定枝に関する正規化ルール
  tPol ans_pol = e0.pol();
  e0.addpol(ans_pol);
  e1.addpol(ans_pol);

  // 節点テーブルを探す．
  ymuint64 pos = hash_func3(e0, e1, level);
  for (BddNode* temp = mNodeTable[pos & mTableSize_1]; temp; temp = temp->mLink) {
    if ( temp->edge0() == e0 && temp->edge1() == e1 && temp->level() == level ) {
      // 同一の節点がすでに登録されている
      return BddEdge(temp, ans_pol);
    }
  }

  // 節点テーブルには登録されていなかったので新しい節点を取ってきて
  // 内容を設定する．
  BddNode* temp = alloc_node();
  if ( !temp ) {
    // メモリアロケーションに失敗した
    return BddEdge::make_overflow();
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
      return BddEdge::make_overflow();
    }
  }
  {
    BddNode*& entry = mNodeTable[pos & mTableSize_1];
    temp->mLink = entry;
    entry = temp;
  }

  return BddEdge(temp, ans_pol);
}

// e の参照回数を増やす．
void
BddMgrClassic::inc_rootref(BddEdge e)
{
  activate(e);
}

// e の参照回数を減らす．
void
BddMgrClassic::dec_rootref(BddEdge e)
{
  deactivate(e);

  // ゴミがたまっていたら回収する．
  // ただし頻繁に gc() を呼びたくないので条件をもうけている．
  if ( check_gc() &&
       mNodeNum > gc_node_limit()	&&
       mGarbageNum > static_cast<ymuint64>(double(mNodeNum) * gc_threshold()) ) {
    gc(false);
  }
}

// mVarTable 中のマークを消す．
void
BddMgrClassic::clear_varmark()
{
  for (Var* var = mVarTop; var; var = var->mNext) {
    var->mMark = 0;
  }
  mVarSet.clear();
}

// level の変数を取り出す．
BmcVar*
BddMgrClassic::var_at(ymuint level) const
{
  ymuint64 pos = var_hash(level) & (mVarTableSize - 1);
  for (Var* var = mVarHashTable[pos]; var; var = var->mLink) {
    if ( var->level() == level ) {
      return var;
    }
  }
  // 見つからなかった．
  return NULL;
}

// varid の変数を取出す．
BmcVar*
BddMgrClassic::var_of(VarId varid) const
{
  return var_at(level(varid));
}

// Var を登録する．
void
BddMgrClassic::reg_var(Var* var)
{
  ymuint64 pos = var_hash(var->level()) & (mVarTableSize - 1);
  Var*& entry = mVarHashTable[pos];
  var->mLink = entry;
  entry = var;
}

// @brief lockall() 用のフック
void
BddMgrClassic::lock_hook(BddNode* vp)
{
  -- mGarbageNum;
}

// @brief unlockall() 用のフック
void
BddMgrClassic::unlock_hook(BddNode* vp)
{
  ++ mGarbageNum;
}

// 演算結果テーブルを登録する．
void
BddMgrClassic::add_table(CompTbl* tbl)
{
  tbl->mNext = mTblTop;
  mTblTop = tbl;
}

// 節点を確保する．
BddNode*
BddMgrClassic::alloc_node()
{
  BddNode* temp;

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
BddMgrClassic::scan_nodechunk(BddNode* blk,
			      ymuint blk_size,
			      BddNode**& prev)
{
  BddNode** prev_orig = prev;
  bool can_delete = true;
  // メモリブロックの先頭のノードは次のブロックを指すポインタとして
  // 使っているのでスキャンから除外する．
  BddNode* temp = &blk[1];
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
BmcVar**
BddMgrClassic::alloc_vartable(ymuint size)
{
  ymuint64 byte_size = sizeof(Var*) * size;
  void* ptr = allocate(byte_size);
  if ( ptr ) {
    memset(ptr, 0, byte_size);
  }
  return static_cast<Var**>(ptr);
}

// 変数テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
BddMgrClassic::dealloc_vartable(Var** table,
				ymuint size)
{
  ymuint64 byte_size = sizeof(Var*) * size;
  deallocate(table, byte_size);
}

// 節点テーブル用のメモリを確保する．
// size はバイト単位ではなくエントリ数
BddNode**
BddMgrClassic::alloc_nodetable(ymuint64 size)
{
  ymuint64 byte_size = sizeof(BddNode*) * size;
  void* ptr = allocate(byte_size);
  if ( ptr ) {
    memset(ptr, 0, byte_size);
  }
  return static_cast<BddNode**>(ptr);
}

// 節点テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
BddMgrClassic::dealloc_nodetable(BddNode** table,
				 ymuint64 size)
{
  ymuint64 byte_size = sizeof(BddNode*) * size;
  deallocate(table, byte_size);
}

// 節点チャンク用のメモリを確保する．
BddNode*
BddMgrClassic::alloc_nodechunk()
{
  const ymuint64 byte_size = sizeof(BddNode) * NODE_UNIT;
  void* ptr = allocate(byte_size);
  return static_cast<BddNode*>(ptr);
}

// 節点チャンク用のメモリを解放する．
void
BddMgrClassic::dealloc_nodechunk(BddNode* chunk)
{
  const ymuint64 byte_size = sizeof(BddNode) * NODE_UNIT;
  deallocate(chunk, byte_size);
}

#if 1
// BDD パッケージ用のメモリ確保ルーティン
void*
BddMgrClassic::allocate(ymuint64 size)
{
  if ( mOverflow || mem_limit() > 0 && mUsedMem + size > mem_limit() ) {
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
    logstream() << "BddMgrClassic::allocate(" << size << ") --> "
		<< setw(8) << hex << reinterpret_cast<ympuint>(ans) << dec
		<< endl;
    logstream().flags(save);
  }
#endif
  return ans;
}

// BDD パッケージ用のメモリ解放ルーティン
void
BddMgrClassic::deallocate(void* ptr,
			  ymuint64 size)
{
#if defined(BDD_DEBUG_MEMALLOC)
  {
    ios::fmtflags save = logstream().flags();
    logstream() << "BddMgrClassic::deallocate("
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
#endif

END_NAMESPACE_YM_BDD
