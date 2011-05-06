
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
  for (ymuint i = 0; i < mSize; ++ i) {
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
ymuint
YmshObjTable::get_name_list(vector<const char*>& name_list) const
{
}

// @brief 要素の追加を行う．
// @param[in] name 名前
// @return 要素番号を返す．
// @note すでに name という要素が登録されていた場合にはその番号を返す．
ymuint
YmshObjTable::reg_obj(const char* name)
{
  ymuint hval = hash_func(name);
  ymuint idx = hval % mTableSize;
  for (Cell* cell = mTable[idx]; cell; cell = cell->mNext) {
    if ( strcmp(cell->mName, name) == 0 ) {
      return cell->mId;
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
	cell = cur_cell->mNext;
	ymuint idx1 = hash_func(cur_cell->mName) % mTableSize;
	cur_cell->mNext = mTable[idx1];
	mTable[idx1] = cur_cell;
      }
    }
    delete [] old_table;
    // テーブルサイズが変わったのでインデックスを更新する．
    idx = hval % mTableSize;
  }

  Cell* cell = new_cell();
  cell->mName = name;
  cell->mId = id;
  cell->mNext = mTable[idx];
  mTable[mIdx] = cell;
  ++ mNum;

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
    if ( strcmp(cell->mName, name) == 0 ) {
      *prev = cell->mNext;
      free_cell(cell);
      -- mNum;
      break;
    }
    prev = &(cell->mNext);
  }
}

// @brief name という名の要素を探す．
// @param[in] name 名前
// @return 要素番号を返す．
// @note 見つからなかった場合には kBadId が返される．
ymuint
YmshObjTable::find_obj(const char* name) const
{
  ymuint idx = hash_func(name) % mTableSize;
  for (Cell* cell = mTable[idx]; cell; cell = cell->mNext) {
    if ( strcmp(cell->mName, name) == 0 ) {
      return cell->mId;
    }
  }
  return kBadId;
}

// @brief Cell を確保する．
Cell*
YmshObjTable::new_cell()
{
}

// @brief Cell を解放する．
void
free_cell(Cell* cell);

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
