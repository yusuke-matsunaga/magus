
/// @file AreaCover_SA.cc
/// @brief AreaCover_SA の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "AreaCover_SA.h"
#include "SaSearch.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス AreaCover_SA
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mode モード
AreaCover_SA::AreaCover_SA(ymuint mode)
{
  mMode = mode;
}

// @brief デストラクタ
AreaCover_SA::~AreaCover_SA()
{
}

// @brief best cut の記録を行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
// @param[in] count 試行回数
// @param[out] maprec マッピング結果を記録するオブジェクト
void
AreaCover_SA::record_cuts(const SbjGraph& sbjgraph,
			  const CutHolder& cut_holder,
			  ymuint count,
			  bool verbose,
			  MapRecord& maprec)
{
  SaSearch sa(sbjgraph, cut_holder, cut_holder.limit(), mMode);

  sa.set_verbose(verbose);

  sa.search(count);

  maprec = sa.best_record();
}

END_NAMESPACE_YM_LUTMAP
