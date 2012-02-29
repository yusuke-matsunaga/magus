
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
    cout << "Node#" << src_id << ": " << src_val << endl;
    for (ImpList::iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      const ImpCell& imp = *p;
      ymuint dst_id = imp.dst_id();
      ymuint dst_val = imp.dst_val();
      cout << "   => Node#" << dst_id << ": " << dst_val << endl;
    }
    cout << endl;
  }
  cout << "Total " << size() << " implications" << endl;
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
  for (ymuint i = 0; i < mArraySize; ++ i) {
    mArray[i].mTop = new_cell();
  }
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
  ImpCell* cell = new_cell();
  cell->set(src_id, src_val, dst_id, dst_val);
  mArray[src_id * 2 + src_val].push_back(cell);

  if ( mImpNum >= mHashLimit ) {
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

// @brief 含意情報を追加する．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
// @param[in] imp_list 含意リスト
void
ImpInfo::put(ymuint src_id,
	     ymuint src_val,
	     const vector<ImpVal>& imp_list)
{
  for (vector<ImpVal>::const_iterator p = imp_list.begin();
       p != imp_list.end(); ++ p) {
    ymuint dst_id = p->id();
    ymuint dst_val = p->val();
    if ( dst_id != src_id ) {
      put(src_id, src_val, dst_id, dst_val);
    }
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

END_NAMESPACE_YM_NETWORKS
