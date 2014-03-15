
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McMatrix.h"
#include "McSolverImpl.h"
#include "utils/MFSet.h"


BEGIN_NAMESPACE_YM_MINCOV

bool mcmatrix_debug = false;

//////////////////////////////////////////////////////////////////////
// クラス McRowHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
McRowHead::McRowHead() :
  mNum(0),
  mDeleted(false)
{
}

/// @brief デストラクタ
McRowHead::~McRowHead()
{
}

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

// @brief コンストラクタ
McColHead::McColHead() :
  mNum(0),
  mDeleted(false)
{
}

/// @brief デストラクタ
McColHead::~McColHead()
{
}

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
McMatrix::McMatrix(ymuint32 row_size,
		   ymuint32 col_size) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mColArray(NULL),
  mColCostArray(NULL),
  mDelStack(NULL),
  mMarkArray(NULL),
  mRowIdList(NULL),
  mColIdList(NULL)
{
  resize(row_size, col_size);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
McMatrix::McMatrix(const McMatrix& src) :
  mCellAlloc(sizeof(McCell), 1024),
  mRowSize(0),
  mColSize(0),
  mRowArray(NULL),
  mColArray(NULL),
  mColCostArray(NULL),
  mDelStack(NULL),
  mMarkArray(NULL),
  mRowIdList(NULL),
  mColIdList(NULL)
{
  resize(src.row_size(), src.col_size());
  copy(src);
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
  clear();
}

// @brief 内容をクリアする．
void
McMatrix::clear()
{
  mCellAlloc.destroy();

  for (ymuint i = 0; i < row_size(); ++ i) {
    mRowArray[i].clear();
  }
  for (ymuint i = 0; i < col_size(); ++ i) {
    mColArray[i].clear();
  }

  delete [] mRowArray;
  delete [] mColArray;
  delete [] mColCostArray;
  delete [] mDelStack;
  delete [] mMarkArray;
  delete [] mRowIdList;
  delete [] mColIdList;
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
    mRowArray = new McRowHead[mRowSize];
    mColArray = new McColHead[mColSize];
    mColCostArray = new ymuint32[mColSize];

    mRowHead.mNext = &mRowHead;
    mRowHead.mPrev = &mRowHead;
    mRowHead.clear();
    for (ymuint i = 0; i < mRowSize; ++ i) {
      McRowHead* row = &mRowArray[i];
      row->clear();
      row->mPos = i;
      row->clear();
    }

    mColHead.mNext = &mColHead;
    mColHead.mPrev = &mColHead;
    mColHead.clear();
    for (ymuint i = 0; i < mColSize; ++ i) {
      McColHead* col = &mColArray[i];
      col->clear();
      col->mPos = i;
      col->clear();
      mColCostArray[i] = 1;
    }

    delete [] mDelStack;
    mDelStack = new ymuint32[row_size + col_size];
    mStackTop = 0;

    ymuint rc = row_size > col_size ? row_size : col_size;
    mMarkArray = new bool[rc];

    mRowIdList = new ymuint32[row_size];
    mColIdList = new ymuint32[col_size];

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
    for (const McCell* src_cell = src_row->front();
	 !src_row->is_end(src_cell); src_cell = src_cell->row_next()) {
      insert_elem(src_cell->row_pos(), src_cell->col_pos());
    }
  }
  for (ymuint i = 0; i < col_size(); ++ i) {
    mColCostArray[i] = src.col_cost(i);
  }
}

// @brief 実効的な行数を返す．
ymuint32
McMatrix::remain_row_size() const
{
  ymuint nr = 0;
  for (const McRowHead* rh = row_front();
       !is_row_end(rh); rh = rh->next()) ++ nr;
  return nr;
}

// @brief 実効的な列数を返す．
ymuint32
McMatrix::remain_col_size() const
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

  McRowHead* row = &mRowArray[row_pos];
  bool stat1 = row->search_insert_pos(cell);
  if ( !stat1 ) {
    // 列番号が重複しているので無視する．
    free_cell(cell);
    return NULL;
  }

  McColHead* col = &mColArray[col_pos];
  bool stat2 = col->search_insert_pos(cell);
  assert_cond(stat2, __FILE__, __LINE__);

  row->insert_elem(cell);
  if ( row->num() == 1 ) {
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
    prev->mNext = row;
    row->mPrev = prev;
    row->mNext = next;
    next->mPrev = row;
  }

  col->insert_elem(cell);
  if ( col->num() == 1 ) {
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
    prev->mNext = col;
    col->mPrev = prev;
    col->mNext = next;
    next->mPrev = col;
  }

  return cell;
}

// @brief 列のコストを設定する．
// @param[in] col_pos 列番号
// @param[in] val_pos 値の位置番号
// @param[in] value 設定する値
void
McMatrix::set_col_cost(ymuint32 col_pos,
		       ymuint32 value)
{
  mColCostArray[col_pos] = value;
}

