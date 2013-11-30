
/// @file BmmVar.cc
/// @brief BmmVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BmmVar.h"
#include "BddMgrModern.h"


BEGIN_NAMESPACE_YM_BDD

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
  return v1 + (v2 >> 2);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// BmmVar
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmmVar::BmmVar(BddMgrModern* mgr,
	       VarId id) :
  mMgr(mgr),
  mId(id)
{
  mLink = NULL;

  // 節点テーブルの初期化
  mTableSize = 0;
  mTableSize_1 = 0;
  mNextLimit = 0;
  mNodeTable = NULL;
  if ( mgr->is_reorderable() ) {
    resize(INIT_SIZE);
  }

  mNodeNum = 0;
}

// デストラクタ
BmmVar::~BmmVar()
{
  // 節点テーブルの解放
  dealloc_nodetable(mNodeTable, mTableSize);
}

// ノードを登録する．
void
BmmVar::reg_node(size_t pos,
		 BddNode* node)
{
  ++ mNodeNum;
  if ( mNodeNum > mNextLimit ) {
    // ノード数が制限値を越えたのでテーブルを2倍に拡張する
    resize(mTableSize << 1);
  }
  BddNode*& entry = mNodeTable[pos & mTableSize_1];
  node->mLink = entry;
  entry = node;
}

// gc 用の sweep 処理
void
BmmVar::sweep()
{
  BddNode** ptr = mNodeTable;
  BddNode** end = mNodeTable + mTableSize;
  ymuint64 nf = 0;
  do {
    BddNode* temp;
    BddNode** prev = ptr;
    while ( (temp = *prev) ) {
      if ( temp->noref() ) {
	// どこからも参照されていないノードは節点テーブルから除く
	// この時点ではフリーリストを作る必要は無い．
	*prev = temp->mLink;
	temp->mLink = NULL;
	++ nf;
      }
      else {
	prev = &(temp->mLink);
      }
    }
  } while ( ++ ptr != end );
  mNodeNum -= nf;
}

// 節点テーブルが縮小可能なら縮小する．
void
BmmVar::shrink(double load_limit)
{
  ymuint64 nn = static_cast<ymuint64>(mNodeNum * 2.0 / load_limit);
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

// 節点テーブルを拡張する
// メモリアロケーションに失敗したら false を返す．
bool
BmmVar::resize(ymuint64 new_size)
{
  logstream() << "BmmVar(" << varid() << ")::resize("
	      << new_size << ")" << endl;

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
  set_next_limit_size(mMgr->nt_load_limit());
  BddNode** tbl = old_table;
  if ( tbl ) {
    BddNode** end = tbl + old_size;
    do {
      BddNode* next;
      BddNode* temp;
      for (temp = *tbl; temp; temp = next) {
	next = temp->mLink;
	ymuint64 pos = hash_func2(temp->edge0(), temp->edge1());
	BddNode*& entry = mNodeTable[pos & mTableSize_1];
	temp->mLink = entry;
	entry = temp;
      }
    } while ( ++ tbl != end );
    dealloc_nodetable(old_table, old_size);
  }
  return true;
}

// 次のリミット値を計算する
void
BmmVar::set_next_limit_size(double load_limit)
{
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * load_limit);
}

// ログ出力用のストリームを得る．
ostream&
BmmVar::logstream() const
{
  return mMgr->logstream();
}

// 節点テーブル用のメモリを確保する．
// size はバイト単位ではなくエントリ数
BddNode**
BmmVar::alloc_nodetable(ymuint64 size)
{
  return mMgr->alloc_nodetable(size);
}

// 節点テーブル用のメモリを解放する．
// size はバイト単位ではなくエントリ数
void
BmmVar::dealloc_nodetable(BddNode** table,
			  ymuint64 size)
{
  mMgr->dealloc_nodetable(table, size);
}

END_NAMESPACE_YM_BDD
