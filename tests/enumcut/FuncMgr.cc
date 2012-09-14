
/// @file FuncMgr.cc
/// @brief FuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "FuncMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// クラス FuncMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncMgr::FuncMgr() :
  mTableSize(0),
  mNum(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
FuncMgr::~FuncMgr()
{
  clear();
  delete [] mTable;
}

// @brief 内容をクリアする．
void
FuncMgr::clear()
{
  for (ymuint32 i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 関数を登録する．
// @note すでに登録されていたらなにもしない．
void
FuncMgr::reg_func(const TvFunc& f)
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

  new_data(f);
}

// @brief 関数のリストを取り出す．
void
FuncMgr::func_list(vector<TvFunc>& func_list) const
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
FuncMgr::func_list(ymuint ni,
		   vector<TvFunc>& func_list) const
{
  func_list.clear();
  // 数を数える．
  ymuint n = 0;
  {
    for (ymuint i = 0; i < mTableSize; ++ i) {
      for (FuncData* data = mTable[i]; data; data = data->mLink) {
	if ( data->mFunc.ni() == ni) {
	  ++ n;
	}
      }
    }
  }
  func_list.reserve(n);
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (FuncData* data = mTable[i]; data; data = data->mLink) {
      if ( data->mFunc.ni() == ni ) {
	func_list.push_back(data->mFunc);
      }
    }
  }
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
FuncMgr::dump(BinO& s) const
{
  s << mNum;
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (FuncData* data = mTable[i]; data; data = data->mLink) {
      s << data->mFunc;
    }
  }
}

// @brief バイナリダンプされたファイルを読み込む．
void
FuncMgr::restore(BinI& s)
{
  clear();
  ymuint32 n;
  s >> n;

  ymuint new_size = mTableSize;
  while ( new_size <= n ) {
    new_size <<= 1;
  }
  if ( new_size > mTableSize ) {
    alloc_table(new_size);
  }

  for (ymuint i = 0; i < n; ++ i) {
    TvFunc f;
    s >> f;
    new_data(f);
  }
}

// @brief 新しい FuncData を作り登録する．
void
FuncMgr::new_data(const TvFunc& f)
{
  FuncData* data = new FuncData;
  data->mFunc = f;

  // テーブルに追加する．
  ymuint pos = f.hash() % mTableSize;
  data->mLink = mTable[pos];
  mTable[pos] = data;

  ++ mNum;
}

// ハッシュ表を拡大する．
void
FuncMgr::alloc_table(ymuint32 new_size)
{
  mTable = new FuncData*[new_size];
  mTableSize = new_size;
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);
  for (ymuint32 i = 0; i < new_size; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_NETWORKS
