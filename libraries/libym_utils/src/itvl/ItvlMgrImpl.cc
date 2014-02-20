
/// @file ItvlMgrImpl.cc
/// @brief ItvlMgrImpl の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ItvlMgrImpl.h"
#include "ItvlCell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ItvlMgrImpl::ItvlMgrImpl() :
  mCellAlloc(sizeof(ItvlCell), 1024)
{
  mRoot = new_cell(0, INT_MAX);
}

// @brief デストラクタ
ItvlMgrImpl::~ItvlMgrImpl()
{
  // mCellAlloc のデストラクタがすべてのメモリを開放する．
}

// @brief クリアする．
void
ItvlMgrImpl::clear()
{
  delete_cell(mRoot);
  mRoot = new_cell(0, INT_MAX);
}

// 使用可能な数字を得る．
// 内容は変化しない．
// 使用可能な区間がない場合(!!!)，-1を返す．
int
ItvlMgrImpl::avail_num()
{
  ItvlCell* cell = mRoot;
  if ( !cell ) {
    return -1;
  }

  while ( cell->mLchd ) {
    cell = cell->mLchd;
  }
  return cell->start();
}

// [d1, d2]という区間が使用可能などうか調べる．
bool
ItvlMgrImpl::check(int d1,
		   int d2)
{
  // d1 <= d2 でなければ入れ換える．
  if ( d1 > d2 ) {
    int tmp = d1;
    d1 = d2;
    d2 = tmp;
  }

  // d1 を含む区間を探す．
  ItvlCell* cell = find(d1, mRoot);
  if ( !cell ) {
    // だめ．
    return false;
  }
  // この区間が d2 を含んでいるか調べる．
  if ( cell->end() >= d2 ) {
    return true;
  }
  return false;
}

// 使用されている区間の最小値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlMgrImpl::min_id()
{
  ItvlCell* cell;
  for (cell = mRoot; cell->mLchd; cell = cell->mLchd) { }
  // cell は最左節点
  if ( cell->start() > 0 ) {
    return 0;
  }
  else if ( cell->end() < INT_MAX ) {
    return cell->end() + 1;
  }
  else {
    return -1;
  }
}

// 使用されている区間の最大値を求める．
// 全区間が未使用の場合は -1 を返す．
int
ItvlMgrImpl::max_id()
{
  ItvlCell* cell;
  for (cell = mRoot; cell->mRchd; cell = cell->mRchd) { }
  // cell は最右節点
  if ( cell->end() < INT_MAX ) {
    // これはあり得ないと思うけど
    return INT_MAX;
  }
  else if ( cell->start() > 0 ) {
    return cell->start() - 1;;
  }
  else {
    return -1;
  }
}

// d を含む区間を求める．
ItvlCell*
ItvlMgrImpl::find(int d,
		  ItvlCell* ptr)
{
  while ( ptr ) {
    if ( ptr->start() > d ) {
      ptr = ptr->mLchd;
    }
    else if ( ptr->end() < d ) {
      ptr = ptr->mRchd;
    }
    else {
      break;
    }
  }
  return ptr;
}

// d よりも小さくもっとも右側にある区間を求める．
ItvlCell*
ItvlMgrImpl::find_left(int d,
		       ItvlCell* ptr)
{
  while ( ptr && ptr->start() > d ) {
    ptr = ptr->mLchd;
  }
  if ( !ptr ) {
    return 0;
  }
  if ( ptr->end() < d ) {
    while ( ptr->mRchd && ptr->mRchd->end() < d ) {
      ptr = ptr->mRchd;
    }
  }
  return ptr;
}

// d よりも大きくもっとも左側にある区間を求める．
ItvlCell*
ItvlMgrImpl::find_right(int d,
			ItvlCell* ptr)
{
  while ( ptr && ptr->end() < d ) {
    ptr = ptr->mRchd;
  }
  if ( !ptr ) {
    return 0;
  }
  if ( ptr->start() > d ) {
    while ( ptr->mLchd && ptr->mLchd->start() > d ) {
      ptr = ptr->mLchd;
    }
  }
  return ptr;
}

