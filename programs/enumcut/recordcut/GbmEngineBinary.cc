
/// @file GbmEngineBinary.cc
/// @brief GbmEngineBinary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngineBinary.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

const bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス GbmEngineBinary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
// @param[in] node_num ノード数
// @param[in] conf_num 設定変数の数
// @param[in] input_num 入力数
GbmEngineBinary::GbmEngineBinary(SatSolver& solver,
				 ymuint node_num,
				 ymuint conf_num,
				 ymuint input_num) :
  GbmEngine(solver, node_num, conf_num)
{
  ymuint m = 0;
  for (ymuint t = 1; t < input_num; t <<= 1, ++ m) ;
  mIorderBitWidth = m;
  mIorderVarArray.resize(input_num * m);
  for (ymuint i = 0; i < input_num; ++ i) {
    for (ymuint j = 0; j < m; ++ j) {
      mIorderVarArray[i * m + j] = solver.new_var();
    }
  }
}

// @brief デストラクタ
GbmEngineBinary::~GbmEngineBinary()
{
}

// @brief 入力値を割り当てて CNF 式を作る．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oid 出力のノード番号
// @param[in] oval 出力の値
// @note 結果のCNF式は SAT ソルバに追加される．
bool
GbmEngineBinary::make_cnf(const RcfNetwork& network,
			  ymuint bit_pat,
			  ymuint oid,
			  bool oval)
{
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    VarId vid = set_node_newvar(id);
    if ( debug ) {
      cout << " lut_input#" << i << ": " << vid << endl;
    }

      // 入力と外部入力の間の関係式を作る．
    vector<Literal> tmp_lits(mIorderBitWidth + 1);
    for (ymuint j = 0; j < ni; ++ j) {
      for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	VarId kvar = mIorderVarArray[i * mIorderBitWidth + k];
	// こちらは含意の左辺なので否定する．
	if ( j & (1U << k) ) {
	  tmp_lits[k] = Literal(kvar, kPolNega);
	}
	else {
	  tmp_lits[k] = Literal(kvar, kPolPosi);
	}
      }
      if ( bit_pat & (1U << j) ) {
	tmp_lits[mIorderBitWidth] = Literal(vid, kPolPosi);
      }
      else {
	tmp_lits[mIorderBitWidth] = Literal(vid, kPolNega);
      }
      if ( debug ) {
	cout << " added clause = ";
	for (ymuint x = 0; x <= mIorderBitWidth; ++ x) {
	  cout << " " << tmp_lits[x];
	}
	cout << endl;
      }
      add_clause(tmp_lits);
    }
    // 使っていない変数の組み合わせを禁止する．
    vector<Literal> tmp_lits2(mIorderBitWidth);
    for (ymuint j = ni; j < (1U << mIorderBitWidth); ++ j) {
      for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	VarId kvar = mIorderVarArray[i * mIorderBitWidth + k];
	if ( j & (1U << k) ) {
	  tmp_lits2[k] = Literal(kvar, kPolNega);
	}
	else {
	  tmp_lits2[k] = Literal(kvar, kPolPosi);
	}
      }
      if ( debug ) {
	cout << " added clause = ";
	for (ymuint x = 0; x < mIorderBitWidth; ++ x) {
	  cout << " " << tmp_lits2[x];
	}
	cout << endl;
      }
      add_clause(tmp_lits2);
    }

#if 0
    // 異なる LUT 入力におなじ入力が接続してはいけないというルール
    for (ymuint j = 0; j < ni; ++ j) {
      vector<Literal> tmp_lits3(mIorderBitWidth * 2);
      for (ymuint k = 0; k < i; ++ k) {
	for (ymuint l = 0; l < mIorderBitWidth; ++ l) {
	  tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	  tmp_lits3[l] = Literal(mIorderVarArray[k * mIorderBitWidth + l], pol);
	  tmp_lits3[l + mIorderBitWidth] = Literal(mIorderVarArray[i * mIorderBitWidth + l], pol);
	}
	if ( debug ) {
	  cout << " added clause = ";
	  for (ymuint x = 0; x < mIorderBitWidth; ++ x) {
	    cout << " " << tmp_lits3[x];
	  }
	  cout << endl;
	}
	add_clause(tmp_lits3);
      }
    }

    // 対称性を考慮したルール
    ymuint pred;
    if ( network.get_pred(i, pred) ) {
      for (ymuint j = 0; j < ni; ++ j) {
	vector<Literal> tmp_lits3(mIorderBitWidth * 2);
	for (ymuint l = 0; l < m; ++ l) {
	  tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	  tmp_lits3[l] = Literal(mIorderVarArray[i * mIorderBitWidth + l], pol);
	}
	for (ymuint k = j + 1; k < ni; ++ k) {
	  for (ymuint l = 0; l < mIorderBitWidth; ++ l) {
	    tPol pol = ( j & (1U << l) ) ? kPolNega : kPolPosi;
	    tmp_lits3[l + mIorderBitWidth] = Literal(mIorderVarArray[pred * mIorderBitWidth + l], pol);
	  }
	  if ( debug ) {
	    cout << " added clause = ";
	    for (ymuint x = 0; x < mIorderBitWidth; ++ x) {
	      cout << " " << tmp_lits3[x];
	    }
	    cout << endl;
	  }
	  add_clause(tmp_lits3);
	}
      }
    }
#endif
  }

  return make_nodes_cnf(network, oid, oval);
}

// @brief SAT モデルから入力順を取り出す．
// @param[in] model SAT モデル
// @param[out] iorder 入力順
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
void
GbmEngineBinary::get_iorder(const vector<Bool3>& model,
			    vector<ymuint>& iorder) const
{
  ymuint ni = iorder.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint pos = 0;
    ymuint base = i * mIorderBitWidth;
    for (ymuint j = 0; j < mIorderBitWidth; ++ j) {
      VarId vid = mIorderVarArray[base + j];
      if ( model[vid.val()] == kB3True ) {
	pos += (1U << j);
      }
    }
    iorder[i] = pos;
  }
}

END_NAMESPACE_YM
