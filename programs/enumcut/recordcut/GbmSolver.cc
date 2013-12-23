
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

// @brief 問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
bool
GbmSolver::solve(const RcfNetwork& network,
		 const TvFunc& func,
		 vector<bool>& conf_bits)
{
  bool stat = _solve(network, func, conf_bits);

  if ( stat ) {
    // 検証を行う．
    ymuint ni = network.input_num();
    vector<ymuint> iorder(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      iorder[i] = i;
    }
    bool vstat = verify(network, func, conf_bits, iorder);
    assert_cond( vstat, __FILE__, __LINE__);
  }

  return stat;
}

// @brief 入力順を考慮したマッチング問題を解く
// @param[in] network RcfNetwork
// @param[in] func マッチング対象の関数
// @param[out] conf_bits configuration ビットの値を収める配列
// @param[out] iorder 入力順序
// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
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
bool
GbmSolver::verify(const RcfNetwork& network,
		  const TvFunc& func,
		  const vector<bool>& conf_bits,
		  const vector<ymuint>& iorder)
{
  ymuint nn = network.node_num();
  vector<TvFunc> func_array(nn);
  vector<bool> done(nn, false);

  ymuint npi = func.input_num();
  // これは network.input_num() と等しいはず
  for (ymuint i = 0; i < npi; ++ i) {
    const RcfNode* node = network.input_node(iorder[i]);
    ymuint id = node->id();
    func_array[id] = TvFunc::posi_literal(npi, VarId(i));
    done[id] = true;
  }

  // ちょっと効率が悪いけどわかりやすいコード
  for ( ; ; ) {
    bool skipped = false;
    for (ymuint i = 0; i < nn; ++ i) {
      if ( done[i] ) {
	// 既に処理したものは飛ばす．
	continue;
      }

      // まだ処理していないファンインを持つノードも飛ばす．
      const RcfNode* node = network.node(i);
      ymuint nfi = node->fanin_num();
      bool not_yet = false;
      for (ymuint j = 0; j < nfi; ++ j) {
	RcfNodeHandle ihandle = node->fanin(j);
	ymuint iid = ihandle.id();
	if ( !done[iid] ) {
	  not_yet = true;
	  break;
	}
      }
      if ( not_yet ) {
	skipped = true;
	continue;
      }

      func_array[i] = node->calc_func(func_array, conf_bits);
      done[i] = true;
    }
    if ( !skipped ) {
      break;
    }
  }

  RcfNodeHandle output = network.output();
  TvFunc ofunc = func_array[output.id()];
  if ( output.inv() ) {
    ofunc = ~ofunc;
  }

  return func == ofunc;
}

END_NAMESPACE_YM
