
/// @file libym_verilog/elaborator/ModuleGen.cc
/// @brief ElbMgr の実装ファイル (module のインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ModuleGen.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ModuleGen.h"
#include "ElbEnv.h"
#include "ElbParamCon.h"
#include "ElbStub.h"

#include "ym_utils/FileRegion.h"

#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtPort.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtMisc.h"
#include "ym_verilog/pt/PtArray.h"

#include "ElbModule.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbParamAssign.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ModuleGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
ModuleGen::ModuleGen(Elaborator& elab,
		     ElbMgr& elb_mgr,
		     ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}
 
// @brief デストラクタ
ModuleGen::~ModuleGen()
{
}

// top module 用の骨組みを作る．
// @param[in] toplevel トップレベルのスコープ
// @param[in] pt_module モジュール定義
void
ModuleGen::phase1_topmodule(const VlNamedObj* toplevel,
			    const PtModule* pt_module)
{
  const FileRegion& file_region = pt_module->file_region();
  const char* name = pt_module->name();
  
  ostringstream buf;
  buf << "instantiating top module \"" << name << "\".";
  put_msg(__FILE__, __LINE__,
	  file_region,
	  kMsgInfo,
	  "ELAB",
	  buf.str());

  // モジュール本体の生成
  ElbModule* module = factory().new_Module(toplevel,
					   pt_module,
					   NULL,
					   NULL);
  reg_module(module);
  
  // attribute instance の生成
  //instantiate_attribute(pt_module->attr_top(), true, module);

  ostringstream buf2;
  buf2 << "module \"" << module->full_name() << "\" has been created.";
  put_msg(__FILE__, __LINE__,
	  file_region,
	  kMsgInfo,
	  "ELAB",
	  buf2.str());

  // 中身のうちスコープに関係する要素の生成
  phase1_module_item(module, pt_module, NULL);
}

