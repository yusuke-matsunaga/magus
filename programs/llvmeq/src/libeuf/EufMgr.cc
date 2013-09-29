
/// @file EufMgr.cc
/// @brief EufMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufMgr.h"
#include "EufBinMgr.h"
#include "EufFuncMgr.h"
#include "EufVarMgr.h"
#include "EufNode.h"
#include "ym_utils/MFSet.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sat_type SAT-solver の種類を表す文字列
// @param[in] sat_opt SAT-solver に渡すオプション文字列
// @param[in] sat_log ログの出力用ストリーム
EufMgr::EufMgr(const string& sat_type,
	       const string& sat_opt,
	       ostream* sat_log) :
  mSolver(sat_type, sat_opt, sat_log)
{
  mVarMgr = new EufVarMgr;
  mFuncMgr = new EufFuncMgr;
  mBinMgr = new EufBinMgr;
  mLastId = 0;
}

// @brief デストラクタ
EufMgr::~EufMgr()
{
  delete mVarMgr;
  delete mFuncMgr;
  delete mBinMgr;
}

// @brief 変数を生成する．
// @param[in] name 変数名
// @param[in] bool_flag ブール変数の時 true にするフラグ
EufNode*
EufMgr::new_variable(const string& name,
		     bool bool_flag)
{
  EufNode* var = mVarMgr->find(name);
  if ( var == NULL ) {
    VarId vid;
    if ( bool_flag ) {
      vid = mSolver.new_var();
    }
    var = mVarMgr->new_variable(mLastId, vid, name, bool_flag);
    ++ mLastId;
  }
  return var;
}

BEGIN_NONAMESPACE

// EufNode を整列させるためのファンクタ
struct Lt
{
  bool
  operator()(EufNode* left,
	     EufNode* right)
  {
    return left->id() < right->id();
  }
};

END_NONAMESPACE

// @brief 関数を生成する．
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @param[in] sym_flag 対称関数の時 true にするフラグ
// @note sym_flag が true の時は arg_list の順番を正規化する．
EufNode*
EufMgr::new_function(const string& name,
		     const vector<EufNode*>& arg_list,
		     bool sym_flag)
{
  // ソートするかもしれないのでローカルな変数にコピーする．
  vector<EufNode*> tmp_list(arg_list.begin(), arg_list.end());

  if ( sym_flag ) {
    // ID番号でソートする．
    sort(tmp_list.begin(), tmp_list.end(), Lt());
  }

  EufNode* node = mFuncMgr->find(name, tmp_list);
  if ( node == NULL ) {
    node = mFuncMgr->new_function(mLastId, name, tmp_list);
    ++ mLastId;

    mFuncList.push_back(node);
  }

  return node;
}

// @brief 等価演算子を生成する．
// @param[in] left 左辺式
// @param[in] right 右辺式
// @note 場合によっては左辺式と右辺式は交換される．
EufNode*
EufMgr::new_equality(EufNode* left,
		     EufNode* right)
{
  assert_cond( left->is_boolean() == right->is_boolean(), __FILE__, __LINE__);

  // left と right は対称なので正規化する．
  if ( left->id() > right->id() ) {
    EufNode* tmp_node = left;
    left = right;
    right = tmp_node;
  }

  EufNode* node = mBinMgr->find_equality(left, right);
  if ( node == NULL ) {
    VarId vid = mSolver.new_var();
    node = mBinMgr->new_equality(mLastId, vid, left, right);
    ++ mLastId;

    mEqList.push_back(node);
  }
  return node;
}

