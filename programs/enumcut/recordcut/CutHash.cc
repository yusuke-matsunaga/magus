
/// @file CutHash.cc
/// @brief CutHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CutHash.h"
#include "Cut.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CutHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CutHash::CutHash() :
  mTableSize(0)
{
  alloc_table(1024);
  mNum = 0;
}

// @brief デストラクタ
CutHash::~CutHash()
{
  clear();
  delete [] mTable;
}

// @brief 内容をクリアする．
void
CutHash::clear()
{
  for (ymuint32 i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 関数に対応するカットを返す．
Cut*
CutHash::find(TvFunc f)
{
  ymuint32 pos = f.hash() % mTableSize;
  for (Cut* cut = mTable[pos]; cut; cut = cut->mLink) {
    if ( cut->mFunc == f ) {
      return cut;
    }
  }

  return NULL;
}

// @brief カットを登録する．
void
CutHash::put(Cut* cut)
{
  if ( mNum >= mNextLimit ) {
    // テーブルを拡張する．
    Cut** old_table = mTable;
    ymuint32 old_size = mTableSize;
    alloc_table(old_size * 2);
    for (ymuint32 i = 0; i < old_size; ++ i) {
      for (Cut* cut = old_table[i]; cut; ) {
	Cut* next = cut->mLink;
	ymuint32 pos1 = cut->mFunc.hash() % mTableSize;
	cut->mLink = mTable[pos1];
	mTable[pos1] = cut;
	cut = next;
      }
    }
  }

  // テーブルに追加する．
  ymuint pos = cut->mFunc.hash() % mTableSize;
  cut->mLink = mTable[pos];
  mTable[pos] = cut;
}

// ハッシュ表を拡大する．
void
CutHash::alloc_table(ymuint32 new_size)
{
  mTable = new Cut*[new_size];
  mTableSize = new_size;
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);
  for (ymuint32 i = 0; i < new_size; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM
