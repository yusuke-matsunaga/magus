
/// @file Gbm.cc
/// @brief Gbm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gbm/Gbm.h"
#include "ym_gbm/GbmNode.h"
#include "GbmInput.h"
#include "GbmAnd.h"
#include "GbmLut.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
// クラス Gbm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SAT-solver の種類を表す文字列
// @param[in] sat_opt SAT-solver に渡すオプション文字列
// @param[in] sat_log ログの出力用ストリーム
Gbm::Gbm(const string& sat_type,
	 const string& sat_opt,
	 ostream* sat_log) :
  mSolver(sat_type, sat_opt, sat_log)
{
}

// @brief デストラクタ
Gbm::~Gbm()
{
}

// @brief 外部入力ノードを作る．
// @return 作成したノードのハンドルを返す．
GbmNodeHandle
Gbm::new_input()
{
  ymuint id = mNodeList.size();
  VarId vid = mSolver.new_var();
  GbmNode* node = new GbmInput(id, vid);

  mNodeList.push_back(node);
  mInputList.push_back(node);

  return GbmNodeHandle(id, false);
}

// @brief ANDゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
GbmNodeHandle
Gbm::new_and(GbmNodeHandle input0,
	     GbmNodeHandle input1)
{
  ymuint id = mNodeList.size();
  VarId vid = mSolver.new_var();
  GbmNode* node = new GbmAnd(id, vid, input0, input1);

  mNodeList.push_back(node);
  mAndList.push_back(node);

  // CNF 式を作る．
  GbmNode* node0 = mNodeList[input0.id()];
  VarId vid0 = node0->var_id();
  tPol pol0 = input0.inv() ? kPolNega : kPolPosi;
  Literal lit0(vid0, pol0);

  GbmNode* node1 = mNodeList[input1.id()];
  VarId vid1 = node1->var_id();
  tPol pol1 = input1.inv() ? kPolNega : kPolPosi;
  Literal lit1(vid1, pol1);

  Literal olit(vid, kPolPosi);
  mSolver.add_clause(~lit0, ~lit1, olit);
  mSolver.add_clause( lit0, ~olit);
  mSolver.add_clause( lit1, ~olit);

  return GbmNodeHandle(id, false);
}

// @brief LUTを作る．
// @param[in] inputs ファンインのハンドルのリスト
// @return 作成したノードのハンドルを返す．
GbmNodeHandle
Gbm::new_lut(const vector<GbmNodeHandle>& inputs)
{
  ymuint id = mNodeList.size();
  VarId vid = mSolver.new_var();
  ymuint ni = inputs.size();
  ymuint ni_exp = (1U << ni);
  vector<VarId> v_array(ni_exp);
  for (ymuint i = 0; i < ni_exp; ++ i) {
    v_array[i] = mSolver.new_var();
  }
  GbmNode* node = new GbmLut(id, vid, inputs, v_array);

  mNodeList.push_back(node);
  mLutList.push_back(node);

  // CNF 式を作る．
  Literal olit(vid, kPolPosi);
  for (ymuint p = 0U; p < ni_exp; ++ p) {
    vector<Literal> lits(ni + 2);
    for (ymuint i = 0; i < ni; ++ i) {
      GbmNode* inode = mNodeList[inputs[i].id()];
      VarId ivid = inode->var_id();
      tPol ipol1 = inputs[i].inv() ? kPolNega : kPolPosi;
      tPol ipol2 = ((p & (1U << i)) == 0U) ? kPolNega : kPolPosi;
      lits[i] = Literal(ivid, ipol1 * ipol2 * kPolNega);
    }
    lits[ni] = Literal(v_array[p], kPolNega);
    lits[ni + 1] = olit;
    mSolver.add_clause(lits);
    lits[ni] = Literal(v_array[p], kPolPosi);
    lits[ni + 1] = ~olit;
    mSolver.add_clause(lits);
  }

  return GbmNodeHandle(id, false);
}

END_NAMESPACE_YM_GBM
