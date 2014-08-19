
/// @file CompTbl2e.cc
/// @brief CompTbl2e の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CompTbl2e.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

const ymuint64 kInitSize = (1UL << 10);

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CompTbl2e
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] name 名前
CompTbl2e::CompTbl2e(BddMgrImpl* mgr,
		     const char* name) :
  CompTbl(mgr, name),
  mTable(NULL),
  mUsedNum(0)
{
  resize(kInitSize);
}

// @brief デストラクタ
CompTbl2e::~CompTbl2e()
{
  deallocate((void*)mTable, table_size() * sizeof(Cell));
}

// @brief ガーベージコレクションが起きた時の処理を行なう．
// 参照されていないノードに関連したエントリを削除する．
void
CompTbl2e::sweep()
{
  // ログを出力
  logstream() << "CompTbl2e[" << name() << "]::sweep()" << endl;

  // 削除されるノードに関連したセルをクリアする．
  Cell* cell = mTable;
  Cell* end = cell + table_size();
  for ( ; cell != end; ++ cell) {
    if ( !cell->mKey1.is_error() &&
	 (cell->mKey1.noref() ||
	  cell->mKey2.noref() ||
	  cell->mAnsBdd.noref()) ) {
      cell->mKey1 = BddEdge::make_error();
      -- mUsedNum;
    }
  }
}

// @brief 内容をクリアする．
void
CompTbl2e::clear()
{
  Cell* cell = mTable;
  Cell* end = cell + table_size();
  for ( ; cell != end; ++ cell) {
    cell->mKey1 = BddEdge::make_error();
  }
  mUsedNum = 0;
}

// @brief テーブルサイズを変更する．
// @param[in] new_size 新しいサイズ
bool
CompTbl2e::resize(ymuint64 new_size)
{
  // ログの出力
  logstream() << "CompTbl2e[" << name() << "]::resize(" << new_size << ")"
	      << endl;

  // 新しいメモリ領域を確保する．
  Cell* new_table = (Cell*)allocate(new_size * sizeof(Cell));
  if ( new_table == NULL ) {
    return false;
  }

  // 昔の値を保存する．
  ymuint64 old_size = table_size();
  Cell* old_table = mTable;

  // 新たなサイズを設定し，テーブルを確保する．
  mTable = new_table;
  set_table_size(new_size);

  // 中身をクリアしておく．
  Cell* top = mTable;
  Cell* end = top + table_size();
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

END_NAMESPACE_YM_BDD
