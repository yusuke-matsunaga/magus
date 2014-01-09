
/// @file GbmSolver.cc
/// @brief GbmSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"
#include "RcfNode.h"
#include "RcfNodeHandle.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmSolver::GbmSolver()
{
}

// @brief デストラクタ
GbmSolver::~GbmSolver()
{
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
bool
GbmSolver::solve(const RcfNetwork& network,
		 const TvFunc& func,
		 vector<bool>& conf_bits,
		 vector<ymuint>& iorder)
{
  bool stat = _solve(network, func, conf_bits, iorder);

  if ( stat ) {
    // 検証を行う．
    bool vstat = verify(network, func, conf_bits, iorder);
    assert_cond( vstat, __FILE__, __LINE__);
  }

  return stat;
}

// @brief 検証を行う．
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[in] conf_bits configuration ビットの値を収める配列
// @param[in] iorder 入力順序
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
bool
GbmSolver::verify(const RcfNetwork& network,
		  const TvFunc& func,
		  const vector<bool>& conf_bits,
		  const vector<ymuint>& iorder)
{
  ymuint nn = network.node_num();
  vector<TvFunc> func_array(nn);

  ymuint npi = func.input_num();
  // これは network.input_num() と等しいはず
  assert_cond( npi == network.input_num(), __FILE__, __LINE__);
  for (ymuint i = 0; i < npi; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    func_array[id] = TvFunc::posi_literal(npi, VarId(iorder[i]));
  }

  ymuint nf = network.func_node_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const RcfNode* node = network.func_node(i);
    func_array[node->id()] = node->calc_func(func_array, conf_bits);
  }

  RcfNodeHandle output = network.output();
  TvFunc ofunc = func_array[output.id()];
  if ( output.inv() ) {
    ofunc = ~ofunc;
  }

  if ( func != ofunc ) {
    cerr << "verification error" << endl
	 << " original func: " << func << endl
	 << " lut func:      " << ofunc << endl;
    cerr << " iorder: ";
    for (ymuint i = 0; i < npi; ++ i) {
      cerr << " " << iorder[i];
    }
    cerr << endl;
    ymuint nc = conf_bits.size();
    cerr << " conf_bits =    ";
    for (ymuint i = 0; i < nc; ++ i) {
      cerr << conf_bits[i];
    }
    cerr << endl;
    for (ymuint i = 0; i < nf; ++ i) {
      const RcfNode* node = network.func_node(i);
      cerr << "Node#" << node->id() << ": " << func_array[node->id()] << endl;
    }
  }
  return func == ofunc;
}

END_NAMESPACE_YM
