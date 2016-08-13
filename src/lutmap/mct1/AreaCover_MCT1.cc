
/// @file AreaCover_MCT1.cc
/// @brief AreaCover_MCT1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover_MCT1.h"
#include "MctSearch.h"


BEGIN_NAMESPACE_YM_LUTMAP_MCT1

//////////////////////////////////////////////////////////////////////
// クラス AreaCover_MCT1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mode モード
AreaCover_MCT1::AreaCover_MCT1(ymuint mode)
{
  mMode = mode;
}

// @brief デストラクタ
AreaCover_MCT1::~AreaCover_MCT1()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover_MCT1::record_cuts(const SbjGraph& sbjgraph,
			    const CutHolder& cut_holder,
			    MapRecord& maprec)
{
  MctSearch mct(sbjgraph, cut_holder, limit);

  mct.search(1000000);
}

END_NAMESPACE_YM_LUTMAP_MCT1
