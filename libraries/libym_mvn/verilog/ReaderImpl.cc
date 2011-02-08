
/// @file libym_mvn/verilog/ReaderImpl.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Driver.h"
#include "Env.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlUdp.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlDeclArray.h"
#include "ym_verilog/vl/VlPort.h"
#include "ym_verilog/vl/VlContAssign.h"
#include "ym_verilog/vl/VlProcess.h"
#include "ym_verilog/vl/VlStmt.h"
#include "ym_verilog/vl/VlControl.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using namespace nsYm::nsVerilog;


BEGIN_NONAMESPACE

const
bool debug_driver = true;

END_NONAMESPACE


// @brief コンストラクタ
ReaderImpl::ReaderImpl() :
  mVlMgr(mMsgMgr),
  mGlobalEnv(mDeclHash)
{
  mMvMgr = NULL;
}

// @brief デストラクタ
ReaderImpl::~ReaderImpl()
{
}

// @brief 内部のデータをクリアする．
void
ReaderImpl::clear()
{
  mVlMgr.clear();
}

// @brief verilog 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウォッチャーのリスト
// @retval true 正常に読み込めた．
// @retval false 読込中にエラーが起こった．
bool
ReaderImpl::read(const string& filename,
		 const SearchPathList& searchpath,
		 const list<VlLineWatcher*> watcher_list)
{
  return mVlMgr.read_file(filename, searchpath, watcher_list);
}

// @brief 今まで読み込んだ情報からネットワークを生成する．
// @param[in] mgr ネットワーク生成用のマネージャ
// @retval true 正常に処理を行った．
// @retval false 生成中にエラーが起こった．
bool
ReaderImpl::gen_network(MvMgr& mgr,
			MvVlMap& node_map)
{
  if ( mMsgMgr.error_num() > 0 ) {
    return false;
  }

  mVlMgr.elaborate();

  if ( mMsgMgr.error_num() > 0 ) {
    return false;
  }

  mMvMgr = &mgr;

  mIODeclMap.clear();
  mDeclHash.clear();
  mGlobalEnv.clear();
  mNodeMap.clear();
  mDriverList.clear();

  MvModule* module0 = NULL;
  list<const VlModule*> tmp_list(mVlMgr.topmodule_list());
  for (list<const VlModule*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const VlModule* vl_module = *p;

    if ( vl_module->is_cell_instance() ) continue;

    // module を実体化
    MvModule* module = gen_module(vl_module);
    if ( module == NULL ) {
      return false;
    }
    if ( module0 == NULL ) {
      module0 = module;
    }
    else {
      cerr << "more than one top modules" << endl;
    }
  }

  // 結線を行う．
  ymuint n = mMvMgr->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = mMvMgr->_node(i);
    if ( node == NULL ) continue;
    const vector<Driver>& dlist = driver_list(node);
    if ( dlist.empty() ) continue;
    ymuint bw = node->input(0)->bit_width();
    vector<Driver> tmp(bw);
    for (vector<Driver>::const_iterator p = dlist.begin();
	 p != dlist.end(); ++ p) {
      const Driver& driver = *p;
      if ( driver.is_simple() ) {
	for (ymuint i = 0; i < bw; ++ i) {
	  if ( tmp[i].rhs_node() != NULL ) {
	    error_drivers(node, tmp[i], driver);
	  }
	  tmp[i] = driver;
	}
      }
      else if ( driver.has_bitselect() ) {
	ymuint index = driver.index();
	if ( tmp[index].rhs_node() != NULL ) {
	  error_drivers(node, tmp[index], driver);
	}
	tmp[index] = driver;
      }
      else {
	ymuint msb = driver.msb();
	ymuint lsb = driver.lsb();
	for (ymuint i = lsb; i <= msb; ++ i) {
	  if ( tmp[i].rhs_node() != NULL ) {
	    error_drivers(node, tmp[i], driver);
	  }
	  tmp[i] = driver;
	}
      }
    }

    // 明示的なドライバがない場合の処理
    for (ymuint i = 0; i < bw; ++ i) {
      if ( tmp[i].rhs_node() == NULL ) {
#warning "TODO: warning メッセージを出すようにする．"
	vector<ymuint32> val(1, 0);
	MvNode* ud_node = mMvMgr->new_const(module0, 1, val);
	if ( bw == 1 ) {
	  tmp[i] = Driver(ud_node);
	}
	else {
	  tmp[i] = Driver(ud_node, i);
	}
      }
    }

    Driver prev;
    vector<Driver> tmp2;
    tmp2.reserve(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      ymuint idx = bw - i - 1;
      const Driver& driver = tmp[idx];
      if ( driver != prev ) {
	tmp2.push_back(driver);
	prev = driver;
	assert_cond( driver.rhs_node() != NULL, __FILE__, __LINE__);
      }
    }

    ymuint nd = tmp2.size();
    if ( nd == 1 ) {
      const Driver& driver = tmp2[0];
      MvNode* src_node = driver.rhs_node();
      mMvMgr->connect(src_node, 0, node, 0);
    }
    else {
      vector<ymuint> bw_array(nd);
      for (ymuint i = 0; i < nd; ++ i) {
	const Driver& driver = tmp2[i];
	assert_cond( driver.rhs_node() != NULL, __FILE__, __LINE__);
	if ( driver.has_bitselect() ) {
	  bw_array[i] = 1;
	}
	else if ( driver.has_partselect() ) {
	  bw_array[i] = driver.msb() - driver.lsb() + 1;
	}
	else {
	  assert_not_reached(__FILE__, __LINE__);
	}
      }
      MvNode* node1 = mMvMgr->new_concat(module0, bw_array);
      mMvMgr->connect(node1, 0, node, 0);
      for (ymuint i = 0; i < nd; ++ i) {
	const Driver& driver = tmp2[i];
	mMvMgr->connect(driver.rhs_node(), 0, node1, i);
      }
    }
  }

  // 冗長な through ノードを取り除く
  {
    vector<MvNode*> node_list;
    node_list.reserve(n);
    for (ymuint i = 0; i < n; ++ i) {
      MvNode* node = mMvMgr->_node(i);
      if ( node == NULL ) continue;
      if ( node->type() != MvNode::kThrough ) continue;
      node_list.push_back(node);
    }
    for (vector<MvNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      MvNode* node = *p;

      const MvInputPin* ipin = node->input(0);
      const MvOutputPin* src_pin = ipin->src_pin();
      if ( src_pin == NULL ) continue;

      MvNode* src_node = src_pin->node();
      ymuint src_pos = src_pin->pos();
      mMvMgr->disconnect(src_node, src_pos, node, 0);
      mMvMgr->reconnect(node, 0, src_node, src_pos);
      mNodeMap.copy(src_node->id(), node->id());
      mMvMgr->delete_node(node);
    }
  }

  // 冗長な through ノードを削除する．
  mMvMgr->sweep();

  node_map = mNodeMap;

  return true;
}

