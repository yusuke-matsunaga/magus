
/// @file libym_networks/verilog/ReaderImpl_stmt.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Env.h"
#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnNode.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlDeclArray.h"
#include "ym_verilog/vl/VlStmt.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_NETWORKS_VERILOG

using namespace nsYm::nsVerilog;

// @brief ステートメントの中身を生成する．
// @param[in] module 親のモジュール
// @param[in] stmt 本体のステートメント
// @param[in] env 環境
bool
ReaderImpl::gen_stmt1(MvnModule* module,
		      const VlStmt* stmt,
		      ProcEnv& env)
{
  switch ( stmt->type() ) {
  case kVpiAssignment:
    gen_assign(module, stmt, env);
    break;

  case kVpiBegin:
  case kVpiNamedBegin:
    {
      ymuint n = stmt->child_stmt_num();
      for (ymuint i = 0; i < n; ++ i) {
	const VlStmt* stmt1 = stmt->child_stmt(i);
	gen_stmt1(module, stmt1, env);
      }
    }
    break;

  case kVpiIf:
    {
      const VlExpr* cond = stmt->expr();
      MvnNode* cond_node = gen_expr(module, cond, env);
      ProcEnv then_env(env);
      gen_stmt1(module, stmt->body_stmt(), then_env);
      merge_env1(module, env, cond_node, then_env, env);
    }
    break;

  case kVpiIfElse:
    {
      const VlExpr* cond = stmt->expr();
      MvnNode* cond_node = gen_expr(module, cond, env);
      ProcEnv then_env(env);
      gen_stmt1(module, stmt->body_stmt(), then_env);
      ProcEnv else_env(env);
      gen_stmt1(module, stmt->else_stmt(), else_env);
      merge_env1(module, env, cond_node, then_env, else_env);
    }
    break;

  case kVpiCase:
#if 0
    {
      const VlExpr* cond = stmt->expr();
      MvnNode* cond_node = gen_expr(module, cond, env);

    }
#else
#warning "TODO: case 文"
#endif
    break;

  default:
#warning "エラーメッセージをまともにする．"
    cerr << "can not synthesized" << endl;
    return false;
  }

  return true;
}

// @brief ステートメントの中身を生成する．
// @param[in] module 親のモジュール
// @param[in] stmt 本体のステートメント
// @param[in] env 環境
bool
ReaderImpl::gen_stmt2(MvnModule* module,
		      const VlStmt* stmt,
		      ProcEnv& env)
{
  switch ( stmt->type() ) {
  case kVpiAssignment:
    gen_assign(module, stmt, env);
    break;

  case kVpiBegin:
  case kVpiNamedBegin:
    {
      ymuint n = stmt->child_stmt_num();
      for (ymuint i = 0; i < n; ++ i) {
	const VlStmt* stmt1 = stmt->child_stmt(i);
	gen_stmt2(module, stmt1, env);
      }
    }
    break;

  case kVpiIf:
    {
      const VlExpr* cond = stmt->expr();
      MvnNode* cond_node = gen_expr(module, cond, env);
      ProcEnv then_env(env);
      gen_stmt2(module, stmt->body_stmt(), then_env);
      merge_env2(module, env, cond_node, then_env, env);
    }
    break;

  case kVpiIfElse:
    {
      const VlExpr* cond = stmt->expr();
      MvnNode* cond_node = gen_expr(module, cond, env);
      ProcEnv then_env(env);
      gen_stmt2(module, stmt->body_stmt(), then_env);
      ProcEnv else_env(env);
      gen_stmt2(module, stmt->else_stmt(), else_env);
      merge_env2(module, env, cond_node, then_env, else_env);
    }
    break;

  case kVpiCase:
#if 0
    {
      const VlExpr* cond = stmt->expr();
      MvnNode* cond_node = gen_expr(module, cond, env);

    }
#else
#warning "TODO: case 文"
#endif
    break;

  default:
#warning "エラーメッセージをまともにする．"
    cerr << "can not synthesized" << endl;
    return false;
  }

  return true;
}

