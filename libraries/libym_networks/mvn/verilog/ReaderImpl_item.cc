
/// @file libym_networks/verilog/ReaderImpl.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Driver.h"
#include "Env.h"
#include "AsyncControl.h"
#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnNode.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlUdp.h"
#include "ym_verilog/vl/VlPort.h"
#include "ym_verilog/vl/VlContAssign.h"
#include "ym_verilog/vl/VlProcess.h"
#include "ym_verilog/vl/VlStmt.h"
#include "ym_verilog/vl/VlControl.h"
#include "ym_verilog/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using namespace nsYm::nsVerilog;

// @brief 要素を生成する．
// @param[in] module モジュール
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::gen_item(MvnModule* module,
		     const VlNamedObj* vl_scope)
{
  // モジュールインスタンスの生成
  {
    vector<const VlModule*> module_list;
    if ( mVlMgr.find_module_list(vl_scope, module_list) ) {
      for (vector<const VlModule*>::iterator p = module_list.begin();
	   p != module_list.end(); ++ p) {
	const VlModule* vl_module = *p;
	gen_moduleinst(module, vl_module);
      }
    }
  }

  // モジュール配列インスタンスの生成
  {
    vector<const VlModuleArray*> modulearray_list;
    if ( mVlMgr.find_modulearray_list(vl_scope, modulearray_list) ) {
      for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	   p != modulearray_list.end(); ++ p) {
	const VlModuleArray* vl_module_array = *p;
	ymuint n = vl_module_array->elem_num();
	for (ymuint i = 0; i < n; ++ i) {
	  const VlModule* vl_module = vl_module_array->elem_by_offset(i);
	  gen_moduleinst(module, vl_module);
	}
      }
    }
  }

  // プリミティブインスタンスの生成
  {
    vector<const VlPrimitive*> primitive_list;
    if ( mVlMgr.find_primitive_list(vl_scope, primitive_list) ) {
      for (vector<const VlPrimitive*>::iterator p = primitive_list.begin();
	   p != primitive_list.end(); ++ p) {
	const VlPrimitive* prim = *p;
	gen_priminst(module, prim);
      }
    }
  }

  // プリミティブ配列インスタンスの生成
  {
    vector<const VlPrimArray*> primarray_list;
    if ( mVlMgr.find_primarray_list(vl_scope, primarray_list) ) {
      for (vector<const VlPrimArray*>::iterator p = primarray_list.begin();
	   p != primarray_list.end(); ++ p) {
	const VlPrimArray* vl_primarray = *p;
	ymuint n = vl_primarray->elem_num();
	for (ymuint i = 0; i < n; ++ i) {
	  const VlPrimitive* prim = vl_primarray->elem_by_offset(i);
	  gen_priminst(module, prim);
	}
      }
    }
  }

  // 継続的代入文の生成
  {
    vector<const VlContAssign*> contassign_list;
    if ( mVlMgr.find_contassign_list(vl_scope, contassign_list) ) {
      for (vector<const VlContAssign*>::iterator p = contassign_list.begin();
	   p != contassign_list.end(); ++ p) {
	const VlContAssign* cont_assign = *p;
	gen_cont_assign(module, cont_assign->lhs(), cont_assign->rhs());
      }
    }
  }

  // プロセスの生成
  {
    vector<const VlProcess*> process_list;
    if ( mVlMgr.find_process_list(vl_scope, process_list) ) {
      for (vector<const VlProcess*>::const_iterator p = process_list.begin();
	   p != process_list.end(); ++ p) {
	const VlProcess* process = *p;
	bool stat1 = gen_process(module, process);
	if ( !stat1 ) {
	  return NULL;
	}
      }
    }
  }

  // 内部スコープ要素の生成
  {
    vector<const VlNamedObj*> scope_list;
    if ( mVlMgr.find_internalscope_list(vl_scope, scope_list) ) {
      for (vector<const VlNamedObj*>::iterator p = scope_list.begin();
	   p != scope_list.end(); ++ p) {
	const VlNamedObj* vl_scope1 = *p;
	bool stat = gen_item(module, vl_scope1);
	if ( !stat ) {
	  return false;
	}
      }
    }
  }

  return true;
}

