
/// @file CiPinHash.cc
/// @brief CiPinHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiPinHash.h"
#include "CiCell.h"
#include "CiPin.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

inline
ymuint
hash_func(const CiCell* cell,
	  ShString name)
{
  return cell->id() + name.hash() * 99;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CiPinHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiPinHash::CiPinHash() :
  mSize(0),
  mTable(NULL),
  mLimit(0),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
CiPinHash::~CiPinHash()
{
  delete [] mTable;
}

// @brief ピンを追加する．
// @param[in] pin 追加するピン
void
CiPinHash::add(CiPin* pin)
{
  if ( mNum > mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    CiPin** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (CiPin* pin = old_table[i]; pin; ) {
	CiPin* tmp = pin;
	pin = pin->mLink;
	ymuint pos = hash_func(tmp->mCell, tmp->mName) % mSize;
	tmp->mLink = mTable[pos];
	mTable[pos] = tmp;
      }
    }
    delete [] old_table;
  }

  ymuint pos = hash_func(pin->mCell, pin->mName) % mSize;
  pin->mLink = mTable[pos];
  mTable[pos] = pin;
  ++ mNum;
}

// @brief ピンを取り出す．
// @param[in] cell セル
// @param[in] name 名前
// @return cell の name というピンを返す．
// @note なければ NULL を返す．
CiPin*
CiPinHash::get(const CiCell* cell,
	       ShString name) const
{
  ymuint pos = hash_func(cell, name) % mSize;
  for (CiPin* pin = mTable[pos]; pin; pin = pin->mLink) {
    if ( pin->mCell == cell && pin->mName == name ) {
      return pin;
    }
  }
  return NULL;
}

// @brief テーブルの領域を確保する．
// @param[in] req_size 要求するサイズ
void
CiPinHash::alloc_table(ymuint req_size)
{
  mSize = req_size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  mTable = new CiPin*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_CELL
