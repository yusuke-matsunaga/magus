
/// @file CiCellHash.cc
/// @brief CiCellHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiCellHash.h"
#include "CiCell.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiCellHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
CiCellHash::CiCellHash(Alloc& alloc) :
  mAlloc(alloc),
  mSize(0),
  mTable(NULL),
  mLimit(0),
  mNum(0)
{
  alloc_table(256);
}

// @brief デストラクタ
CiCellHash::~CiCellHash()
{
  delete [] mTable;
}

// @brief セルを追加する．
// @param[in] cell 追加するセル
void
CiCellHash::add(CiCell* cell)
{
  if ( mNum > mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    CiCell** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (CiCell* cell = old_table[i]; cell; ) {
	CiCell* tmp = cell;
	cell = tmp->mLink;
	ymuint pos = tmp->mName.hash() % mSize;
	tmp->mLink = mTable[pos];
	mTable[pos] = tmp;
      }
    }
    mAlloc.put_memory(sizeof(CiCell*) * old_size, old_table);
  }

  ymuint pos = cell->mName.hash() % mSize;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  ++ mNum;
}

// @brief セルを取り出す．
// @param[in] name 名前
// @return name という名前のセルを返す．
// @note なければ NULL を返す．
CiCell*
CiCellHash::get(ShString name) const
{
  ymuint pos = name.hash() % mSize;
  for (CiCell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( cell->mName == name ) {
      return cell;
    }
  }
  return NULL;
}

// @brief テーブルの領域を確保する．
// @param[in] req_size 要求するサイズ
void
CiCellHash::alloc_table(ymuint req_size)
{
  mSize = req_size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  void* p = mAlloc.get_memory(sizeof(CiCell*) * mSize);
  mTable = new (p) CiCell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_CELL
