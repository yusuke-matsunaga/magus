
/// @file libym_mvn/verilog/ReaderImpl.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReaderImpl.h"
#include "DeclMap.h"
#include "Driver.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPort.h"
#include "ym_mvn/MvNode.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlUdp.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlPort.h"
#include "ym_verilog/vl/VlContAssign.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

BEGIN_NONAMESPACE

const
bool debug_driver = false;

END_NONAMESPACE


// @brief コンストラクタ
ReaderImpl::ReaderImpl() :
  mVlMgr(mMsgMgr)
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
ReaderImpl::gen_network(MvMgr& mgr)
{
  using namespace nsVerilog;

  mVlMgr.elaborate();
  
  mMvMgr = &mgr;

  mDeclMap.clear();
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
      cout << "more than one top modules" << endl;
    }
  }

  // 結線を行う．
  ymuint n = mMvMgr->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = mMvMgr->_node(i);
    if ( node == NULL ) continue;
    if ( mDriverList.size() <= i ) break;
    const vector<Driver>& dlist = mDriverList[node->id()];
    if ( dlist.empty() ) {
      continue;
    }
    ymuint bw = node->input(0)->bit_width();
    vector<const Driver*> tmp(bw, NULL);
    for (vector<Driver>::const_iterator p = dlist.begin();
	 p != dlist.end(); ++ p) {
      const Driver* driver = &*p;
      if ( driver->is_simple() ) {
	for (ymuint i = 0; i < bw; ++ i) {
	  if ( tmp[i] != NULL ) {
	    // TODO: エラーメッセージをちゃんとする．
	    cerr << "more than one drivers" << endl;
	    abort();
	  }
	  tmp[i] = driver;
	}
      }
      else if ( driver->has_bitselect() ) {
	ymuint index = driver->index();
	if ( tmp[index] != NULL ) {
	  // TODO: エラーメッセージをちゃんとする．
	  cerr << "more than one drivers" << endl;
	  abort();
	}
	tmp[index] = driver;
      }
      else {
	ymuint msb = driver->msb();
	ymuint lsb = driver->lsb();
	for (ymuint i = lsb; i <= msb; ++ i) {
	  if ( tmp[i] != NULL ) {
	    // TODO: エラーメッセージをちゃんとする．
	    cerr << "more than one drivers" << endl;
	    abort();
	  }
	  tmp[i] = driver;
	}
      }
    }
    const Driver* prev = NULL;
    vector<const Driver*> tmp2;
    tmp2.reserve(bw);
    for (ymuint i = 0; i < bw; ++ i) {
      const Driver* driver = tmp[i];
      if ( driver == NULL ) {
	// TODO: エラーメッセージをちゃんとする．
	// というか本当は warning で指定されていないビットは
	// 0 にしなければならない．
	cerr << "no driver" << endl;
	cerr << "bw = " << bw
	     << ", i = " << i << endl;
	abort();
      }
      if ( driver != prev ) {
	tmp2.push_back(driver);
	prev = driver;
      }
    }

    ymuint n = tmp2.size();
    if ( n == 1 ) {
      const Driver* driver = tmp2[0];
      MvNode* src_node = driver->rhs_node();
      mMvMgr->connect(src_node, 0, node, 0);
    }
    else {
      vector<ymuint> bw_array(n);
      for (ymuint i = 0; i < n; ++ i) {
	const Driver* driver = tmp2[i];
	if ( driver->has_bitselect() ) {
	  bw_array[i] = 1;
	}
	else if ( driver->has_partselect() ) {
	  bw_array[i] = driver->msb() - driver->lsb() + 1;
	}
	else {
	  assert_not_reached(__FILE__, __LINE__);
	}
      }
      MvNode* node1 = mMvMgr->new_concat(module0, bw_array);
      mMvMgr->connect(node1, 0, node, 0);
      for (ymuint i = 0; i < n; ++ i) {
	const Driver* driver = tmp2[i];
	mMvMgr->connect(driver->rhs_node(), 0, node1, i);
      }
    }
  }

  // 冗長な through ノードを削除する．
  mMvMgr->sweep();
  
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
ReaderImpl::gen_module(const nsVerilog::VlModule* vl_module)
{
  using namespace nsVerilog;
  
  // ポート数，入出力のビット幅を調べる．
  ymuint np = vl_module->port_num();
  ymuint nio = vl_module->io_num();
  ymuint ni = 0;
  ymuint no = 0;
  for (ymuint i = 0; i < nio; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ++ ni; break;
    case kVpiOutput: ++ no; break;
    default:
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      io->file_region(),
		      kMsgError,
		      "MVN_VL01",
		      "Only Input/Output types are supported");
      return NULL;
    }
  }
  vector<ymuint> ibw_array(ni);
  vector<ymuint> obw_array(no);
  ni = 0;
  no = 0;
  for (ymuint i = 0; i < nio; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ibw_array[ni] = io->bit_size(); ++ ni; break;
    case kVpiOutput: obw_array[no] = io->bit_size(); ++ no; break;
    default: break;
    }
  }
  
  MvModule* module = mMvMgr->new_module(vl_module->name(),
					np, ibw_array, obw_array);

  // 入出力ノードの対応表を作る．
  ymuint i1 = 0;
  ymuint i2 = 0;
  for (ymuint i = 0; i < nio; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:
      mDeclMap.add(io->decl(), module->input(i1));
      ++ i1;
      break;

    case kVpiOutput:
      mDeclMap.add(io->decl(), module->output(i2));
      ++ i2;
      break;

    default:
      break;
    }
  }

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

  // ポートの接続を行う．
  for (ymuint i = 0; i < np; ++ i) {
    const VlPort* port = vl_module->port(i);
    const VlExpr* expr = port->low_conn();
    if ( expr->type() == kVpiOperation ) {
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
		     const nsVerilog::VlNamedObj* vl_scope)
{
  using namespace nsVerilog;

  // ネットの生成
  {
    vector<const VlDecl*> net_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiNet, net_list) ) {
      for (vector<const VlDecl*>::iterator p = net_list.begin();
	   p != net_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	// 入出力の場合，既に登録されている場合がある．
	if ( mDeclMap.get(vl_decl) == NULL ) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvNode* node = mMvMgr->new_through(module, bw);
	  mDeclMap.add(vl_decl, node);
	}
      }
    }
  }

  // ネット配列の生成
  {
    vector<const VlDecl*> netarray_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiNetArray, netarray_list) ) {
      for (vector<const VlDecl*>::iterator p = netarray_list.begin();
	   p != netarray_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	ymuint d = vl_decl->dimension();
	ymuint array_size = 1;
	for (ymuint i = 0; i < d; ++ i) {
	  array_size *= vl_decl->range(i)->size();
	}
	for (ymuint i = 0; i < array_size; ++ i) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvNode* node = mMvMgr->new_through(module, bw);
	  mDeclMap.add(vl_decl, i, node);
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
		     const nsVerilog::VlNamedObj* vl_scope)
{
  using namespace nsVerilog;

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
	const VlPrimitive* vl_prim = *p;
	gen_priminst(vl_prim, module);
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
	  const VlPrimitive* vl_prim = vl_primarray->elem_by_offset(i);
	  gen_priminst(vl_prim, module);
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
	const VlContAssign* vl_contassign = *p;
	const VlExpr* lhs = vl_contassign->lhs();
	const VlExpr* rhs = vl_contassign->rhs();
	MvNode* node = gen_expr1(module, rhs);
	connect_lhs(module, lhs, node);
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
  node = mDeclMap.get(decl);
  if ( node == NULL ) {
    cout << decl->full_name() << " is not found in mDeclMap" << endl;
  }
  assert_cond( node != NULL, __FILE__, __LINE__);

  switch ( expr->type() ) {
  case kVpiBitSelect:
    assert_cond( node != NULL, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    assert_cond( expr->declarray_dimension() == 0, __FILE__, __LINE__);
    msb = expr->index_val();
    return 1;

  case kVpiPartSelect:
    assert_cond( node != NULL, __FILE__, __LINE__);
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    assert_cond( expr->declarray_dimension() == 0, __FILE__, __LINE__);
    msb = expr->left_range_val();
    lsb = expr->right_range_val();
    return 2;

  default:
    break;
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
  if ( strcmp(vl_module->def_name(), "GTECH_FD2") == 0 ) {
    // 特例
    // GTECH_FD2( input D, input CP, input CD, output Q, output QN );
    //   D:  データ入力
    //   CP: クロック
    //   CD: 非同期リセット
    //   Q:  データ出力
    //   QN: データ出力(反転)
    MvNode* node = mMvMgr->new_dff1(parent_module, 1);
    ymuint np = vl_module->port_num();
    assert_cond( np == 5, __FILE__, __LINE__);
    for (ymuint i = 0; i < 5; ++ i) {
      const VlPort* vl_port = vl_module->port(i);
      const char* port_name = vl_port->name();
      const VlExpr* expr = vl_port->high_conn();
      if ( strcmp(port_name, "D") == 0 ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 0);
      }
      else if ( strcmp(port_name, "CP") == 0 ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 1);
      }
      else if ( strcmp(port_name, "CD") == 0 ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 2);
      }
      else if ( strcmp(port_name, "Q") == 0 ) {
	connect_lhs(parent_module, expr, node);
      }
      else if ( strcmp(port_name, "QN") == 0 ) {
	if ( expr != NULL ) {
	  MvNode* node1 = mMvMgr->new_not(parent_module, 1);
	  mMvMgr->connect(node, 0, node1, 0);
	  connect_lhs(parent_module, expr, node1);
	}
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    return;
  }
  if ( strcmp(vl_module->def_name(), "GTECH_MUX2") == 0 ) {
    // 特例
    // GTECH_MUX2( input A, input B, input S, output Z );
    //  A, B: データ入力
    //  S   : 選択信号
    //  Z   : データ出力
    MvNode* node = mMvMgr->new_ite(parent_module, 1);
    ymuint np = vl_module->port_num();
    assert_cond( np == 4, __FILE__, __LINE__);
    for (ymuint i = 0; i < 4; ++ i) {
      const VlPort* vl_port = vl_module->port(i);
      const char* port_name = vl_port->name();
      const VlExpr* expr = vl_port->high_conn();
      if ( strcmp(port_name, "A") == 0 ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 1);
      }
      else if ( strcmp(port_name, "B") == 0 ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 2);
      }
      else if ( strcmp(port_name, "S") == 0 ) {
	MvNode* node1 = gen_expr1(parent_module, expr);
	mMvMgr->connect(node1, 0, node, 0);
      }
      else if ( strcmp(port_name, "Z") == 0 ) {
	connect_lhs(parent_module, expr, node);
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }
    return;
  }

  // 通常の処理

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
    const VlExpr* lo = vl_port->low_conn();
    switch ( vl_port->direction() ) {
    case kVpiInput:
      // hi は右辺式
      // lo は左辺式
      {
	MvNode* node1 = gen_expr1(parent_module, hi);
	connect_lhs(parent_module, lo, node1);
      }
      break;
      
    case kVpiOutput:
      // hi は左辺式
      // lo は右辺式
      {
	MvNode* node1 = gen_expr1(parent_module, lo);
	connect_lhs(parent_module, hi, node1);
      }
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


// @brief 左辺式に接続する．
// @param[in] parent_module 親のモジュール
// @param[in] expr 左辺式
// @param[in] node 右辺に対応するノード
void
ReaderImpl::connect_lhs(MvModule* parent_module,
			const VlExpr* expr,
			MvNode* node)
{
  switch ( expr->type() ) {
  case kVpiBitSelect:
    {
      assert_cond( node->output(0)->bit_width() == 1, __FILE__, __LINE__);
      assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
      MvNode* node1 = gen_expr2(expr);
      reg_driver(node1, Driver(node, expr->index_val()));
    }
    break;

  case kVpiNetBit:
    {
      assert_cond( node->output(0)->bit_width() == 1, __FILE__, __LINE__);
      MvNode* node1 = gen_expr2(expr);
      reg_driver(node1, Driver(node, expr->index_val()));
    }
    break;
    
  case kVpiPartSelect:
    {
      assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
      MvNode* node1 = gen_expr2(expr);
      ymuint msb = expr->left_range_val();
      ymuint lsb = expr->right_range_val();
      assert_cond( node->output(0)->bit_width() == msb - lsb + 1,
		   __FILE__, __LINE__);
      reg_driver(node1, Driver(node, msb, lsb));
    }
    break;

  case kVpiOperation:
    {
      assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
      ymuint n = expr->operand_num();
      ymuint offset = 0;
      for (ymuint i = 0; i < n; ++ i) {
	const VlExpr* expr1 = expr->operand(i);
	connect_lhs_sub(parent_module, expr1, node, offset);
	offset += expr1->bit_size();
      }
    }
    break;

  default:
    {
      assert_cond( node->output(0)->bit_width() == expr->bit_size(),
		   __FILE__, __LINE__);
      MvNode* node1 = gen_expr2(expr);
      reg_driver(node1, Driver(node));
    }
    break;
  }
}

// @brief 左辺式に接続する．
// @param[in] parent_module 親のモジュール
// @param[in] expr 左辺式
// @param[in] node 右辺に対応するノード
void
ReaderImpl::connect_lhs_sub(MvModule* parent_module,
			    const VlExpr* expr,
			    MvNode* node,
			    ymuint offset)
{
  switch ( expr->type() ) {
  case kVpiBitSelect:
  case kVpiNetBit:
    {
      MvNode* node1 = gen_expr2(expr);
      ymuint bw = node->output(0)->bit_width();
      MvNode* node2 = mMvMgr->new_constbitselect(parent_module,
						 offset,
						 bw);
      mMvMgr->connect(node, 0, node2, 0);
      reg_driver(node1, Driver(node2, expr->index_val()));
    }
    break;
    
  case kVpiPartSelect:
    {
      MvNode* node1 = gen_expr2(expr);
      ymuint msb = expr->left_range_val();
      ymuint lsb = expr->right_range_val();
      ymuint bw = node->output(0)->bit_width();
      ymuint bw1 = msb - lsb + 1;
      MvNode* node2 = mMvMgr->new_constpartselect(parent_module,
						  offset + bw1 - 1,
						  offset,
						  bw);
      mMvMgr->connect(node, 0, node2, 0);
      reg_driver(node1, Driver(node2, msb, lsb));
    }
    break;

  case kVpiOperation:
    assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
    {
      ymuint n = expr->operand_num();
      ymuint offset1 = 0;
      for (ymuint i = 0; i < n; ++ i) {
	const VlExpr* expr1 = expr->operand(i);
	connect_lhs_sub(parent_module, expr1, node, offset + offset1);
	offset1 += expr1->bit_size();
      }
    }
    break;

  default:
    {
      MvNode* node1 = gen_expr2(expr);
      ymuint bw = node1->input(0)->bit_width();
      MvNode* node2 = NULL;
      if ( bw == 1 ) {
	node2 = mMvMgr->new_constbitselect(parent_module,
					   offset,
					   bw);
      }
      else {
	node2 = mMvMgr->new_constpartselect(parent_module,
					    offset + bw - 1,
					    offset,
					    bw);
      }
      mMvMgr->connect(node, 0, node2, 0);
      mMvMgr->connect(node2, 0, node1, 0);
    }
    break;
  }
}

// @brief プリミティブインスタンスの生成を行う．
// @param[in] prim プリミティブ
// @param[in] parent_module 親のモジュール
void
ReaderImpl::gen_priminst(const VlPrimitive* prim,
			 MvModule* parent_module)
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
      MvNode* node = gen_andtree(parent_module, ni, inputs, 0);
      outputs[0] = node;
    }
    break;
    
  case kVpiNandPrim:
    {
      MvNode* node = gen_andtree(parent_module, ni, inputs, 0);
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;
    
  case kVpiOrPrim:
    {
      MvNode* node = gen_ortree(parent_module, ni, inputs, 0);
      outputs[0] = node;
    }
    break;
    
  case kVpiNorPrim:
    {
      MvNode* node = gen_ortree(parent_module, ni, inputs, 0);
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;
    
  case kVpiXorPrim:
    {
      MvNode* node = gen_xortree(parent_module, ni, inputs, 0);
      outputs[0] = node;
    }
    break;
    
  case kVpiXnorPrim:
    {
      MvNode* node = gen_xortree(parent_module, ni, inputs, 0);
      MvNode* node1 = mMvMgr->new_not(parent_module, 1);
      mMvMgr->connect(node, 0, node1, 0);
      outputs[0] = node1;
    }
    break;
    
  case kVpiCombPrim:
    {
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
    connect_lhs(parent_module, expr, outputs[i]);
  }
  for (ymuint i = 0; i < ni; ++ i) {
    const VlPrimTerm* term = prim->prim_term(pos);
    ++ pos;
    const VlExpr* expr = term->expr();
    MvNode* node = gen_expr1(parent_module, expr);
    const pair<MvNode*, ymuint>& p = inputs[i];
    mMvMgr->connect(node, 0, p.first, p.second);
  }
}

// @brief AND のバランス木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] ni 入力数
// @param[in] inputs 入力ピンを格納する配列
// @param[in] offset inputs のオフセット
MvNode*
ReaderImpl::gen_andtree(MvModule* parent_module,
			ymuint ni,
			vector<pair<MvNode*, ymuint> >& inputs,
			ymuint offset)
{
  assert_cond( ni > 1, __FILE__, __LINE__);

  if ( ni == 2 ) {
    MvNode* node = mMvMgr->new_and(parent_module, 1);
    inputs[offset + 0] = make_pair(node, 0);
    inputs[offset + 1] = make_pair(node, 1);
    return node;
  }

  if ( ni == 3 ) {
    MvNode* node0 = gen_andtree(parent_module, 2, inputs, offset);
    MvNode* node = mMvMgr->new_and(parent_module, 1);
    mMvMgr->connect(node0, 0, node, 0);
    inputs[offset + 2] = make_pair(node, 1);
    return node;
  }

  ymuint nr = ni / 2;
  ymuint nl = ni - nr;

  MvNode* node0 = gen_andtree(parent_module, nl, inputs, offset);
  MvNode* node1 = gen_andtree(parent_module, nr, inputs, offset + nl);

  MvNode* node = mMvMgr->new_and(parent_module, 1);
  mMvMgr->connect(node0, 0, node, 0);
  mMvMgr->connect(node1, 0, node, 1);

  return node;
}

// @brief OR のバランス木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] ni 入力数
// @param[in] inputs 入力ピンを格納する配列
// @param[in] offset inputs のオフセット
MvNode*
ReaderImpl::gen_ortree(MvModule* parent_module,
		       ymuint ni,
		       vector<pair<MvNode*, ymuint> >& inputs,
		       ymuint offset)
{
  assert_cond( ni > 1, __FILE__, __LINE__);

  if ( ni == 2 ) {
    MvNode* node = mMvMgr->new_or(parent_module, 1);
    inputs[offset + 0] = make_pair(node, 0);
    inputs[offset + 1] = make_pair(node, 1);
    return node;
  }

  if ( ni == 3 ) {
    MvNode* node0 = gen_ortree(parent_module, 2, inputs, offset);
    MvNode* node = mMvMgr->new_or(parent_module, 1);
    mMvMgr->connect(node0, 0, node, 0);
    inputs[offset + 2] = make_pair(node, 1);
    return node;
  }

  ymuint nr = ni / 2;
  ymuint nl = ni - nr;

  MvNode* node0 = gen_ortree(parent_module, nl, inputs, offset);
  MvNode* node1 = gen_ortree(parent_module, nr, inputs, offset + nl);

  MvNode* node = mMvMgr->new_or(parent_module, 1);
  mMvMgr->connect(node0, 0, node, 0);
  mMvMgr->connect(node1, 0, node, 1);

  return node;
}

// @brief XOR のバランス木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] ni 入力数
// @param[in] inputs 入力ピンを格納する配列
// @param[in] offset inputs のオフセット
MvNode*
ReaderImpl::gen_xortree(MvModule* parent_module,
			ymuint ni,
			vector<pair<MvNode*, ymuint> >& inputs,
			ymuint offset)
{
  assert_cond( ni > 1, __FILE__, __LINE__);

  if ( ni == 2 ) {
    MvNode* node = mMvMgr->new_xor(parent_module, 1);
    inputs[offset + 0] = make_pair(node, 0);
    inputs[offset + 1] = make_pair(node, 1);
    return node;
  }

  if ( ni == 3 ) {
    MvNode* node0 = gen_xortree(parent_module, 2, inputs, offset);
    MvNode* node = mMvMgr->new_xor(parent_module, 1);
    mMvMgr->connect(node0, 0, node, 0);
    inputs[offset + 2] = make_pair(node, 1);
    return node;
  }

  ymuint nr = ni / 2;
  ymuint nl = ni - nr;

  MvNode* node0 = gen_xortree(parent_module, nl, inputs, offset);
  MvNode* node1 = gen_xortree(parent_module, nr, inputs, offset + nl);

  MvNode* node = mMvMgr->new_xor(parent_module, 1);
  mMvMgr->connect(node0, 0, node, 0);
  mMvMgr->connect(node1, 0, node, 1);

  return node;
}

// @brief 式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
MvNode*
ReaderImpl::gen_expr1(MvModule* parent_module,
		      const VlExpr* expr)
{
  switch ( expr->type() ) {
  case kVpiBitSelect:
  case kVpiNetBit:
   {
      MvNode* node = gen_expr2(expr);
      if ( expr->is_constant_select() ) {
	ymuint bitpos = expr->index_val();
	const MvOutputPin* pin = node->output(0);
	MvNode* node1 = mMvMgr->new_constbitselect(parent_module,
						   bitpos,
						   pin->bit_width());
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }
      else {
	MvNode* node1 = gen_expr1(parent_module, expr->index());
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
    break;

  case kVpiPartSelect:
    {
      MvNode* node = gen_expr2(expr);
      if ( expr->is_constant_select() ) {
	ymuint msb = expr->left_range_val();
	ymuint lsb = expr->right_range_val();
	const MvOutputPin* pin = node->output(0);
	MvNode* node1 = mMvMgr->new_constpartselect(parent_module,
						    msb, lsb,
						    pin->bit_width());
	mMvMgr->connect(node, 0, node1, 0);
	return node1;
      }
      else {
	// まだできてない．
	// というか可変 part_select は VPI がおかしいと思う．
	assert_not_reached(__FILE__, __LINE__);
	return NULL;
      }
    }
    break;

  case kVpiOperation:
    {
      ymuint n = expr->operand_num();
      vector<MvNode*> inputs(n);
      for (ymuint i = 0; i < n; ++ i) {
	inputs[i] = gen_expr1(parent_module, expr->operand(i));
      }

      switch ( expr->op_type() ) {
      case kVpiNullOp:
	break;
	
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
	  MvNode* node = mMvMgr->new_and(parent_module, bw);
	  mMvMgr->connect(inputs[0], 0, node, 0);
	  mMvMgr->connect(inputs[1], 0, node, 1);
	  return node;
	}

      case kVpiBitOrOp:
	{
	  ymuint bw = expr->bit_size();
	  MvNode* node = mMvMgr->new_or(parent_module, bw);
	  mMvMgr->connect(inputs[0], 0, node, 0);
	  mMvMgr->connect(inputs[1], 0, node, 1);
	  return node;
	}

      case kVpiBitXNorOp:
	{
	  ymuint bw = expr->bit_size();
	  MvNode* node = mMvMgr->new_xor(parent_module, bw);
	  mMvMgr->connect(inputs[0], 0, node, 0);
	  mMvMgr->connect(inputs[1], 0, node, 1);
	  MvNode* node1 = mMvMgr->new_not(parent_module, bw);
	  mMvMgr->connect(node, 0, node1, 0);
	  return node1;
	}

      case kVpiBitXorOp:
	{
	  ymuint bw = expr->bit_size();
	  MvNode* node = mMvMgr->new_xor(parent_module, bw);
	  mMvMgr->connect(inputs[0], 0, node, 0);
	  mMvMgr->connect(inputs[1], 0, node, 1);
	  return node;
	}

      case kVpiLogAndOp:
	{
	  MvNode* node = mMvMgr->new_and(parent_module, 1);
	  mMvMgr->connect(inputs[0], 0, node, 0);
	  mMvMgr->connect(inputs[1], 0, node, 1);
	  return node;
	}

      case kVpiLogOrOp:
	{
	  MvNode* node = mMvMgr->new_or(parent_module, 1);
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
	  ymint r;
	  bool stat = expr->operand(0)->eval_int(r);
	  assert_cond( stat , __FILE__, __LINE__);
	  ymuint n = expr->operand_num() - 1;
	  vector<ymuint> bw_array(n * r);
	  for (ymuint i = 0; i < n; ++ i) {
	    for (ymuint j = 0; j < r; ++ j) {
	      bw_array[j * r + i] = inputs[i + 1]->output(0)->bit_width();
	    }
	  }
	  MvNode* node = mMvMgr->new_concat(parent_module, bw_array);
	  for (ymuint i = 0; i < n; ++ i) {
	    for (ymuint j = 0; j < r; ++ j) {
	      mMvMgr->connect(inputs[i + 1], 0, node, j * r + i);
	    }
	  }
	  return node;
	}

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
    }
    break;

  case kVpiConstant:
    {
      assert_not_reached(__FILE__, __LINE__);
    }
    break;

  default:
    return gen_expr2(expr);
  }
  return NULL;
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
MvNode*
ReaderImpl::gen_expr2(const VlExpr* expr)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  ymuint dim = expr->declarray_dimension();
  assert_cond( decl->dimension() == dim, __FILE__, __LINE__);
  if ( dim > 1 ) {
    // 配列型
    ymuint offset = 0;
    ymuint mlt = 1;
    for (ymuint i = 0; i < dim; ++ i) {
      const VlExpr* index = expr->declarray_index(i);
      int index_val;
      bool stat = index->eval_int(index_val);
      assert_cond( stat, __FILE__, __LINE__);
      offset += index_val * mlt;
      mlt *= decl->range(i)->size();
    }
    MvNode* node = mDeclMap.get(decl, offset);
    if ( node == NULL ) {
      cout << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
  else {
    MvNode* node = mDeclMap.get(decl);
    if ( node == NULL ) {
      cout << decl->name() << " is not found in mDeclMap" << endl;
    }
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
}

// @brief ドライバーを登録する．
// @param[in] node 左辺に対応するノード
// @param[in] driver ドライバー
void
ReaderImpl::reg_driver(MvNode* node,
		       const Driver& driver)
{
  if ( debug_driver ) {
    cout << "reg_driver(" << node->id()
	 << ", " << driver.rhs_node()->id();
    if ( driver.has_bitselect() ) {
      cout << "[" << driver.index() << "]";
    }
    else if ( driver.has_partselect() ) {
      cout << "[" << driver.msb() << ":" << driver.lsb() << "]";
    }
    cout << ")" << endl;
  }
  ymuint id = node->id();
  while ( mDriverList.size() <= id ) {
    mDriverList.push_back(vector<Driver>());
  }
  mDriverList[id].push_back(driver);
}

END_NAMESPACE_YM_MVN_VERILOG
