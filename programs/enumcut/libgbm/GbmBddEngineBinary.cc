
/// @file GbmBddEngineBinary.cc
/// @brief GbmBddEngineBinary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmBddEngineBinary.h"
#include "YmLogic/BddLitSet.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmBddEngineBinary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr BddMgr
GbmBddEngineBinary::GbmBddEngineBinary(BddMgr& mgr) :
  mMgr(mgr),
  mDebug(false)
{
}

// @brief デストラクタ
GbmBddEngineBinary::~GbmBddEngineBinary()
{
}

// @brief debug フラグを立てる
void
GbmBddEngineBinary::debug_on()
{
  mDebug = true;
}

// @brief debug フラグを降ろす
void
GbmBddEngineBinary::debug_off()
{
  mDebug = false;
}

// @brief debug フラグの値を得る．
bool
GbmBddEngineBinary::debug() const
{
  return mDebug;
}

// @brief 対称性を考慮して初期解を作る．
// @param[in] rep 関数の対称変数の代表番号を収める配列
void
GbmBddEngineBinary::init_vars(const RcfNetwork& network,
			      const vector<ymuint>& rep)
{
  ymuint nc = network.conf_var_num();
  mConfVarArray.clear();
  mConfVarArray.resize(nc);

  mInputNum = network.input_num();
  ymuint m = 0;
  for (ymuint t = 1; t < mInputNum; t <<= 1, ++ m) ;
  mIorderBitWidth = m;
  mIorderVarArray.clear();
  mIorderVarArray.resize(mInputNum * m);

  // 最初に入力順を表す変数を確保する．
  for (ymuint i = 0; i < mInputNum; ++ i) {
    for (ymuint j = 0; j < mIorderBitWidth; ++ j) {
      VarId vid(i * mIorderBitWidth + j);
      mIorderVarArray[i * mIorderBitWidth + j] = vid;
      bool stat = mMgr.new_var(vid);
      assert_cond( stat, __FILE__, __LINE__);
    }
  }

  // 次に configuration bit 用の変数を確保する．
  ymuint base0 = mInputNum * mIorderBitWidth;
  for (ymuint i = 0; i < mConfVarArray.size(); ++ i) {
    VarId vid(base0 + i);
    mConfVarArray[i] = vid;
    bool stat = mMgr.new_var(vid);
    assert_cond( stat, __FILE__, __LINE__);
  }

#if 0
  // one-hot 制約と対称性を考慮して初期解を作る．
  ymuint var_list = 0U;
  for (ymuint i = 0; i < mInputNum; ++ i) {
    var_list |= (1U << i);
  }
  ymuint ni_exp = 1U << mInputNum;
  mIorderBddArray.clear();
  mIorderBddArray.resize(ni_exp, mMgr.make_zero());
  mIorderBddArray[0] = mMgr.make_one();
  mSolution = make_iorder_bdd(0, var_list, network);

  if ( true || debug() ) {
    mpz_class mc = mSolution.minterm_count(nc);
    cout << "Total " << mc << " permutations" << endl;
    mSolution.print(cout);
  }
#else
  mSolution = mMgr.make_one();
  for (ymuint i = 0; i < mInputNum; ++ i) {
    ymuint base_i = i * mIorderBitWidth;
    ymuint nexp = 1U << mIorderBitWidth;

    // 使っていない変数の組み合わせを禁止する．
    Bdd ng = mMgr.make_zero();
    for (ymuint b = mInputNum; b < nexp; ++ b) {
      Bdd ng1 = mMgr.make_one();
      for (ymuint j = 0; j < mIorderBitWidth; ++ j) {
	if ( b & (1U << (mIorderBitWidth - j - 1)) ) {
	  ng1 &= mMgr.make_posiliteral(mIorderVarArray[base_i + j]);
	}
	else {
	  ng1 &= mMgr.make_negaliteral(mIorderVarArray[base_i + j]);
	}
      }
      ng |= ng1;
    }
    mSolution &= ~ng;

    // 異なる LUT 入力におなじ入力が接続してはいけないというルール
    for (ymuint j = 0; j < i; ++ j) {
      ymuint base_j = j * mIorderBitWidth;
      Bdd ng = mMgr.make_one();
      for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	VarId ivar = mIorderVarArray[base_i + k];
	VarId jvar = mIorderVarArray[base_j + k];
	ng &= ~(mMgr.make_posiliteral(ivar) ^ mMgr.make_posiliteral(jvar));
      }
      mSolution &= ~ng;
    }

#if 0
    // LUT ネットワークの対称性を考慮したルール
    ymuint pred;
    if ( network.get_pred(i, pred) ) {
      ymuint base_j = pred * mIorderBitWidth;
      // Vi > Vj が ng
      Bdd ng = mMgr.make_zero();
      Bdd eq = mMgr.make_one();
      for (ymuint k = 0; k < mIorderBitWidth; ++k) {
	VarId ivar = mIorderVarArray[base_i + k];
	VarId jvar = mIorderVarArray[base_j + k];
	Bdd ibdd = mMgr.make_posiliteral(ivar);
	Bdd jbdd = mMgr.make_posiliteral(jvar);
	ng |= eq & ibdd & ~jbdd;
	eq &= ~(ibdd ^ jbdd);
      }
      mSolution &= ~ng;
    }
#endif
  }

  mpz_class mc = mSolution.minterm_count(nc);
  cout << "Total " << mc << " permutations" << endl;
  mSolution.print(cout);
