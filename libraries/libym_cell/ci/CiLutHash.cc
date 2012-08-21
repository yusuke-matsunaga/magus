
/// @file CiLutHash.cc
/// @brief CiLutHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CiLutHash.h"
#include "CiLut.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLutHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
CiLutHash::CiLutHash(Alloc& alloc) :
  mAlloc(alloc),
  mSize(0),
  mTable(NULL),
  mLimit(0),
  mNum(0)
{
  alloc_table(64);
}

// @brief デストラクタ
CiLutHash::~CiLutHash()
{
  mAlloc.put_memory(sizeof(CiLutTemplate*) * mNum, mTable);
}

// @brief テンプレートを追加する．
// @param[in] templ 追加するテンプレート
void
CiLutHash::add(CiLutTemplate* templ)
{
  if ( mNum > mLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mSize;
    CiLutTemplate** old_table = mTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (CiLutTemplate* templ = old_table[i]; templ; ) {
	CiLutTemplate* tmp = templ;
	templ = tmp->mLink;
	ymuint pos = tmp->mName.hash() % mSize;
	tmp->mLink = mTable[pos];
	mTable[pos] = tmp;
      }
    }
    mAlloc.put_memory(sizeof(CiLutTemplate*) * old_size, old_table);
  }

  ymuint pos = templ->mName.hash() % mSize;
  templ->mLink = mTable[pos];
  mTable[pos] = templ;
  ++ mNum;
}

// @brief テンプレートを取り出す．
// @param[in] name 名前
// @return name という名前のテンプレートを返す．
// @note なければ NULL を返す．
CiLutTemplate*
CiLutHash::get(ShString name) const
{
  ymuint pos = name.hash() % mSize;
  for (CiLutTemplate* templ = mTable[pos]; templ; templ = templ->mLink) {
    if ( templ->mName == name ) {
      return templ;
    }
  }
  return NULL;
}

// @brief テーブルの領域を確保する．
// @param[in] req_size 要求するサイズ
void
CiLutHash::alloc_table(ymuint req_size)
{
  mSize = req_size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  void* p = mAlloc.get_memory(sizeof(CiLutTemplate*) * mSize);
  mTable = new (p) CiLutTemplate*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_CELL
