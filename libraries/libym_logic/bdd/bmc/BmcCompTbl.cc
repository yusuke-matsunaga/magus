
/// @file BmcCompTbl.cc
/// @brief BmcCompTbl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BmcCompTbl.h"


BEGIN_NONAMESPACE

const ymuint64 kInitSize = (1UL << 10);
const double kLoadLimit = 0.8;
const ymuint64 kMaxSize = (1UL << 20);

END_NONAMESPACE


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BmcCompTbl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmcCompTbl::BmcCompTbl(BddMgrClassic* mgr,
		       const char* name) :
  mMgr(mgr)
{
  if ( name == 0 ) {
    // 適当な名前を付ける．
    static int num = 1;
    ostringstream s;
    s << "table#" << num ++;
    mName = s.str();
  }
  else {
    mName = name;
  }

  mLoadLimit = kLoadLimit;
  mMaxSize = kMaxSize;
  mTableSize = 0;

  // リストに追加する．
  mMgr->add_table(this);
}

// デストラクタ
BmcCompTbl::~BmcCompTbl()
{
}

// テーブルサイズを取り出す．
ymuint64
BmcCompTbl::table_size() const
{
  return mTableSize;
}

// next_limit を再計算する．
void
BmcCompTbl::update_next_limit()
{
  mNextLimit = static_cast<ymuint64>(double(mTableSize) * mLoadLimit);
}

// 使用されているセル数を取り出す．
ymuint64
BmcCompTbl::used_num() const
{
  return mUsedNum;
}

// load_limitの(再)設定を行なう．
void
BmcCompTbl::load_limit(double load_limit)
{
  mLoadLimit = load_limit;
}

// 最大のテーブルサイズの設定を行なう．
// 現在，このサイズを越えているテーブルがあっても縮小するような
// ことはしない．
void
BmcCompTbl::max_size(ymuint64 max_size)
{
  mMaxSize = max_size;
}

// BddMgr からメモリを獲得する．
void*
BmcCompTbl::allocate(ymuint64 size)
{
  return mMgr->allocate(size);
}

// BddMgr にメモリを返す．
void
BmcCompTbl::deallocate(void* ptr,
		       ymuint64 size)
{
  mMgr->deallocate(ptr, size);
}

// ログ出力用のストリームを得る．
ostream&
BmcCompTbl::logstream() const
{
  return mMgr->logstream();
}


//////////////////////////////////////////////////////////////////////
// BmcCompTbl1
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmcCompTbl1::BmcCompTbl1(BddMgrClassic* mgr,
			 const char* name) :
  BmcCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmcCompTbl1::~BmcCompTbl1()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
BmcCompTbl1::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "BmcCompTbl1[" << mName << "]::resize(" << new_size << ")"
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
    top->mKey1 = BddEdge::make_error();
    ++ top;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( !top->mKey1.is_error() ) {
	ymuint64 pos = hash_func(top->mKey1);
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

// BddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmcCompTbl1::sweep()
{
  // ログを出力
  logstream() << "BmcCompTbl1[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = BddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
BmcCompTbl1::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = BddEdge::make_error();
    -- mUsedNum;
  }
}


//////////////////////////////////////////////////////////////////////
// BmcCompTbl2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmcCompTbl2::BmcCompTbl2(BddMgrClassic* mgr,
			 const char* name) :
  BmcCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmcCompTbl2::~BmcCompTbl2()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
BmcCompTbl2::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "BmcCompTbl2[" << mName << "]::resize(" << new_size << ")"
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
    top->mKey1 = BddEdge::make_error();
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

// BddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmcCompTbl2::sweep()
{
  // ログを出力
  logstream() << "BmcCompTbl2[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = BddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
BmcCompTbl2::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = BddEdge::make_error();
    -- mUsedNum;
  }
}


//////////////////////////////////////////////////////////////////////
// BmcCompTbl3
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmcCompTbl3::BmcCompTbl3(BddMgrClassic* mgr,
			 const char* name) :
  BmcCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmcCompTbl3::~BmcCompTbl3()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
BmcCompTbl3::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "BmcCompTbl3[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 新たなサイズを設定し，テーブルを確保する．
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
    top->mKey1 = BddEdge::make_error();
    ++ top;
  } while ( top != end );

  // ハッシュし直して新しいテーブルに登録する．
  mUsedNum = 0;
  if ( old_size > 0 ) {
    Cell* top = old_table;
    Cell* end = top + old_size;
    do {
      if ( !top->mKey1.is_error() ) {
	ymuint64 pos = hash_func(top->mKey1, top->mKey2, top->mKey3);
	Cell* temp = mTable + pos;
	if ( temp->mKey1.is_error() ) ++ mUsedNum;
	temp->mKey1 = top->mKey1;
	temp->mKey2 = top->mKey2;
	temp->mKey3 = top->mKey3;
	temp->mAns = top->mAns;
      }
      ++ top;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }

  return true;
}

// BddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmcCompTbl3::sweep()
{
  // ログを出力
  logstream() << "BmcCompTbl3[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mKey3) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = BddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
BmcCompTbl3::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; cell ++) {
    cell->mKey1 = BddEdge::make_error();
    -- mUsedNum;
  }
}


//////////////////////////////////////////////////////////////////////
// BmcIsopTbl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
BmcIsopTbl::BmcIsopTbl(BddMgrClassic* mgr,
		       const char* name) :
  BmcCompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
BmcIsopTbl::~BmcIsopTbl()
{
  clear();
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
BmcIsopTbl::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "BmcIsopTbl[" << mName << "]::resize(" << new_size << ")"
	      << endl;

  // 新たなサイズを設定し，テーブルを確保する．
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
    top->mKey1 = BddEdge::make_error();
    top->mAnsCov = 0;
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
	temp->mAnsBdd = top->mAnsBdd;
	delete temp->mAnsCov;
	temp->mAnsCov = top->mAnsCov;
      }
      ++ top;
    } while ( top != end );
    deallocate((void*)old_table, old_size * sizeof(Cell));
  }

  return true;
}

// BddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
BmcIsopTbl::sweep()
{
  // ログを出力
  logstream() << "BmcIsopTbl[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mAnsBdd)) ) {
      cell->mKey1 = BddEdge::make_error();
      delete cell->mAnsCov;
      cell->mAnsCov = 0;
      -- mUsedNum;
    }
  }
}

// クリアする．
void
BmcIsopTbl::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = BddEdge::make_error();
    delete cell->mAnsCov;
    cell->mAnsCov = 0;
    -- mUsedNum;
  }
}

END_NAMESPACE_YM_BDD