// @brief メッセージハンドラを付加する．
void
ReaderImpl::add_msg_handler(MsgHandler* msg_handler)
{
  mMsgMgr.reg_handler(msg_handler);
}

// @brief module を生成する．
// @param[in] vl_module 対象のモジュール
MvModule*
ReaderImpl::gen_module(const VlModule* vl_module)
{
  // ポート数，入出力のビット幅を調べる．
  ymuint np = vl_module->port_num();
  ymuint nio_all = vl_module->io_num();
  ymuint ni = 0;
  ymuint no = 0;
  ymuint nio = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ++ ni; break;
    case kVpiOutput: ++ no; break;
    case kVpiInout:  ++ nio; break;
    default:
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      io->file_region(),
		      kMsgError,
		      "MVN_VL01",
		      "Only Input/Output/Inout types are supported");
      return NULL;
    }
  }
  vector<ymuint> ibw_array(ni);
  vector<ymuint> obw_array(no);
  vector<ymuint> iobw_array(nio);
  ni = 0;
  no = 0;
  nio = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ibw_array[ni] = io->bit_size(); ++ ni; break;
    case kVpiOutput: obw_array[no] = io->bit_size(); ++ no; break;
    case kVpiInout:  iobw_array[nio] = io->bit_size(); ++ nio; break;
    default: break;
    }
  }

  MvModule* module = mMvMgr->new_module(vl_module->name(), np,
					ibw_array, obw_array, iobw_array);

  // 宣言要素を生成する．
  bool stat = gen_decl(module, vl_module);
  if ( !stat ) {
    return NULL;
  }

  // 要素を生成する．
  stat = gen_item(module, vl_module);
  if ( !stat ) {
    return NULL;
  }

  // 入出力ノードの対応表を作る．
  ymuint i1 = 0;
  ymuint i2 = 0;
  ymuint i3 = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    const VlDecl* decl = io->decl();
    MvNode* node = mGlobalEnv.get(decl);
    assert_cond( node != NULL, __FILE__, __LINE__);
    switch ( io->direction() ) {
    case kVpiInput:
      mMvMgr->connect(module->input(i1), 0, node, 0);
      reg_ionode(decl, module->input(i1));
      ++ i1;
      break;

    case kVpiOutput:
      mMvMgr->connect(node, 0, module->output(i2), 0);
      reg_ionode(decl, module->output(i2));
      ++ i2;
      break;

    case kVpiInout:
      mMvMgr->connect(module->inout(i3), 0, node, 0);
      reg_ionode(decl, module->inout(i3));
      ++ i3;
      break;

    default:
      break;
    }
  }

  // ポートの接続を行う．
  for (ymuint i = 0; i < np; ++ i) {
    const VlPort* port = vl_module->port(i);
    const VlExpr* expr = port->low_conn();
    if ( expr->is_operation() ) {
      assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
      ymuint n = expr->operand_num();
      mMvMgr->init_port(module, i, port->name(), n);
      for (ymuint j = 0; j < n; ++ j) {
	MvNode* node;
	ymuint msb;
	ymuint lsb;
	switch ( gen_portref(expr->operand(j), node, msb, lsb) ) {
	case 0:
	  mMvMgr->set_port_ref(module, i, j, node);
	  break;

	case 1:
	  mMvMgr->set_port_ref(module, i, j, node, msb);
	  break;

	case 2:
	  mMvMgr->set_port_ref(module, i, j, node, msb, lsb);
	  break;

	default:
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
    }
    else {
      mMvMgr->init_port(module, i, port->name(), 1);
      MvNode* node;
      ymuint msb;
      ymuint lsb;
      switch ( gen_portref(expr, node, msb, lsb) ) {
      case 0:
	mMvMgr->set_port_ref(module, i, 0, node);
	break;

      case 1:
	mMvMgr->set_port_ref(module, i, 0, node, msb);
	break;

      case 2:
	mMvMgr->set_port_ref(module, i, 0, node, msb, lsb);
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
    }
  }

  return module;
}

// @brief 宣言要素を生成する．
// @param[in] module モジュール
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::gen_decl(MvModule* module,
		     const VlNamedObj* vl_scope)
{
  // ネットの生成
  {
    vector<const VlDecl*> net_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiNet, net_list) ) {
      for (vector<const VlDecl*>::iterator p = net_list.begin();
	   p != net_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	// 仮の through ノードに対応させる．
	ymuint bw = vl_decl->bit_size();
	MvNode* node = mMvMgr->new_through(module, bw);
	reg_node(vl_decl, node);
      }
    }
  }

  // ネット配列の生成
  {
    vector<const VlDeclArray*> netarray_list;
    if ( mVlMgr.find_declarray_list(vl_scope, vpiNetArray, netarray_list) ) {
      for (vector<const VlDeclArray*>::iterator p = netarray_list.begin();
	   p != netarray_list.end(); ++ p) {
	const VlDeclArray* vl_decl = *p;
	ymuint array_size = vl_decl->array_size();
	for (ymuint i = 0; i < array_size; ++ i) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvNode* node = mMvMgr->new_through(module, bw);
	  reg_node(vl_decl, i, node);
	}
      }
    }
  }

  // REG の生成
  {
    vector<const VlDecl*> reg_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiReg, reg_list) ) {
      for (vector<const VlDecl*>::iterator p = reg_list.begin();
	   p != reg_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	// 仮の through ノードに対応させる．
	ymuint bw = vl_decl->bit_size();
	MvNode* node = mMvMgr->new_through(module, bw);
	reg_node(vl_decl, node);
	(void) mDeclHash.get_id(vl_decl);
      }
    }
  }

  // REG配列の生成
  {
    vector<const VlDeclArray*> regarray_list;
    if ( mVlMgr.find_declarray_list(vl_scope, vpiRegArray, regarray_list) ) {
      for (vector<const VlDeclArray*>::iterator p = regarray_list.begin();
	   p != regarray_list.end(); ++ p) {
	const VlDeclArray* vl_decl = *p;
	ymuint array_size = vl_decl->array_size();
	(void) mDeclHash.get_id(vl_decl, 0);
	for (ymuint i = 0; i < array_size; ++ i) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvNode* node = mMvMgr->new_through(module, bw);
	  reg_node(vl_decl, i, node);
	}
      }
    }
  }

  // 内部スコープ要素の生成
  {
    vector<const VlNamedObj*> scope_list;
    if ( mVlMgr.find_genblock_list(vl_scope, scope_list) ) {
      for (vector<const VlNamedObj*>::iterator p = scope_list.begin();
	   p != scope_list.end(); ++ p) {
	const VlNamedObj* vl_scope1 = *p;
	bool stat = gen_decl(module, vl_scope1);
	if ( !stat ) {
	  return false;
	}
      }
    }
  }

  return true;
}