#endif
}

// @brief 入力値を割り当てて解の候補を求める．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oid 出力のノード番号
// @param[in] oval 出力の値
// @return 結果が空でなければ true を返し，model にその1つを収める．
Bool3
GbmBddEngineBinary::make_bdd(const RcfNetwork& network,
			     ymuint bitpat,
			     bool oval)
{
  ymuint nn = network.node_num();
  mNodeBddArray.clear();
  mNodeBddArray.resize(nn);

  for (ymuint i = 0; i < mInputNum; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();

    ymuint base_i = i * mIorderBitWidth;

    Bdd y = mMgr.make_zero();
    for (ymuint j = 0; j < mInputNum; ++ j) {
      if ( bitpat & (1U << j) ) {
	Bdd p = mMgr.make_one();
	for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	  VarId kvar = mIorderVarArray[base_i + k];
	  if ( j & (1U << (mIorderBitWidth - k - 1)) ) {
	    p &= mMgr.make_posiliteral(kvar);
	  }
	  else {
	    p &= mMgr.make_negaliteral(kvar);
	  }
	}
	y |= p;
      }
    }
    mNodeBddArray[id] = y;
  }

  ymuint nf = network.func_node_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const RcfNode* node = network.func_node(i);
    make_node_func(node);
  }

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  if ( oinv ) {
    oval = !oval;
  }

  if ( oval ) {
    mSolution &= mNodeBddArray[oid];
  }
  else {
    mSolution &= ~mNodeBddArray[oid];
  }

  if ( mSolution.is_zero() ) {
    return kB3False;
  }
  if ( mSolution.is_invalid() ) {
    return kB3X;
  }
  return kB3True;
}

// @brief 結果からモデルを一つ取り出す．
void
GbmBddEngineBinary::get_model(vector<Bool3>& model)
{
  assert_cond( !mSolution.is_invalid(), __FILE__, __LINE__);

  if ( mSolution.is_zero() ) {
    return;
  }

  ymuint nall = mConfVarArray.size() + mIorderVarArray.size();
  model.clear();
  model.resize(nall, kB3X);

  BddLitSet opath = mSolution.onepath();
  LiteralVector lv;
  opath.to_vector(lv);
  for (LiteralVector::iterator p = lv.begin();
       p != lv.end(); ++ p) {
    Literal l = *p;
    if ( l.is_positive() ) {
      model[l.varid().val()] = kB3True;
    }
    else {
      model[l.varid().val()] = kB3False;
    }
  }
}

// @brief SAT モデルから設定変数の割り当てを取り出す．
// @param[in] model SAT モデル
// @param[out] conf_bits 設定変数の割り当て
void
GbmBddEngineBinary::get_conf_bits(const vector<Bool3>& model,
				  vector<bool>& conf_bits) const
{
  ymuint nc = mConfVarArray.size();
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = mConfVarArray[i];
    if ( vid.val() >= model.size() ) {
      cout << " vid.val() >= model.size()" << endl
	   << " vid.val() = " << vid.val() << endl
	   << " model.size() = " << model.size() << endl;
    }
    if ( model[vid.val()] == kB3True ) {
      conf_bits[i] = true;
    }
    else {
      conf_bits[i] = false;
    }
  }
}