// @brief 代入文を生成する．
// @param[in] module 親のモジュール
// @param[in] stmt 本体のステートメント
// @param[in] env 環境
void
ReaderImpl::gen_assign(MvnModule* module,
		       const VlStmt* stmt,
		       ProcEnv& env)
{
  const VlExpr* rhs = stmt->rhs();
  const VlExpr* lhs = stmt->lhs();
  MvnNode* node = gen_expr(module, rhs, env);
  ymuint n = lhs->lhs_elem_num();
  ymuint offset = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const VlExpr* lhs1 = lhs->lhs_elem(i);
    const VlDecl* lhs_decl = lhs1->decl_obj();
    const VlDeclArray* lhs_declarray = lhs1->declarray_obj();
    const VlDeclBase* lhs_declbase = lhs1->decl_base();
    ymuint bw = lhs_declbase->bit_size();
    AssignInfo old_dst;
    ymuint lhs_offset = 0;
    if ( lhs_decl ) {
      old_dst = env.get_info(lhs_decl);
    }
    else if ( lhs_declarray ) {
      if ( lhs1->is_constant_select() ) {
	lhs_offset = lhs1->declarray_offset();
	old_dst = env.get_info(lhs_declarray, lhs_offset);
      }
      else {
	assert_cond( lhs1->declarray_dimension() == lhs_declarray->dimension(),
		     __FILE__, __LINE__ );
	// lhs_offset は lhs->declarray_index() から計算される．
#warning "TODO: 配列の可変インデックス時の処理"
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }

    MvnNode* dst_node = NULL;
    if ( lhs1->is_primary() ) {
      cout << "lhs1->is_primary()" << endl;
      MvnNode* src_node = gen_rhs(module, node, offset, bw);
      dst_node = mMvnMgr->new_through(module, bw);
      mMvnMgr->connect(src_node, 0, dst_node, 0);
    }
    else if ( lhs1->is_bitselect() ) {
      cout << "lhs1->is_bitselect()" << endl;
#if 0
      assert_cond( lhs1->is_constant_select(), __FILE__, __LINE__);
#warning "TODO: reg 型なら可変ビットセレクトもあり"
      MvnNode* src_node = gen_rhs(module, node, offset, 0);
      ymuint index = lhs_declbase->bit_offset(lhs1->index_val());
      vector<ymuint> bw_array;
      bw_array.reserve(3);
      if ( index < bw - 1 ) {
	bw_array.push_back(bw - index - 1);
      }
      bw_array.push_back(1);
      if ( index > 0 ) {
	bw_array.push_back(index);
      }
      dst_node = mMvnMgr->new_concat(module, bw_array);
      cout << "dst_node = mMvnMgr->new_concat" << endl;
      ymuint pos = 0;
      if ( index < bw - 1 ) {
	MvnNode* tmp_node = mMvnMgr->new_constpartselect(module,
							 bw - 1,
							 index + 1,
							 bw);
	mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	mMvnMgr->connect(tmp_node, 0, dst_node, pos);
	++ pos;
      }
      mMvnMgr->connect(src_node, 0, dst_node, pos);
      ++ pos;
      if ( index > 0 ) {
	MvnNode* tmp_node = mMvnMgr->new_constpartselect(module,
							 index - 1,
							 0,
							 bw);
	mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	mMvnMgr->connect(tmp_node, 0, dst_node, pos);
      }
#endif
    }
    else if ( lhs1->is_partselect() ) {
      cout << "lhs1->is_partselect()" << endl;
#if 0
      assert_cond( lhs1->is_constant_select(), __FILE__, __LINE__);
#warning "TODO: reg 型なら可変範囲セレクトもあり"
      ymuint msb = lhs_declbase->bit_offset(lhs1->left_range_val());
      ymuint lsb = lhs_declbase->bit_offset(lhs1->right_range_val());
      ymuint lbw = msb - lsb + 1;
      MvnNode* src_node = gen_rhs(module, node, offset, lbw);
      vector<ymuint> bw_array;
      bw_array.reserve(3);
      if ( msb < bw - 1 ) {
	bw_array.push_back(bw - msb - 1);
      }
      bw_array.push_back(msb - lsb + 1);
      if ( lsb > 0 ) {
	bw_array.push_back(lsb);
      }
      dst_node = mMvnMgr->new_concat(module, bw_array);
      ymuint pos = 0;
      if ( msb < bw - 1 ) {
	MvnNode* tmp_node = mMvnMgr->new_constpartselect(module,
							 bw - 1,
							 msb + 1,
							 bw);
	mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	mMvnMgr->connect(tmp_node, 0, dst_node, pos);
	++ pos;
      }
      mMvnMgr->connect(src_node, 0, dst_node, pos);
      ++ pos;
      if ( lsb > 0 ) {
	MvnNode* tmp_node = mMvnMgr->new_constpartselect(module,
							 lsb - 1,
							 0,
							 bw);
	mMvnMgr->connect(old_dst, 0, tmp_node, 0);
	mMvnMgr->connect(tmp_node, 0, dst_node, pos);
      }
#endif
    }
    assert_cond( dst_node, __FILE__, __LINE__);
    if ( lhs_decl ) {
      env.add(lhs_decl, dst_node, stmt->is_blocking());
    }
    else {
      env.add(lhs_declarray, lhs_offset, dst_node, stmt->is_blocking());
    }
    offset += lhs1->bit_size();
  }
}

