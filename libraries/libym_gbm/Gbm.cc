
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
  mTvBase = 0;
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
  GbmNode* node = new GbmLut(id, vid, inputs, mTvBase, v_array);
  mTvBase += ni_exp;

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

// @brief 全ノード数を返す．
// @note ノードIDの最大値 + 1 と一致する．
ymuint
Gbm::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] id ID番号 ( 0 <= id < node_num() )
const GbmNode*
Gbm::node(ymuint id) const
{
  assert_cond( id < node_num(), __FILE__, __LINE__);
  return mNodeList[id];
}

// @brief 外部入力数を返す．
ymuint
Gbm::input_num() const
{
  return mInputList.size();
}

// @brief 外部入力ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const GbmNode*
Gbm::input_node(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

// @brief ANDノード数を返す．
ymuint
Gbm::and_num() const
{
  return mAndList.size();
}

// @brief ANDノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < and_num() )
const GbmNode*
Gbm::and_node(ymuint pos) const
{
  assert_cond( pos < and_num(), __FILE__, __LINE__);
  return mAndList[pos];
}

// @brief LUTノード数を返す．
ymuint
Gbm::lut_num() const
{
  return mLutList.size();
}

// @brief LUTノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < lut_num() )
const GbmNode*
Gbm::lut_node(ymuint pos) const
{
  assert_cond( pos < lut_num(), __FILE__, __LINE__);
  return mLutList[pos];
}

// @brief Boolean Matching を解く．
// @param[in] sat_var 1にする変数
// @param[out] model 真理値変数の割り当て結果
Bool3
Gbm::solve(GbmNodeHandle sat_var,
	   vector<Bool3>& model)
{
  ymuint nv = node_num() + mTvBase;
  ymuint nc = 0;
  for (vector<GbmNode*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    GbmNode* node = *p;
    if ( node->is_and() ) {
      nc += 3;
    }
    else if ( node->is_lut() ) {
      ymuint ni = node->fanin_num();
      nc += (1U << ni) * 2;
    }
  }
  nc += 2;

  cout << "p cnf " << nv << " " << nc << endl;

  cout << "e ";
  for (vector<GbmNode*>::iterator p = mLutList.begin();
       p != mLutList.end(); ++ p) {
    GbmNode* node = *p;
    ymuint base = node->tv_base() + node_num();
    ymuint ni = node->fanin_num();
    ymuint ni_exp = 1U << ni;
    for (ymuint i = 0; i < ni_exp; ++ i) {
      cout << " " << (base + i + 1);
    }
  }
  cout << " 0" << endl;

  cout << "a ";
  for (vector<GbmNode*>::iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    GbmNode* node = *p;
    cout << " " << (node->id() + 1);
  }
  cout << " 0" << endl;

  cout << "e ";
  for (vector<GbmNode*>::iterator p = mAndList.begin();
       p != mAndList.end(); ++ p) {
    GbmNode* node = *p;
    cout << " " << (node->id() + 1);
  }
  for (vector<GbmNode*>::iterator p = mLutList.begin();
       p != mLutList.end(); ++ p) {
    GbmNode* node = *p;
    cout << " " << (node->id() + 1);
  }
  cout << " 0" << endl;

  for (vector<GbmNode*>::iterator p = mAndList.begin();
       p != mAndList.end(); ++ p) {
    GbmNode* node = *p;
    int oid = node->id() + 1;

    GbmNodeHandle i0 = node->fanin(0);
    int iid0 = i0.id() + 1;
    if ( i0.inv() ) {
      iid0 = -iid0;
    }

    GbmNodeHandle i1 = node->fanin(1);
    int iid1 = i1.id() + 1;
    if ( i1.inv() ) {
      iid1 = -iid1;
    }

    cout << -iid0 << " " << -iid1 << " " << oid << " 0" << endl
	 <<  iid0 << " " << -oid << " 0" << endl
	 <<  iid1 << " " << -oid << " 0" << endl;
  }
  for (vector<GbmNode*>::iterator p = mLutList.begin();
       p != mLutList.end(); ++ p) {
    GbmNode* node = *p;
    int oid = node->id() + 1;
    ymuint ni = node->fanin_num();
    ymuint ni_exp = 1U << ni;
    vector<int> iid_array(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      GbmNodeHandle ihandle = node->fanin(i);
      int iid = ihandle.id() + 1;
      if ( ihandle.inv() ) {
	iid = -iid;
      }
      iid_array[i] = iid;
    }
    ymuint tv_base = node->tv_base();
    for (ymuint j = 0; j < ni_exp; ++ j) {
      for (ymuint i = 0; i < ni; ++ i) {
	int iid = iid_array[i];
	if ( (j & (1U << i)) == 0U ) {
	  iid = -iid;
	}
	cout << " " << -iid;
      }
      int tv = tv_base + j + node_num() + 1;
      cout << " " << tv << " " << -oid << " 0" << endl;

      for (ymuint i = 0; i < ni; ++ i) {
	int iid = iid_array[i];
	if ( (j & (1U << i)) == 0U ) {
	  iid = -iid;
	}
	cout << " " << -iid;
      }
      cout << " " << -tv << " " << oid << " 0" << endl;
    }
  }

  return kB3True;
}

END_NAMESPACE_YM_GBM
