
/// @file DetOp.cc
/// @brief DetOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetOp.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス DetOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DetOp::DetOp()
{
}

// @brief デストラクタ
DetOp::~DetOp()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
DetOp::operator()(const TpgFault* f,
		  PackedVal dpat)
{
  ymuint f_id = f->id();
  mDetList.push_back(make_pair(f_id, dpat));
}

// @brief 検出された故障のID番号のリストを返す．
const vector<pair<ymuint, PackedVal> >&
DetOp::det_list()
{
  return mDetList;
}

// @brief det_list をクリアする．
void
DetOp::clear_det_list()
{
  mDetList.clear();
}

END_NAMESPACE_YM_SATPG
