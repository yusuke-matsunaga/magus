
/// @file GdsRecMgr.cc
/// @brief GdsRecMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsRecMgr.h"
#include "gds/GdsRecord.h"
#include "gds/GdsScanner.h"


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
// @param[in] scanner 字句解析器
GdsRecord*
GdsRecMgr::new_record(const GdsScanner& scanner)
{
  ymuint size = scanner.cur_size();
  ymuint dsize = size - 4;
  ymuint32 recsize = sizeof(GdsRecord) + (dsize - 1) * sizeof(ymuint8);
  void* p = mAlloc.get_memory(recsize);
  GdsRecord* rec = new (p) GdsRecord;
  rec->mOffset = scanner.cur_offset();
  rec->mSize = size;
  rec->mRtype = scanner.cur_rtype();
  rec->mDtype = scanner.cur_dtype();
  for (ymuint i = 0; i < dsize; ++ i) {
    rec->mData[i] = scanner.cur_data()[i];
  }
  return rec;
}

// @brief レコードの破壊
// @param[in] rec 破壊するレコード
void
GdsRecMgr::free_record(GdsRecord* rec)
{
  ymuint32 recsize = sizeof(GdsRecord) + (rec->dsize() - 1) * sizeof(ymuint8);
  mAlloc.put_memory(recsize, rec);
}

END_NAMESPACE_YM_GDS