// セルを追加するためのサブルーティン．
bool
ItvlMgrImpl::add_cell(ItvlCell* cell,
		      ItvlCell*& ptr)
{
  if ( ptr == NULL ) {
    ptr = cell;
    ptr->mBalance = 0;
    return true;
  }
  if ( ptr->start() > cell->end() ) {
    bool chg = add_cell(cell, ptr->mLchd);
    if ( chg ) {
      // 左の部分木が高くなった．
      switch ( ptr->mBalance ) {
      case 1: // 今まで左が低かったのでこれでバランスした．
	ptr->mBalance = 0;
	return false;

      case 0: // 左が高くなった．
	ptr->mBalance = -1;
	return true;

      case -1: // 再平衡化が必要
	{
	  ItvlCell* left = ptr->mLchd;
	  if ( left->mBalance == -1 ) {
	    // 単LL回転
	    ptr->mLchd = left->mRchd;
	    left->mRchd = ptr;
	    ptr->mBalance = 0;
	    ptr = left;
	  }
	  else {
	    // 二重LR回転
	    ItvlCell* right = left->mRchd;
	    left->mRchd = right->mLchd;
	    right->mLchd = left;
	    ptr->mLchd = right->mRchd;
	    right->mRchd = ptr;
	    if ( right->mBalance == -1 ) {
	      ptr->mBalance = 1;
	    }
	    else {
	      ptr->mBalance = 0;
	    }
	    if ( right->mBalance == 1 ) {
	      left->mBalance = -1;
	    }
	    else {
	      left->mBalance = 0;
	    }
	    ptr = right;
	  }
	  ptr->mBalance = 0;
	  return false;
	}
      }
    }
  }
  else if ( ptr->end() < cell->start() ) {
    bool chg = add_cell(cell, ptr->mRchd);
    if ( chg ) {
      // 右の部分木が高くなった．
      switch ( ptr->mBalance ) {
      case -1: // 今まで右が低かったのでバランスした．
	ptr->mBalance = 0;
	return false;

      case 0: // 右が高くなった．(全体も高くなった．)
	ptr->mBalance = 1;
	return true;

      case 1: // 再平衡化が必要
	{
	  ItvlCell* right = ptr->mRchd;
	  if ( right->mBalance == 1 ) {
	    // 単RR回転
	    ptr->mRchd = right->mLchd;
	    right->mLchd = ptr;
	    ptr->mBalance = 0;
	    ptr = right;
	  }
	  else {
	    // 二重RL回転
	    ItvlCell* left = right->mLchd;
	    right->mLchd = left->mRchd;
	    left->mRchd = right;
	    ptr->mRchd = left->mLchd;
	    left->mLchd = ptr;
	    if ( left->mBalance == 1 ) {
	      ptr->mBalance = -1;
	    }
	    else {
	      ptr->mBalance = 0;
	    }
	    if ( left->mBalance == -1 ) {
	      right->mBalance = 1;
	    }
	    else {
	      right->mBalance = 0;
	    }
	    ptr = left;
	  }
	  ptr->mBalance = 0;
	  return false;
	}
      }
    }
  }
  else {
    // 重複した追加は違反
    assert_not_reached(__FILE__, __LINE__);
  }
  return false;
}

// 左の部分木の高さが減少したときの処理
// 自分自身の高さも減少する時に true を返す．
bool
ItvlMgrImpl::balance_left(ItvlCell*& ptr)
{
  bool chg = true;

  switch ( ptr->mBalance ) {
  case -1: // もともと左が高かったのでバランスする．
    ptr->mBalance = 0;
    // でも高さは減少する．
    chg = true;
    break;

  case 0: // もともとバランスしていたので左が低くなる．
    ptr->mBalance = 1;
    // 高さは変わらないので false を返す．
    chg = false;
    break;

  case 1: // 再平衡化を行う．
    {
      ItvlCell* right = ptr->mRchd;
      int br = right->mBalance;
      if ( br >= 0 ) {
	// 単RR回転
	ptr->mRchd = right->mLchd;
	right->mLchd = ptr;
	if ( br == 0 ) {
	  ptr->mBalance = 1;
	  right->mBalance = -1;
	  chg = false;
	}
	else {
	  ptr->mBalance = 0;
	  right->mBalance = 0;
	}
	ptr = right;
      }
      else {
	// 二重RL回転
	ItvlCell* left = right->mLchd;
	int bl = left->mBalance;
	right->mLchd = left->mRchd;
	left->mRchd = right;
	ptr->mRchd = left->mLchd;
	left->mLchd = ptr;
	if ( bl == 1 ) {
	  ptr->mBalance = -1;
	}
	else {
	  ptr->mBalance = 0;
	}
	if ( bl == -1 ) {
	  right->mBalance = 1;
	}
	else {
	  right->mBalance = 0;
	}
	ptr = left;
	ptr->mBalance = 0;
      }
    }
  }
  return chg;
}

