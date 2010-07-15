
/// @file libym_mvn/verilog/ReaderImpl.cc
/// @brief ReaderImpl の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ReaderImpl.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlPort.h"
#include "ym_verilog/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

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

  mMvMgr = &mgr;
  
  list<const VlModule*> tmp_list(mVlMgr.topmodule_list());
  for (list<const VlModule*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const VlModule* vl_module = *p;

    // module を実体化
    MvModule* module = gen_module(vl_module);
    if ( module == NULL ) {
      return false;
    }
  }

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
  hash_map<ympuint, MvNode*> iomap;
  ymuint i1 = 0;
  ymuint i2 = 0;
  for (ymuint i = 0; i < nio; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:
      iomap.insert(make_pair(reinterpret_cast<ympuint>(io), module->input(i1)));
      ++ i1;
      break;

    case kVpiOutput:
      iomap.insert(make_pair(reinterpret_cast<ympuint>(io), module->output(i2)));
      ++ i2;
      break;

    default:
      break;
    }
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
	switch ( gen_portref(expr->operand(j), iomap, node, msb, lsb) ) {
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
      switch ( gen_portref(expr, iomap, node, msb, lsb) ) {
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

  // モジュールの要素を生成する．
  bool stat = gen_scopeitem(module, vl_module);
  if ( !stat ) {
    return NULL;
  }

#if 0
  // 接続を行う．
  bool stat = link_scopeitem(vl_module);
#endif
  
  return module;
}

// @brief scope item を生成する．
// @param[in] module モジュール
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::gen_scopeitem(MvModule* module,
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
	// 仮の through ノードに対応させる．
	ymuint bw = vl_decl->bit_size();
	MvNode* node = mMvMgr->new_through(module, bw);
	
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
#if 0
	// 仮の through ノードに対応させる．
	ymuint bw = vl_decl->bit_size();
	MvNode* node = mMvMgr->new_through(bw);
#endif 
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
	bool stat = gen_scopeitem(module, vl_scope1);
	if ( !stat ) {
	  return false;
	}
      }
    }
  }

  // モジュールインスタンスの生成
  {
    vector<const VlModule*> module_list;
    if ( mVlMgr.find_module_list(vl_scope, module_list) ) {
      for (vector<const VlModule*>::iterator p = module_list.begin();
	   p != module_list.end(); ++ p) {
	const VlModule* vl_module1 = *p;
	MvModule* module1 = gen_module(vl_module1);
	if ( module1 == NULL ) {
	  return false;
	}
	ymuint np = vl_module1->port_num();
	for (ymuint i = 0; i < np; ++ i) {
	  const VlPort* vl_port = vl_module1->port(i);
	  
	}
	MvNode* node = mMvMgr->new_inst(module, module1);
      }
    }
  }

  // モジュール配列インスタンスの生成
  {
    vector<const VlModuleArray*> modulearray_list;
    if ( mVlMgr.find_modulearray_list(vl_scope, modulearray_list) ) {
      for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	   p != modulearray_list.end(); ++ p) {
	const VlModuleArray* vl_scope_array = *p;
	ymuint n = vl_scope_array->elem_num();
	for (ymuint i = 0; i < n; ++ i) {
	  const VlModule* vl_scope1 = vl_scope_array->elem_by_offset(i);
	  MvModule* module1 = gen_module(vl_scope1);
	  if ( module1 == NULL ) {
	    return false;;
	  }
	  MvNode* node1 = mMvMgr->new_inst(module, module1);
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
	}
      }
    }
  }
  
  return true;
}

#if 0
// @brief scope item 間の接続を行う．
// @param[in] vl_scope 対象のスコープ
// @retval true 成功した．
// @retval false エラーが起こった．
bool
ReaderImpl::link_scopeitem(const nsVerilog::VlNamedObj* vl_scope)
{
  using namespace nsVerilog;

  // モジュールインスタンスの生成
  {
    vector<const VlModule*> module_list;
    if ( mVlMgr.find_module_list(vl_scope, module_list) ) {
      for (vector<const VlModule*>::iterator p = module_list.begin();
	   p != module_list.end(); ++ p) {
	const VlModule* vl_scope1 = *p;
	MvModule* module1 = gen_module(vl_scope1);
	if ( module1 == NULL ) {
	  return false;
	}
	MvNode* node = mMvMgr->new_inst(module, module1);
      }
    }
  }

  // モジュール配列インスタンスの生成
  {
    vector<const VlModuleArray*> modulearray_list;
    if ( mVlMgr.find_modulearray_list(vl_scope, modulearray_list) ) {
      for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	   p != modulearray_list.end(); ++ p) {
	const VlModuleArray* vl_scope_array = *p;
	ymuint n = vl_scope_array->elem_num();
	for (ymuint i = 0; i < n; ++ i) {
	  const VlModule* vl_scope1 = vl_scope_array->elem_by_offset(i);
	  MvModule* module1 = gen_module(vl_scope1);
	  if ( module1 == NULL ) {
	    return false;;
	  }
	  MvNode* node1 = mMvMgr->new_inst(module, module1);
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
	}
      }
    }
  }

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
#if 0
	// 仮の through ノードに対応させる．
	ymuint bw = vl_decl->bit_size();
	MvNode* node = mMvMgr->new_through(bw);
#endif 
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
	bool stat = gen_scopeitem(module, vl_scope1);
	if ( !stat ) {
	  return false;
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
	
      }
    }
  }
  
  return true;
}

#endif

// @brief portref の実体化を行う．
// @param[in] expr 対象の式
// @param[in] iomap 入出力ノードの対応表
// @param[out] node 対応するノードを格納する変数
// @param[out] msb ビット指定位置か範囲指定の MSB を格納する変数
// @param[out] lsb 範囲指定の LSB を格納する変数
// @retval 0 単純な形式だった．
// @retval 1 ビット指定形式だった．
// @retval 2 範囲指定形式だった．
int
ReaderImpl::gen_portref(const VlExpr* expr,
			const hash_map<ympuint, MvNode*>& iomap,
			MvNode*& node,
			ymuint& msb,
			ymuint& lsb)
{
  const VlDecl* decl = expr->decl_obj();
  assert_cond( decl != NULL, __FILE__, __LINE__);
  ympuint tmp = reinterpret_cast<ympuint>(decl);
  hash_map<ympuint, MvNode*>::const_iterator p = iomap.find(tmp);
  assert_cond( p != iomap.end(), __FILE__, __LINE__);
  node = p->second;

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

END_NAMESPACE_YM_MVN_VERILOG