// @brief module の中身のうちスコープに関係する要素のインスタンス化をする．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
// @param[in] pt_inst_header インスタンス定義のヘッダ
void
ModuleGen::phase1_module_item(ElbModule* module,
			      const PtModule* pt_module,
			      const ElbParamCon* param_con)
{
  // ループチェック用のフラグを立てる．
  pt_module->set_in_use();

  // パラメータポートを実体化する．
  PtDeclHeadArray paramport_array = pt_module->paramport_array();
  bool has_paramportdecl = (paramport_array.size() > 0);
  // もともと外部にパラメータポートリストが無い場合には
  // 内部の parameter 宣言がパラメータポートリストとなる．
  paramport_array = pt_module->paramhead_array();
  instantiate_param(module, paramport_array, false);

  // パラメータの割り当てを作る．
  if ( param_con ) {
    ymuint32 n = param_con->elem_num();
    if ( param_con->named_con() ) {
      // 名前による割り当て
      for (ymuint32 i = 0; i < n; ++ i) {
	const PtConnection* pt_con = param_con->pt_con(i);
	ElbObjHandle* handle = find_obj(module, pt_con->name());
	if ( handle == NULL || handle->type() != kVpiParameter ) {
	  ostringstream buf;
	  buf << param_con->name(i) << " : No such parameter.";
	  put_msg(__FILE__, __LINE__,
		  pt_con->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  continue;
	}
	ElbParameter* param = handle->parameter();
	assert_cond(param, __FILE__, __LINE__);

	ElbExpr* expr = param_con->expr(i);
	param->set_expr(expr);
	ElbParamAssign* pa = factory().new_ParamAssign(module, pt_con,
						       param, expr,
						       true);
	reg_paramassign(pa);
      }
    }
    else {
      // 順序による割り当て

      // パラメータポートリストの名前を現れた順番に paramport_list に入れる．
      vector<const char*> paramport_list;
      for (ymuint32 i = 0; i < paramport_array.size(); ++ i) {
	const PtDeclHead* pt_param = paramport_array[i];
	for (ymuint32 j = 0; j < pt_param->item_num(); ++ j) {
	  const PtDeclItem* pt_item = pt_param->item(j);
	  paramport_list.push_back(pt_item->name());
	}
      }
      if ( paramport_list.size() < n ) {
	put_msg(__FILE__, __LINE__,
		param_con->file_region(),
		kMsgError,
		"ELAB",
		"Too many parameters.");
      }
      else {
	for (ymuint32 i = 0; i < n; ++ i) {
	  const PtConnection* pt_con = param_con->pt_con(i);
	  const char* tmp_name = paramport_list[i];
	  ElbObjHandle* handle = find_obj(module, tmp_name);
	  assert_cond(handle, __FILE__, __LINE__);
	  ElbParameter* param = handle->parameter();
	  assert_cond(param, __FILE__, __LINE__);
	  assert_cond(param->type() == kVpiParameter, __FILE__, __LINE__);
	  
	  ElbExpr* expr = param_con->expr(i);
	  param->set_expr(expr);
	  ElbParamAssign* pa = factory().new_ParamAssign(module, pt_con,
							 param, expr,
							 false);
	  reg_paramassign(pa);
	}
      }
    }
  }
  
  // 内側の parameter 宣言の実体化
  // 外側にパラメータポート宣言リストを持っていた場合には
  // 内側の parameter 宣言が localparam に格下げされる．
  if ( has_paramportdecl ) {
    instantiate_param(module, pt_module->paramhead_array(), true);
  }
  else {
    // パラメータポート宣言リストがない場合には上ですでに実体化している．
  }
  
  // localparam を実体化する．
  instantiate_param(module, pt_module->localparamhead_array(), true);
  
  // それ以外の要素を実体化する．
  phase1_item(module, pt_module->item_array());
  
  // phase2 で行う処理を登録しておく．
  add_phase2stub(module, pt_module);
  
  // ループチェック用のフラグを下ろす．
  pt_module->reset_in_use();
}

// @brief module の中身のインスタンス化を行う．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
void
ModuleGen::phase2_module_item(ElbModule* module,
			      const PtModule* pt_module)
{
  // 宣言要素を実体化する．
  instantiate_decl(module, pt_module->declhead_array());

  // IODecl を実体化する．
  instantiate_iodecl(module, pt_module);
  
  // ポートを実体化する
  instantiate_port(module, pt_module);
}

// @brief モジュール用のIO宣言要素をインスタンス化する．
// @param[in] modle モジュール
// @param[in] pt_header パース木のIO宣言ヘッダ
// @param[in] pt_item パース木のIO宣言要素
void
ModuleGen::instantiate_iodecl(ElbModule* module,
			      const PtModule* pt_module)
{
  ymuint32 index = 0;
  PtIOHeadArray io_array = pt_module->iohead_array();
  for (ymuint32 i = 0; i < io_array.size(); ++ i) {
    const PtIOHead* pt_head = io_array[i];
    tVpiAuxType def_aux_type = pt_head->aux_type();
    bool sign = pt_head->is_signed();
  
    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(module, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      continue;
    }
    
    ElbIOHead* head = factory().new_ModIOHead(module, pt_head);

    for (ymuint32 j = 0; j < pt_head->item_num(); ++ j, ++ index) {
      const PtIOItem* pt_item = pt_head->item(j);

      // IO定義と変数/ネット定義が一致しているか調べる．
      ElbObjHandle* handle = find_obj(module, pt_item->name());
      ElbDecl* decl = NULL;
      if ( handle ) {
	// 同名の要素が見つかった．
	if ( def_aux_type != kVpiAuxNone ) {
	  // なのに IO 宣言の aux_type もある．
	  ostringstream buf;
	  buf << pt_item->name() << " : has an aux-type declaration"
	      << ", while it also has another declaration in "
	      << handle->file_region() << ".";
	  put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  return;
	}
	decl = handle->decl();
	if ( decl ) {
	  tVpiObjType type = decl->type();
	  if ( type != kVpiNet &&
	       type != kVpiReg &&
	       type != kVpiIntegerVar &&
	       type != kVpiTimeVar) {
	    decl = NULL;
	  }
	}
	if ( !decl ) {
	  ostringstream buf;
	  buf << pt_item->name() << " should be a net/reg/variable.";
	  put_msg(__FILE__, __LINE__,
		  pt_item->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  continue;
	}
	
	if ( decl->dimension() > 0 ) {
	  ostringstream buf;
	  buf << pt_item->name()
	      << ": Array object shall not be connected to IO port.";
	  put_msg(__FILE__, __LINE__,
		  decl->file_region(),
		  kMsgError,
		  "ELAB",
		  buf.str());
	  continue;
	}
	// decl と型が一致しているか調べる．
	// IEEE 1364-2001 12.3.3 Port declarations
	ElbExpr* left2 = decl->_left_range();
	ElbExpr* right2 = decl->_right_range();
	if ( left2 && right2 ) {
	  if ( left == NULL && right == NULL ) {
	    // decl は範囲を持っているが IO は持っていない．
	    // これはエラーにしなくてよいのだろうか？
	    // たぶんコンパイルオプションで制御すべき
	    if ( allow_empty_io_range() ) {
	      left = left2;
	      right = right2;
	    }
	    else {
	      ostringstream buf;
	      buf << "Conflictive range declaration of \""
		  << pt_item->name() << "\".";
	      put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgError,
		      "ELAB",
		      buf.str());
	      continue;
	    }
	  }
	  else {
	    int left2_val;
	    bool stat1 = expr_to_int(left2, left2_val);
	    assert_cond(stat1, __FILE__, __LINE__);
	    int right2_val;
	    bool stat2 = expr_to_int(right2, right2_val);
	    assert_cond(stat2, __FILE__, __LINE__);
	    if ( left_val != left2_val || right_val != right2_val ) {
	      ostringstream buf;
	      buf << "Conflictive range declaration of \""
		  << pt_item->name() << "\".";
	      put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgError,
		      "ELAB",
		      buf.str());
	      cout << "IO range: [" << left_val << ":" << right_val << "]" << endl
		   << "Decl range: [" << left2_val << ":" << right2_val << "]" << endl;
	      continue;
	    }
	  }
	}
	else {
	  if ( left && right ) {
	    // decl は範囲を持っていないが IO は持っている．
	    // エラーとする．
	    ostringstream buf;
	    buf << "Conflictive range declaration of \""
		<< pt_item->name() << "\".";
	    put_msg(__FILE__, __LINE__,
		    pt_item->file_region(),
		    kMsgError,
		    "ELAB",
		    buf.str());
	    continue;
	  }
	}
	// どちらか一方でも符号付きなら両方符号付きにする．
	// ちょっと ad-hoc な仕様
	if ( !decl->is_signed() && sign ) {
	  decl->set_signed();
	}
      }
      else {
	// 同名の要素が見つからなかったので作る必要がある．
	tVpiNetType net_type = pt_head->net_type();
	tVpiAuxType aux_type = def_aux_type;
	if ( aux_type == kVpiAuxNone ) {
	  // 型指定が無い場合
	  // モジュール IO の場合は `default_net_type を参照する．
	  net_type = module->def_net_type();
	  if ( net_type == kVpiNone ) {
	    ostringstream buf;
	    buf << pt_item->name() << " : implicit declaration is inhibited "
		<< " because default_nettype = \"none\".";
	    put_msg(__FILE__, __LINE__,
		    pt_item->file_region(),
		    kMsgError,
		    "ELAB",
		    buf.str());
	    continue;
	  }
	  aux_type = kVpiAuxNet;
	}

	ElbDeclHead* head = factory().new_DeclHead(module, pt_head, aux_type,
						   left, right);
	
	const PtExpr* pt_init = pt_item->init_value();
	ElbExpr* init = NULL;
	if ( pt_init ) {
	  // 初期値を持つ場合
	  if ( aux_type == kVpiAuxNet ) {
	    // net 型の場合(ここに来るのは暗黙宣言のみ)は初期値を持てない．
	    ostringstream buf;
	    buf << pt_item->name()
		<< " : implicit net declaration cannot have initial value.";
	    put_msg(__FILE__, __LINE__,
		    pt_item->file_region(),
		    kMsgError,
		    "ELAB",
		    buf.str());
	    continue;
	  }
	  // これは verilog_grammer.yy の list_of_variable_port_identifiers
	  // に対応するので必ず constant_expression である．
	  init = instantiate_constant_expr(module, pt_init);
	  // エラーの場合には init = NULL となるが処理は続ける．
	}
	
	decl = factory().new_Decl(head, pt_item, init);
	int tag;
	switch ( aux_type ) {
	case kVpiAuxNet: tag = vpiNet; break;
	case kVpiAuxReg: tag = vpiReg; break;
	case kVpiAuxVar: tag = vpiVariables; break;
	default:
	  assert_not_reached(__FILE__, __LINE__);
	}
	reg_decl(tag, decl);
      }

      module->init_iodecl(index, head, pt_item, decl);
      
      ostringstream buf2;
      buf2 << "IODecl(" << pt_item->name() << ")@"
	   << module->full_name() << " created.";
      put_msg(__FILE__, __LINE__,
	      pt_head->file_region(),
	      kMsgInfo,
	      "ELAB",
	      buf2.str());
    }
  }
}

