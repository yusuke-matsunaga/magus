
/// @file CompTbl.cc
/// @brief CompTbl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl.h"


BEGIN_NONAMESPACE

const ymuint64 kInitSize = (1UL << 10);
const double kLoadLimit = 0.8;
const ymuint64 kMaxSize = (1UL << 20);

END_NONAMESPACE


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス CompTbl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl::CompTbl(ZddMgrClassic* mgr,
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

  mLoadLimit = kLoadLimit;
  mMaxSize = kMaxSize;
  mTableSize = 0;

  // リストに追加する．
  mMgr->add_table(this);
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
  mNextLimit = size_t(double(mTableSize) * mLoadLimit);
}

// 使用されているセル数を取り出す．
ymuint64
CompTbl::used_num() const
{
  return mUsedNum;
}

// load_limitの(再)設定を行なう．
void
CompTbl::load_limit(double load_limit)
{
  mLoadLimit = load_limit;
}

// 最大のテーブルサイズの設定を行なう．
// 現在，このサイズを越えているテーブルがあっても縮小するような
// ことはしない．
void
CompTbl::max_size(ymuint64 max_size)
{
  mMaxSize = max_size;
}

// ZddMgr からメモリを獲得する．
void*
CompTbl::allocate(ymuint64 size)
{
  return mMgr->allocate(size);
}

// ZddMgr にメモリを返す．
void
CompTbl::deallocate(void* ptr,
		    ymuint64 size)
{
  mMgr->deallocate(ptr, size);
}

// ログ出力用のストリームを得る．
ostream&
CompTbl::logstream() const
{
  return mMgr->logstream();
}


//////////////////////////////////////////////////////////////////////
// クラス CompTbl1
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl1::CompTbl1(ZddMgrClassic* mgr,
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
    top->mKey1 = ZddEdge::make_error();
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

// ZddMgr::GC()に対応する．
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
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = ZddEdge::make_error();
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
    cell->mKey1 = ZddEdge::make_error();
    -- mUsedNum;
  }
}


//////////////////////////////////////////////////////////////////////
// CompTbl2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl2::CompTbl2(ZddMgrClassic* mgr,
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
    top->mKey1 = ZddEdge::make_error();
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

// ZddMgr::GC()に対応する．
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
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = ZddEdge::make_error();
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
    cell->mKey1 = ZddEdge::make_error();
    -- mUsedNum;
  }
}


//////////////////////////////////////////////////////////////////////
// CompTbl3
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CompTbl3::CompTbl3(ZddMgrClassic* mgr,
		   const char* name) :
  CompTbl(mgr, name)
{
  resize(kInitSize);
}

// デストラクタ
CompTbl3::~CompTbl3()
{
  deallocate((void*)mTable, mTableSize * sizeof(Cell));
}

// テーブルサイズを変更する．
bool
CompTbl3::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "CompTbl3[" << mName << "]::resize(" << new_size << ")"
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
    top->mKey1 = ZddEdge::make_error();
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

// ZddMgr::GC()に対応する．
// 具体的には GC で削除されるノードに関連したセルをクリアする．
void
CompTbl3::sweep()
{
  // ログを出力
  logstream() << "CompTbl3[" << mName << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (check_noref(cell->mKey1) ||
	  check_noref(cell->mKey2) ||
	  check_noref(cell->mKey3) ||
	  check_noref(cell->mAns)) ) {
      cell->mKey1 = ZddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// クリアする．
void
CompTbl3::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + mTableSize;
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = ZddEdge::make_error();
    -- mUsedNum;
  }
}

END_NAMESPACE_YM_ZDD