// 右の部分木の高さが減少したときの処理
// 自分自身の高さも減少する時に true を返す．
bool
ItvlMgrImpl::balance_right(ItvlCell*& ptr)
{
  bool chg = true;

  switch ( ptr->mBalance ) {
  case 1: // もともと右が高かったのでバランスする．
    ptr->mBalance = 0;
    // でも高さは減少する．
    break;

  case 0: // もともとバランスしていたので右が低くなる．
    ptr->mBalance = -1;
    // 高さは変わらないので chg フラグを降ろす．
    chg = false;
    break;

  case -1: // 再平衡化を行う．
    {
      ItvlCell* left = ptr->mLchd;
      int bl = left->mBalance;
      if ( bl <= 0 ) {
	// 単LL回転
	ptr->mLchd = left->mRchd;
	left->mRchd = ptr;
	if ( bl == 0 ) {
	  ptr->mBalance = -1;
	  left->mBalance = 1;
	  chg = false;
	}
	else {
	  ptr->mBalance = 0;
	  left->mBalance = 0;
	}
	ptr = left;
      }
      else {
	// 二重LR回転
	ItvlCell* right = left->mRchd;
	int br = right->mBalance;
	left->mRchd = right->mLchd;
	right->mLchd = left;
	ptr->mLchd = right->mRchd;
	right->mRchd = ptr;
	if ( br == -1 ) {
	  ptr->mBalance = 1;
	}
	else {
	  ptr->mBalance = 0;
	}
	if ( br == 1 ) {
	  left->mBalance = -1;
	}
	else {
	  left->mBalance = 0;
	}
	ptr = right;
	ptr->mBalance = 0;
      }
    }
  }
  return chg;
}

// もっとも右にある節点の内容を cell にコピーして削除する．
// 木の高さが変化した時には true を返す．
bool
ItvlMgrImpl::remove_right(ItvlCell* cell,
			  ItvlCell*& ptr)
{
  if ( ptr->mRchd ) {
    // まだ右に子供がいる
    bool chg = remove_right(cell, ptr->mRchd);
    if ( chg ) {
      chg = balance_right(ptr);
    }
    return chg;
  }

  // ptr が最右端の節点を指している．
  cell->mStart = ptr->mStart;
  cell->mEnd = ptr->mEnd;
  ItvlCell* dcell = ptr;
  ptr = ptr->mLchd;
  dcell->mLchd = 0;
  delete_cell(dcell);
  return true;
}

// delete のためのサブルーティン
// ptr を根とする部分木から cell を削除する．
// この部分木の高さが変わった時には true を返す．
bool
ItvlMgrImpl::remove_cell(ItvlCell* cell,
			 ItvlCell*& ptr)
{
  if ( ptr == NULL ) {
    // セルが存在しない?
    // 本当はエラーだけど無視しちゃう．
    return false;
  }

  if ( ptr->start() > cell->end() ) {
    bool chg = remove_cell(cell, ptr->mLchd);
    if ( chg ) {
      chg = balance_left(ptr);
    }
    return chg;
  }
  if ( ptr->end() < cell->start() ) {
    bool chg = remove_cell(cell, ptr->mRchd);
    if ( chg ) {
      chg = balance_right(ptr);
    }
    return chg;
  }

  // この場合，ptr と cell の区間がオーバーラップしているので
  // ptr == cell でないとおかしい．
  assert_cond( ptr == cell, __FILE__, __LINE__);
  bool chg = false;
  if ( !ptr->mLchd ) {
    // 右の子供しか持たない場合
    ptr = cell->mRchd;
    cell->mRchd = 0;
    delete_cell(cell);
    chg = true;
  }
  else if ( !ptr->mRchd  ) {
    // 左の子供しか持たない場合
    ptr = cell->mLchd;
    cell->mLchd = 0;
    delete_cell(cell);
    chg = true;
  }
  else {
    // 二人の子供を持つ場合．
    // 左の部分木のもっとも右にある節点で置き換える．
    // この場合 cell は削除してはいけない．
    chg = remove_right(cell, ptr->mLchd);
    if ( chg ) {
      chg = balance_left(ptr);
    }
  }
  return chg;
}

// d を使用可能な区間から削除する．
void
ItvlMgrImpl::erase(int d)
{
  // d を含む区間を探す．
  ItvlCell* cell = find(d, mRoot);
  if ( cell == NULL ) {
    // もともと入っていない？
    // ほんとはおかしいけど，無視しておこう．
    return;
  }

  if ( cell->start() == cell->end() ) {
    // この場合はこの要素を削除すれば良い．
    remove_cell(cell, mRoot);
  }
  else {
    if ( cell->start() == d ) {
      ++ cell->mStart;
    }
    else if ( cell->end() == d ) {
      -- cell->mEnd;
    }
    else {
      // 2つの区間に分割
      ItvlCell* cell2 = new_cell(d + 1, cell->end());
      cell->mEnd = d - 1;
      add_cell(cell2, mRoot);
    }
  }
}