// port の生成を行う．
void
ModuleGen::instantiate_port(ElbModule* module,
			    const PtModule* pt_module)
{
  ymuint32 port_num = pt_module->port_num();
  for (ymuint32 index = 0; index < port_num; ++ index) {
    const PtPort* pt_port = pt_module->port(index);
    // 内側の接続と向きを作る．
    ymuint32 n = pt_port->portref_num();

    ElbExpr* low_conn = NULL;
    tVpiDirection dir = kVpiNoDirection;
  
    if ( n == 1 ) {
      // 単一の要素の場合
      const PtPortRef* pt_portref = pt_port->portref(0);
      dir = pt_portref->dir();
      low_conn = instantiate_portref(module, pt_portref);
    }
    else if ( n > 1 ) {
      // 複数要素の結合の場合
      ElbExpr** expr_list = factory().new_ExprList(n);
      
      for (ymuint32 i = 0; i < n; ++ i) {
	const PtPortRef* pt_portref = pt_port->portref(i);
	ElbExpr* portexpr = instantiate_portref(module, pt_portref);
	if ( !portexpr ) {
	  return;
	}
	expr_list[i] = portexpr;
	
	tVpiDirection dir1 = pt_portref->dir();
	if ( dir == kVpiNoDirection ) {
	  dir = dir1;
	}
	else if ( dir != dir1 ) {
	  dir = kVpiMixedIO;
	}
      }
      
      low_conn = factory().new_ConcatOp(pt_port, n, expr_list);
    }
    module->init_port(index, pt_port, low_conn, dir);
  }
}

