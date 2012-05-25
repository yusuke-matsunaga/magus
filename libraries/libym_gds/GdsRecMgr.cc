
/// @file GdsRecMgr.cc
/// @brief GdsRecMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsRecMgr.h"
#include "ym_gds/GdsRecord.h"


BEGIN_NAMESPACE_YM_GDS

// @brief コンストラクタ
GdsRecMgr::GdsRecMgr() :
  mAlloc(4096)
{
}

// @brief デストラクタ
GdsRecMgr::~GdsRecMgr()
{
}

// @brief レコードの生成
GdsRecord*
GdsRecMgr::alloc_rec(ymuint32 dsize)
{
  ymuint32 recsize = sizeof(GdsRecord) + (dsize - 1) * sizeof(ymuint8);
  void* p = mAlloc.get_memory(recsize);
  return new (p) GdsRecord;
}

// @brief レコードの破壊
// @param[in] rec 破壊するレコード
void
GdsRecMgr::free_rec(GdsRecord* rec)
{
  ymuint32 recsize = sizeof(GdsRecord) + (rec->dsize() - 1) * sizeof(ymuint8);
  mAlloc.put_memory(recsize, rec);
}

END_NAMESPACE_YM_GDS
