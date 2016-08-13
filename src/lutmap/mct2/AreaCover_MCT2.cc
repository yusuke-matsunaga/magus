
/// @file AreaCover_MCT2.cc
/// @brief AreaCover_MCT2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover_MCT2.h"
#include "MctSearch.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_YM_LUTMAP_MCT2

//////////////////////////////////////////////////////////////////////
// クラス AreaCover_MCT2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mode モード
AreaCover_MCT2::AreaCover_MCT2(ymuint mode)
{
  mMode = mode;
}

// @brief デストラクタ
AreaCover_MCT2::~AreaCover_MCT2()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover_MCT2::record_cuts(const SbjGraph& sbjgraph,
			    const CutHolder& cut_holder,
			    MapRecord& maprec)
{
  MctSearch mct(sbjgraph, cut_holder, cut_holder.limit(), mMode);

  mct.search(1000000);

  maprec = mct.best_record();
}

END_NAMESPACE_YM_LUTMAP_MCT2