// @brief 要素を生成する．
// @param[in] module モジュール
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::gen_item(MvModule* module,
		     const VlNamedObj* vl_scope)
{
  // モジュールインスタンスの生成
  {
    vector<const VlModule*> module_list;
    if ( mVlMgr.find_module_list(vl_scope, module_list) ) {
      for (vector<const VlModule*>::iterator p = module_list.begin();
	   p != module_list.end(); ++ p) {
	const VlModule* vl_module = *p;
	gen_moduleinst(vl_module, module);
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
	  gen_moduleinst(vl_module, module);
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
    if ( mVlMgr.find_genblock_list(vl_scope, scope_list) ) {
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

// @brief portref の実体化を行う．
// @param[in] expr 対象の式
// @param[out] node 対応するノードを格納する変数
// @param[out] msb ビット指定位置か範囲指定の MSB を格納する変数
// @param[out] lsb 範囲指定の LSB を格納する変数
// @retval 0 単純な形式だった．
// @retval 1 ビット指定形式だった．
// @retval 2 範囲指定形式だった．
int
ReaderImpl::gen_portref(const VlExpr* expr,
			MvNode*& node,
			ymuint& msb,
			ymuint& lsb)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  node = mIODeclMap.get(decl);
  if ( node == NULL ) {
    cerr << decl->full_name() << " is not found in mIODeclMap" << endl;
  }
  assert_cond( node != NULL, __FILE__, __LINE__);

  if ( expr->is_bitselect() ) {
    assert_cond( node != NULL, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    assert_cond( expr->declarray_dimension() == 0, __FILE__, __LINE__);
    msb = expr->index_val();
    return 1;
  }
  if ( expr->is_partselect() ) {
    assert_cond( node != NULL, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    assert_cond( expr->declarray_dimension() == 0, __FILE__, __LINE__);
    msb = expr->left_range_val();
    lsb = expr->right_range_val();
    return 2;
  }
  return 0;
}

// @brief モジュールインスタンスの生成を行う．
// @param[in] vl_module モジュール
// @param[in] parent_module 親のモジュール
// @return 対応するノードを返す．
void
ReaderImpl::gen_moduleinst(const VlModule* vl_module,
			   MvModule* parent_module)
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

// @brief プロセス文を生成する．
// @param[in] vl_process プロセス文
bool
ReaderImpl::gen_process(MvModule* parent_module,
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

  bool has_edge_event = false;
  bool has_normal_event = false;
  const VlControl* control = stmt->control();
  for (ymuint i = 0; i < control->event_num(); ++ i) {
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
    else if ( expr->decl_obj() != NULL ) {
      has_normal_event = true;
    }
    else {
      cerr << "illegal type" << endl;
      return false;
    }
  }

  if ( has_edge_event && has_normal_event ) {
    cerr << "edge-type events and normal events are mutual exclusive." << endl;
    return false;
  }

  TmpEnv top_env(mGlobalEnv);
  gen_stmt(parent_module, stmt->body_stmt(), top_env);

  ymuint n = mGlobalEnv.max_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node1 = top_env.get_from_id(i);
    if ( node1 == NULL ) {
      continue;
    }
    MvNode* node0 = mGlobalEnv.get_from_id(i);
    MvNode* cond = NULL;
    switch ( latch_check(parent_module, node1, cond) ) {
    case 0: // latch 条件なし
      mMvMgr->connect(node1, 0, node0, 0);
      break;

    case 1: // latch 条件あり
      assert_cond( cond != NULL, __FILE__, __LINE__);
      {
	ymuint bw = node0->output(0)->bit_width();
	MvNode* latch = mMvMgr->new_latch(parent_module, bw);
	mMvMgr->connect(node1, 0, latch, 0);
	mMvMgr->connect(cond, 0, latch, 1);
	mMvMgr->connect(latch, 0, node0, 0);
      }
      break;

    case 2: // 常に latch
      // これはあるわけない．
      assert_not_reached(__FILE__, __LINE__);
    }
  }

  return true;
}

// @brief ステートメントの中身を生成する．
// @param[in] module 親のモジュール
// @param[in] stmt 本体のステートメント
// @param[in] env 環境
bool
ReaderImpl::gen_stmt(MvModule* module,
		     const VlStmt* stmt,
		     TmpEnv& env)
{
  switch ( stmt->type() ) {
  case kVpiAssignment:
    {
      const VlExpr* rhs = stmt->rhs();
      const VlExpr* lhs = stmt->lhs();
      MvNode* node = gen_expr(module, rhs, env);
      ymuint n = lhs->lhs_elem_num();
      ymuint offset = 0;
      for (ymuint i = 0; i < n; ++ i) {
	const VlExpr* lhs1 = lhs->lhs_elem(i);
	const VlDecl* lhs_decl = lhs1->decl_obj();
	const VlDeclArray* lhs_declarray = lhs1->declarray_obj();
	const VlDeclBase* lhs_declbase = NULL;
	ymuint bw;
	MvNode* old_dst = NULL;
	ymuint lhs_offset = 0;
	if ( lhs_decl ) {
	  lhs_declbase = lhs_decl;
	  bw = lhs_decl->bit_size();
	  old_dst = env.get(lhs_decl);
	}
	else if ( lhs_declarray ) {
	  lhs_declbase = lhs_declarray;
	  bw = lhs_declarray->bit_size();
	  if ( lhs1->is_constant_select() ) {
	    lhs_offset = lhs1->declarray_offset();
	    old_dst = env.get(lhs_declarray, lhs_offset);
	  }
	  else {
	    assert_cond( lhs1->declarray_dimension()
			 == lhs_declarray->dimension(),
			 __FILE__, __LINE__ );
	    // lhs_offset は lhs->declarray_index() から計算される．
#warning "TODO: 配列の可変インデックス時の処理"
	  }
	}
	else {
	  assert_not_reached(__FILE__, __LINE__);
	}

	MvNode* src_node = gen_rhs(module, node, offset, bw);
	MvNode* dst_node = NULL;
	if ( lhs1->is_primary() ) {
	  dst_node = mMvMgr->new_through(module, bw);
	  mMvMgr->connect(src_node, 0, dst_node, 0);
	}
	else if ( lhs1->is_bitselect() ) {
	  assert_cond( lhs1->is_constant_select(), __FILE__, __LINE__);
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
	  dst_node = mMvMgr->new_concat(module, bw_array);
	  ymuint pos = 0;
	  if ( index < bw - 1 ) {
	    MvNode* tmp_node = mMvMgr->new_constpartselect(module,
							   bw - 1,
							   index + 1,
							   bw);
	    mMvMgr->connect(old_dst, 0, tmp_node, 0);
	    mMvMgr->connect(tmp_node, 0, dst_node, pos);
	    ++ pos;
	  }
	  mMvMgr->connect(src_node, 0, dst_node, pos);
	  ++ pos;
	  if ( index > 0 ) {
	    MvNode* tmp_node = mMvMgr->new_constpartselect(module,
							   index - 1,
							   0,
							   bw);
	    mMvMgr->connect(old_dst, 0, tmp_node, 0);
	    mMvMgr->connect(tmp_node, 0, dst_node, pos);
	  }
	}
	else if ( lhs1->is_partselect() ) {
	  assert_cond( lhs1->is_constant_select(), __FILE__, __LINE__);
	  ymuint msb = lhs_declbase->bit_offset(lhs1->left_range_val());
	  ymuint lsb = lhs_declbase->bit_offset(lhs1->right_range_val());
	  vector<ymuint> bw_array;
	  bw_array.reserve(3);
	  if ( msb < bw - 1 ) {
	    bw_array.push_back(bw - msb - 1);
	  }
	  bw_array.push_back(msb - lsb + 1);
	  if ( lsb > 0 ) {
	    bw_array.push_back(lsb);
	  }
	  dst_node = mMvMgr->new_concat(module, bw_array);
	  ymuint pos = 0;
	  if ( msb < bw - 1 ) {
	    MvNode* tmp_node = mMvMgr->new_constpartselect(module,
							   bw - 1,
							   msb + 1,
							   bw);
	    mMvMgr->connect(old_dst, 0, tmp_node, 0);
	    mMvMgr->connect(tmp_node, 0, dst_node, pos);
	    ++ pos;
	  }
	  mMvMgr->connect(src_node, 0, dst_node, pos);
	  ++ pos;
	  if ( lsb > 0 ) {
	    MvNode* tmp_node = mMvMgr->new_constpartselect(module,
							   lsb - 1,
							   0,
							   bw);
	    mMvMgr->connect(old_dst, 0, tmp_node, 0);
	    mMvMgr->connect(tmp_node, 0, dst_node, pos);
	  }
	}
	assert_cond( dst_node, __FILE__, __LINE__);
	if ( lhs_decl ) {
	  env.add(lhs_decl, dst_node);
	}
	else {
	  env.add(lhs_declarray, lhs_offset, dst_node);
	}
	offset += lhs1->bit_size();
      }
    }
    break;

  case kVpiBegin:
  case kVpiNamedBegin:
    {
      ymuint n = stmt->child_stmt_num();
      for (ymuint i = 0; i < n; ++ i) {
	const VlStmt* stmt1 = stmt->child_stmt(i);
	gen_stmt(module, stmt1, env);
      }
    }
    break;

  case kVpiIf:
    {
      const VlExpr* cond = stmt->expr();
      MvNode* cond_node = gen_expr(module, cond, env);
      TmpEnv then_env(env);
      gen_stmt(module, stmt->body_stmt(), then_env);
      merge_env(module, env, cond_node, then_env, env);
    }
    break;

  case kVpiIfElse:
    {
      const VlExpr* cond = stmt->expr();
      MvNode* cond_node = gen_expr(module, cond, env);
      TmpEnv then_env(env);
      gen_stmt(module, stmt->body_stmt(), then_env);
      TmpEnv else_env(env);
      gen_stmt(module, stmt->else_stmt(), else_env);
      merge_env(module, env, cond_node, then_env, else_env);
    }
    break;

  case kVpiCase:
#if 0
    {
      const VlExpr* cond = stmt->expr();
      MvNode* cond_node = gen_expr(module, cond, env);

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

// @brief always latch のチェック
// @param[in] parent_module 親のモジュール
// @param[in] src_node ソースノード
// @param[out] cond_node 条件を表すノード
// @retval 0 latch 条件はない．
// @retval 1 常に latch
// @retval 2 部分的な latch 条件あり
ymuint
ReaderImpl::latch_check(MvModule* parent_module,
			MvNode* src_node,
			MvNode*& cond_node)
{
  if ( src_node->type() == MvNode::kIte ) {
    MvNode* cond = src_node->input(0)->src_pin()->node();
    MvNode* cond1_node = NULL;
    MvNode* cond2_node = NULL;
    ymuint cond1 = 0;
    ymuint cond2 = 0;
    if ( src_node->input(1)->src_pin() == NULL ) {
      cond1 = 1;
    }
    else {
      MvNode* node1 = src_node->input(1)->src_pin()->node();
      cond1 = latch_check(parent_module, node1, cond1_node);
    }
    if ( src_node->input(2)->src_pin() == NULL ) {
      cond2 = 1;
    }
    else {
      MvNode* node2 = src_node->input(2)->src_pin()->node();
      cond2 = latch_check(parent_module, node2, cond2_node);
    }
    switch ( cond1 ) {
    case 0:
      switch ( cond2 ) {
      case 0:
	return 0;

      case 1:
	cond_node = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(cond, 0, cond_node, 0);
	return 2;

      case 2:
	{
	  MvNode* cond_bar = mMvMgr->new_not(parent_module, 1);
	  mMvMgr->connect(cond, 0, cond_bar, 0);
	  cond_node = mMvMgr->new_and(parent_module, 2, 1);
	  mMvMgr->connect(cond_bar, 0, cond_node, 0);
	  mMvMgr->connect(cond2_node, 0, cond_node, 1);
	  return 2;
	}
      }
      break;

    case 1:
      switch ( cond2 ) {
      case 0:
	cond_node = cond;
	return 2;

      case 1:
	return 1;

      case 2:
	cond_node = mMvMgr->new_or(parent_module, 2, 1);
	mMvMgr->connect(cond, 0, cond_node, 0);
	mMvMgr->connect(cond2_node, 0, cond_node, 1);
	return 2;
      }
      break;

    case 2:
      switch ( cond2 ) {
      case 0:
	cond_node = mMvMgr->new_and(parent_module, 2, 1);
	mMvMgr->connect(cond, 0, cond_node, 0);
	mMvMgr->connect(cond1_node, 0, cond_node, 1);
	return 2;

      case 1:
	{
	  MvNode* cond_bar = mMvMgr->new_not(parent_module, 1);
	  mMvMgr->connect(cond, 0, cond_bar, 0);
	  cond_node = mMvMgr->new_or(parent_module, 2, 1);
	  mMvMgr->connect(cond_bar, 0, cond_node, 0);
	  mMvMgr->connect(cond1_node, 0, cond_node, 1);
	  return 2;
	}

      case 2:
	cond_node = mMvMgr->new_ite(parent_module, 1);
	mMvMgr->connect(cond, 0, cond_node, 0);
	mMvMgr->connect(cond1_node, 0, cond_node, 1);
	mMvMgr->connect(cond2_node, 1, cond_node, 2);
	return 2;
      }
    }
  }
  return 0;
}

// @brief プリミティブインスタンスの生成を行う．
// @param[in] parent_module 親のモジュール
// @param[in] prim プリミティブ
void
ReaderImpl::gen_priminst(MvModule* parent_module,
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

  vector<pair<MvNode*, ymuint> > inputs(ni);
  vector<MvNode*> outputs(no);

  switch ( prim->prim_type() ) {
  case kVpiBufPrim:
    {
      MvNode* node = mMvMgr->new_through(parent_module, 1);
      inputs[0] = make_pair(node, 0);
      for (ymuint i = 0; i < no; ++ i) {
	outputs[i] = node;
      }
    }
    break;

  case kVpiNotPrim:
    {
      MvNode* node = mMvMgr->new_not(parent_module, 1);
      inputs[0] = make_pair(node, 0);
      outputs[0] = node;
    }
    break;

  case kVpiAndPrim:
    {
      MvNode* node = mMvMgr->new_and(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiNandPrim:
    {
      MvNode* node = mMvMgr->new_and(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiOrPrim:
    {
      MvNode* node = mMvMgr->new_or(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiNorPrim:
    {
      MvNode* node = mMvMgr->new_or(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiXorPrim:
    {
      MvNode* node = mMvMgr->new_xor(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiXnorPrim:
    {
      MvNode* node = mMvMgr->new_xor(parent_module, ni, 1);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;

  case kVpiCombPrim:
    {
      assert_not_reached(__FILE__, __LINE__);
      const VlUdpDefn* udp = prim->udp_defn();
      ymuint ni = udp->port_num() - 1;
      MvNode* node = mMvMgr->new_combudp(parent_module, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
    }
    break;

  case kVpiSeqPrim:
    {
      assert_not_reached(__FILE__, __LINE__);
      const VlUdpDefn* udp = prim->udp_defn();
      ymuint ni = udp->port_num() - 1;
      MvNode* node = mMvMgr->new_sequdp(parent_module, ni);
      for (ymuint i = 0; i < ni; ++ i) {
	inputs[i] = make_pair(node, i);
      }
      outputs[0] = node;
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
    MvNode* dst_node = gen_primary(expr, mGlobalEnv);
    connect_lhs(dst_node, expr, outputs[i]);
  }
  for (ymuint i = 0; i < ni; ++ i) {
    const VlPrimTerm* term = prim->prim_term(pos);
    ++ pos;
    const VlExpr* expr = term->expr();
    MvNode* node = gen_expr(parent_module, expr, mGlobalEnv);
    const pair<MvNode*, ymuint>& p = inputs[i];
    mMvMgr->connect(node, 0, p.first, p.second);
  }
}

// @brief 継続的代入文の生成を行う．
// @param[in] parent_module 親のモジュール
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
void
ReaderImpl::gen_cont_assign(MvModule* parent_module,
			    const VlExpr* lhs,
			    const VlExpr* rhs)
{
  MvNode* node = gen_expr(parent_module, rhs, mGlobalEnv);
  ymuint n = lhs->lhs_elem_num();
  ymuint offset = 0;
  for (ymuint i = 0; i < n; i ++ ) {
    const VlExpr* lhs_elem = lhs->lhs_elem(i);
    MvNode* dst_node = gen_primary(lhs_elem, mGlobalEnv);
    ymuint dst_bw = lhs_elem->bit_size();
    MvNode* src_node = gen_rhs(parent_module, node, offset, dst_bw);
    connect_lhs(dst_node, lhs_elem, src_node);
    offset += dst_bw;
  }
}

// @brief 環境をマージする．
// @param[in] parent_module 親のモジュール
// @param[in] env 対象の環境
// @param[in] cond 条件を表すノード
// @param[in] then_env 条件が成り立ったときに通るパスの環境
// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
void
ReaderImpl::merge_env(MvModule* parent_module,
		      TmpEnv& env,
		      MvNode* cond,
		      const TmpEnv& then_env,
		      const TmpEnv& else_env)
{
  ymuint n = env.max_id();
  for (ymuint i = 0; i < n; ++ i) {
    // TODO: node0, node1, node2 が NULL の場合がある．
    MvNode* node0 = env.get_from_id(i);
    MvNode* node1 = then_env.get_from_id(i);
    MvNode* node2 = else_env.get_from_id(i);
    if ( node1 == node2 ) {
      // 両方の結果が等しければ ITE を挿入しない．
      if ( node1 == node0 ) {
	// さらにもとのノードと同じならなにもしない．
	continue;
      }
      env.add_by_id(i, node1);
    }
    else {
      ymuint bw;
      if ( node0 ) {
	bw = node0->output(0)->bit_width();
      }
      else if ( node1 ) {
	bw = node1->output(0)->bit_width();
      }
      else if ( node2 ) {
	bw = node2->output(0)->bit_width();
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      MvNode* node = mMvMgr->new_ite(parent_module, bw);
      mMvMgr->connect(cond, 0, node, 0);
      if ( node1 ) {
	mMvMgr->connect(node1, 0, node, 1);
      }
      if ( node2 ) {
	mMvMgr->connect(node2, 0, node, 2);
      }
      env.add_by_id(i, node);
    }
  }
}

// @brief 左辺式に接続する．
// @param[in] dst_node 左辺に対応するノード
// @param[in] expr 左辺式
// @param[in] src_node 右辺に対応するノード
void
ReaderImpl::connect_lhs(MvNode* dst_node,
			const VlExpr* expr,
			MvNode* src_node)
{
  if ( expr->is_primary() ) {
    reg_driver(dst_node, Driver(src_node));
  }
  else if ( expr->is_bitselect() ) {
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    const VlDecl* decl = expr->decl_obj();
    ymuint index = decl->bit_offset(expr->index_val());
    reg_driver(dst_node, Driver(src_node, index));
  }
  else if ( expr->is_partselect() ) {
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    const VlDecl* decl = expr->decl_obj();
    ymuint msb = decl->bit_offset(expr->left_range_val());
    ymuint lsb = decl->bit_offset(expr->right_range_val());
    assert_cond( src_node->output(0)->bit_width() == msb - lsb + 1,
		 __FILE__, __LINE__);
    reg_driver(dst_node, Driver(src_node, msb, lsb));
  }
}

// @brief 右辺式に対応するノードを返す．
// @param[in] parent_module 親のモジュール
// @param[in] node 右辺式のノード
// @param[in] offset オフセット
// @param[in] bit_width ビット幅
// @note node から [offset: offset + bit_width - 1] の選択するノードを返す．
// @note 全範囲を選択する場合には node を返す．
// @note 範囲が合わなかったら NULL を返す．
MvNode*
ReaderImpl::gen_rhs(MvModule* parent_module,
		    MvNode* node,
		    ymuint offset,
		    ymuint bit_width)
{
  ymuint src_bw = node->output(0)->bit_width();
  if ( offset + bit_width > src_bw ) {
    cerr << "bit width mismatch" << endl;
    assert_not_reached(__FILE__, __LINE__);
  }
  MvNode* src_node = NULL;
  if ( offset == 0 && bit_width == src_bw ) {
    // 全範囲の選択
    src_node = node;
  }
  else if ( bit_width == 1 ) {
    src_node = mMvMgr->new_constbitselect(parent_module,
					  offset,
					  src_bw);
    mMvMgr->connect(node, 0, src_node, 0);
  }
  else {
    src_node = mMvMgr->new_constpartselect(parent_module,
					   offset,
					   offset + bit_width - 1,
					   src_bw);
    mMvMgr->connect(node, 0, src_node, 0);
  }
  return src_node;
}

// @brief 式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] env 環境
MvNode*
ReaderImpl::gen_expr(MvModule* parent_module,
		     const VlExpr* expr,
		     const Env& env)
{
  if ( expr->is_const() ) {
    assert_cond( is_bitvector_type(expr->value_type()), __FILE__, __LINE__);
    BitVector bv;
    //expr->eval_bitvector(bv);
    ymuint bit_size = bv.size();
    ymuint n = (bit_size + 31) / 32;
    vector<ymuint32> tmp(n);
    for (ymuint i = 0; i < bit_size; ++ i) {
      ymuint blk = i / 32;
      ymuint pos = i % 32;
      switch ( bv.value(i) ) {
      case kVpiScalar1: tmp[blk] |= (1 << pos); break;
      case kVpiScalar0: break;
      case kVpiScalarX:
      case kVpiScalarZ:
	mMsgMgr.put_msg(__FILE__, __LINE__,
			expr->file_region(),
			kMsgError,
			"MVN_VLXXX",
			"'X' or 'Z' in constant expression");
	break;
      }
    }
    return mMvMgr->new_const(parent_module, bit_size, tmp);
  }
  if ( expr->is_operation() ) {
    ymuint n = expr->operand_num();
    vector<MvNode*> inputs(n);
    for (ymuint i = 0; i < n; ++ i) {
      inputs[i] = gen_expr(parent_module, expr->operand(i), env);
    }

    switch ( expr->op_type() ) {
    case kVpiNullOp:
      return inputs[0];

    case kVpiMinusOp:
      {
	MvNode* node = mMvMgr->new_cmpl(parent_module, expr->bit_size());
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiNotOp:
      {
	MvNode* node = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiBitNegOp:
      {
	MvNode* node = mMvMgr->new_not(parent_module, expr->bit_size());
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiPlusOp:
      return inputs[0];

    case kVpiUnaryAndOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rand(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiUnaryNandOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rand(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiUnaryOrOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_ror(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiUnaryNorOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_ror(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiUnaryXorOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rxor(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	return node;
      }

    case kVpiUnaryXNorOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_rxor(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiPosedgeOp:
    case kVpiNegedgeOp:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kVpiAddOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_add(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiSubOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sub(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiMultOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_mult(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiDivOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_div(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiModOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_mod(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiPowerOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_pow(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiLShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sll(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiRShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_srl(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiArithLShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sla(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiArithRShiftOp:
      {
	ymuint bw1 = inputs[0]->output(0)->bit_width();
	ymuint bw2 = inputs[1]->output(0)->bit_width();
	ymuint bw3 = expr->bit_size();
	MvNode* node = mMvMgr->new_sra(parent_module, bw1, bw2, bw3);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiBitAndOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_and(parent_module, 2, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiBitOrOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_or(parent_module, 2, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiBitXNorOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_xor(parent_module, 2, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, bw);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiBitXorOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_xor(parent_module, 2, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiLogAndOp:
      {
	MvNode* node = mMvMgr->new_and(parent_module, 2, 1);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiLogOrOp:
      {
	MvNode* node = mMvMgr->new_or(parent_module, 2, 1);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiCaseEqOp:
    case kVpiCaseNeqOp:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kVpiEqOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_equal(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiNeqOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_equal(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiLtOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	return node;
      }

    case kVpiGeOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiGtOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[1], 0, node, 0);
	mMvMgr->connect(inputs[0], 0, node, 1);
	return node;
      }

    case kVpiLeOp:
      {
	ymuint bw = inputs[0]->output(0)->bit_width();
	MvNode* node = mMvMgr->new_lt(parent_module, bw);
	mMvMgr->connect(inputs[1], 0, node, 0);
	mMvMgr->connect(inputs[0], 0, node, 1);
	MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }

    case kVpiConditionOp:
      {
	ymuint bw = expr->bit_size();
	MvNode* node = mMvMgr->new_ite(parent_module, bw);
	mMvMgr->connect(inputs[0], 0, node, 0);
	mMvMgr->connect(inputs[1], 0, node, 1);
	mMvMgr->connect(inputs[2], 0, node, 2);
	return node;
      }

    case kVpiMinTypMaxOp:
      assert_not_reached(__FILE__, __LINE__);
      break;

    case kVpiConcatOp:
      {
	ymuint n = expr->operand_num();
	vector<ymuint> bw_array(n);
	for (ymuint i = 0; i < n; ++ i) {
	  bw_array[i] = inputs[i]->output(0)->bit_width();
	}
	MvNode* node = mMvMgr->new_concat(parent_module, bw_array);
	for (ymuint i = 0; i < n; ++ i) {
	  mMvMgr->connect(inputs[i], 0, node, i);
	}
	return node;
      }

    case kVpiMultiConcatOp:
      {
	ymint r = expr->rep_num();
	ymuint n = expr->operand_num() - 1;
	vector<ymuint> bw_array(n * r);
	for (ymint j = 0; j < r; ++ j) {
	  ymuint base = j * r;
	  for (ymuint i = 0; i < n; ++ i) {
	    bw_array[base + i] = inputs[i + 1]->output(0)->bit_width();
	  }
	}
	MvNode* node = mMvMgr->new_concat(parent_module, bw_array);
	for (ymint j = 0; j < r; ++ j) {
	  ymuint base = j * r;
	  for (ymuint i = 0; i < n; ++ i) {
	    mMvMgr->connect(inputs[i + 1], 0, node, base + i);
	  }
	}
	return node;
      }

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }

  MvNode* node = gen_primary(expr, env);
  if ( expr->is_primary() ) {
    return node;
  }
  if ( expr->is_bitselect() ) {
    if ( expr->is_constant_select() ) {
      const VlDecl* decl = expr->decl_obj();
      ymuint bitpos = decl->bit_offset(expr->index_val());
      const MvOutputPin* pin = node->output(0);
      MvNode* node1 = mMvMgr->new_constbitselect(parent_module,
						 bitpos,
						 pin->bit_width());
      assert_cond( node, __FILE__, __LINE__);
      mMvMgr->connect(node, 0, node1, 0);
      return node1;
    }
    else {
#warning "TODO: [msb:lsb] のオフセット変換をしていない"
      MvNode* node1 = gen_expr(parent_module, expr->index(), env);
      const MvOutputPin* pin0 = node->output(0);
      const MvOutputPin* pin1 = node1->output(0);
      MvNode* node2 = mMvMgr->new_bitselect(parent_module,
					    pin0->bit_width(),
					    pin1->bit_width());
      mMvMgr->connect(node, 0, node2, 0);
      mMvMgr->connect(node1, 0, node2, 1);
      return node2;
    }
  }
  if ( expr->is_partselect() ) {
    if ( expr->is_constant_select() ) {
      const VlDecl* decl = expr->decl_obj();
      ymuint msb = decl->bit_offset(expr->left_range_val());
      ymuint lsb = decl->bit_offset(expr->right_range_val());
      const MvOutputPin* pin = node->output(0);
      MvNode* node1 = mMvMgr->new_constpartselect(parent_module,
						  msb, lsb,
						  pin->bit_width());
      mMvMgr->connect(node, 0, node1, 0);
      return node1;
    }
    else {
#warning "TODO: [msb:lsb] のオフセット変換をしていない"
      // まだできてない．
      // というか可変 part_select は VPI がおかしいと思う．
      assert_not_reached(__FILE__, __LINE__);
      return NULL;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
// @param[in] env 環境
MvNode*
ReaderImpl::gen_primary(const VlExpr* expr,
			const Env& env)
{
  const VlDecl* decl = expr->decl_obj();
  const VlDeclArray* declarray = expr->declarray_obj();
  if ( decl ) {
    assert_cond(expr->declarray_dimension() == 0, __FILE__, __LINE__);
    MvNode* node = env.get(decl);
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mGlobalEnv" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
  else if ( declarray ) {
    if ( expr->is_constant_select() ) {
      // インデックス固定の配列要素
      ymuint offset = expr->declarray_offset();
      MvNode* node = env.get(declarray, offset);
      if ( node == NULL ) {
	cerr << decl->name() << " is not found in mGlobalEnv" << endl;
      }
      assert_cond( node != NULL, __FILE__, __LINE__);
      return node;
    }
    else {
      // インデックス可変の配列要素
#if 0
      ymuint dim = expr->declarray_dimension();
      assert_cond( declarray->dimension() == dim, __FILE__, __LINE__);
      ymuint offset = 0;
      ymuint mlt = 1;
      for (ymuint i = 0; i < dim; ++ i) {
	const VlExpr* index = expr->declarray_index(i);
	int index_val;
	bool stat = index->eval_int(index_val);
	assert_cond( stat, __FILE__, __LINE__);
	offset += index_val * mlt;
	mlt *= declarray->range(i)->size();
      }
      MvNode* node = env.get(declarray, offset);
      if ( node == NULL ) {
	cerr << decl->name() << " is not found in mGlobalEnv" << endl;
      }
      assert_cond( node != NULL, __FILE__, __LINE__);
      return node;
#else
      assert_not_reached(__FILE__, __LINE__);
      return NULL;
#endif
    }
  }
  cout << "Error in " << expr->decompile() << endl;
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素
// @param[in] node 登録するノード
void
ReaderImpl::reg_node(const VlDecl* decl,
		     MvNode* node)
{
  mGlobalEnv.add(decl, node);
  mNodeMap.reg_node(node->id(), decl);
}

// @brief 入出力宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素
// @param[in] node 登録するノード
void
ReaderImpl::reg_ionode(const VlDecl* decl,
		       MvNode* node)
{
  mIODeclMap.add(decl, node);
  mNodeMap.reg_node(node->id(), decl);
}

// @brief 宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素(配列型)
// @param[in] offset オフセット
// @param[in] node 登録するノード
void
ReaderImpl::reg_node(const VlDeclArray* decl,
		     ymuint offset,
		     MvNode* node)
{
  mGlobalEnv.add(decl, offset, node);
  mNodeMap.reg_node(node->id(), decl, offset);
}

// @brief ドライバーを登録する．
// @param[in] node 左辺に対応するノード
// @param[in] driver ドライバー
void
ReaderImpl::reg_driver(MvNode* node,
		       const Driver& driver)
{
  if ( debug_driver ) {
    cerr << "reg_driver(" << node->id()
	 << ", " << driver.rhs_node()->id();
    if ( driver.has_bitselect() ) {
      cerr << "[" << driver.index() << "]";
    }
    else if ( driver.has_partselect() ) {
      cerr << "[" << driver.msb() << ":" << driver.lsb() << "]";
    }
    cerr << ")" << endl;
  }
  driver_list(node).push_back(driver);
}

// @brief ドライバーリストを取り出す．
// @param[in] node 対応するノード
// @note なければ空のリストを作る．
vector<Driver>&
ReaderImpl::driver_list(MvNode* node)
{
  assert_cond( node != NULL, __FILE__, __LINE__);
  ymuint id = node->id();
  while ( mDriverList.size() <= id ) {
    mDriverList.push_back(vector<Driver>());
  }
  return mDriverList[id];
}

// @brief 複数のドライバがある時にエラーメッセージを出力する．
void
ReaderImpl::error_drivers(MvNode* node,
			  const Driver& driver1,
			  const Driver& driver2)
{
  cerr << "more than one drivers for "
       << node->id()
       << endl;
  cerr << "previous driver: "
       << driver1.rhs_node()->id();
  if ( driver1.has_bitselect() ) {
    cerr << "[" << driver1.index() << "]";
  }
  else if ( driver1.has_partselect() ) {
    cerr << "[" << driver1.msb() << ":" << driver1.lsb()
	 << "]";
  }
  cerr << endl;
  cerr << "current driver: "
       << driver2.rhs_node()->id();
  if ( driver2.has_bitselect() ) {
    cerr << "[" << driver2.index() << "]";
  }
  else if ( driver2.has_partselect() ) {
    cerr << "[" << driver2.msb() << ":" << driver2.lsb()
	 << "]";
  }
  cerr << endl;
  abort();
}

END_NAMESPACE_YM_MVN_VERILOG