// @brief プロセス文を生成する．
// @param[in] vl_process プロセス文
bool
ReaderImpl::gen_process(MvnModule* parent_module,
			const VlProcess* process)
{
  if ( process->type() != kVpiAlways ) {
    // always 文以外(initial文)はダメ
    cerr << "initial should not be used." << endl;
    return false;
  }

  const VlStmt* stmt = process->stmt();
  if ( stmt->type() != kVpiEventControl ) {
    // always の直後は '@' でなければダメ
    cerr << "only '@' is allowed here." << endl;
    return false;
  }

  // イベントリストをスキャンして edge 記述の有無を調べる．
  bool has_edge_event = false;
  bool has_normal_event = false;
  const VlControl* control = stmt->control();
  ymuint ev_num = control->event_num();
  for (ymuint i = 0; i < ev_num; ++ i) {
    const VlExpr* expr = control->event(i);
    if ( expr->type() == kVpiOperation ) {
      if ( expr->op_type() == kVpiPosedgeOp ||
	   expr->op_type() == kVpiNegedgeOp ) {
	has_edge_event = true;
      }
      else {
	cerr << "only edge descriptor should be used." << endl;
	return false;
      }
    }
    else if ( expr->decl_base() != NULL ) {
      has_normal_event = true;
    }
    else {
      cerr << "illegal type" << endl;
      return false;
    }
  }

  if ( has_edge_event ) {
    // edge 記述があったらすべて edge 記述でなければならない．
    if ( has_normal_event ) {
      cerr << "edge-type events and normal events are "
	   << "mutual exclusive." << endl;
      return false;
    }

    // イベントリストの解析を行う．
    vector<pair<MvnNode*, ymuint32> > event_node_array(ev_num);
    for (ymuint i = 0; i < ev_num; ++ i) {
      const VlExpr* expr = control->event(i);
      const VlExpr* opr1 = expr->operand(0);
      MvnNode* node1 = gen_primary(opr1, mGlobalEnv);
      ymuint pol = (expr->op_type() == kVpiPosedgeOp) ? 1 : 0;
      event_node_array[i] = make_pair(node1, pol);
    }
    vector<bool> event_map(ev_num, false);
    vector<AsyncControl*> event_list;
    event_list.reserve(ev_num);
    const VlStmt* stmt1 = stmt->body_stmt();
    while ( stmt1 != NULL ) {
      if ( (stmt1->type() == kVpiNamedBegin || stmt1->type() == kVpiBegin) &&
	   stmt1->child_stmt_num() == 1 ) {
	stmt1 = stmt1->child_stmt(0);
      }
      if ( stmt1->type() != kVpiIf && stmt1->type() != kVpiIfElse ) {
	break;
      }
      const VlExpr* cond = stmt1->expr();
      ymuint pol = 1;
      MvnNode* node = NULL;
      if ( cond->type() == kVpiOperation &&
	   (cond->op_type() == kVpiNotOp  ||
	    cond->op_type() == kVpiBitNegOp) ) {
	pol = 0;
	node = gen_primary(cond->operand(0), mGlobalEnv);
      }
      else {
	node = gen_primary(cond, mGlobalEnv);
      }
      assert_cond( node != NULL, __FILE__, __LINE__);

      bool found = false;
      for (ymuint i = 0; i < ev_num; ++ i) {
	MvnNode* node1 = event_node_array[i].first;
	if ( node == node1 ) {
	  if ( pol != event_node_array[i].second ) {
	    cerr << "Polarity mismatch." << endl;
	    return false;
	  }
	  found = true;
	  AsyncControl* ctrl = new AsyncControl(mGlobalEnv);
	  ctrl->mNode = node;
	  ctrl->mPol = pol;
	  gen_stmt2(parent_module, stmt1->body_stmt(), ctrl->mEnv);
	  event_list.push_back(ctrl);
	  event_map[i] = true;
	  break;
	}
      }
      assert_cond( found , __FILE__, __LINE__);
      stmt1 = stmt1->else_stmt();
    }
    if ( event_list.size() != ev_num - 1 ) {
      cerr << "Too few if branch against the event list" << endl;
      return false;
    }

    // クロック信号を調べる．
    MvnNode* clock_node = NULL;
    ymuint clock_pol = 0;
    for (ymuint i = 0; i < ev_num; ++ i) {
      if ( !event_map[i] ) {
	clock_node = event_node_array[i].first;
	clock_pol = event_node_array[i].second;
	break;
      }
    }
    assert_cond( clock_node != NULL, __FILE__, __LINE__);

    ProcEnv top_env(mGlobalEnv);
    gen_stmt2(parent_module, stmt1, top_env);

    ymuint n = mGlobalEnv.max_id();
    for (ymuint i = 0; i < n; ++ i) {
      AssignInfo info1 = top_env.get_from_id(i);
      MvnNode* rhs = info1.mRhs;
      if ( rhs == NULL ) {
	continue;
      }
      assert_cond( info1.mCond == NULL, __FILE__, __LINE__);
      MvnNode* node0 = mGlobalEnv.get_from_id(i);
      // FF を挿入
      // このノードに関係しているコントロールを調べる．
      vector<ymuint32> pol_array;
      vector<MvnNode*> control_array;
      vector<MvnNode*> rhs_array;
      pol_array.reserve(ev_num);
      control_array.reserve(ev_num);
      rhs_array.reserve(ev_num);
      for (ymuint j = 0; j < ev_num - 1; ++ j) {
	AsyncControl* control = event_list[j];
	MvnNode* rhs1 = control->mEnv.get_from_id(i).mRhs;
	if ( rhs1 ) {
	  pol_array.push_back(control->mPol);
	  control_array.push_back(control->mNode);
	  rhs_array.push_back(rhs1);
	}
      }
      ymuint n = pol_array.size();
      ymuint bw = node0->output(0)->bit_width();
      MvnNode* ff = mMvnMgr->new_dff(parent_module, clock_pol, pol_array, bw);
      mMvnMgr->connect(rhs, 0, ff, 0);
      mMvnMgr->connect(clock_node, 0, ff, 1);
      for (ymuint j = 0; j < n; ++ j) {
	mMvnMgr->connect(control_array[j], 0, ff, (j * 2) + 2);
	mMvnMgr->connect(rhs_array[j], 0, ff, (j * 2) + 3);
      }
      mMvnMgr->connect(ff, 0, node0, 0);
    }
  }
  else {
    ProcEnv top_env(mGlobalEnv);
    gen_stmt1(parent_module, stmt->body_stmt(), top_env);

    ymuint n = mGlobalEnv.max_id();
    for (ymuint i = 0; i < n; ++ i) {
      AssignInfo info1 = top_env.get_from_id(i);
      MvnNode* rhs = info1.mRhs;
      MvnNode* cond = info1.mCond;
      if ( rhs == NULL ) {
	continue;
      }
      MvnNode* node0 = mGlobalEnv.get_from_id(i);
      if ( cond == NULL ) {
	// 単純な組み合わせ論理
	mMvnMgr->connect(rhs, 0, node0, 0);
      }
      else {
	// latch を挿入
	ymuint bw = node0->output(0)->bit_width();
	MvnNode* latch = mMvnMgr->new_latch(parent_module, bw);
	mMvnMgr->connect(rhs, 0, latch, 0);
	mMvnMgr->connect(cond, 0, latch, 1);
	mMvnMgr->connect(latch, 0, node0, 0);
      }
    }
  }

  return true;
}

