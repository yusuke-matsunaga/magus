
/// @file GbmEngineEnum.cc
/// @brief GbmEngineEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngineEnum.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmEngineEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
GbmEngineEnum::GbmEngineEnum(SatSolver& solver) :
  GbmEngine(solver)
{
}

// @brief デストラクタ
GbmEngineEnum::~GbmEngineEnum()
{
}

// @brief 入力値を割り当てて CNF 式を作る．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] iorder 入力順
//　          iorder[pos] に network の pos 番めの入力に対応した
//            関数の入力番号が入る．
// @param[in] oval 出力の値
// @note 結果のCNF式は SAT ソルバに追加される．
bool
GbmEngineEnum::make_cnf(const RcfNetwork& network,
			ymuint bit_pat,
			const vector<ymuint>& iorder,
			bool oval)
{
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    ymuint src_pos = iorder[i];
    if ( bit_pat & (1U << src_pos) ) {
      set_node_var(id, GbmLit::make_one());
    }
    else {
      set_node_var(id, GbmLit::make_zero());
    }
  }
  return make_nodes_cnf(network, oval);
}

END_NAMESPACE_YM
