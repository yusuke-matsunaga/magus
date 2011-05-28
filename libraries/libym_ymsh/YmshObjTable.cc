
/// @file libym_ymsh/YmshObjTable.cc
/// @brief YmshObjTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmshObjTable.h"


BEGIN_NAMESPACE_YM_YMSH

BEGIN_NONAMESPACE

inline
ymuint
hash_func(const char* str)
{
  ymuint h = 0;
  ymuint c;
  for ( ; (c = static_cast<ymuint>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス YmshObjTable
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmshObjTable::YmshObjTable() :
  mAlloc(sizeof(Cell), 1024)
{
  alloc_table(1024);
}

// @brief デストラクタ
YmshObjTable::~YmshObjTable()
{
  clear();
}

// @brief 内容を空にする．
void
YmshObjTable::clear()
{
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (Cell* cell = mTable[i]; cell; cell = cell->mLink) {
      cell->~Cell();
    }
    mTable[i] = NULL;
  }
  mNum = 0;
  mAlloc.destroy();
}

// @brief 登録されている要素数を返す．
ymuint
YmshObjTable::num() const
{
  return mNum;
}

// @brief 登録されている名前のリストを返す．
// @param[out] name_list 答を格納するリスト
// @return 要素数を返す．
void
YmshObjTable::get_name_list(vector<const char*>& name_list) const
{
  name_list.clear();
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (Cell* cell = mTable[i]; cell; cell = cell->mLink) {
      name_list.push_back(cell->name());
    }
  }
}

// @brief name という名の要素が登録されているか調べる．
// @param[in] name 名前
bool
YmshObjTable::check_obj(const char* name) const
{
  ymuint hval = hash_func(name);
  ymuint idx = hval % mTableSize;
  for (Cell* cell = mTable[idx]; cell; cell = cell->mLink) {
    if ( strcmp(cell->name(), name) == 0 ) {
      return true;
    }
  }
  return false;
}

// @brief name という名の値を返す．
// @param[in] name 名前
// @note 登録してなければ新しく作る．
vector<string>&
YmshObjTable::get_obj(const char* name)
{
  ymuint hval = hash_func(name);
  ymuint idx = hval % mTableSize;
  for (Cell* cell = mTable[idx]; cell; cell = cell->mLink) {
    if ( strcmp(cell->name(), name) == 0 ) {
      return cell->mValue;
    }
  }

  if ( mNum >= mNextLimit ) {
    // テーブルを2倍にする．
    ymuint old_size = mTableSize;
    Cell** old_table = mTable;
    alloc_table(old_size * 2);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* cur_cell = cell;
	cell = cur_cell->mLink;
	ymuint idx1 = hash_func(cur_cell->name()) % mTableSize;
	cur_cell->mLink = mTable[idx1];
	mTable[idx1] = cur_cell;
      }
    }
    delete [] old_table;
    // テーブルサイズが変わったのでインデックスを更新する．
    idx = hval % mTableSize;
  }

  Cell* cell = new_cell(name);
  cell->mLink = mTable[idx];
  mTable[idx] = cell;
  ++ mNum;

  return cell->mValue;
}

// @brief 要素をテーブルから取り除く
// @param[in] name 取り除く名前
// @note name という要素が登録されていなければなにもしない．
void
YmshObjTable::unreg_obj(const char* name)
{
  ymuint idx = hash_func(name) % mTableSize;
  Cell** prev = &mTable[idx];
  while ( *prev ) {
    Cell* cell = *prev;
    if ( strcmp(cell->name(), name) == 0 ) {
      *prev = cell->mLink;
      free_cell(cell);
      -- mNum;
      break;
    }
    prev = &(cell->mLink);
  }
}

// @brief Cell を確保する．
// @param[in] name 名前
YmshObjTable::Cell*
YmshObjTable::new_cell(const char* name)
{
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell(name);
  return cell;
}

// @brief Cell を解放する．
void
YmshObjTable::free_cell(Cell* cell)
{
  cell->~Cell();
  mAlloc.put_memory(sizeof(Cell), cell);
}

// @brief テーブルの領域を確保する．
// @param[in] req_size 要求するサイズ
void
YmshObjTable::alloc_table(ymuint req_size)
{
  mTableSize = req_size;
  mNextLimit = static_cast<ymuint>(mTableSize * 1.8);
  mTable = new Cell*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_YMSH