// @brief 環境をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] env 対象の環境
// @param[in] cond 条件を表すノード
// @param[in] then_env 条件が成り立ったときに通るパスの環境
// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
void
ReaderImpl::merge_env1(MvnModule* parent_module,
		       ProcEnv& env,
		       MvnNode* cond,
		       const ProcEnv& then_env,
		       const ProcEnv& else_env)
{
  ymuint n = env.max_id();
  for (ymuint i = 0; i < n; ++ i) {
    AssignInfo info0 = env.get_from_id(i);
    MvnNode* node0 = info0.mRhs;
    MvnNode* cond0 = info0.mCond;
    AssignInfo info1 = then_env.get_from_id(i);
    MvnNode* node1 = info1.mRhs;
    MvnNode* cond1 = info1.mCond;
    AssignInfo info2 = else_env.get_from_id(i);
    MvnNode* node2 = info2.mRhs;
    MvnNode* cond2 = info2.mCond;

    MvnNode* new_node = NULL;
    MvnNode* new_cond = NULL;
    bool new_block = false;
    if ( node1 == node2 ) {
      if ( node1 == NULL ) {
	continue;
      }
      // 両方の結果が等しければ ITE を挿入しない．
      new_node = node1;
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
      // 条件は ITE になる．
      new_cond = merge_cond(parent_module, cond, cond1, cond2);
    }
    else if ( node1 == NULL ) {
      // node1 が NULL
      // cond1 も NULL
      assert_cond( node2 != NULL, __FILE__, __LINE__);
      new_node = node2;
      new_block = info2.mBlock;

      MvnNode* cond_bar = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(cond, 0, cond_bar, 0);
      if ( cond2 == NULL ) {
	// 代入条件は ~cond
	new_cond = cond_bar;
      }
      else {
	// 代入条件は ~cond & cond2
	new_cond = mMvnMgr->new_and(parent_module, 1);
	mMvnMgr->connect(cond_bar, 0, new_cond, 0);
	mMvnMgr->connect(cond2, 0, new_cond, 1);
      }
    }
    else if ( node2 == NULL ) {
      // node2 が NULL
      // cond2 も NULL
      assert_cond( node1 != NULL, __FILE__, __LINE__);
      new_node = node1;
      new_block = info1.mBlock;

      if ( cond1 == NULL ) {
	// 代入条件は cond
	new_cond = cond;
      }
      else {
	// 代入条件は cond & cond1
	new_cond = mMvnMgr->new_and(parent_module, 1);
	mMvnMgr->connect(cond, 0, new_cond, 0);
	mMvnMgr->connect(cond1, 0, new_cond, 1);
      }
    }
    else {
      // node1 も node2 も NULL ではない．
      ymuint bw = node1->output(0)->bit_width();
      new_node = mMvnMgr->new_ite(parent_module, bw);
      mMvnMgr->connect(cond, 0, new_node, 0);
      mMvnMgr->connect(node1, 0, new_node, 1);
      mMvnMgr->connect(node2, 0, new_node, 2);
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
      new_cond = merge_cond(parent_module, cond, cond1, cond2);
    }
    env.add_by_id(i, new_node, new_cond, new_block);
  }
}

