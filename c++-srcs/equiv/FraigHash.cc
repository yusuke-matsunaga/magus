
/// @file FraigHash.cc
/// @brief FraigHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "FraigHash.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
// FraigHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigHash::FraigHash() :
  mNum(0),
  mHashSize(0),
  mTable(nullptr)
{
  alloc_table(1024);
}

// @brief デストラクタ
FraigHash::~FraigHash()
{
  delete [] mTable;
}

// @brief 内容を空にする．
// @note テーブルサイズはそのまま
void
FraigHash::clear()
{
  for ( int i: Range(mHashSize) ) {
    mTable[i] = nullptr;
  }
  mNum = 0;
}

// @brief ハッシュ表を確保する．
void
FraigHash::alloc_table(int req_size)
{
  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }

  mTable = new FraigNode*[mHashSize];
  for ( int i: Range(mHashSize) ) {
    mTable[i] = nullptr;
  }

  mNextLimit = static_cast<int>(mHashSize * 1.8);
}

END_NAMESPACE_FRAIG
