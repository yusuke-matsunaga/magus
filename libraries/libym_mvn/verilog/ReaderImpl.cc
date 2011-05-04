
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
#include "ym_mvn/MvnMgr.h"
#include "ym_mvn/MvnModule.h"
#include "ym_mvn/MvnNode.h"
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

#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using namespace nsYm::nsVerilog;


BEGIN_NONAMESPACE

const
bool debug_driver = true;

END_NONAMESPACE


// @brief コンストラクタ
ReaderImpl::ReaderImpl() :
  mGlobalEnv(mDeclHash)
{
  mMvnMgr = NULL;
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
ReaderImpl::gen_network(MvnMgr& mgr,
			MvnVlMap& node_map)
{
  if ( MsgMgr::error_num() > 0 ) {
    return false;
  }

  mVlMgr.elaborate();

  if ( MsgMgr::error_num() > 0 ) {
    return false;
  }

  mMvnMgr = &mgr;

  mIODeclMap.clear();
  mDeclHash.clear();
  mGlobalEnv.clear();
  mNodeMap.clear();
  mDriverList.clear();

  MvnModule* module0 = NULL;
  list<const VlModule*> tmp_list(mVlMgr.topmodule_list());
  for (list<const VlModule*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const VlModule* vl_module = *p;

    if ( vl_module->is_cell_instance() ) continue;

    // module を実体化
    MvnModule* module = gen_module(vl_module);
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
  ymuint n = mMvnMgr->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvnNode* node = mMvnMgr->_node(i);
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
	MvnNode* ud_node = mMvnMgr->new_const(module0, 1, val);
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
      MvnNode* src_node = driver.rhs_node();
      mMvnMgr->connect(src_node, 0, node, 0);
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
      MvnNode* node1 = mMvnMgr->new_concat(module0, bw_array);
      mMvnMgr->connect(node1, 0, node, 0);
      for (ymuint i = 0; i < nd; ++ i) {
	const Driver& driver = tmp2[i];
	mMvnMgr->connect(driver.rhs_node(), 0, node1, i);
      }
    }
  }

  // 冗長な through ノードを取り除く
  {
    for (ymuint i = 0; i < n; ++ i) {
      MvnNode* node = mMvnMgr->_node(i);
      if ( node != NULL && node->type() == MvnNode::kThrough ) {
	const MvnOutputPin* src_pin = node->input(0)->src_pin();
	if ( src_pin != NULL ) {
	  MvnNode* src_node = src_pin->node();
	  mNodeMap.copy(node->id(), src_node->id());
	}
      }
    }
  }

  // 冗長な through ノードを削除する．
  mMvnMgr->sweep();

  node_map = mNodeMap;

  return true;
}