// @brief モジュールインスタンスの生成を行う．
// @param[in] parent_module 親のモジュール
// @param[in] vl_module モジュール
// @return 対応するノードを返す．
void
ReaderImpl::gen_moduleinst(MvnModule* parent_module,
			   const VlModule* vl_module)
{
  // 宣言要素を生成する．
  bool stat = gen_decl(parent_module, vl_module);
  if ( !stat ) {
    return;
  }

  // 要素を生成する．
  stat = gen_item(parent_module, vl_module);
  if ( !stat ) {
    return;
  }

  // ポートの接続を行う．
  ymuint np = vl_module->port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const VlPort* vl_port = vl_module->port(i);
    const VlExpr* hi = vl_port->high_conn();
    if ( hi == NULL ) continue;
    const VlExpr* lo = vl_port->low_conn();
    switch ( vl_port->direction() ) {
    case kVpiInput:
      // hi は右辺式
      // lo は左辺式
      gen_cont_assign(parent_module, lo, hi);
      break;

    case kVpiOutput:
      // hi は左辺式
      // lo は右辺式
      gen_cont_assign(parent_module, hi, lo);
      break;

    case kVpiInout:
      // hi は単純な参照か連結のみ
      break;

    case kVpiMixedIO:
      // hi は単純な参照か連結のみ
      //connect_port2(port, hi);
      // TODO: connect_port2 を作る
      assert_not_reached(__FILE__, __LINE__);
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
}

// @brief プリミティブインスタンスの生成を行う．
// @param[in] parent_module 親のモジュール
// @param[in] prim プリミティブ
void
ReaderImpl::gen_priminst(MvnModule* parent_module,
			 const VlPrimitive* prim)
{
  ymuint nt = prim->port_num();
  ymuint ni = 0;
  ymuint no = 0;
  if ( prim->prim_type() == kVpiBufPrim ) {
    ni = 1;
    no = nt - 1;
  }
  else {
    ni = nt - 1;
    no = 1;
  }

  vector<pair<MvnNode*, ymuint> > inputs(ni);
  vector<MvnNode*> outputs(no);

  switch ( prim->prim_type() ) {
  case kVpiBufPrim:
    {
      MvnNode* node = mMvnMgr->new_through(parent_module, 1);
      inputs[0] = make_pair(node, 0);
      for (ymuint i = 0; i < no; ++ i) {
	outputs[i] = node;
      }
    }
    break;

  case kVpiNotPrim:
    {
      MvnNode* node = mMvnMgr->new_not(parent_module, 1);
      inputs[0] = make_pair(node, 0);
      outputs[0] = node;
    }
    break;

  case kVpiAndPrim:
    {
      MvnNode* node = mMvnMgr->new_and(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiNandPrim:
    {
      MvnNode* node = mMvnMgr->new_and(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiOrPrim:
    {
      MvnNode* node = mMvnMgr->new_or(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiNorPrim:
    {
      MvnNode* node = mMvnMgr->new_or(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiXorPrim:
    {
      MvnNode* node = mMvnMgr->new_xor(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiXnorPrim:
    {
      MvnNode* node = mMvnMgr->new_xor(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiCombPrim:
    {
#warning "TODO: エラーメッセージを出力する．"
      assert_not_reached(__FILE__, __LINE__);
    }
    break;

  case kVpiSeqPrim:
    {
#warning "TODO: エラーメッセージを出力する．"
      assert_not_reached(__FILE__, __LINE__);
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  ymuint pos = 0;
  for (ymuint i = 0; i < no; ++ i) {
    const VlPrimTerm* term = prim->prim_term(pos);
    ++ pos;
    const VlExpr* expr = term->expr();
    MvnNode* dst_node = gen_primary(expr, mGlobalEnv);
    connect_lhs(dst_node, expr, outputs[i]);
  }
  for (ymuint i = 0; i < ni; ++ i) {
    const VlPrimTerm* term = prim->prim_term(pos);
    ++ pos;
    const VlExpr* expr = term->expr();
    MvnNode* node = gen_expr(parent_module, expr, mGlobalEnv);
    const pair<MvnNode*, ymuint>& p = inputs[i];
    mMvnMgr->connect(node, 0, p.first, p.second);
  }
}

// @brief 継続的代入文の生成を行う．
// @param[in] parent_module 親のモジュール
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
void
ReaderImpl::gen_cont_assign(MvnModule* parent_module,
			    const VlExpr* lhs,
			    const VlExpr* rhs)
{
  MvnNode* node = gen_expr(parent_module, rhs, mGlobalEnv);
  ymuint n = lhs->lhs_elem_num();
  ymuint offset = 0;
  for (ymuint i = 0; i < n; i ++ ) {
    const VlExpr* lhs_elem = lhs->lhs_elem(i);
    MvnNode* dst_node = gen_primary(lhs_elem, mGlobalEnv);
    ymuint dst_bw = lhs_elem->bit_size();
    MvnNode* src_node = gen_rhs(parent_module, node, offset, dst_bw);
    connect_lhs(dst_node, lhs_elem, src_node);
    offset += dst_bw;
  }
}

END_NAMESPACE_YM_MVN_VERILOG
