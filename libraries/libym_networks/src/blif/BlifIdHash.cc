
/// @file BlifIdHash.cc
/// @brief BlifIdHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BlifIdHash.h"
#include "IdCell.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
// クラス IdCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IdCell::IdCell(ymuint32 id,
	       const char* str) :
  mId(id),
  mFlags(0U),
  mLink(NULL)
{
  const char* s = str;
  char* d = mStr;
  while ( ((*d ++) = (*s ++)) ) ;
}

// @brief デストラクタ
IdCell::~IdCell()
{
}


//////////////////////////////////////////////////////////////////////
// クラス BlifIdHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifIdHash::BlifIdHash() :
  mAlloc(4096),
  mTableSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
BlifIdHash::~BlifIdHash()
{
  clear();
  delete [] mTable;
}

// @brief 内容をクリアする．
void
BlifIdHash::clear()
{
  for (ymuint32 i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mCellArray.clear();
  mAlloc.destroy();
}


BEGIN_NONAMESPACE

inline
ymuint
hash_func(const char* str)
{
  ymuint h = 0;
  for (char c; (c = *str); ++ str) {
    h = h * 33 + static_cast<ymuint>(c);
  }
  return h;
}

END_NONAMESPACE

// @brief 識別子に対応するセルを探す．
// @param[in] str 文字列
// @param[in] create 存在しないときに新規生成するなら true
// @return 対応する IdCell を返す．
IdCell*
BlifIdHash::find(const char* str,
		 bool create)
{
  assert_cond(str, __FILE__, __LINE__);
  ymuint32 pos0 = hash_func(str);
  ymuint32 pos = pos0 % mTableSize;
  for (IdCell* cell = mTable[pos]; cell; cell = cell->mLink) {
    if ( strcmp(cell->mStr, str) == 0 ) {
      return cell;
    }
  }

  if ( !create ) {
    return NULL;
  }

  if ( mCellArray.size() >= mNextLimit ) {
    // テーブルを拡張する．
    IdCell** old_table = mTable;
    ymuint32 old_size = mTableSize;
    alloc_table(old_size * 2);
    for (ymuint32 i = 0; i < old_size; ++ i) {
      for (IdCell* cell = old_table[i]; cell; ) {
	IdCell* next = cell->mLink;
	ymuint32 pos1 = hash_func(cell->mStr) % mTableSize;
	cell->mLink = mTable[pos1];
	mTable[pos1] = cell;
	cell = next;
      }
    }
  }

  // 新しいセルを確保する．
  ymuint32 l = strlen(str);
  ymuint32 reqsize = sizeof(IdCell) + l;
  void* p = mAlloc.get_memory(reqsize);
  IdCell* cell = new (p) IdCell(mCellArray.size(), str);
  mCellArray.push_back(cell);

  // テーブルに追加する．
  pos = pos0 % mTableSize;
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
  return cell;
}

// ハッシュ表を拡大する．
void
BlifIdHash::alloc_table(ymuint32 new_size)
{
  mTable = new IdCell*[new_size];
  mTableSize = new_size;
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);
  for (ymuint32 i = 0; i < new_size; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ID 番号から文字列を得る．
const char*
BlifIdHash::str(ymuint32 id) const
{
  return cell(id)->str();
}

// @brief ID番号から位置情報を得る．
const FileRegion&
BlifIdHash::loc(ymuint32 id) const
{
  return cell(id)->loc();
}

// @brief この識別子を定義している位置情報を返す．
const FileRegion&
BlifIdHash::def_loc(ymuint32 id) const
{
  return cell(id)->def_loc();
}

END_NAMESPACE_YM_NETWORKS_BLIF
