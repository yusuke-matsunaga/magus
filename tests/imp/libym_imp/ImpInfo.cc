
/// @file ImpInfo.cc
/// @brief ImpInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpInfo.h"
#include "ImpCell.h"
#include "ImpList.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
ImpInfo::ImpInfo() :
  mAlloc(sizeof(ImpCell), 1024)
{
  mImpNum = 0;
  mArraySize = 0;
  mArray = NULL;
  mHashSize = 0;
  mHashTable = NULL;
  alloc_table(1024);
}

// @brief デストラクタ
ImpInfo::~ImpInfo()
{
}

// @brief 含意情報のリストを取り出す．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
const ImpList&
ImpInfo::get(ymuint src_id,
	     ymuint src_val) const
{
  return mArray[src_id * 2 + src_val];
}

// @brief 該当する含意情報が含まれているか調べる．
bool
ImpInfo::check(ymuint src_id,
	       ymuint src_val,
	       ymuint dst_id,
	       ymuint dst_val) const
{
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

// @brief 含意の総数を得る．
ymuint
ImpInfo::size() const
{
  ymuint n = 0;
  for (ymuint i = 0; i < mArraySize; ++ i) {
    ymuint src_id = i / 2;
    const ImpList& imp_list = mArray[i];
    for (ImpList::iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      const ImpCell& imp = *p;
      ymuint dst_id = imp.dst_id();
      ++ n;
    }
  }
  return n;
}

// @brief 実際に保持している含意の総数を返す．
ymuint
ImpInfo::imp_num() const
{
  return mImpNum;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
ImpInfo::print(ostream& s) const
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    const ImpList& imp_list = mArray[i];
    if ( imp_list.empty() ) continue;
    ymuint src_id = i / 2;
    ymuint src_val = i % 2;
    s << "Node#" << src_id << ": " << src_val << endl;
    for (ImpList::iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      const ImpCell& imp = *p;
      ymuint dst_id = imp.dst_id();
      ymuint dst_val = imp.dst_val();
      s << "   => Node#" << dst_id << ": " << dst_val << endl;
    }
    s << endl;
  }
  s << "Total " << size() << " implications" << endl;
}

// @brief 統計情報を出力する．
void
ImpInfo::print_stats(ostream& s) const
{
  s << "Total " << size() << " implications ("
    << static_cast<double>(size()) / (mArraySize * mArraySize) * 100
    << "%)" << endl;
}

// @brief 内容をクリアする．
void
ImpInfo::clear()
{
}

// @brief サイズを設定する．
// @param[in] max_id ID番号の最大値
void
ImpInfo::set_size(ymuint max_id)
{
  clear();

  mArraySize = max_id * 2;
  mArray = new ImpList[mArraySize];
}

// @brief 含意情報を追加する．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
// @param[in] dst_id 含意先のノード番号
// @param[in] dst_val 含意先の値 ( 0 or 1 )
void
ImpInfo::put(ymuint src_id,
	     ymuint src_val,
	     ymuint dst_id,
	     ymuint dst_val)
{
  if ( check(src_id, src_val, dst_id, dst_val) ) {
    return;
  }
  ImpCell* cell = new_cell();
  cell->set(src_id, src_val, dst_id, dst_val);
  mArray[src_id * 2 + src_val].push_back(cell);

  if ( mImpNum >= mHashLimit ) {
    // テーブルを拡張する．
    cout << "ImpInfo::expand()" << endl;
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

  ++ mImpNum;

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
ImpInfo::reserve(ymuint size)
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
ImpInfo::new_cell()
{
  void* p = mAlloc.get_memory(sizeof(ImpCell));
  ImpCell* cell = new (p) ImpCell;
  cell->mLink = NULL;
  return cell;
}

// @brief テーブルの領域を確保する．
void
ImpInfo::alloc_table(ymuint size)
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
ImpInfo::hash_func(ymuint id1,
		   ymuint id2) const
{
  return (((id1 + 1023) * id2) >> 8) % mHashSize;
}

// @brief 引き算
// @param[in] right 比較対象
// @param[out] result 結果を格納する変数
// @return 結果の要素数を返す．( = result.size() )
ymuint
ImpInfo::compare(const ImpInfo& right,
		 ImpInfo& result) const
{
  ymuint n = mArraySize;
  assert_cond( n == right.mArraySize, __FILE__, __LINE__);
  result.clear();
  result.set_size(n);
  for (ymuint src_i = 0; src_i < n; ++ src_i) {
    const ImpList& imp_list0 = mArray[src_i];
    const ImpList& imp_list1 = right.mArray[src_i];
    ImpList::iterator p0 = imp_list0.begin();
    ImpList::iterator p1 = imp_list1.begin();
    ImpList::iterator e0 = imp_list0.end();
    ImpList::iterator e1 = imp_list1.end();
    ymuint n0 = imp_list0.size();
    vector<ymuint> list0(n0);
    for (ymuint i = 0; p0 != e0; ++ p0, ++ i) {
      list0[i] = p0->id2();
    }
    sort(list0.begin(), list0.end());
    ymuint n1 = imp_list1.size();
    vector<ymuint> list1(n1);
    for (ymuint i = 0; p1 != e1; ++ p1, ++ i) {
      list1[i] = p1->id2();
    }
    sort(list1.begin(), list1.end());
    ymuint i0 = 0;
    ymuint i1 = 0;
    while ( i0 < n0 && i1 < n1 ) {
      ymuint id0 = list0[i0];
      ymuint id1 = list1[i1];
      if ( id0 < id1 ) {
	++ i0;
      }
      else if ( id0 > id1 ) {
	ymuint dst_id = id1 / 2;
	result.put(src_i / 2, src_i % 2,
		   dst_id, id1 % 2);
	++ i1;
      }
      else { // id0 == id1
	++ i0;
	++ i1;
      }
    }
    for ( ; i1 < n1; ++ i1) {
      ymuint id2 = list1[i1];
      ymuint dst_id = id2 / 2;
      result.put(src_i / 2, src_i % 2,
		 dst_id, id2 % 2);
    }
  }
  return result.size();
}

END_NAMESPACE_YM_NETWORKS