// @brief 列を選択し，被覆される行を削除する．
void
McMatrix::select_col(ymuint32 col_pos)
{
  McColHead* col = &mColArray[col_pos];
  assert_cond( !col->mDeleted, __FILE__, __LINE__);

  mRowIdListNum = 0;
  for (const McCell* cell = col->front();
       !col->is_end(cell); cell = cell->col_next()) {
    mRowIdList[mRowIdListNum] = cell->row_pos();
    ++ mRowIdListNum;
  }
  delete_col(col_pos);
  for (ymuint i = 0; i < mRowIdListNum; ++ i) {
    ymuint32 r = mRowIdList[i];
    delete_row(r);
  }
}

// @brief ブロック分割を行う．
bool
McMatrix::block_partition(vector<McSolverImpl*>& solver_list) const
{
  ymuint nr = remain_row_size();
  ymuint nc = remain_col_size();

  if ( true || nr < nc ) {
    MFSet mfset(row_size());
    for (const McColHead* col = col_front();
	 !is_col_end(col); col = col->next()) {
      vector<ymuint> row_list;
      row_list.reserve(col->num());
      for (const McCell* cell = col->front();
	   !col->is_end(cell); cell = cell->col_next()) {
	row_list.push_back(cell->row_pos());
      }
      ymuint n = row_list.size();
      for (ymuint i1 = 0; i1 < n; ++ i1) {
	ymuint r1 = row_list[i1];
	for (ymuint i2 = i1 + 1; i2 < n; ++ i2) {
	  ymuint r2 = row_list[i2];
	  mfset.merge(r1, r2);
	}
      }
    }
    vector<ymuint> rep_list;
    for (const McRowHead* row = row_front();
	 !is_row_end(row); row = row->next()) {
      ymuint row_pos = row->pos();
      ymuint rep = mfset.find(row_pos);
      if ( rep == row_pos ) {
	rep_list.push_back(rep);
      }
    }
    if ( rep_list.size() == 1 ) {
      return false;
    }

    for (vector<ymuint>::iterator p = rep_list.begin();
	 p != rep_list.end(); ++ p) {
      ymuint rep = *p;
      McSolverImpl* solver = new McSolverImpl();
      solver->set_size(row_size(), col_size());
      for (const McRowHead* row = row_front();
	   !is_row_end(row); row = row->next()) {
	ymuint row_pos = row->pos();
	if ( mfset.find(row_pos) != rep ) {
	  continue;
	}
	for (const McCell* cell = row->front();
	     !row->is_end(cell); cell = cell->row_next()) {
	  ymuint col_pos = cell->col_pos();
	  solver->insert_elem(row_pos, col_pos);
	}
      }
      for (ymuint i = 0; i < col_size(); ++ i) {
	solver->set_col_cost(i, col_cost(i));
      }
      solver_list.push_back(solver);
    }
    if ( 0 ) {
      cout << "BLOCK PARTITION" << endl;
      print(cout);
      cout << endl;
      ymuint id = 0;
      for (vector<McSolverImpl*>::iterator p = solver_list.begin();
	   p != solver_list.end(); ++ p, ++ id) {
	McSolverImpl* solver = *p;
	cout << "Matrix#" << id << endl;
	solver->print_matrix(cout);
      }
    }
    return true;
  }
  else {
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
  McRowHead* row = &mRowArray[row_pos];
  if ( row->mDeleted ) {
    return;
  }
  row->mDeleted = true;

  McRowHead* prev = row->mPrev;
  McRowHead* next = row->mNext;
  prev->mNext = next;
  next->mPrev = prev;

  for (McCell* cell = row->mDummy.mRightLink;
       !row->is_end(cell); cell = cell->mRightLink) {
    ymuint col_pos = cell->col_pos();
    McColHead* col = &mColArray[col_pos];
    col->delete_elem(cell);
    if ( col->num() == 0 ) {
      delete_col(col_pos);
    }
  }

  push_row(row_pos);
}

// @brief 行を復元する．
void
McMatrix::restore_row(ymuint32 row_pos)
{
  McRowHead* row = &mRowArray[row_pos];
  assert_cond( row->mDeleted, __FILE__, __LINE__);
  row->mDeleted = false;

  McRowHead* prev = row->mPrev;
  McRowHead* next = row->mNext;
  prev->mNext = row;
  next->mPrev = row;

  for (McCell* cell = row->mDummy.mRightLink;
       !row->is_end(cell); cell = cell->mRightLink) {
    mColArray[cell->col_pos()].insert_elem(cell);
  }

  {
    for (McColHead* col = mColHead.mNext;
	 !is_col_end(col); col = col->mNext) {
      ymuint n = 0;
      for (const McCell* cell = col->front();
	   !col->is_end(cell); cell = cell->col_next()) ++ n;
      col->mNum = n;
    }
  }
}

// @brief 列を削除する．
void
McMatrix::delete_col(ymuint32 col_pos)
{
  McColHead* col = &mColArray[col_pos];
  if ( col->mDeleted ) {
    return;
  }
  col->mDeleted = true;

  McColHead* prev = col->mPrev;
  McColHead* next = col->mNext;
  prev->mNext = next;
  next->mPrev = prev;

  for (McCell* cell = col->mDummy.mDownLink;
       !col->is_end(cell); cell = cell->mDownLink) {
    ymuint row_pos = cell->row_pos();
    McRowHead* row = &mRowArray[row_pos];
    row->delete_elem(cell);
    if ( row->num() == 0 ) {
      delete_row(row_pos);
    }
  }

  push_col(col_pos);
}

// @brief 列を復元する．
void
McMatrix::restore_col(ymuint32 col_pos)
{
  McColHead* col = &mColArray[col_pos];
  assert_cond( col->mDeleted, __FILE__, __LINE__);
  col->mDeleted = false;

  McColHead* prev = col->mPrev;
  McColHead* next = col->mNext;
  prev->mNext = col;
  next->mPrev = col;

  for (McCell* cell = col->mDummy.mDownLink;
       !col->is_end(cell); cell = cell->mDownLink) {
    mRowArray[cell->row_pos()].insert_elem(cell);
  }

  {
    for (McRowHead* row = mRowHead.mNext;
	 !is_row_end(row); row = row->mNext) {
      ymuint n = 0;
      for (const McCell* cell = row->front();
	   !row->is_end(cell); cell = cell->row_next()) ++ n;
      row->mNum = n;
    }
  }
}

// @brief 簡単化を行う．
// @param[out] selected_cols 簡単化中で選択された列の集合
void
McMatrix::reduce(vector<ymuint32>& selected_cols)
{
  for ( ; ; ) {
    bool change = false;

    // 行支配を探し，行の削除を行う．
    if ( row_dominance() ) {
      change = true;
    }

    // 列支配を探し，列の削除を行う．
    if ( col_dominance() ) {
      change = true;
    }

    // 必須列を探し，列の選択を行う．
    if ( essential_col(selected_cols) ) {
      change = true;
    }

    if ( !change ) {
      break;
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

  // 削除された行番号に印をつけるための配列
  for (ymuint i = 0; i < row_size(); ++ i) {
    mMarkArray[i] = false;
  }

  // 残っている行のリストを作る．
  mRowVector.clear();
  mRowVector.reserve(row_size());
  for (const McRowHead* row1 = row_front();
       !is_row_end(row1); row1 = row1->next()) {
    mRowVector.push_back(row1);
  }

  // 要素数の少ない順にソートする．
  sort(mRowVector.begin(), mRowVector.end(), RowLt());

  for (vector<const McRowHead*>::iterator p = mRowVector.begin();
       p != mRowVector.end(); ++ p) {
    const McRowHead* row1 = *p;
    if ( mMarkArray[row1->pos()] ) continue;

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
	mMarkArray[row_pos] = true;
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

  // 残っている列のリストを作る．
  mColVector.clear();
  mColVector.reserve(col_size());
  for (const McColHead* col1 = col_front();
       !is_col_end(col1); col1 = col1->next()) {
    mColVector.push_back(col1);
  }
  // 要素数の少ない順にソートする．
  sort(mColVector.begin(), mColVector.end(), ColLt());

  for (vector<const McColHead*>::iterator p = mColVector.begin();
       p != mColVector.end(); ++ p) {
    const McColHead* col1 = *p;

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
  for (ymuint i = 0; i < col_size(); ++ i) {
    mMarkArray[i] = false;
  }

  mColIdListNum = 0;
  for (const McRowHead* row = row_front();
       !is_row_end(row); row = row->next()) {
    if ( row->num() == 1 ) {
      const McCell* cell = row->front();
      ymuint32 col_pos = cell->col_pos();
      assert_cond( !col(col_pos)->mDeleted, __FILE__, __LINE__);
      if ( !mMarkArray[col_pos] ) {
	mMarkArray[col_pos] = true;
	mColIdList[mColIdListNum] = col_pos;
	++ mColIdListNum;
      }
    }
  }
  for (ymuint i = 0; i < mColIdListNum; ++ i) {
    ymuint32 col_pos = mColIdList[i];
    if ( mcmatrix_debug ) {
      cout << "Col#" << col_pos << " is essential" << endl;
    }
    selected_cols.push_back(col_pos);
    select_col(col_pos);
  }
  return mColIdListNum > 0;
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
