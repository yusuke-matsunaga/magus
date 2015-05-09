
/// @file McOp.cc
/// @brief McOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "McOp.h"
#include "TpgFault.h"
#include "Fsim.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス McOP
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mincov 最小被覆問題のオブジェクト
// @param[in] row_map 故障番号から行番号を得る表
// @param[in] col_pos 列番号
McOp::McOp(MinCov& mincov,
	   const vector<ymuint>& row_map,
	   ymuint col_pos) :
  mMinCov(mincov),
  mRowMap(row_map),
  mColPos(col_pos)
{
}

// @brief デストラクタ
McOp::~McOp()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
// @param[in] dpat 検出したパタンを表すビットベクタ
void
McOp::operator()(const TpgFault* f,
		 PackedVal dpat)
{
  ymuint row_pos = mRowMap[f->id()];
  if ( row_pos > 0 ) {
    -- row_pos;
    mMinCov.insert_elem(row_pos, mColPos);
    cout << " " << f->id() << " , " << mColPos << endl;
  }
}

END_NAMESPACE_YM_SATPG