// [d1, d2] を使用可能な区間から削除する．
void
ItvlMgrImpl::erase(int d1,
		   int d2)
{
  // d1 <= d2 でなければ入れ換える．
  if (d1 > d2) {
    int tmp = d1;
    d1 = d2;
    d2 = tmp;
  }

  // d1 を含む区間を探す．
  ItvlCell* cell = find(d1, mRoot);
  if ( !cell || cell->end() < d2 ) {
    // もともと入っていない？
    // ほんとはおかしいけど，無視しておこう．
    return;
  }

  if ( cell->start() == d1 && cell->end() == d2 ) {
    // この場合はこの要素を削除すれば良い．
    remove_cell(cell, mRoot);
  }
  else {
    if ( cell->start() == d1 ) {
      ++ cell->mStart;
    }
    else if ( cell->end() == d2 ) {
      -- cell->mEnd;
    }
    else {
      // 2つの区間に分割
      ItvlCell* cell2 = new_cell(d2 + 1, cell->end());
      cell->mEnd = d1 - 1;
      add_cell(cell2, mRoot);
    }
  }
}

// d を使用可能区間に追加する．
void
ItvlMgrImpl::add(int d)
{
  // d に隣接する区間を探す．
  ItvlCell* left = find_left(d, mRoot);
  ItvlCell* right = find_right(d, mRoot);

  // ちょっとしたhack
  int start = d + 2;
  int end = d - 2;
  if ( left ) {
    end = left->end();
  }
  if ( right ) {
    start = right->start();
  }

  assert_cond(end < d, __FILE__, __LINE__);
  assert_cond(start > d, __FILE__, __LINE__);

  if ( end + 2 == start ) {
    // 2つの区間が d で一つにつながる．
    int end2 = right->end();
    remove_cell(right, mRoot);
    // left の節点は置き換わっている可能性があるのでもう一度取り出す．
    left = find_left(d, mRoot);
    left->mEnd = end2;
  }
  else if ( end + 1 == d ) {
    ++ left->mEnd;
  }
  else if ( start - 1 == d ) {
    -- right->mStart;
  }
  else {
    // 新しい区間を作る．
    ItvlCell* cell = new_cell(d, d);
    add_cell(cell, mRoot);
  }
}

// [d1, d2] を使用可能区間に追加する．
void
ItvlMgrImpl::add(int d1,
		 int d2)
{
  // d1 に隣接する区間を探す．
  ItvlCell* left = find_left(d1, mRoot);
  // d2 に隣接する区間を探す．
  ItvlCell* right = find_right(d2, mRoot);
  // left と right の間に他の区間が入っていたらおかしいので何もしないで
  // 抜ける．
  if ( find_right(d1, mRoot) != right ) {
    // 今は何もしないで戻る．
    return;
  }

  // ちょっとしたhack
  int start = d2 + 2;
  int end = d1 - 2;
  if ( left ) {
    end = left->end();
  }
  if ( right ) {
    start = right->mStart;
  }

  assert_cond(end < d1, __FILE__, __LINE__);
  assert_cond(start > d2, __FILE__, __LINE__);

  if ( end + 1 == d1 && start - 1 == d2 ) {
    // 2つの区間が [d1, d2] で一つにつながる．
    int end2 = right->end();
    remove_cell(right, mRoot);
    // left の節点は置き換わっている可能性があるのでもう一度取り出す．
    left = find_left(d1, mRoot);
    left->mEnd = end2;
  }
  else if ( end + 1 == d1 ) {
    ++ left->mEnd;
  }
  else if ( start - 1 == d2 ) {
    -- right->mStart;
  }
  else {
    // 新しい区間を作る．
    ItvlCell* cell = new_cell(d1, d2);
    add_cell(cell, mRoot);
  }
}

// @brief 内部構造が正しいかチェックする．
// @note おかしい時は例外を投げる．
void
ItvlMgrImpl::sanity_check() const
{
#ifdef DEBUG_ITVLMGR
  int d1, d2; // ダミー変数
  (void) check_cell(mRoot, d1, d2);
#endif
}