// PtPortRef から expression を生成する．
ElbExpr*
ModuleGen::instantiate_portref(ElbModule* module,
			       const PtPortRef* pt_portref)
{
  const char* name = pt_portref->name();
  ElbObjHandle* handle = find_obj(module, name);
  if ( !handle ) {
    return NULL;
  }

  ElbDecl* decl = handle->decl();
  if ( !decl ) {
    return NULL;
  }
  
  if ( decl->dimension() > 0 ) {
    ostringstream buf;
    buf << decl->full_name()
	<< ": Array shall not be connected to a module port.";
    put_msg(__FILE__, __LINE__,
	    pt_portref->file_region(),
	    kMsgError,
	    "ELAB",
	    buf.str());
    return NULL;
  }
  
  // 添字の部分を実体化する．
  const PtExpr* pt_index = pt_portref->index();
  const PtExpr* pt_left = pt_portref->left_range();
  const PtExpr* pt_right = pt_portref->right_range();
  if ( pt_index ) {
    ElbExpr* index1 = instantiate_constant_expr(module, pt_index);
    if ( !index1 ) {
      return NULL;
    }
    return factory().new_BitSelect(pt_portref, decl, index1);
  }
  else if ( pt_left && pt_right ) {
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(module, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      return NULL;
    }
    return factory().new_PartSelect(pt_portref, decl,
				    left, right,
				    left_val, right_val);
  }
  else {
    return factory().new_Primary(pt_portref, decl);
  }
}

END_NAMESPACE_YM_VERILOG
