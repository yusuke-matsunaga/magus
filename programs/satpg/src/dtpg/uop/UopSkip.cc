
/// @file UopSkip.h
/// @brief UopSkip の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "UopSkip.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'skip' タイプを生成する．
// @param[in] threshold しきい値
UntestOp*
new_UopSkip(AtpgMgr& mgr,
	    ymuint threshold)
{
  return new UopSkip(threshold);
}


//////////////////////////////////////////////////////////////////////
// クラス UopSkip
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] threshold しきい値
UopSkip::UopSkip(ymuint threshold)
{
  mThreshold = threshold;
}

// @brief デストラクタ
UopSkip::~UopSkip()
{
  clear();
}

// @brief テスト不能故障と判定された時の処理
// @param[in] f 故障
void
UopSkip::operator()(TpgFault* f)
{
  if ( f->untest_num() == 0 ) {
    // はじめて検出不能になった．
    mUntestList.push_back(f);
  }

  // 検出不能回数を1増やす．
  f->inc_untest_num();

  if ( f->untest_num() >= mThreshold ) {
    // 検出不能回数がしきい値を越えたのでスキップする．
    f->set_skip();
    mSkipList.push_back(f);
  }
}

// @brief 検出不能回数とスキップフラグをクリアする．
void
UopSkip::clear()
{
  for (vector<TpgFault*>::iterator p = mUntestList.begin();
       p != mUntestList.end(); ++ p) {
    TpgFault* f = *p;
    f->clear_untest_num();
  }
  for (vector<TpgFault*>::iterator p = mSkipList.begin();
       p != mSkipList.end(); ++ p) {
    TpgFault* f = *p;
    f->clear_skip();
  }
  mUntestList.clear();
  mSkipList.clear();
}

END_NAMESPACE_YM_SATPG
