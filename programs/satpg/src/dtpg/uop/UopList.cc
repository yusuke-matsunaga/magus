
/// @file UopList.cc
/// @brief UopList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "UopList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス UopList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UopList::UopList()
{
}

// @brief デストラクタ
UopList::~UopList()
{
  for (vector<UntestOp*>::iterator p = mUopList.begin();
       p != mUopList.end(); ++ p) {
    delete *p;
  }
}

// @brief uop を追加する．
void
UopList::add(UntestOp* uop)
{
  mUopList.push_back(uop);
}

// @brief テスト不能故障と判定された時の処理
// @param[in] f 故障
void
UopList::operator()(TpgFault* f)
{
  for (vector<UntestOp*>::iterator p = mUopList.begin();
       p != mUopList.end(); ++ p) {
    UntestOp& uop = **p;
    uop(f);
  }
}

END_NAMESPACE_YM_SATPG
