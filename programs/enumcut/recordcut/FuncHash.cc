
/// @file FuncHash.cc
/// @brief FuncHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "FuncHash.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス FuncHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncHash::FuncHash() :
  mAlloc(sizeof(FuncData), 1024),
  mTableSize(0),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
FuncHash::~FuncHash()
{
  clear();
  delete [] mTable;
}

// @brief 内容をクリアする．
void
FuncHash::clear()
{
  for (ymuint32 i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
  mAlloc.destroy();
}

// @brief 関数を登録する．
// @note すでに登録されていたらなにもしない．
void
FuncHash::reg_func(const TvFunc& f)
{
  ymuint32 pos = f.hash() % mTableSize;
  for (FuncData* data = mTable[pos]; data; data = data->mLink) {
    if ( data->mFunc == f ) {
      return;
    }
  }

  if ( mNum >= mNextLimit ) {
    // テーブルを拡張する．
    FuncData** old_table = mTable;
    ymuint32 old_size = mTableSize;
    alloc_table(old_size * 2);
    // 再ハッシュする．
    for (ymuint32 i = 0; i < old_size; ++ i) {
      for (FuncData* data = old_table[i]; data; ) {
	FuncData* next = data->mLink;
	ymuint32 pos1 = data->mFunc.hash() % mTableSize;
	data->mLink = mTable[pos1];
	mTable[pos1] = data;
	data = next;
      }
    }
  }

  void* p = mAlloc.get_memory(sizeof(FuncData));
  FuncData* data = new (p) FuncData;
  data->mFunc = f;

  // テーブルに追加する．
  pos = f.hash() % mTableSize;
  data->mLink = mTable[pos];
  mTable[pos] = data;

  ++ mNum;
}

// @brief 関数のリストを取り出す．
void
FuncHash::func_list(vector<TvFunc>& func_list) const
{
  func_list.clear();
  func_list.reserve(mNum);
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (FuncData* data = mTable[i]; data; data = data->mLink) {
      func_list.push_back(data->mFunc);
    }
  }
}

// @brief 指定された入力数の関数のリストを取り出す．
void
FuncHash::func_list(ymuint ni,
		    vector<TvFunc>& func_list) const
{
  func_list.clear();
  // 数を数える．
  ymuint n = 0;
  {
    for (ymuint i = 0; i < mTableSize; ++ i) {
      for (FuncData* data = mTable[i]; data; data = data->mLink) {
	if ( data->mFunc.input_num() == ni) {
	  ++ n;
	}
      }
    }
  }
  func_list.reserve(n);
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (FuncData* data = mTable[i]; data; data = data->mLink) {
      if ( data->mFunc.input_num() == ni ) {
	func_list.push_back(data->mFunc);
      }
    }
  }
}

// ハッシュ表を拡大する．
void
FuncHash::alloc_table(ymuint32 new_size)
{
  mTable = new FuncData*[new_size];
  mTableSize = new_size;
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);
  for (ymuint32 i = 0; i < new_size; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM
