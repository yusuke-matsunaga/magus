
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McMatrix.h"
#include "McSolverImpl.h"


//#define VERIFY_MCMATRIX 1


BEGIN_NAMESPACE_YM_MINCOV

bool mcmatrix_debug = false;

//////////////////////////////////////////////////////////////////////
// クラス McRowHead
//////////////////////////////////////////////////////////////////////

// @brief 要素を追加する位置を探す．
// @param[in] cell 追加する要素
// @retval true 追加が成功した．
// @retval false 同じ要素がすでに存在した．
// @note 結果は cell の mLeftLink, mRightLink に設定される．
bool
McRowHead::search_insert_pos(McCell* cell)
{
  ymuint32 col_pos = cell->col_pos();
  McCell* pcell;
  McCell* ncell;
  if ( num() == 0 || back()->col_pos() < col_pos ) {
    // 末尾への追加
    ncell = &mDummy;
    pcell = ncell->mLeftLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->col_pos() >= col_pos が成り立っている．
    for (pcell = &mDummy; ; pcell = ncell) {
      ncell = pcell->mRightLink;
      if ( ncell->col_pos() == col_pos ) {
	// 列番号が重複しているので無視する．
	return false;
      }
      if ( ncell->col_pos() > col_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      assert_cond(!is_end(ncell), __FILE__, __LINE__);
    }
  }
  cell->mLeftLink = pcell;
  cell->mRightLink = ncell;

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス McColHead
//////////////////////////////////////////////////////////////////////

// @brief 要素を追加する位置を探す．
// @param[in] cell 追加する要素
// @retval true 追加が成功した．
// @retval false 同じ要素がすでに存在した．
// @note 結果は cell の mUpLink, mDownLink に設定される．
bool
McColHead::search_insert_pos(McCell* cell)
{
  ymuint32 row_pos = cell->row_pos();
  McCell* pcell;
  McCell* ncell;
  if ( num() == 0 || back()->row_pos() < row_pos ) {
    // 末尾への追加
    ncell = &mDummy;
    pcell = ncell->mUpLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->row_pos() >= row_pos が成り立っている．
    for (pcell = &mDummy; ; pcell = ncell) {
      ncell = pcell->mDownLink;
      if ( ncell->row_pos() == row_pos ) {
	// 列番号が重複しているので無視する．
	return false;
      }
      if ( ncell->row_pos() > row_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      assert_cond(!is_end(ncell), __FILE__, __LINE__);
    }
  }
  cell->mUpLink = pcell;
  cell->mDownLink = ncell;

  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス McMatrix
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] row_size 行数
// @param[in] col_size 列数
// @param[in] cost_array コストの配列
McMatrix::McMatrix(ymuint32 row_size,
		   ymuint32 col_size,
		   const ymuint32* cost_array) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mRowHead(0),
  mColArray(NULL),
  mColHead(0),
  mCostArray(cost_array),
  mDelStack(NULL)
{
  mRowNum = 0;
  mColNum = 0;
  resize(row_size, col_size);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
McMatrix::McMatrix(const McMatrix& src) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mRowHead(0),
  mColArray(NULL),
  mColHead(0),
  mDelStack(NULL)
{
  mRowNum = 0;
  mColNum = 0;
  resize(src.row_size(), src.col_size());
  copy(src);
}

// @brief 部分的なコピーコンストラクタ
// @param[in] src コピー元のオブジェクト
// @param[in] row_list コピーする行番号のリスト
// @param[in] col_list コピーする列番号のリスト
McMatrix::McMatrix(McMatrix& src,
		   const vector<ymuint32>& row_list,
		   const vector<ymuint32>& col_list) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mRowHead(0),
  mColArray(NULL),
  mColHead(0),
  mDelStack(NULL)
{
  mRowNum = 0;
  mColNum = 0;
  resize(src.row_size(), src.col_size());

  McRowHead* prev_row = &mRowHead;
  for (vector<ymuint32>::const_iterator p = row_list.begin();
       p != row_list.end(); ++ p) {
    ymuint row_pos = *p;
    McRowHead* row1 = src.row(row_pos);
    mRowArray[row_pos] = row1;
    prev_row->mNext = row1;
    row1->mPrev = prev_row;
    prev_row = row1;
    ++ mRowNum;
  }
  prev_row->mNext = &mRowHead;
  mRowHead.mPrev = prev_row;

  McColHead* prev_col = &mColHead;
  for (vector<ymuint32>::const_iterator p = col_list.begin();
       p != col_list.end(); ++ p) {
    ymuint col_pos = *p;
    McColHead* col1 = src.col(col_pos);
    mColArray[col_pos] = col1;
    prev_col->mNext = col1;
    col1->mPrev = prev_col;
    prev_col = col1;
    ++ mColNum;
  }
  prev_col->mNext = &mColHead;
  mColHead.mPrev = prev_col;

  mCostArray = src.mCostArray;
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
const McMatrix&
McMatrix::operator=(const McMatrix& src)
{
  if ( this != &src ) {
    clear();
    resize(src.row_size(), src.col_size());
    copy(src);
  }
  return *this;
}

// @brief デストラクタ
McMatrix::~McMatrix()
{
  delete [] mRowArray;
  delete [] mColArray;
  delete [] mDelStack;
}

// @brief 内容をクリアする．
void
McMatrix::clear()
{
  mCellAlloc.destroy();

  for (ymuint i = 0; i < row_size(); ++ i) {
    delete mRowArray[i];
    mRowArray[i] = NULL;
  }
  for (ymuint i = 0; i < col_size(); ++ i) {
    delete mColArray[i];
    mColArray[i] = NULL;
  }

  delete [] mRowArray;
  delete [] mColArray;
  delete [] mDelStack;

  mRowArray = NULL;
  mColArray = NULL;

  mRowNum = 0;
  mColNum = 0;

  mRowHead.mNext = mRowHead.mPrev = &mRowHead;
  mColHead.mNext = mColHead.mPrev = &mColHead;

  mDelStack = NULL;
  mStackTop = 0;
}

// @brief サイズを変更する．
// @param[in] row_size 行数
// @param[in] col_size 列数
void
McMatrix::resize(ymuint32 row_size,
		 ymuint32 col_size)
{
  if ( mRowSize != row_size || mColSize != col_size ) {
    clear();

    mRowSize = row_size;
    mColSize = col_size;
    mRowArray = new McRowHead*[mRowSize];
    for (ymuint i = 0; i < mRowSize; ++ i) {
      mRowArray[i] = NULL;
    }
    mColArray = new McColHead*[mColSize];
    for (ymuint i = 0; i < mColSize; ++ i) {
      mColArray[i] = NULL;
    }

    mRowHead.mNext = &mRowHead;
    mRowHead.mPrev = &mRowHead;
    mRowHead.clear();

    mColHead.mNext = &mColHead;
    mColHead.mPrev = &mColHead;
    mColHead.clear();

    delete [] mDelStack;
    mDelStack = new ymuint32[row_size + col_size];
    mStackTop = 0;
  }
}

// @brief 内容をコピーする．
void
McMatrix::copy(const McMatrix& src)
{
  assert_cond(row_size() == src.row_size(), __FILE__, __LINE__);
  assert_cond(col_size() == src.col_size(), __FILE__, __LINE__);

  for (const McRowHead* src_row = src.row_front();
       !src.is_row_end(src_row); src_row = src_row->next()) {
    ymuint row_pos = src_row->pos();
    for (const McCell* src_cell = src_row->front();
	 !src_row->is_end(src_cell); src_cell = src_cell->row_next()) {
      ymuint col_pos = src_cell->col_pos();
      insert_elem(row_pos, col_pos);
    }
  }

  mCostArray = src.mCostArray;
}

// @brief 分割した行列をもとに戻す．
void
McMatrix::merge(McMatrix& matrix1,
		McMatrix& matrix2)
{
  McRowHead* prev_row = &mRowHead;
  McRowHead* row1 = matrix1.mRowHead.mNext;
  McRowHead* row2 = matrix2.mRowHead.mNext;
  while ( row1 != &matrix1.mRowHead && row2 != &matrix2.mRowHead ) {
    if ( row1->pos() < row2->pos() ) {
      matrix1.mRowArray[row1->pos()] = NULL;
      prev_row->mNext = row1;
      row1->mPrev = prev_row;
      prev_row = row1;
      row1 = row1->mNext;
    }
    else if ( row1->pos() > row2->pos() ) {
      matrix2.mRowArray[row2->pos()] = NULL;
      prev_row->mNext = row2;
      row2->mPrev = prev_row;
      prev_row = row2;
      row2 = row2->mNext;
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  for ( ; row1 != &matrix1.mRowHead; row1 = row1->mNext) {
    matrix1.mRowArray[row1->pos()] = NULL;
    prev_row->mNext = row1;
    row1->mPrev = prev_row;
    prev_row = row1;
  }
  for ( ; row2 != &matrix2.mRowHead; row2 = row2->mNext) {
    matrix2.mRowArray[row2->pos()] = NULL;
    prev_row->mNext = row2;
    row2->mPrev = prev_row;
    prev_row = row2;
  }
  prev_row->mNext = &mRowHead;
  mRowHead.mPrev = prev_row;

  McColHead* prev_col = &mColHead;
  McColHead* col1 = matrix1.mColHead.mNext;
  McColHead* col2 = matrix2.mColHead.mNext;
  while ( col1 != &matrix1.mColHead && col2 != &matrix2.mColHead ) {
    if ( col1->pos() < col2->pos() ) {
      matrix1.mColArray[col1->pos()] = NULL;
      prev_col->mNext = col1;
      col1->mPrev = prev_col;
      prev_col = col1;
      col1 = col1->mNext;
    }
    else if ( col1->pos() > col2->pos() ) {
      matrix2.mColArray[col2->pos()] = NULL;
      prev_col->mNext = col2;
      col2->mPrev = prev_col;
      prev_col = col2;
      col2 = col2->mNext;
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  for ( ; col1 != &matrix1.mColHead; col1 = col1->mNext) {
    matrix1.mColArray[col1->pos()] = NULL;
    prev_col->mNext = col1;
    col1->mPrev = prev_col;
    prev_col = col1;
  }
  for ( ; col2 != &matrix2.mColHead; col2 = col2->mNext) {
    matrix2.mColArray[col2->pos()] = NULL;
    prev_col->mNext = col2;
    col2->mPrev = prev_col;
    prev_col = col2;
  }
  prev_col->mNext = &mColHead;
  mColHead.mPrev = prev_col;
}

// @brief 実効的な行数を返す．
ymuint32
McMatrix::_remain_row_size() const
{
  ymuint nr = 0;
  for (const McRowHead* rh = row_front();
       !is_row_end(rh); rh = rh->next()) ++ nr;
  return nr;
}

// @brief 実効的な列数を返す．
ymuint32
McMatrix::_remain_col_size() const
{
  ymuint nc = 0;
  for (const McColHead* ch = col_front();
       !is_col_end(ch); ch = ch->next()) ++ nc;
  return nc;
}

// @brief 列集合のコストを返す．
// @param[in] col_list 列のリスト
ymuint32
McMatrix::cost(const vector<ymuint32>& col_list) const
{
  ymuint32 cur_cost = 0;
  for (vector<ymuint32>::const_iterator p = col_list.begin();
       p != col_list.end(); ++ p) {
    ymuint32 col = *p;
    cur_cost += col_cost(col);
  }
  return cur_cost;
}

// @brief ブロック分割を行う．
// @param[in] row_list1 1つめのブロックの行番号のリスト
// @param[in] row_list2 2つめのブロックの行番号のリスト
// @param[in] col_list1 1つめのブロックの列番号のリスト
// @param[in] col_list2 2つめのブロックの列番号のリスト
// @retval true ブロック分割が行われた．
// @retval false ブロック分割が行えなかった．
bool
McMatrix::block_partition(vector<ymuint32>& row_list1,
			  vector<ymuint32>& row_list2,
			  vector<ymuint32>& col_list1,
			  vector<ymuint32>& col_list2) const
{
  // マークを消す．
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    row1->mWork = 0;
  }
  for (const McColHead* col1 = col_front();
       !is_col_end(col1); col1 = col1->next()) {
    col1->mWork = 0;
  }

  // 最初の行から到達可能な行と列にマークをつける．
  const McRowHead* row0 = row_front();
  ymuint nc1 = mark_cols(row0);

  ymuint nr = row_num();
  ymuint nc = col_num();

  if ( nc == nc1 ) {
    return false;
  }

  ymuint bitmask = 0;
  if ( nc1 > nc - nc1 ) {
    bitmask = 1;
  }

  row_list1.clear();
  row_list1.reserve(nr);
  row_list2.clear();
  row_list2.reserve(nr);
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    if ( row1->mWork ^ bitmask ) {
      row_list1.push_back(row1->pos());
    }
    else {
      row_list2.push_back(row1->pos());
    }
  }

  col_list1.clear();
  col_list1.reserve(nc);
  col_list2.clear();
  col_list2.reserve(nc);
  for (const McColHead* col1 = col_front();
       !is_col_end(col1); col1 = col1->next()) {
    if ( col1->mWork ^ bitmask ) {
      col_list1.push_back(col1->pos());
    }
    else {
      col_list2.push_back(col1->pos());
    }
  }

  return true;
}

// @brief col に接続している行をマークする．
// @param[in] col 対象の列
// @return マークされた列数を返す．
ymuint
McMatrix::mark_rows(const McColHead* col) const
{
  ymuint nc = 0;
  for (const McCell* cell = col->front();
       !col->is_end(cell); cell = cell->col_next()) {
    ymuint row_pos = cell->row_pos();
    const McRowHead* row1 = row(row_pos);
    if ( row1->mWork == 0 ) {
      row1->mWork = 1;
      nc += mark_cols(row1);
    }
  }
  return nc;
}

// @brief row に接続している列をマークする．
// @param[in] row 対象の行
// @return マークされた列数を返す．
ymuint
McMatrix::mark_cols(const McRowHead* row) const
{
  ymuint nc = 0;
  for (const McCell* cell = row->front();
       !row->is_end(cell); cell = cell->row_next()) {
    ymuint col_pos = cell->col_pos();
    const McColHead* col1 = col(col_pos);
    if ( col1->mWork == 0 ) {
      col1->mWork = 1;
      ++ nc;
      nc += mark_rows(col1);
    }
  }
  return nc;
}

// @brief 列集合がカバーになっているか検証する．
// @param[in] col_list 列のリスト
// @retval true col_list がカバーになっている．
// @retval false col_list でカバーされていない行がある．
bool
McMatrix::verify(const vector<ymuint32>& col_list) const
{
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    row1->mWork = 0;
  }
  for (ymuint i = 0; i < col_list.size(); ++ i) {
    ymuint32 col_pos = col_list[i];
    const McColHead* col1 = col(col_pos);
    for (const McCell* cell = col1->front();
	 !col1->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      row(row_pos)->mWork = 1;
    }
  }
  bool status = true;
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    if ( row1->mWork == 0 ) {
      status = false;
    }
  }
  return status;
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
// @return 追加された要素を返す．
McCell*
McMatrix::insert_elem(ymuint32 row_pos,
		      ymuint32 col_pos)
{
  McCell* cell = alloc_cell();
  cell->mRowPos = row_pos;
  cell->mColPos = col_pos;

  McRowHead* row1 = row(row_pos);
  bool stat1 = row1->search_insert_pos(cell);
  if ( !stat1 ) {
    // 列番号が重複しているので無視する．
    free_cell(cell);
    return NULL;
  }

  McColHead* col1 = col(col_pos);
  bool stat2 = col1->search_insert_pos(cell);
  assert_cond(stat2, __FILE__, __LINE__);

  row1->insert_elem(cell);
  if ( row1->num() == 1 ) {
    McRowHead* prev = mRowHead.mPrev; // 末尾
    McRowHead* next = &mRowHead;
    if ( prev == next || prev->pos() < row_pos ) {
      // 末尾に追加
      ;
    }
    else {
      for (prev = &mRowHead; ; prev = next) {
	next = prev->mNext;
	assert_cond(next->pos() != row_pos, __FILE__, __LINE__);
	if ( next->pos() > row_pos ) {
	  // prev と next の間に挿入する．
	  break;
	}
	assert_cond(next != &mRowHead, __FILE__, __LINE__);
      }
    }
    prev->mNext = row1;
    row1->mPrev = prev;
    row1->mNext = next;
    next->mPrev = row1;
    ++ mRowNum;
  }

  col1->insert_elem(cell);
  if ( col1->num() == 1 ) {
    McColHead* prev = mColHead.mPrev; // 末尾
    McColHead* next = &mColHead;
    if ( prev == next || prev->pos() < col_pos ) {
      // 末尾に追加
      ;
    }
    else {
      for (prev = &mColHead; ; prev = next) {
	next = prev->mNext;
	assert_cond(next->pos() != col_pos, __FILE__, __LINE__);
	if ( next->pos() > col_pos ) {
	  // prev と next の間に挿入する．
	  break;
	}
	assert_cond(next != &mColHead, __FILE__, __LINE__);
      }
    }
    prev->mNext = col1;
    col1->mPrev = prev;
    col1->mNext = next;
    next->mPrev = col1;
    ++ mColNum;
  }

  return cell;
}

// @brief 列を選択し，被覆される行を削除する．
void
McMatrix::select_col(ymuint32 col_pos)
{
  McColHead* col1 = col(col_pos);
  assert_cond( !col1->mDeleted, __FILE__, __LINE__);

  vector<ymuint32> row_list;
  row_list.reserve(col1->num());
  for (const McCell* cell = col1->front();
       !col1->is_end(cell); cell = cell->col_next()) {
    row_list.push_back(cell->row_pos());
  }
  delete_col(col_pos);
  for (ymuint i = 0; i < row_list.size(); ++ i) {
    ymuint32 r = row_list[i];
    delete_row(r);
  }
}

// @brief 削除スタックにマーカーを書き込む．
void
McMatrix::save()
{
  push_marker();
}

// @brief 直前のマーカーまで処理を戻す．
void
McMatrix::restore()
{
  while ( !stack_empty() ) {
    ymuint32 tmp = pop();
    if ( tmp == 0U ) {
      break;
    }
    if ( tmp & 2U ) {
      ymuint32 col_pos = tmp >> 2;
      // col_pos の列を元に戻す．
      restore_col(col_pos);
    }
    else {
      ymuint32 row_pos = tmp >> 2;
      // row_pos の行を元に戻す．
      restore_row(row_pos);
    }
  }
}

// @brief 行を削除する．
void
McMatrix::delete_row(ymuint32 row_pos)
{
  McRowHead* row1 = row(row_pos);
  if ( row1->mDeleted ) {
    return;
  }
  row1->mDeleted = true;
  -- mRowNum;

  McRowHead* prev = row1->mPrev;
  McRowHead* next = row1->mNext;
  prev->mNext = next;
  next->mPrev = prev;

  for (McCell* cell = row1->mDummy.mRightLink;
       !row1->is_end(cell); cell = cell->mRightLink) {
    ymuint col_pos = cell->col_pos();
    McColHead* col1 = col(col_pos);
    col1->delete_elem(cell);
    if ( col1->num() == 0 ) {
      delete_col(col_pos);
    }
  }

  push_row(row_pos);
}

// @brief 行を復元する．
void
McMatrix::restore_row(ymuint32 row_pos)
{
  McRowHead* row1 = row(row_pos);
  assert_cond( row1->mDeleted, __FILE__, __LINE__);
  row1->mDeleted = false;
  ++ mRowNum;

  McRowHead* prev = row1->mPrev;
  McRowHead* next = row1->mNext;
  prev->mNext = row1;
  next->mPrev = row1;

  for (McCell* cell = row1->mDummy.mRightLink;
       !row1->is_end(cell); cell = cell->mRightLink) {
    McColHead* col1 = col(cell->col_pos());
    col1->insert_elem(cell);
  }
}

// @brief 列を削除する．
void
McMatrix::delete_col(ymuint32 col_pos)
{
  McColHead* col1 = col(col_pos);
  if ( col1->mDeleted ) {
    return;
  }
  col1->mDeleted = true;
  -- mColNum;

  McColHead* prev = col1->mPrev;
  McColHead* next = col1->mNext;
  prev->mNext = next;
  next->mPrev = prev;

  for (McCell* cell = col1->mDummy.mDownLink;
       !col1->is_end(cell); cell = cell->mDownLink) {
    ymuint row_pos = cell->row_pos();
    McRowHead* row1 = row(row_pos);
    row1->delete_elem(cell);
    if ( row1->num() == 0 ) {
      delete_row(row_pos);
    }
  }

  push_col(col_pos);
}

// @brief 列を復元する．
void
McMatrix::restore_col(ymuint32 col_pos)
{
  McColHead* col1 = col(col_pos);
  assert_cond( col1->mDeleted, __FILE__, __LINE__);
  col1->mDeleted = false;
  ++ mColNum;

  McColHead* prev = col1->mPrev;
  McColHead* next = col1->mNext;
  prev->mNext = col1;
  next->mPrev = col1;

  for (McCell* cell = col1->mDummy.mDownLink;
       !col1->is_end(cell); cell = cell->mDownLink) {
    McRowHead* row1 = row(cell->row_pos());
    row1->insert_elem(cell);
  }
}

// @brief 簡単化を行う．
// @param[out] selected_cols 簡単化中で選択された列の集合
void
McMatrix::reduce(vector<ymuint32>& selected_cols)
{
  ymuint no_change = 0;
  for ( ; ; ) {
    // 列支配を探し，列の削除を行う．
    if ( col_dominance() ) {
      no_change = 0;
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }

    // 必須列を探し，列の選択を行う．
    if ( essential_col(selected_cols) ) {
      no_change = 0;
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }

    // 行支配を探し，行の削除を行う．
    if ( row_dominance() ) {
      no_change = 0;
    }
    else {
      ++ no_change;
      if ( no_change >= 3 ) {
	break;
      }
    }
  }
}


BEGIN_NONAMESPACE

struct RowLt
{
  bool
  operator()(const McRowHead* a,
	     const McRowHead* b)
  {
    return a->num() < b->num();
  }
};

END_NONAMESPACE


// @brief 行支配を探し，行を削除する．
// @return 削除された行があったら true を返す．
bool
McMatrix::row_dominance()
{
  bool change = false;

  // 行のマークをクリアする．
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    row1->mWork = 0;
  }

  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    if ( row1->mWork ) continue;

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    ymuint32 min_num = row_size() + 1;
    const McColHead* min_col = NULL;
    for (const McCell* cell = row1->front();
	 !row1->is_end(cell); cell = cell->row_next()) {
      ymuint32 col_pos = cell->col_pos();
      const McColHead* col1 = col(col_pos);
      ymuint32 col_num = col1->num();
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col1;
      }
    }
    // min_col に要素を持つ行のうち row1 に支配されている行を求める．
    for (const McCell* cell = min_col->front();
	 !min_col->is_end(cell); cell = cell->col_next()) {
      const McRowHead* row2 = row(cell->row_pos());
      if ( row2 == row1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( row2->num() < row1->num() ) {
	// 要素数が少ない行も比較しない．
	continue;
      }
      if ( row2->mWork ) {
	// 削除された行も比較しない.
	continue;
      }

      // row1 が row2 を支配しているか調べる．
      const McCell* cell1 = row1->front();
      ymuint32 pos1 = cell1->col_pos();
      const McCell* cell2 = row2->front();
      ymuint32 pos2 = cell2->col_pos();
      bool found = false;
      for ( ; ; ) {
	if ( pos1 < pos2 ) {
	  // row1 に含まれていて row2 に含まれていない列があるので
	  // row1 は row2 を支配しない．
	  break;
	}
	else if ( pos1 == pos2 ) {
	  cell1 = cell1->row_next();
	  if ( row1->is_end(cell1) ) {
	    found = true;
	    break;
	  }
	  pos1 = cell1->col_pos();
	}
	cell2 = cell2->row_next();
	if ( row2->is_end(cell2) ) {
	  break;
	}
	pos2 = cell2->col_pos();
      }
      if ( found ) {
	// row1 は row2 を支配している．
	ymuint row_pos = row2->pos();
	delete_row(row_pos);
	row2->mWork = 1;
	change = true;
	if ( mcmatrix_debug ) {
	  cout << "Row#" << row_pos << " is dominated by Row#" << row1->pos() << endl;
	}
      }
    }
  }

  return change;
}


BEGIN_NONAMESPACE

struct ColLt
{
  bool
  operator()(const McColHead* a,
	     const McColHead* b)
  {
    return a->num() < b->num();
  }
};

END_NONAMESPACE


// @brief 列支配を探し，列を削除する．
// @return 削除された列があったら true を返す．
bool
McMatrix::col_dominance()
{
  bool change = false;

  for (const McColHead* col1 = col_front();
       !is_col_end(col1); col1 = col1->next()) {
    if ( col1->num() == 0 ) continue;

    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    ymuint32 min_num = col_size() + 1;
    const McRowHead* min_row = NULL;
    for (const McCell* cell = col1->front();
	 !col1->is_end(cell); cell = cell->col_next()) {
      ymuint32 row_pos = cell->row_pos();
      const McRowHead* row1 = row(row_pos);
      ymuint32 row_num = row1->num();
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row1;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for (const McCell* cell = min_row->front();
	 !min_row->is_end(cell); cell = cell->row_next()) {
      const McColHead* col2 = col(cell->col_pos());
      if ( col2 == col1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( col2->num() < col1->num() ) {
	// ただし col1 よりも要素数の少ない列は調べる必要はない．
	continue;
      }
      if ( col_cost(col2->pos()) > col_cost(col1->pos()) ) {
	// col2 のコストが col のコストより高ければ調べる必要はない．
	continue;
      }

      const McCell* cell1 = col1->front();
      ymuint32 pos1 = cell1->row_pos();
      const McCell* cell2 = col2->front();
      ymuint32 pos2 = cell2->row_pos();
      bool found = false;
      for ( ; ; ) {
	if ( pos1 < pos2 ) {
	  // col1 に含まれていて col2 に含まれない行があるので
	  // col2 は col1 を支配しない．
	  break;
	}
	if ( pos1 == pos2 ) {
	  cell1 = cell1->col_next();
	  if ( col1->is_end(cell1) ) {
	    found = true;
	    break;
	  }
	  pos1 = cell1->row_pos();
	}
	cell2 = cell2->col_next();
	if ( col2->is_end(cell2) ) {
	  break;
	}
	pos2 = cell2->row_pos();
      }
      if ( found ) {
	// col2 は col1 を支配している．
	delete_col(col1->pos());
	if ( mcmatrix_debug ) {
	  cout << "Col#" << col1->pos() << " is dominated by Col#"
	       << col2->pos() << endl;
	}
	change = true;
	break;
      }
    }
  }

  return change;
}

// @brief 必須列を探し，列を選択する．
// @param[out] selected_cols 選択された列を追加する列集合
// @return 選択された列があったら true を返す．
bool
McMatrix::essential_col(vector<ymuint32>& selected_cols)
{
  // マークをクリアする．
  for (const McColHead* col1 = col_front();
       !is_col_end(col1); col1 = col1->next()) {
    col1->mWork = 0;
  }

  ymuint old_size = selected_cols.size();
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    if ( row1->num() == 1 ) {
      const McCell* cell = row1->front();
      ymuint32 col_pos = cell->col_pos();
      assert_cond( !col(col_pos)->mDeleted, __FILE__, __LINE__);
      McColHead* col1 = col(col_pos);
      if ( col1->mWork == 0 ) {
	col1->mWork = 1;
	selected_cols.push_back(col_pos);
	if ( mcmatrix_debug ) {
	  cout << "Col#" << col_pos << " is essential" << endl;
	}
      }
    }
  }
  ymuint size = selected_cols.size();
  for (ymuint i = old_size; i < size; ++ i) {
    ymuint32 col_pos = selected_cols[i];
    select_col(col_pos);
  }

  return size > old_size;
}

// @brief セルの生成
McCell*
McMatrix::alloc_cell()
{
  void* p = mCellAlloc.get_memory(sizeof(McCell));
  return new (p) McCell;
}

// @brief セルの解放
void
McMatrix::free_cell(McCell* cell)
{
  mCellAlloc.put_memory(sizeof(McCell), cell);
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
McMatrix::print(ostream& s) const
{
  for (ymuint i = 0; i < col_size(); ++ i) {
    if ( col_cost(i) != 1 ) {
      s << "Col#" << i << ": " << col_cost(i) << endl;
    }
  }
  for (const McRowHead* row = row_front();
       !is_row_end(row); row = row->next()) {
    s << "Row#" << row->pos() << ":";
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      s << " " << cell->col_pos();
    }
    s << endl;
  }
}

END_NAMESPACE_YM_MINCOV
