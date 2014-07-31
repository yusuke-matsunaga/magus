
/// @file GbmBddEngineOneHot.cc
/// @brief GbmBddEngineOneHot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmBddEngineOneHot.h"
#include "YmLogic/BddLitSet.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmBddEngineOneHot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr BddMgr
GbmBddEngineOneHot::GbmBddEngineOneHot(BddMgr& mgr) :
  mMgr(mgr),
  mDebug(false)
{
}

// @brief デストラクタ
GbmBddEngineOneHot::~GbmBddEngineOneHot()
{
}

// @brief debug フラグを立てる
void
GbmBddEngineOneHot::debug_on()
{
  mDebug = true;
}

// @brief debug フラグを降ろす
void
GbmBddEngineOneHot::debug_off()
{
  mDebug = false;
}

// @brief debug フラグの値を得る．
bool
GbmBddEngineOneHot::debug() const
{
  return mDebug;
}

// @brief 対称性を考慮して初期解を作る．
// @param[in] rep 関数の対称変数の代表番号を収める配列
void
GbmBddEngineOneHot::init_vars(const RcfNetwork& network,
			      const vector<ymuint>& rep)
{
  ymuint nc = network.conf_var_num();
  mConfVarArray.clear();
  mConfVarArray.resize(nc);

  mInputNum = network.input_num();
  mIorderVarArray.clear();
  mIorderVarArray.resize(mInputNum * mInputNum);

  // 最初に入力順を表す変数を確保する．
  for (ymuint i = 0; i < mInputNum; ++ i) {
    for (ymuint j = 0; j < mInputNum; ++ j) {
      VarId vid(i * mInputNum + j);
      mIorderVarArray[i * mInputNum + j] = vid;
      bool stat = mMgr.new_var(vid);
      assert_cond( stat, __FILE__, __LINE__);
    }
  }

  // 次に configuration bit 用の変数を確保する．
  ymuint base = mInputNum * mInputNum;
  for (ymuint i = 0; i < mConfVarArray.size(); ++ i) {
    VarId vid(base + i);
    mConfVarArray[i] = vid;
    bool stat = mMgr.new_var(vid);
    assert_cond( stat, __FILE__, __LINE__);
  }

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

  if ( debug() ) {
    mpz_class mc = mSolution.minterm_count(nc);
    cout << "Total " << mc << " permutations" << endl;
    mSolution.print(cout);
  }
}

// @brief 入力値を割り当てて解の候補を求める．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oid 出力のノード番号
// @param[in] oval 出力の値
// @return 結果が空でなければ true を返し，model にその1つを収める．
Bool3
GbmBddEngineOneHot::make_bdd(const RcfNetwork& network,
			     ymuint bitpat,
			     bool oval)
{
  ymuint nn = network.node_num();
  mNodeBddArray.clear();
  mNodeBddArray.resize(nn);

  for (ymuint i = 0; i < mInputNum; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    Bdd y = mMgr.make_zero();
    for (ymuint j = 0; j < mInputNum; ++ j) {
      if ( bitpat & (1U << j) ) {
	VarId vid_j = mIorderVarArray[i * mInputNum + j];
	Bdd x1 = mMgr.make_posiliteral(vid_j);
	y |= x1;
      }
    }
    mNodeBddArray[id] = y;
  }

  // 外部出力のノード番号と極性
  RcfNodeHandle output = network.output();
  ymuint oid = output.id();
  bool oinv = output.inv();

  if ( oinv ) {
    oval = !oval;
  }

  ymuint nf = network.func_node_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const RcfNode* node = network.func_node(i);
    make_node_func(node);
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
GbmBddEngineOneHot::get_model(vector<Bool3>& model)
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
GbmBddEngineOneHot::get_conf_bits(const vector<Bool3>& model,
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
GbmBddEngineOneHot::get_iorder(const vector<Bool3>& model,
			       vector<ymuint>& iorder) const
{
  ymuint ni = iorder.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint base = i * ni;
    for (ymuint j = 0; j < ni; ++ j) {
      VarId vid = mIorderVarArray[base + j];
      if ( model[vid.val()] == kB3True ) {
	iorder[i] = j;
	break;
      }
    }
  }
}

// @brief 変数順を表すBDDを作る．
// @param[in] level レベル
// @param[in] var_list 変数の集合を表すビットベクタ
//
// var_list に含まれる変数の順列をすべて表す
// BDD を返す．
Bdd
GbmBddEngineOneHot::make_iorder_bdd(ymuint level,
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
	for (ymuint j = 0; j < mInputNum; ++ j) {
	  if ( j == i ) {
	    f1 &= mMgr.make_posiliteral(mIorderVarArray[level * mInputNum + j]);
	  }
	  else {
	    f1 &= ~mMgr.make_posiliteral(mIorderVarArray[level * mInputNum + j]);
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
GbmBddEngineOneHot::make_node_func(const RcfNode* node)
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
GbmBddEngineOneHot::make_LUT(const vector<Bdd>& inputs,
			     const vector<Bdd>& lut_vars)
{
  return make_MUX(lut_vars, inputs);
}

// @brief セレクタの出力の論理関数を計算する．
// @param[in] inputs ファンインの論理関数
// @param[in] s_vars 選択変数
Bdd
GbmBddEngineOneHot::make_MUX(const vector<Bdd>& inputs,
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