// @brief 連言(conjunction)を生成する．
// @param[in] left 左辺式
// @param[in] right 右辺式
EufNode*
EufMgr::new_conjunction(EufNode* left,
			EufNode* right)
{
  assert_cond( left->is_boolean(), __FILE__, __LINE__);
  assert_cond( right->is_boolean(), __FILE__, __LINE__);

  // left と right は対称なので正規化する．
  if ( left->id() > right->id() ) {
    EufNode* tmp_node = left;
    left = right;
    right = tmp_node;
  }

  EufNode* node = mBinMgr->find_conjunction(left, right);
  if ( node == NULL ) {
    VarId vid = mSolver.new_var();
    node = mBinMgr->new_conjunction(mLastId, vid, left, right);
    ++ mLastId;

    Literal olit(vid, kPolPosi);
    Literal llit(left->var_id(), kPolPosi);
    Literal rlit(right->var_id(), kPolPosi);
    mSolver.add_clause(~llit, ~rlit, olit);
    mSolver.add_clause( llit, ~olit);
    mSolver.add_clause( rlit, ~olit);
  }
  return node;
}

// @brief 選言(disjunction)を生成する．
// @param[in] left 左辺式
// @param[in] right 右辺式
EufNode*
EufMgr::new_disjunction(EufNode* left,
			EufNode* right)
{
  assert_cond( left->is_boolean(), __FILE__, __LINE__);
  assert_cond( right->is_boolean(), __FILE__, __LINE__);

  // left と right は対称なので正規化する．
  if ( left->id() > right->id() ) {
    EufNode* tmp_node = left;
    left = right;
    right = tmp_node;
  }

  EufNode* node = mBinMgr->find_disjunction(left, right);
  if ( node == NULL ) {
    VarId vid = mSolver.new_var();
    node = mBinMgr->new_disjunction(mLastId, vid, left, right);
    ++ mLastId;

    Literal olit(vid, kPolPosi);
    Literal llit(left->var_id(), kPolPosi);
    Literal rlit(right->var_id(), kPolPosi);
    mSolver.add_clause( llit,  rlit, ~olit);
    mSolver.add_clause(~llit,  olit);
    mSolver.add_clause(~rlit,  olit);
  }
  return node;
}

// @brief 否定(negation)を生成する．
// @param[in] operand 式
EufNode*
EufMgr::new_negation(EufNode* operand)
{
  assert_cond( operand->is_boolean(), __FILE__, __LINE__);

  EufNode* node = mBinMgr->find_negation(operand);
  if ( node == NULL ) {
    VarId vid = mSolver.new_var();
    node = mBinMgr->new_negation(mLastId, vid, operand);
    ++ mLastId;

    Literal olit(vid, kPolPosi);
    Literal llit(operand->var_id(), kPolPosi);
    mSolver.add_clause( llit,  olit);
    mSolver.add_clause(~llit, ~olit);
  }
  return node;
}

// @brief validity check を行う．
// @param[in] node 対象のノード
// @note node->is_boolean() が true である必要がある．
bool
EufMgr::check_validity(EufNode* node)
{
  assert_cond( node->is_boolean(), __FILE__, __LINE__);

  Literal vlit(node->var_id(), kPolPosi);
  vector<Literal> assumption(1, ~vlit);
  vector<Bool3> model;

  for ( ; ; ) {
    Bool3 sat_stat = mSolver.solve(assumption, model);
    if ( sat_stat == kB3False ) {
      // vlit の否定が unsat ということは vlit は valid
      cout << "Valid" << endl;
      return true;
    }
    if ( sat_stat == kB3X ) {
      // アボート
      cout << "Aborted" << endl;
      return false;
    }

    // model の内容から成り立っている等価式を求める．
    vector<EufNode*> eq_list;
    vector<EufNode*> neq_list;
    for (vector<EufNode*>::iterator p = mEqList.begin();
	 p != mEqList.end(); ++ p) {
      EufNode* eq_node = *p;
      Bool3 val = model[eq_node->var_id().val()];
      if ( val == kB3True ) {
	eq_list.push_back(eq_node);
      }
      else if ( val == kB3False ) {
	neq_list.push_back(eq_node);
      }
    }

    // eq_list 内の等価式から congruence closure を求める．
    MFSet mfset(mLastId);
    for (vector<EufNode*>::iterator p = eq_list.begin();
	 p != eq_list.end(); ++ p) {
      EufNode* eq_node = *p;
      mfset.merge(eq_node->left()->id(), eq_node->right()->id());
    }

  }
}


END_NAMESPACE_YM_LLVMEQ
