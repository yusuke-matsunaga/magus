
/// @file ImpHash.cc
/// @brief ImpHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpHash.h"
#include "ImpCell.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpHash
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
ImpHash::ImpHash() :
  mAlloc(sizeof(ImpCell), 1024)
{
  mNum = 0;
  mHashSize = 0;
  mHashTable = NULL;
  alloc_table(1024);
}

// @brief デストラクタ
ImpHash::~ImpHash()
{
}

// @brief 該当する含意情報が含まれているか調べる．
bool
ImpHash::check(ymuint src_id,
	       ymuint src_val,
	       ymuint dst_id,
	       ymuint dst_val) const
{
  if ( src_id > dst_id ) {
    ymuint tmp_id = src_id;
    ymuint tmp_val = src_val;
    src_id = dst_id;
    src_val = dst_val ^ 1;
    dst_id = tmp_id;
    dst_val = tmp_val ^ 1;
  }

  ymuint id1 = src_id * 2 + src_val;
  ymuint id2 = dst_id * 2 + dst_val;
  ymuint pos = hash_func(id1, id2);
  for (ImpCell* cell = mHashTable[pos]; cell; cell = cell->mLink) {
    if ( cell->id1() == id1 && cell->id2() == id2 ) {
      return true;
    }
  }
  return false;
}

// @brief 含意情報を追加する．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
// @param[in] dst_id 含意先のノード番号
// @param[in] dst_val 含意先の値 ( 0 or 1 )
void
ImpHash::put(ymuint src_id,
	     ymuint src_val,
	     ymuint dst_id,
	     ymuint dst_val)
{
  if ( src_id > dst_id ) {
    ymuint tmp_id = src_id;
    ymuint tmp_val = src_val;
    src_id = dst_id;
    src_val = dst_val ^ 1;
    dst_id = tmp_id;
    dst_val = tmp_val ^ 1;
  }

  if ( check(src_id, src_val, dst_id, dst_val) ) {
    return;
  }
  ImpCell* cell = new_cell();
  cell->set(src_id, src_val, dst_id, dst_val);

  if ( mNum >= mHashLimit ) {
    // テーブルを拡張する．
    ymuint old_size = mHashSize;
    ImpCell** old_table = mHashTable;
    alloc_table(old_size << 1);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (ImpCell* cell = old_table[i]; cell; ) {
	ImpCell* next = cell->mLink;
	ymuint pos = hash_func(cell->id1(), cell->id2());
	cell->mLink = mHashTable[pos];
	mHashTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
  ymuint id1 = src_id * 2 + src_val;
  ymuint id2 = dst_id * 2 + dst_val;
  ymuint pos = hash_func(id1, id2);
  cell->mLink = mHashTable[pos];
  mHashTable[pos] = cell;

#if defined(ANALYZE_HASH)
  {
    ymuint n = 0;
    ymuint m = 0;
    ymuint max = 0;
    for (ymuint i = 0; i < mHashSize; ++ i) {
      ymuint c = 0;
      for (ImpCell* cell = mHashTable[i]; cell; cell = cell->mLink) {
	++ c;
      }
      if ( c > 0 ) {
	++ n;
	if ( max < c ) {
	  max = c;
	}
	m += c;
      }
    }
    double avr = static_cast<double>(m) / static_cast<double>(n);
    cout << "Hash avr. = " << avr << ", max = " << max << endl;
  }
#endif
}

// @brief 要素数のヒントを与える．
void
ImpHash::reserve(ymuint size)
{
  ymuint old_size = mHashSize;
  ImpCell** old_table = mHashTable;
  ymuint new_size = old_size;
  while ( new_size < size ) {
    new_size <<= 1;
  }
  if ( new_size > old_size ) {
    alloc_table(new_size);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (ImpCell* cell = old_table[i]; cell; ) {
	ImpCell* next = cell->mLink;
	ymuint pos = hash_func(cell->id1(), cell->id2());
	cell->mLink = mHashTable[pos];
	mHashTable[pos] = cell;
	cell = next;
      }
    }
    delete [] old_table;
  }
}

// @brief ImpCell を確保する．
ImpCell*
ImpHash::new_cell()
{
  void* p = mAlloc.get_memory(sizeof(ImpCell));
  ImpCell* cell = new (p) ImpCell;
  cell->mLink = NULL;
  ++ mNum;
  return cell;
}

// @brief テーブルの領域を確保する．
void
ImpHash::alloc_table(ymuint size)
{
  mHashSize = size;
  mHashLimit = static_cast<ymuint>(mHashSize * 1.8);
  mHashTable = new ImpCell*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
}

// @brief ハッシュ関数
inline
ymuint
ImpHash::hash_func(ymuint id1,
		   ymuint id2) const
{
  return (((id1 + 1023) * id2) >> 8) % mHashSize;
}

END_NAMESPACE_YM_NETWORKS
