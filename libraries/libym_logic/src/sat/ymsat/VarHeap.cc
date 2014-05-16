
/// @file VarHeap.cc
/// @brief VarHeap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VarHeap.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// VarHeap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VarHeap::VarHeap()
{
  mVarBump = 1.0;
  mVarDecay = 0.95;
  mVarNum = 0;
  mVarSize = 0;
  mHeapPos = NULL;
  mActivity = NULL;
  mHeap = NULL;
  mHeapNum = 0;
}

// @brief デストラクタ
VarHeap::~VarHeap()
{
  delete [] mHeapPos;
  delete [] mActivity;
  delete [] mHeap;
}

// @brief size 個の要素を格納出来るだけの領域を確保する．
// @param[in] size 必要なサイズ
void
VarHeap::alloc_var(ymuint size)
{
  ymuint old_var_num = mVarNum;
  ymuint old_size = mVarSize;
  ymint32* old_heap_pos = mHeapPos;
  double* old_activity = mActivity;
  ymuint32* old_heap = mHeap;
  if ( mVarSize == 0 ) {
    mVarSize = 1024;
  }
  while ( mVarSize < size ) {
    mVarSize <<= 1;
  }
  if ( mVarSize != old_size ) {
    mHeapPos = new ymint32[mVarSize];
    mActivity = new double[mVarSize];
    mHeap = new ymuint32[mVarSize];
    for (ymuint i = 0; i < old_var_num; ++ i) {
      mHeapPos[i] = old_heap_pos[i];
      mActivity[i] = old_activity[i];
    }
    for (ymuint i = 0; i < mHeapNum; ++ i) {
      mHeap[i] = old_heap[i];
    }
    if ( old_size > 0 ) {
      delete [] old_heap_pos;
      delete [] old_activity;
      delete [] old_heap;
    }
  }
  mVarNum = size;
}

// 変数のアクティビティを増加させる．
void
VarHeap::bump_var_activity(VarId varid)
{
  ymuint vindex = varid.val();
  double& act = mActivity[vindex];
  act += mVarBump;
  if ( act > 1e+100 ) {
    for (ymuint i = 0; i < mVarNum; ++ i) {
      mActivity[i] *= 1e-100;
    }
    mVarBump *= 1e-100;
  }
  ymint pos = mHeapPos[vindex];
  if ( pos >= 0 ) {
    move_up(pos);
  }
}

// 変数のアクティビティを定率で減少させる．
void
VarHeap::decay_var_activity()
{
  mVarBump /= mVarDecay;
}

// 引数の位置にある要素を適当な位置まで沈めてゆく
void
VarHeap::move_down(ymuint pos)
{
  ymuint vindex_p = mHeap[pos];
  double val_p = mActivity[vindex_p];
  for ( ; ; ) {
    // ヒープ木の性質から親から子の位置がわかる
    ymuint pos_l = left(pos);
    ymuint pos_r = pos_l + 1;
    if ( pos_r > mHeapNum ) {
      // 左右の子どもを持たない場合
      break;
    }
    ymuint pos_c = pos_l;
    ymuint vindex_c = mHeap[pos_c];
    double val_c = mActivity[vindex_c];
    if ( pos_r < mHeapNum ) {
      ymuint vindex_r = mHeap[pos_r];
      double val_r = mActivity[vindex_r];
      if ( val_c < val_r ) {
	pos_c = pos_r;
	vindex_c = vindex_r;
	val_c = val_r;
      }
    }
    if ( val_c <= val_p ) {
      break;
    }
    // 逆転
    set(vindex_p, pos_c);
    set(vindex_c, pos);
    pos = pos_c;
  }
}

// @brief 内容を出力する
void
VarHeap::dump(ostream& s) const
{
  s << "heap num = " << mHeapNum << endl;
  ymuint j = 0;
  ymuint nc = 1;
  const char* spc = "";
  for (ymuint i = 0; i < mHeapNum; ++ i) {
    ymuint vindex = mHeap[i];
    assert_cond(mHeapPos[vindex] == static_cast<ymint>(i),
		__FILE__, __LINE__);
    if ( i > 0 ) {
      ymint p = (i - 1) >> 1;
      assert_cond(mActivity[mHeap[p]] >= mActivity[vindex], __FILE__, __LINE__);
    }
    s << spc << vindex << "("
      << mActivity[vindex]
      << ")";
    ++ j;
    if ( j == nc ) {
      j = 0;
      nc <<= 1;
      s << endl;
      spc = "";
    }
    else {
      spc = " ";
    }
  }
  if ( j > 0 ) {
    s << endl;
  }
}

END_NAMESPACE_YM_SAT