// @brief 環境をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] env 対象の環境
// @param[in] cond 条件を表すノード
// @param[in] then_env 条件が成り立ったときに通るパスの環境
// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
void
ReaderImpl::merge_env2(MvnModule* parent_module,
		       ProcEnv& env,
		       MvnNode* cond,
		       const ProcEnv& then_env,
		       const ProcEnv& else_env)
{
  ymuint n = env.max_id();
  for (ymuint i = 0; i < n; ++ i) {
    AssignInfo info0 = env.get_from_id(i);
    MvnNode* node0 = info0.mRhs;
    assert_cond( info0.mCond == NULL, __FILE__, __LINE__);

    AssignInfo info1 = then_env.get_from_id(i);
    MvnNode* node1 = info1.mRhs;
    assert_cond( info1.mCond == NULL, __FILE__, __LINE__);

    AssignInfo info2 = else_env.get_from_id(i);
    MvnNode* node2 = info2.mRhs;
    assert_cond( info2.mCond == NULL, __FILE__, __LINE__);

    MvnNode* new_node = NULL;
    bool new_block = false;
    if ( node1 == node2 ) {
      if ( node1 == NULL ) {
	continue;
      }
      // 両方の結果が等しければ ITE を挿入しない．
      new_node = node1;
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
    }
    else if ( node1 == NULL ) {
      // node1 が NULL
      assert_cond( node0 != NULL, __FILE__, __LINE__);
      assert_cond( node2 != NULL, __FILE__, __LINE__);
      ymuint bw = node0->output(0)->bit_width();
      if ( node2->output(0)->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      new_node = mMvnMgr->new_ite(parent_module, bw);
      mMvnMgr->connect(cond, 0, new_node, 0);
      mMvnMgr->connect(node0, 0, new_node, 1);
      mMvnMgr->connect(node2, 0, new_node, 2);
      new_block = info2.mBlock;
    }
    else if ( node2 == NULL ) {
      // node2 が NULL
      assert_cond( node0 != NULL, __FILE__, __LINE__);
      assert_cond( node1 != NULL, __FILE__, __LINE__);
      ymuint bw = node0->output(0)->bit_width();
      if ( node1->output(0)->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      new_node = mMvnMgr->new_ite(parent_module, bw);
      mMvnMgr->connect(cond, 0, new_node, 0);
      mMvnMgr->connect(node1, 0, new_node, 1);
      mMvnMgr->connect(node0, 0, new_node, 2);
      new_block = info1.mBlock;
    }
    else {
      // node1 も node2 も NULL ではない．
      //assert_cond( node0 != NULL, __FILE__, __LINE__);
      ymuint bw = node1->output(0)->bit_width();
      if ( node1->output(0)->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      if ( node2->output(0)->bit_width() != bw ) {
	// ビット幅が異なる．
#warning "TODO: エラー処理"
      }
      new_node = mMvnMgr->new_ite(parent_module, bw);
      mMvnMgr->connect(cond, 0, new_node, 0);
      mMvnMgr->connect(node1, 0, new_node, 1);
      mMvnMgr->connect(node2, 0, new_node, 2);
      if ( info1.mBlock != info2.mBlock ) {
	// blocking 代入と non-blocking 代入の混在はエラー
#warning "TODO: エラー処理"
	continue;
      }
      new_block = info1.mBlock;
    }
    env.add_by_id(i, new_node, NULL, new_block);
  }
}

// @brief 代入条件をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] cond 切り替え条件
// @param[in] then_cond cond が成り立ったときの代入条件
// @param[in] else_cond cond が成り立たなかったときの代入条件
// @note 基本的には ITE(cond, then_cond, else_cond) だが，NULL の場合がある．
MvnNode*
ReaderImpl::merge_cond(MvnModule* parent_module,
		       MvnNode* cond,
		       MvnNode* then_cond,
		       MvnNode* else_cond)
{
  MvnNode* new_cond = NULL;
  if ( then_cond == else_cond ) {
    // 両方の条件が等しければ ITE を挿入しない
    new_cond = then_cond;
  }
  else if ( then_cond == NULL ) {
    // 代入条件は cond | else_cond
    new_cond = mMvnMgr->new_or(parent_module, 1);
    mMvnMgr->connect(cond, 0, new_cond, 0);
    mMvnMgr->connect(else_cond, 0, new_cond, 1);
  }
  else if ( else_cond == NULL ) {
    // 代入条件は ~cond | then_cond
    MvnNode* cond_bar = mMvnMgr->new_not(parent_module, 1);
    mMvnMgr->connect(cond, 0, cond_bar, 0);
    new_cond = mMvnMgr->new_or(parent_module, 1);
    mMvnMgr->connect(cond_bar, 0, new_cond, 0);
    mMvnMgr->connect(then_cond, 0, new_cond, 1);
  }
  else {
    new_cond = mMvnMgr->new_ite(parent_module, 1);
    mMvnMgr->connect(cond, 0, new_cond, 0);
    mMvnMgr->connect(then_cond, 0, new_cond, 1);
    mMvnMgr->connect(else_cond, 0, new_cond, 2);
  }
  return new_cond;
}

END_NAMESPACE_YM_NETWORKS_VERILOG
