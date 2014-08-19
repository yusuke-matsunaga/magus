
/// @file CompTbl.cc
/// @brief CompTbl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"
#include "CNFddMgrImpl.h"


BEGIN_NONAMESPACE

const ymuint64 kInitSize = (1UL << 10);
const ymuint64 kMaxSize = (1UL << 20);

END_NONAMESPACE


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス CompTbl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl::CompTbl(CNFddMgrImpl& mgr,
		 const char* name) :
  mMgr(mgr)
{
  if ( name == 0 ) {
    // 適当な名前を付ける．
    static int num = 1;
    ostringstream s;
    s << "table#" << num;
    ++ num;
    mName = s.str();
  }
  else {
    mName = name;
  }

  mMaxSize = kMaxSize;
  mTableSize = 0;
}

// デストラクタ
CompTbl::~CompTbl()
{
}

// テーブルサイズを取り出す．
ymuint64
CompTbl::table_size() const
{
  return mTableSize;
}

// next_limit を再計算する．
void
CompTbl::update_next_limit()
{
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * mMgr.rt_load_limit());
}

// 使用されているセル数を取り出す．
ymuint64
CompTbl::used_num() const
{
  return mUsedNum;
}

// 最大のテーブルサイズの設定を行なう．
// 現在，このサイズを越えているテーブルがあっても縮小するような
// ことはしない．
void
CompTbl::max_size(ymuint64 max_size)
{
  mMaxSize = max_size;
}

// CNFddMgr からメモリを獲得する．
void*
CompTbl::allocate(ymuint64 size)
{
  return mMgr.allocate(size);
}

// CNFddMgr にメモリを返す．
void
CompTbl::deallocate(void* ptr,
		    ymuint64 size)
{
  mMgr.deallocate(ptr, size);
}

// ログ出力用のストリームを得る．
ostream&
CompTbl::logstream() const
{
  return mMgr.logstream();
}


//////////////////////////////////////////////////////////////////////
// クラス CompTbl1
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl1::CompTbl1(CNFddMgrImpl& mgr,
		   const char* name) :
  CompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
CompTbl1::~CompTbl1()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
CompTbl1::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "CompTbl1[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 新しいメモリ領域を確保する．
  Cell* new_table = (Cell*)allocate(new_size * sizeof(Cell));
  if ( new_table == NULL ) {
    return false;
  }

  // 昔の値を保存する．
  ymuint64 old_size = mTableSize;
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = new_table;
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = CNFddEdge::make_error();
    ++ top;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( !top->mKey1.is_error() ) {
	ymuint pos = hash_func(top->mKey1);
	Cell* temp = mTable + pos;
	if ( temp->mKey1.is_error() ) ++ mUsedNum;
	temp->mKey1 = top->mKey1;
	temp->mAns = top->mAns;
      }
      ++ top;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }

  return true;
}

// CNFddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
CompTbl1::sweep()
{
  // ログを出力
  logstream() << "CompTbl1[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (cell->mKey1.noref() ||
	  cell->mAns.noref()) ) {
      cell->mKey1 = CNFddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
CompTbl1::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = CNFddEdge::make_error();
  }
  mUsedNum = 0;
}


//////////////////////////////////////////////////////////////////////
// クラス CompTbl1n
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl1n::CompTbl1n(CNFddMgrImpl& mgr,
		     const char* name) :
  CompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
CompTbl1n::~CompTbl1n()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
CompTbl1n::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "CompTbl1n[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 新しいメモリ領域を確保する．
  Cell* new_table = (Cell*)allocate(new_size * sizeof(Cell));
  if ( new_table == NULL ) {
    return false;
  }

  // 昔の値を保存する．
  ymuint64 old_size = mTableSize;
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = new_table;
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = CNFddEdge::make_error();
    ++ top;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( !top->mKey1.is_error() ) {
	ymuint pos = hash_func(top->mKey1, top->mKey2);
	Cell* temp = mTable + pos;
	if ( temp->mKey1.is_error() ) ++ mUsedNum;
	temp->mKey1 = top->mKey1;
	temp->mKey2 = top->mKey2;
	temp->mAns = top->mAns;
      }
      ++ top;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }

  return true;
}

// CNFddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
CompTbl1n::sweep()
{
  // ログを出力
  logstream() << "CompTbl1n[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (cell->mKey1.noref() ||
	  cell->mAns.noref()) ) {
      cell->mKey1 = CNFddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
CompTbl1n::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = CNFddEdge::make_error();
  }
  mUsedNum = 0;
}


//////////////////////////////////////////////////////////////////////
// CompTbl2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl2::CompTbl2(CNFddMgrImpl& mgr,
		   const char* name) :
  CompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
CompTbl2::~CompTbl2()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
CompTbl2::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "CompTbl2[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 新たなサイズを設定し，テーブルを確保する．
  Cell* new_table = (Cell*)allocate(new_size * sizeof(Cell));
  if ( new_table == NULL ) {
    return false;
  }

  // 昔の値を保存する．
  ymuint64 old_size = mTableSize;
  Cell* old_table = mTable;

  mTable = new_table;
  mTableSize = new_size;
  mTableSize_1 = mTableSize - 1;
  update_next_limit();

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + mTableSize;
  do {
    top->mKey1 = CNFddEdge::make_error();
    ++ top;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( !top->mKey1.is_error() ) {
	ymuint64 pos = hash_func(top->mKey1, top->mKey2);
	Cell* temp = mTable + pos;
	if ( temp->mKey1.is_error() ) ++ mUsedNum;
	temp->mKey1 = top->mKey1;
	temp->mKey2 = top->mKey2;
	temp->mAns = top->mAns;
      }
      ++ top;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }

  return true;
}

// CNFddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
CompTbl2::sweep()
{
  // ログを出力
  logstream() << "CompTbl2[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (cell->mKey1.noref() ||
	  cell->mKey2.noref() ||
	  cell->mAns.noref()) ) {
      cell->mKey1 = CNFddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
CompTbl2::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = CNFddEdge::make_error();
  }
  mUsedNum = 0;
}

END_NAMESPACE_YM_CNFDD