// 正しい構造かチェックする．
int
ItvlMgrImpl::check_cell(ItvlCell* cell,
			int& l,
			int& r) const
{
  if ( cell == NULL ) {
    return 0;
  }
  int l1, r1;
  int hl = check_cell(cell->mLchd, l1, r1);
  int l2, r2;
  int hr = check_cell(cell->mRchd, l2, r2);

  switch ( cell->balance() ) {
  case 1: // 右が高いはず
    assert_cond( hl < hr, __FILE__, __LINE__);
    break;

  case -1: // 左が高いはず
    assert_cond( hl > hr, __FILE__, __LINE__);
    break;

  case 0: // 高さは等しいはず
    assert_cond( hl == hr, __FILE__, __LINE__);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  if ( cell->mLchd ) {
    assert_cond( r1 < cell->start(), __FILE__, __LINE__);
    l = l1;
  }
  else {
    l = cell->start();
  }
  if ( cell->mRchd ) {
    assert_cond( l2 > cell->end(), __FILE__, __LINE__);
    r = r2;
  }
  else {
    r = cell->end();
  }
  int h = hl;
  if ( h < hr ) {
    h = hr;
  }
  return h + 1;
}

// @brief 内容を表示する
// @param[in] s 出力ストリーム
void
ItvlMgrImpl::print(ostream& s) const
{
  print_cell(s, mRoot);
}

// @brief print() の下請け関数
// @param[in] s 出力先のストリーム
// @param[in] cell 対象のセル
void
ItvlMgrImpl::print_cell(ostream& s,
			ItvlCell* cell) const
{
  if ( cell ) {
    print_cell(s, cell->mLchd);
    s << " " << cell->start()
      << " - " << cell->end()
      << endl;
    print_cell(s, cell->mRchd);
  }
}

// @brief 木構造を表示する
// @param[in] s 出力ストリーム
void
ItvlMgrImpl::print_tree(ostream& s) const
{
  print_tree_cell(s, mRoot, 0);
}

// @brief print_tree() の下請け関数
// @param[in] s 出力先のストリーム
// @param[in] cell 対象のセル
// @param[in] level レベル
void
ItvlMgrImpl::print_tree_cell(ostream& s,
			     ItvlCell* cell,
			     int level) const
{
  if ( cell ) {
    print_tree_cell(s, cell->mLchd, level + 1);

    for (int i = 0; i < level; i ++) {
      s << "    ";
    }
    s << " [" << cell->start()
      << " - " << cell->end()
      << "]";
    switch ( cell->balance() ) {
    case -1: s << ">"; break;
    case 0:  s << "="; break;
    case 1:  s << "<"; break;
    }
    s << endl;

    print_tree_cell(s, cell->mRchd, level + 1);
  }
}

// @brief バイナリファイルに書き出す．
void
ItvlMgrImpl::dump(ODO& s) const
{
  dump_cell(s, mRoot);
}

// @brief dump() の下請け関数
// @param[in] s 出力先のストリーム
// @param[in] cell 対象のセル
void
ItvlMgrImpl::dump_cell(ODO& s,
		       ItvlCell* cell) const
{
  if ( cell ) {
    s << static_cast<ymuint8>(cell->mBalance)
      << cell->mStart
      << cell->mEnd;
    dump_cell(s, cell->mLchd);
    dump_cell(s, cell->mRchd);
  }
  else {
    s << static_cast<ymuint8>(0xFF);
  }
}

// @brief バイナリファイルを読み込む．
void
ItvlMgrImpl::restore(IDO& s)
{
  mRoot = restore_cell(s);
}

// @brief restore() の下請け関数
// @param[in] s 入力元のストリーム
// @return 作成したセルを返す．
ItvlCell*
ItvlMgrImpl::restore_cell(IDO& s)
{
  ymuint8 balance;
  s >> balance;
  if ( balance < 0xFF ) {
    ymuint32 start;
    ymuint32 end;
    s >> start
      >> end;
    ItvlCell* cell = new_cell(start, end);
    cell->mBalance = static_cast<ymint8>(balance);
    ItvlCell* lchd = restore_cell(s);
    ItvlCell* rchd = restore_cell(s);
    cell->mLchd = lchd;
    cell->mRchd = rchd;
    return cell;
  }
  else {
    return NULL;
  }
}

// @brief 新しいセルを確保する．
// @param[in] start 開始位置
// @param[in] end 終了位置
ItvlCell*
ItvlMgrImpl::new_cell(int start,
		      int end)
{
  void* p = mCellAlloc.get_memory(sizeof(ItvlCell));
  return new (p) ItvlCell(start, end);
}

// @brief セルを削除する．
void
ItvlMgrImpl::delete_cell(ItvlCell* cell)
{
  if ( cell ) {
    delete_cell(cell->mLchd);
    delete_cell(cell->mRchd);
    mCellAlloc.put_memory(sizeof(ItvlCell), cell);
  }
}

END_NAMESPACE_YM
