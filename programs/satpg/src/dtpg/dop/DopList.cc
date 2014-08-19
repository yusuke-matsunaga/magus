
/// @file DopList.cc
/// @brief DopList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DopList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス DopList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DopList::DopList()
{
}

// @brief デストラクタ
DopList::~DopList()
{
  for (vector<DetectOp*>::iterator p = mDopList.begin();
       p != mDopList.end(); ++ p) {
    delete *p;
  }
}

// @brief dop を追加する．
void
DopList::add(DetectOp* dop)
{
  mDopList.push_back(dop);
}

// @brief テストパタンが見つかった時の処理
// @param[in] f 故障
// @param[in] tv テストパタン
void
DopList::operator()(TpgFault* f,
		    TestVector* tv)
{
  for (vector<DetectOp*>::iterator p = mDopList.begin();
       p != mDopList.end(); ++ p) {
    DetectOp& dop = **p;
    dop(f, tv);
  }
}

END_NAMESPACE_YM_SATPG
