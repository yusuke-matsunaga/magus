
/// @file libym_mvn/verilog/DeclHash.cc
/// @brief DeclHash の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "DeclHash.h"
#include "ym_mvn/MvNode.h"
#include "ym_verilog/vl/VlDecl.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

// @brief コンストラクタ
DeclHash::DeclHash() :
  mAlloc(sizeof(Cell), 1024),
  mNum(0),
  mNextId(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
DeclHash::~DeclHash()
{
  delete [] mTable;
}

// @brief 内部のデータをクリアする．
void
DeclHash::clear()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
  mNextId = 0;
  mAlloc.destroy();
}

// @brief ID番号を得る．
// @param[in] decl 宣言要素
// @return ID番号
// @note 登録されていなかった場合には新しい番号を割り当てる．
ymuint
DeclHash::get_id(const VlDecl* decl)
{
  Cell* cell = find_cell(decl);
  if ( cell ) {
    return cell->mId;
  }
  else {
    ymuint id = mNextId;
    ++ mNextId;
    put_cell(decl, id);
    return id;
  }
}

// @brief ID番号を得る．
// @param[in] decl 配列型宣言要素
// @param[in] offset オフセット
// @return ID番号
// @note 登録されていなかった場合には新しい番号を割り当てる．
ymuint
DeclHash::get_id(const VlDecl* decl,
		 ymuint offset)
{
  Cell* cell = find_cell(decl);
  ymuint base = 0;
  if ( cell ) {
    base = cell->mId;
  }
  else {
    base = mNextId;
    ++ mNextId;
    put_cell(decl, base);
  }
  return base + offset;
}

// @brief ID番号の最大値 + 1を返す．
ymuint
DeclHash::max_id() const
{
  return mNextId;
}

// @brief Cell を登録する．
void
DeclHash::put_cell(const VlDecl* decl,
		   ymuint id)
{
  if ( mNum >= mLimit ) {
    // テーブルを拡大する．
    ymuint old_size = mSize;
    Cell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (Cell* cell = old_table[i]; cell; ) {
	Cell* next = cell->mLink;
	ymuint pos = hash_func(cell->mDecl);
	cell->mLink = mTable[pos];
	mTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint pos = hash_func(decl);
  void* p = mAlloc.get_memory(sizeof(Cell));
  Cell* cell = new (p) Cell;
  cell->mDecl = decl;
  cell->mId = id;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief Cell を探す．
DeclHash::Cell*
DeclHash::find_cell(const VlDecl* decl) const
{
  ymuint pos = hash_func(decl);
  for (Cell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mDecl == decl ) {
      return cell;
    }
  }
  return NULL;
}

// @brief テーブルの領域を確保する．
// @param[in] size 必要なサイズ
void
DeclHash::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint32>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ハッシュ値を計算する．
ymuint
DeclHash::hash_func(const VlDecl* decl) const
{
  ympuint tmp = reinterpret_cast<ympuint>(decl);
  return ((tmp * tmp) >> 10) % mSize;
}

END_NAMESPACE_YM_MVN_VERILOG
