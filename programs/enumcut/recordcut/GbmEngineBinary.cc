
/// @file GbmEngineBinary.cc
/// @brief GbmEngineBinary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmEngineBinary.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmEngineBinary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] solver SATソルバ
// @param[in] node_num ノード数
// @param[in] conf_num 設定変数の数
// @param[in] input_num 入力数
// @param[in] rep 関数の対称変数の代表番号を収める配列
//            rep[pos] に pos 番めの入力の代表番号が入る．
GbmEngineBinary::GbmEngineBinary(SatSolver& solver,
				 ymuint node_num,
				 ymuint conf_num,
				 ymuint input_num,
				 const vector<ymuint>& rep) :
  GbmEngine(solver, node_num, conf_num),
  mInputNum(input_num),
  mRep(rep)
{
  ymuint m = 0;
  for (ymuint t = 1; t < input_num; t <<= 1, ++ m) ;
  mIorderBitWidth = m;
  mIorderVarArray.resize(input_num * m);
}

// @brief デストラクタ
GbmEngineBinary::~GbmEngineBinary()
{
}

// @brief 変数の初期化を行う．
// @param[in] network 対象の LUT ネットワーク
void
GbmEngineBinary::init_vars(const RcfNetwork& network)
{
  init_conf_vars();

  for (ymuint i = 0; i < mInputNum; ++ i) {
    for (ymuint j = 0; j < mIorderBitWidth; ++ j) {
      VarId vid = new_var();
      mIorderVarArray[i * mIorderBitWidth + j] = vid;
      if ( debug() ) {
	cout << "Iorder[" << i << "][" << j << "] = " << vid << endl;
      }
    }
  }

  for (ymuint i = 0; i < mInputNum; ++ i) {
    ymuint base_i = i * mIorderBitWidth;
    // 使っていない変数の組み合わせを禁止する．
    vector<Literal> tmp_lits2(mIorderBitWidth);
    for (ymuint b = mInputNum; b < (1U << mIorderBitWidth); ++ b) {
      for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	VarId kvar = mIorderVarArray[base_i + k];
	if ( b & (1U << k) ) {
	  tmp_lits2[k] = Literal(kvar, kPolNega);
	}
	else {
	  tmp_lits2[k] = Literal(kvar, kPolPosi);
	}
      }
      add_clause(tmp_lits2);
    }

    // 異なる LUT 入力におなじ入力が接続してはいけないというルール
    for (ymuint j = 0; j < i; ++ j) {
      ymuint base_j = j * mIorderBitWidth;
      // 0ビット目が異なるときに真となる変数
      VarId d0 = new_var();
      if ( debug() ) {
	cout << "diff[" << i << ", " << j << "][0] = " << d0 << endl;
      }
      Literal d0_lit(d0, kPolPosi);
      Literal xi0_lit(mIorderVarArray[base_i + 0], kPolPosi);
      Literal xj0_lit(mIorderVarArray[base_j + 0], kPolPosi);
      add_clause(~d0_lit,  xi0_lit,  xj0_lit);
      add_clause(~d0_lit, ~xi0_lit, ~xj0_lit);
      add_clause( d0_lit,  xi0_lit, ~xj0_lit);
      add_clause( d0_lit, ~xi0_lit,  xj0_lit);
      for (ymuint k = 1; k < mIorderBitWidth; ++ k) {
	Literal xik_lit(mIorderVarArray[base_i + k], kPolPosi);
	Literal xjk_lit(mIorderVarArray[base_j + k], kPolPosi);
	if ( k == mIorderBitWidth - 1 ) {
	  add_clause( d0_lit,  xik_lit,  xjk_lit);
	  add_clause( d0_lit, ~xik_lit, ~xjk_lit);
	}
	else {
	  VarId dk = new_var();
	  if ( debug() ) {
	    cout << "diff[" << i << ", " << j << "][" << k << "] = " << dk << endl;
	  }
	  Literal dk_lit(dk, kPolPosi);
	  add_clause(~dk_lit,  d0_lit,  xik_lit,  xjk_lit);
	  add_clause(~dk_lit,  d0_lit, ~xik_lit, ~xjk_lit);

	  add_clause( dk_lit, ~d0_lit,  xik_lit, ~xjk_lit);
	  add_clause( dk_lit, ~d0_lit, ~xik_lit,  xjk_lit);
	  d0_lit = dk_lit;
	}
      }
    }

    // LUT ネットワークの対称性を考慮したルール
    ymuint pred;
    if ( network.get_pred(i, pred) ) {
      // 最上位ビットで Vi > Vj となる条件
      VarId gm = new_var();
      if ( debug() ) {
	cout << "gt[" << i << ", " << pred << "][" << (mIorderBitWidth - 1) << "] = " << gm << endl;
      }
      Literal gm_lit(gm, kPolPosi);
      // 最上位ビットが等しい条件
      VarId em = new_var();
      if ( debug() ) {
	cout << "eq[" << i << ", " << pred << "][" << (mIorderBitWidth - 1) << "] = " << em << endl;
      }
      ymuint base_j = pred * mIorderBitWidth;
      Literal em_lit(em, kPolPosi);
      Literal xm_lit(mIorderVarArray[base_i + mIorderBitWidth - 1], kPolPosi);
      Literal ym_lit(mIorderVarArray[base_j + mIorderBitWidth - 1], kPolPosi);
      add_clause(~gm_lit,  xm_lit);
      add_clause(~gm_lit, ~ym_lit);

      add_clause( gm_lit, ~xm_lit,  ym_lit);

      add_clause(~em_lit,  xm_lit, ~ym_lit);
      add_clause(~em_lit, ~xm_lit,  ym_lit);

      add_clause( em_lit,  xm_lit,  ym_lit);
      add_clause( em_lit, ~xm_lit, ~ym_lit);
      for (ymuint k = mIorderBitWidth - 2; ; -- k) {
	Literal xk_lit(mIorderVarArray[base_i + k], kPolPosi);
	Literal yk_lit(mIorderVarArray[base_j + k], kPolPosi);
	if ( k == 0 ) {
	  add_clause( gm_lit,  em_lit);
	  add_clause( gm_lit,  xk_lit);
	  add_clause( gm_lit, ~yk_lit);
	  break;
	}
	else {
	  // k ビット以上で Vi > Vj となる条件
	  VarId gk = new_var();
	  if ( debug() ) {
	    cout << "gt[" << i << ", " << pred << "][" << k << "] = " << gk << endl;
	  }
	  Literal gk_lit(gk, kPolPosi);
	  // k ビット以上で Vi = Vj となる条件
	  VarId ek = new_var();
	  if ( debug() ) {
	    cout << "eq[" << i << ", " << pred << "][" << k << "] = " << ek << endl;
	  }
	  Literal ek_lit(ek, kPolPosi);
	  add_clause(~gk_lit,  gm_lit,  em_lit);
	  add_clause(~gk_lit,  gm_lit,  xk_lit);
	  add_clause(~gk_lit,  gm_lit, ~yk_lit);

	  add_clause( gk_lit, ~gm_lit);
	  add_clause( gk_lit, ~em_lit, ~xk_lit,  yk_lit);

	  add_clause(~ek_lit,  em_lit);
	  add_clause(~ek_lit,  xk_lit, ~yk_lit);
	  add_clause(~ek_lit, ~xk_lit,  yk_lit);

	  add_clause( ek_lit, ~em_lit,  xk_lit,  yk_lit);
	  add_clause( ek_lit, ~em_lit, ~xk_lit, ~yk_lit);
	  gm_lit = gk_lit;
	  em_lit = ek_lit;
	}
      }
    }

    // 関数の対称性に関するルール
    for (ymuint j = 0; j < mInputNum; ++ j) {
      ymuint cur_rep = mRep[j];
      if ( cur_rep == j ) {
	continue;
      }
      // 0 - (i - 1) までの入力で cur_rep が選ばれていなければならない．
      // ~X(i,j) + X(0, cur_rep) + X(1, cur_rep) + ...
      // だが，X はビットベクタなので積項となり面倒くさい．
      vector<Literal> tmp_lits(i + mIorderBitWidth);
      vector<ymuint> pos_array(i);
      for (ymuint k = 0; k < i; ++ k) {
	pos_array[k] = 0;
      }
      for (ymuint l = 0; l < mIorderBitWidth; ++ l) {
	tPol pol = (j & (1U << l)) ? kPolPosi : kPolNega;
	Literal lit(mIorderVarArray[i * mIorderBitWidth + l], pol);
	tmp_lits[i + l] = ~lit;
      }
      // 全部展開するとオーバーヘッドが大きい様なので mIorderBitWidth 個だけ展開する．
      // 完全ではなくても枝刈りには使える．
#if 1
      for (ymuint l = 0; l < mIorderBitWidth; ++ l) {
	for (ymuint k = 0; k < i; ++ k) {
	  tPol pol = (cur_rep & (1U << l)) ? kPolPosi : kPolNega;
	  Literal lit(mIorderVarArray[k * mIorderBitWidth + l], pol);
	  tmp_lits[k] = lit;
	}
	add_clause(tmp_lits);
      }
#else
      for ( ; ; ) {
	for (ymuint k = 0; k < i; ++ k) {
	  ymuint pos = pos_array[k];
	  tPol pol = (cur_rep & (1U << pos)) ? kPolPosi : kPolNega;
	  Literal lit(mIorderVarArray[k * mIorderBitWidth + pos], pol);
	  tmp_lits[k] = lit;
	}
	add_clause(tmp_lits);
	bool end = true;
	for (ymuint k = 0; k < i; ++ k) {
	  ++ pos_array[k];
	  if ( pos_array[k] < mIorderBitWidth ) {
	    end = false;
	    break;
	  }
	  pos_array[k] = 0;
	}
	if ( end ) {
	  break;
	}
      }
#endif
    }
  }
}

// @brief 入力値を割り当てて CNF 式を作る．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oval 出力の値
// @note 結果のCNF式は SAT ソルバに追加される．
bool
GbmEngineBinary::make_cnf(const RcfNetwork& network,
			  ymuint bit_pat,
			  bool oval)
{
  for (ymuint i = 0; i < mInputNum; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    VarId vid = new_var();
    set_node_var(id, GbmLit(vid));
    if ( debug() ) {
      cout << " lut_input#" << i << ": " << vid << endl;
    }

    ymuint base_i = i * mIorderBitWidth;

    // 入力と外部入力の間の関係式を作る．
    vector<Literal> tmp_lits(mIorderBitWidth + 1);
    for (ymuint j = 0; j < mInputNum; ++ j) {
      for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	VarId kvar = mIorderVarArray[base_i + k];
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
      add_clause(tmp_lits);
    }
  }

  return make_nodes_cnf(network, oval);
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