// @brief SAT モデルから入力順を取り出す．
// @param[in] model SAT モデル
// @param[out] iorder 入力順
//             iorder[pos] に network の pos 番めの入力に対応した
//             関数の入力番号が入る．
void
GbmBddEngineBinary::get_iorder(const vector<Bool3>& model,
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

// @brief 変数順を表すBDDを作る．
// @param[in] level レベル
// @param[in] var_list 変数の集合を表すビットベクタ
//
// var_list に含まれる変数の順列をすべて表す
// BDD を返す．
Bdd
GbmBddEngineBinary::make_iorder_bdd(ymuint level,
				    ymuint var_list,
				    const RcfNetwork& network)
{
#if 0
  cout << "make_iorder_bdd(" << level << ")" << endl;
  for (ymuint i = 0; i < mInputNum; ++ i) {
    if ( var_list & (1U << i) ) {
      cout << "1";
    }
    else {
      cout << "0";
    }
  }
  cout << endl;
#endif

  Bdd f = mIorderBddArray[var_list];
  if ( f.is_zero() ) {
    f = mMgr.make_zero();
    for (ymuint i = 0; i < mInputNum; ++ i) {
      if ( var_list & (1U << i) ) {
#if 1
	ymuint pred;
	if ( network.get_pred(i, pred) ) {
	  if ( var_list & (1U << pred) ) {
	    // LUTの対称性による枝刈り
	    continue;
	  }
	}
#endif
	ymuint var_list1 = var_list & ~(1U << i);
	Bdd f1 = make_iorder_bdd(level + 1, var_list1, network);
	for (ymuint k = 0; k < mIorderBitWidth; ++ k) {
	  if ( i & (1U << (mIorderBitWidth - k - 1)) ) {
	    f1 &= mMgr.make_posiliteral(mIorderVarArray[level * mIorderBitWidth + k]);
	  }
	  else {
	    f1 &= mMgr.make_negaliteral(mIorderVarArray[level * mIorderBitWidth + k]);
	  }
	}
	f |= f1;
      }
    }
    mIorderBddArray[var_list] = f;
  }
#if 0
  cout << "==> " << endl;
  f.print(cout);
#endif
  return f;
}

// @brief RcfNode に対応する関数を計算する．
// @param[in] node 対象のノード
// @note 結果は mNodeBddArray に格納される．
void
GbmBddEngineBinary::make_node_func(const RcfNode* node)
{
  assert_cond( !node->is_input(), __FILE__, __LINE__);

  ymuint ni = node->fanin_num();
  vector<Bdd> inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    RcfNodeHandle ih = node->fanin(i);
    assert_cond( !ih.is_const(), __FILE__, __LINE__);
    ymuint id = ih.id();
    Bdd f = mNodeBddArray[id];
    if ( ih.inv() ) {
      f = ~f;
    }
    inputs[i] = f;
  }

  Bdd output;
  if ( node->is_and() ) {
    output = inputs[0];
    for (ymuint i = 1; i < ni; ++ i) {
      output &= inputs[i];
    }
  }
  else if ( node->is_lut() ) {
    ymuint ni_exp = 1U << ni;
    vector<Bdd> lut_vars(ni_exp);
    ymuint conf0 = node->conf_base();
    for (ymuint i = 0; i < ni_exp; ++ i) {
      lut_vars[i] = mMgr.make_posiliteral(mConfVarArray[conf0 + i]);
    }
    output = make_LUT(inputs, lut_vars);
  }
  else if ( node->is_mux() ) {
    ymuint ns = node->conf_size();
    vector<Bdd> s_vars(ns);
    ymuint s0 = node->conf_base();
    for (ymuint i = 0; i < ns; ++ i) {
      s_vars[i] = mMgr.make_posiliteral(mConfVarArray[s0 + i]);
    }
    output = make_MUX(inputs, s_vars);
  }

  mNodeBddArray[node->id()] = output;
}

// @brief LUT ノードの出力の論理関数を計算する．
// @param[in] inputs ファンインの論理関数
// @param[in] lut_vars LUT のコンフィグレーションメモリ
Bdd
GbmBddEngineBinary::make_LUT(const vector<Bdd>& inputs,
			     const vector<Bdd>& lut_vars)
{
  return make_MUX(lut_vars, inputs);
}

// @brief セレクタの出力の論理関数を計算する．
// @param[in] inputs ファンインの論理関数
// @param[in] s_vars 選択変数
Bdd
GbmBddEngineBinary::make_MUX(const vector<Bdd>& inputs,
			     const vector<Bdd>& s_vars)
{
  ymuint nd = inputs.size();
  ymuint ns = s_vars.size();
  assert_cond( (1U << ns) == nd, __FILE__, __LINE__);
  Bdd output = mMgr.make_zero();
  for (ymuint b = 0; b < nd; ++ b) {
    Bdd dvar = inputs[b];
    for (ymuint i = 0; i < ns; ++ i) {
      Bdd svar = s_vars[i];
      if ( (1 << i) & b ) {
	dvar &= svar;
      }
      else {
	dvar &= ~svar;
      }
    }
    output |= dvar;
  }
  return output;
}

END_NAMESPACE_YM
