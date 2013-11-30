
/// @file ItvlCell.cc
/// @brief ItvlCell の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ItvlCell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ItvlCell の実装
//////////////////////////////////////////////////////////////////////

// 正しい構造かチェックする．
int
ItvlCell::check_internal(int& l,
			 int& r) const
{
  if ( !this ) {
    return 0;
  }
  int l1, r1;
  int hl = mLchd->check_internal(l1, r1);
  int l2, r2;
  int hr = mRchd->check_internal(l2, r2);

  switch ( mBalance ) {
  case 1: // 右が高いはず
    assert_cond(hl < hr, __FILE__, __LINE__);
    break;

  case -1: // 左が高いはず
    assert_cond(hl > hr, __FILE__, __LINE__);
    break;

  case 0: // 高さは等しいはず
    assert_cond(hl == hr, __FILE__, __LINE__);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  if ( mLchd ) {
    assert_cond(r1 < mStart, __FILE__, __LINE__);
    l = l1;
  }
  else {
    l = mStart;
  }
  if ( mRchd ) {
    assert_cond(l2 > mEnd, __FILE__, __LINE__);
    r = r2;
  }
  else {
    r = mEnd;
  }
  int h = hl;
  if ( h < hr ) {
    h = hr;
  }
  return h + 1;
}

END_NAMESPACE_YM
