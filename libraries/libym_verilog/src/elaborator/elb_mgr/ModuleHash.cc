
/// @file ModuleHash.cc
/// @brief ModuleHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ModuleHash.h"
#include "verilog/vl/VlModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// ElbNamedObj を格納するハッシュ表
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ModuleHash::ModuleHash(Alloc& alloc) :
  mAlloc(alloc),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
ModuleHash::~ModuleHash()
{
  // Cell は mAlloc のデストラクタで削除される．
  delete [] mTable;
}

// @brief 内容を空にする．
void
ModuleHash::clear()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 要素を追加する．
void
ModuleHash::add(const VlModule* obj)
{
  const VlNamedObj* parent = obj->parent();
  const char* name = obj->def_name();
  ymuint pos = hash_func(parent, name);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mParent == parent && cell->mName == name ) {
      // 同じモジュールが既に登録されていたらモジュール名での検索はできない．
      cell->mModule = NULL;
      return;
    }
  }
  if ( mNum >= mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    Cell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* next = cell->mLink;
	ymuint pos = hash_func(cell->mParent, cell->mName);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
    // サイズが変わったのでハッシュ値を再計算する．
    pos = hash_func(parent, name);
  }
  Cell* cell = static_cast<Cell*>(mAlloc.get_memory(sizeof(Cell)));;
  cell->mParent = parent;
  cell->mName = name;
  cell->mModule = obj;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief 名前から該当する要素を検索する．
const VlModule*
ModuleHash::find(const VlNamedObj* parent,
		 const char* name) const
{
  ymuint pos = hash_func(parent, name);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mParent == parent && strcmp(cell->mName, name) ) {
      return cell->mModule;
    }
  }
  return NULL;
}

// @brief このオブジェクトが使用しているメモリ量を返す．
ymuint
ModuleHash::allocated_size() const
{
  return sizeof(Cell*) * mSize;
}

// @brief テーブルの領域を確保する．
void
ModuleHash::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint
ModuleHash::hash_func(const VlNamedObj* parent,
		      const char* name) const
{
  ymuint h = 0;
  ymuint c;
  for ( ; (c = static_cast<ymuint>(*name)); ++ name) {
    h = h * 37 + c;
  }
  return ((reinterpret_cast<ympuint>(parent) * h) >> 8) % mSize;
}


END_NAMESPACE_YM_VERILOG