// @brief module を生成する．
// @param[in] vl_module 対象のモジュール
MvnModule*
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
      MsgMgr::put_msg(__FILE__, __LINE__,
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

  MvnModule* module = mMvnMgr->new_module(vl_module->name(), np,
					ibw_array, obw_array, iobw_array);

  // 入出力ノードの対応表を作る．
  ymuint i1 = 0;
  ymuint i2 = 0;
  ymuint i3 = 0;
  for (ymuint i = 0; i < nio_all; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    const VlDecl* decl = io->decl();
    MvnNode* node = NULL;
    switch ( io->direction() ) {
    case kVpiInput:
      node = module->input(i1);
      ++ i1;
      break;

    case kVpiOutput:
      node = module->output(i2);
      ++ i2;
      break;

    case kVpiInout:
      node = module->inout(i3);
      ++ i3;
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    reg_ionode(decl, node);
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
    if ( expr->is_operation() ) {
      assert_cond( expr->op_type() == kVpiConcatOp, __FILE__, __LINE__);
      ymuint n = expr->operand_num();
      mMvnMgr->init_port(module, i, port->name(), n);
      for (ymuint j = 0; j < n; ++ j) {
	MvnNode* node;
	ymuint msb;
	ymuint lsb;
	switch ( gen_portref(expr->operand(j), node, msb, lsb) ) {
	case 0:
	  mMvnMgr->set_port_ref(module, i, j, node);
	  break;

	case 1:
	  mMvnMgr->set_port_ref(module, i, j, node, msb);
	  break;

	case 2:
	  mMvnMgr->set_port_ref(module, i, j, node, msb, lsb);
	  break;

	default:
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
    }
    else {
      mMvnMgr->init_port(module, i, port->name(), 1);
      MvnNode* node;
      ymuint msb;
      ymuint lsb;
      switch ( gen_portref(expr, node, msb, lsb) ) {
      case 0:
	mMvnMgr->set_port_ref(module, i, 0, node);
	break;

      case 1:
	mMvnMgr->set_port_ref(module, i, 0, node, msb);
	break;

      case 2:
	mMvnMgr->set_port_ref(module, i, 0, node, msb, lsb);
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
ReaderImpl::gen_decl(MvnModule* module,
		     const VlNamedObj* vl_scope)
{
  // ネットの生成
  {
    vector<const VlDecl*> net_list;
    if ( mVlMgr.find_decl_list(vl_scope, vpiNet, net_list) ) {
      for (vector<const VlDecl*>::iterator p = net_list.begin();
	   p != net_list.end(); ++ p) {
	const VlDecl* vl_decl = *p;
	// IO に接続している要素は生成済みなので除外する．
	if ( mGlobalEnv.get(vl_decl) == NULL ) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvnNode* node = mMvnMgr->new_through(module, bw);
	  reg_node(vl_decl, node);
	}
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
	  MvnNode* node = mMvnMgr->new_through(module, bw);
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
	// IO に接続している要素は生成済みなので除外する．
	if ( mGlobalEnv.get(vl_decl) == NULL ) {
	  // 仮の through ノードに対応させる．
	  ymuint bw = vl_decl->bit_size();
	  MvnNode* node = mMvnMgr->new_through(module, bw);
	  reg_node(vl_decl, node);
	  (void) mDeclHash.get_id(vl_decl);
	}
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
	  MvnNode* node = mMvnMgr->new_through(module, bw);
	  reg_node(vl_decl, i, node);
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
	bool stat = gen_decl(module, vl_scope1);
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
			MvnNode*& node,
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

// @brief 左辺式に接続する．
// @param[in] dst_node 左辺に対応するノード
// @param[in] expr 左辺式
// @param[in] src_node 右辺に対応するノード
void
ReaderImpl::connect_lhs(MvnNode* dst_node,
			const VlExpr* expr,
			MvnNode* src_node)
{
  if ( expr->is_primary() ) {
    reg_driver(dst_node, Driver(src_node));
  }
  else if ( expr->is_bitselect() ) {
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    const VlDeclBase* decl = expr->decl_base();
    assert_cond( decl != NULL, __FILE__, __LINE__);
    ymuint index = decl->bit_offset(expr->index_val());
    reg_driver(dst_node, Driver(src_node, index));
  }
  else if ( expr->is_partselect() ) {
    assert_cond( expr->is_constant_select(), __FILE__, __LINE__);
    const VlDeclBase* decl = expr->decl_base();
    assert_cond( decl != NULL, __FILE__, __LINE__);
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
MvnNode*
ReaderImpl::gen_rhs(MvnModule* parent_module,
		    MvnNode* node,
		    ymuint offset,
		    ymuint bit_width)
{
  ymuint src_bw = node->output(0)->bit_width();
  if ( offset + bit_width > src_bw ) {
    cerr << "bit width mismatch" << endl;
    assert_not_reached(__FILE__, __LINE__);
  }
  MvnNode* src_node = NULL;
  if ( offset == 0 && bit_width == src_bw ) {
    // 全範囲の選択
    src_node = node;
  }
  else if ( bit_width == 1 ) {
    src_node = mMvnMgr->new_constbitselect(parent_module,
					  offset,
					  src_bw);
    mMvnMgr->connect(node, 0, src_node, 0);
  }
  else {
    src_node = mMvnMgr->new_constpartselect(parent_module,
					   offset,
					   offset + bit_width - 1,
					   src_bw);
    mMvnMgr->connect(node, 0, src_node, 0);
  }
  return src_node;
}

// @brief 宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素
// @param[in] node 登録するノード
void
ReaderImpl::reg_node(const VlDecl* decl,
		     MvnNode* node)
{
  mGlobalEnv.add(decl, node);
  mNodeMap.reg_node(node->id(), decl);
}

// @brief 入出力宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素
// @param[in] node 登録するノード
void
ReaderImpl::reg_ionode(const VlDecl* decl,
		       MvnNode* node)
{
  mIODeclMap.add(decl, node);
  mGlobalEnv.add(decl, node);
  mNodeMap.reg_node(node->id(), decl);
}

// @brief 宣言要素に対応するノードを登録する．
// @param[in] decl 宣言要素(配列型)
// @param[in] offset オフセット
// @param[in] node 登録するノード
void
ReaderImpl::reg_node(const VlDeclArray* decl,
		     ymuint offset,
		     MvnNode* node)
{
  mGlobalEnv.add(decl, offset, node);
  mNodeMap.reg_node(node->id(), decl, offset);
}

// @brief ドライバーを登録する．
// @param[in] node 左辺に対応するノード
// @param[in] driver ドライバー
void
ReaderImpl::reg_driver(MvnNode* node,
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
ReaderImpl::driver_list(MvnNode* node)
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
ReaderImpl::error_drivers(MvnNode* node,
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
