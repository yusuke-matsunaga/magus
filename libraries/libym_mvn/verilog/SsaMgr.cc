
/// @file libym_mvn/verilog/SsaMgr.cc
/// @brief SsaMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SsaMgr.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
SsaMgr::SsaMgr() :
  mAlloc(sizeof(Cell), 1024),
  mNum(0)
{
  // ダミー
  mDeclArray.push_back(pair<const DfgBlock*, const VlDecl*>());

  alloc_table(1024);
}

// @brief デストラクタ
SsaMgr::~SsaMgr()
{
  delete [] mTable;
}

// @brief 内容を空にする
void
SsaMgr::clear()
{
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
  mAlloc.destroy();
}

// @brief 新しい ID を割り当てる．
ymuint
SsaMgr::new_id(const DfgBlock* block,
	       const VlDecl* decl)
{
  ymuint id = mDeclArray.size();
  mDeclArray.push_back(make_pair(block, decl));

  Cell* cell = find_cell(block, decl);
  if ( cell == NULL ) {
    // (block, decl) に対応する Cell を作る．
    if ( mNum >= mLimit ) {
      // テーブルを拡張する．
      ymuint old_size = mSize;
      Cell** old_table = mTable;
      alloc_table(old_size << 1);
      for (ymuint i = 0; i < old_size; ++ i) {
	for (Cell* cell = old_table[i]; cell; ) {
	  Cell* next = cell->mLink;
	  ymuint pos = hash_func(cell->mBlock, cell->mDecl);
	  cell->mLink = mTable[pos];
	  mTable[pos] = cell;
	  cell = next;
	}
      }
      delete [] old_table;
    }

    ymuint pos = hash_func(block, decl);
    ymuint idx = pos % mSize;
    void* p = mAlloc.get_memory(sizeof(Cell));
    cell = new (p) Cell(block, decl);
    cell->mLink = mTable[idx];
    mTable[idx] = cell;
    ++ mNum;
  }
  cell->mId = id;

  return id;
}

// @brief ID 番号を取り出す．
ymuint
SsaMgr::last_id(const DfgBlock* block,
		const VlDecl* decl)
{
  Cell* cell = find_cell(block, decl);
  if ( cell ) {
    return cell->mId;
  }
  else {
    return 0;
  }
}

// @brief ID番号をキーにして対応する要素を返す．
// @param[in] id インスタンス番号
const VlDecl*
SsaMgr::decl(ymuint id)
{
  return mDeclArray[id].second;
}

// @brief テーブルの領域を確保する．
// @param[in] size 必要なサイズ
void
SsaMgr::alloc_table(ymuint size)
{
  mSize = size;
  mLimit = static_cast<ymuint>(mSize * 1.8);
  mTable = new Cell*[mSize];
  for (ymuint i = 0; i < mSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief セルを探す．
// @param[in] block 親のブロック
// @param[in] decl 宣言要素
SsaMgr::Cell*
SsaMgr::find_cell(const DfgBlock* block,
		  const VlDecl* decl)
{
  ymuint pos = hash_func(block, decl);
  ymuint idx = pos % mSize;
  for (Cell* cell = mTable[idx]; cell; cell = cell->mLink) {
    if ( cell->mBlock == block && cell->mDecl == decl ) {
      return cell;
    }
  }
  return NULL;
}

// @brief ハッシュ値を計算する．
// @param[in] block 親のブロック
// @param[in] decl 宣言要素
ymuint
SsaMgr::hash_func(const DfgBlock* block,
		  const VlDecl* decl)
{
  ympuint a = reinterpret_cast<ympuint>(block);
  ympuint b = reinterpret_cast<ympuint>(decl);
  return (a * b) >> 8;
}

END_NAMESPACE_YM_MVN
