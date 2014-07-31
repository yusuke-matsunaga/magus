
/// @file GbmBddEngineEnum.cc
/// @brief GbmBddEngineEnum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmBddEngineEnum.h"
#include "YmLogic/BddLitSet.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmBddEngineEnum
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr BddMgr
GbmBddEngineEnum::GbmBddEngineEnum(BddMgr& mgr) :
  mMgr(mgr),
  mDebug(false)
{
}

// @brief デストラクタ
GbmBddEngineEnum::~GbmBddEngineEnum()
{
}

// @brief debug フラグを立てる
void
GbmBddEngineEnum::debug_on()
{
  mDebug = true;
}

// @brief debug フラグを降ろす
void
GbmBddEngineEnum::debug_off()
{
  mDebug = false;
}

// @brief debug フラグの値を得る．
bool
GbmBddEngineEnum::debug() const
{
  return mDebug;
}

// @brief 対称性を考慮して初期解を作る．
// @param[in] rep 関数の対称変数の代表番号を収める配列
void
GbmBddEngineEnum::init_vars(const RcfNetwork& network)
{
  ymuint nc = network.conf_var_num();
  mConfVarArray.clear();
  mConfVarArray.resize(nc);

  mInputNum = network.input_num();

  // configuration bit 用の変数を確保する．
  for (ymuint i = 0; i < mConfVarArray.size(); ++ i) {
    VarId vid(i);
    mConfVarArray[i] = vid;
    bool stat = mMgr.new_var(vid);
    assert_cond( stat, __FILE__, __LINE__);
  }

  mSolution = mMgr.make_one();
}

// @brief 入力値を割り当てて解の候補を求める．
// @param[in] network 対象の LUT ネットワーク
// @param[in] bit_pat 外部入力の割り当てを表すビットパタン
// @param[in] oid 出力のノード番号
// @param[in] oval 出力の値
// @return 結果が空でなければ true を返し，model にその1つを収める．
bool
GbmBddEngineEnum::make_bdd(const RcfNetwork& network,
			   ymuint bitpat,
			   const vector<ymuint>& iorder,
			   bool oval)
{
  ymuint nn = network.node_num();
  mNodeBddArray.clear();
  mNodeBddArray.resize(nn);

  for (ymuint i = 0; i < mInputNum; ++ i) {
    const RcfNode* node = network.input_node(i);
    ymuint id = node->id();
    ymuint src_pos = iorder[i];
    Bdd ifunc;
    if ( bitpat & (1U << src_pos) ) {
      ifunc = mMgr.make_one();
    }
    else {
      ifunc = mMgr.make_zero();
    }
    mNodeBddArray[id] = ifunc;
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

  assert_cond( !mSolution.is_invalid(), __FILE__, __LINE__);

  if ( mSolution.is_zero() ) {
    return false;
  }
  return true;
}

// @brief 結果からモデルを一つ取り出す．
void
GbmBddEngineEnum::get_model(vector<Bool3>& model)
{
  if ( mSolution.is_zero() ) {
    return;
  }

  ymuint nall = mConfVarArray.size();
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
GbmBddEngineEnum::get_conf_bits(const vector<Bool3>& model,
			    vector<bool>& conf_bits) const
{
  ymuint nc = mConfVarArray.size();
  for (ymuint i = 0; i < nc; ++ i) {
    VarId vid = mConfVarArray[i];
    if ( model[vid.val()] == kB3True ) {
      conf_bits[i] = true;
    }
    else {
      conf_bits[i] = false;
    }
  }
}

// @brief RcfNode に対応する関数を計算する．
// @param[in] node 対象のノード
// @note 結果は mNodeBddArray に格納される．
void
GbmBddEngineEnum::make_node_func(const RcfNode* node)
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
    assert_cond( !f.is_invalid(), __FILE__, __LINE__);
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

  assert_cond( !output.is_invalid(), __FILE__, __LINE__);
  mNodeBddArray[node->id()] = output;
}

// @brief LUT ノードの出力の論理関数を計算する．
// @param[in] inputs ファンインの論理関数
// @param[in] lut_vars LUT のコンフィグレーションメモリ
Bdd
GbmBddEngineEnum::make_LUT(const vector<Bdd>& inputs,
		       const vector<Bdd>& lut_vars)
{
  return make_MUX(lut_vars, inputs);
}

// @brief セレクタの出力の論理関数を計算する．
// @param[in] inputs ファンインの論理関数
// @param[in] s_vars 選択変数
Bdd
GbmBddEngineEnum::make_MUX(const vector<Bdd>& inputs,
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
