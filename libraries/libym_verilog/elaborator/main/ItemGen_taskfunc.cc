
/// @file libym_verilog/elaborator/ItemGen_taskfunc.cc
/// @brief ElbMgr の実装ファイル(タスク/関数の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ItemGen_taskfunc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ItemGen.h"
#include "ElbEnv.h"

#include "ym_verilog/BitVector.h"

#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ElbTaskFunc.h"
#include "ElbDecl.h"
#include "ElbExpr.h"

#include "ElbStub.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// task/function 関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief task の生成を行う．
// @param[in] parent 親のヘッダ
// @param[in] pt_task タスク定義
void
ItemGen::phase1_task(const VlNamedObj* parent,
		     const PtItem* pt_task)
{
  if ( debug ) {
    cout << endl
	 << "phase1_task( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "], "
	 << pt_task->name() << " )" << endl
	 << endl;
  }
  
  ElbTask* task = factory().new_Task(parent, pt_task);
  reg_task(task);

  // parameter の生成
  instantiate_param(task, pt_task->paramhead_array(), false);

  // localparam の生成
  instantiate_param(task, pt_task->localparamhead_array(), true);
  
  // attribute instance の生成
  //instantiate_attribute(pt_task->attr_top(), false, task);

  ostringstream buf;
  buf << "instantiating task : " << task->full_name() << ".";
  put_msg(__FILE__, __LINE__,
	  pt_task->file_region(),
	  kMsgInfo,
	  "ELAB",
	  buf.str());

  // 本体のステートメント内部のスコープの生成
  const PtStmt* pt_body = pt_task->body();
  phase1_stmt(task, pt_body);

  // 残りの仕事は phase2, phase3 で行う．
  add_phase2stub(make_stub(this, &ItemGen::phase2_task, task, pt_task));
  add_phase3stub(make_stub(this, &ItemGen::phase3_task, task, pt_task));

  if ( debug ) {
    cout << "phase1_task end" << endl
	 << endl;
  }
}

// @brief function の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_function パース木の関数定義
void
ItemGen::phase1_function(const VlNamedObj* parent,
			 const PtItem* pt_function)
{
  if ( debug ) {
    cout << endl
	 << "phase1_function( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "], "
	 << pt_function->name() << " )" << endl
	 << endl;
  }
  
  const PtExpr* pt_left = pt_function->left_range();
  const PtExpr* pt_right = pt_function->right_range();

  ElbExpr* left = NULL;
  ElbExpr* right = NULL;
  int left_val = 0;
  int right_val = 0;
  if ( !instantiate_range(parent, pt_left, pt_right,
			  left, right, left_val, right_val) ) {
    return;
  }
  ElbFunction* func = factory().new_Function(parent,
					     pt_function,
					     left, right,
					     left_val, right_val);
  reg_function(func);

  // parameter の生成
  instantiate_param(func, pt_function->paramhead_array(), false);

  // localparam の生成
  instantiate_param(func, pt_function->localparamhead_array(), true);
  
  // attribute instance の生成
  //instantiate_attribute(pt_function->attr_top(), false, func);

  ostringstream buf;
  buf << "instantiating function: " << func->full_name() << ".";
  put_msg(__FILE__, __LINE__,
	  pt_function->file_region(),
	  kMsgInfo,
	  "ELAB",
	  buf.str());
  
#if 1
  // 関数名と同名の変数の生成
  ElbDeclHead* head = factory().new_DeclHead(func, pt_function,
					     left, right,
					     left_val, right_val);
  ElbDecl* decl = factory().new_Decl(head, pt_function);
  int tag = vpiVariables;
  if ( pt_function->data_type() == kVpiVarNone ) {
    tag = vpiReg;
  }
  reg_decl(tag, decl);
  
  func->set_ovar(decl);
#endif
  
  // 本体のステートメント内部のスコープの生成
  const PtStmt* pt_body = pt_function->body();
  phase1_stmt(func, pt_body);

  // 残りの仕事は phase2, phase3 で行う．
  add_phase2stub(make_stub(this, &ItemGen::phase2_function,
			   func, pt_function));
  add_phase3stub(make_stub(this, &ItemGen::phase3_function,
			   func, pt_function));

  if ( debug ) {
    cout << "phase1_function end" << endl
	 << endl;
  }
}

// @param[in] task の生成を行う．
// @param[in] task タスク本体
// @param[in] pt_item パース木の関数定義
void
ItemGen::phase2_task(ElbTask* task,
		     const PtItem* pt_item)
{
  if ( debug ) {
    cout << endl
	 << "phase2_task( "
	 << task->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(task) << dec
	 << "] )" << endl
	 << endl;
  }
  
  // 宣言要素の生成
  instantiate_decl(task, pt_item->declhead_array());

  // 入出力の生成
  instantiate_iodecl(task, pt_item->iohead_array());

  if ( debug ) {
    cout << "phase2_task end" << endl
	 << endl;
  }
}

// @param[in] function の生成を行う．
// @param[in] func 関数本体
// @param[in] pt_item パース木の関数定義
void
ItemGen::phase2_function(ElbFunction* func,
			 const PtItem* pt_item)
{
  if ( debug ) {
    cout << endl
	 << "phase2_function( "
	 << func->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(func) << dec
	 << "] )" << endl
	 << endl;
  }
  
  // 宣言要素の生成
  instantiate_decl(func, pt_item->declhead_array());

  // 入出力の生成
  instantiate_iodecl(func, pt_item->iohead_array());

#if 0
  // 関数名と同名の変数の生成
  ElbDeclHead* head = factory().new_DeclHead(func, pt_item,
					     left, right,
					     left_val, right_val);
  ElbDecl* decl = factory().new_Decl(head, pt_item);
  int tag = vpiVariables;
  if ( pt_function->data_type() == kVpiVarNone ) {
    tag = vpiReg;
  }
  reg_decl(tag, decl);
  
  func->set_ovar(decl);
#endif
  
  if ( debug ) {
    cout << "phase2_function end" << endl
	 << endl;
  }
}

// @param[in] task 内のステートメントの生成を行う．
// @param[in] task task 本体
// @param[in] pt_item パース木の関数定義
void
ItemGen::phase3_task(ElbTask* task,
		     const PtItem* pt_item)
{
  if ( debug ) {
    cout << endl
	 << "phase3_task( "
	 << task->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(task) << dec
	 << "] )" << endl
	 << endl;
  }
  
  // 本体のステートメントの生成
  ElbTaskEnv env(task);
  const PtStmt* pt_body = pt_item->body();
  ElbStmt* body = instantiate_stmt(task, NULL, env, pt_body);
  if ( body ) {
    task->set_stmt(body);
  }
  
  if ( debug ) {
    cout << "phase3_task end" << endl
	 << endl;
  }
}

// @param[in] function 内のステートメントの生成を行う．
// @param[in] func 関数本体
// @param[in] pt_item パース木の関数定義
void
ItemGen::phase3_function(ElbFunction* func,
			 const PtItem* pt_item)
{
  if ( debug ) {
    cout << endl
	 << "phase3_function( "
	 << func->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(func) << dec
	 << "] )" << endl
	 << endl;
  }
  
  // 本体のステートメントの生成
  ElbFunctionEnv env(func);
  const PtStmt* pt_body = pt_item->body();
  ElbStmt* body = instantiate_stmt(func, NULL, env, pt_body);
  if ( body ) {
    func->set_stmt(body);
  }
  
  if ( debug ) {
    cout << "phase3_function end" << endl
	 << endl;
  }
}

// @brief constant function の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_function 関数定義
ElbFunction*
ItemGen::instantiate_constant_function(const VlNamedObj* parent,
				       const PtItem* pt_function)
{
  if ( debug ) {
    cout << endl
	 << "instantiate_constant_function( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "] , "
	 << pt_function->name() << " )" << endl
	 << endl;
  }
  
  const PtExpr* pt_left = pt_function->left_range();
  const PtExpr* pt_right = pt_function->right_range();
  ElbExpr* left = NULL;
  ElbExpr* right = NULL;
  int left_val = 0;
  int right_val = 0;
  if ( !instantiate_range(parent, pt_left, pt_right,
			  left, right,
			  left_val, right_val) ) {
    return NULL;
  }
  ElbFunction* func = factory().new_Function(parent,
					     pt_function,
					     left, right,
					     left_val, right_val);
  // 登録しておく．
  reg_constant_function(parent, pt_function->name(), func);

  // parameter の生成
  instantiate_param(func, pt_function->paramhead_array(), false);

  // localparam の生成
  instantiate_param(func, pt_function->localparamhead_array(), true);

  // 宣言要素の生成
  instantiate_decl(func, pt_function->declhead_array());

  // 関数名と同名の変数の生成
  ElbDeclHead* head = factory().new_DeclHead(func, pt_function,
					     left, right,
					     left_val, right_val);
  ElbDecl* decl = factory().new_Decl(head, pt_function);
  int tag = vpiVariables;
  if ( pt_function->data_type() == kVpiVarNone ) {
    tag = vpiReg;
  }
  reg_decl(tag, decl);
  
  func->set_ovar(decl);
  
  // 入出力の生成
  instantiate_iodecl(func, pt_function->iohead_array());
  
  // 本体のステートメント内部のスコープの生成
  const PtStmt* pt_body = pt_function->body();
  phase1_stmt(func, pt_body, true);

  // 本体のステートメントの生成
  ElbConstantFunctionEnv env(func);
  ElbStmt* body = instantiate_stmt(func, NULL, env, pt_body);
  if ( body ) {
    func->set_stmt(body);
  }

  if ( debug ) {
    cout << "instantiate_constant_function end" << endl
	 << endl;
  }
  
  return func;
}

// @brief task 用のIO宣言要素をインスタンス化する．
// @brief IO宣言要素を実体化する．
// @param[in] task 親のタスク
// @param[in] pt_head_array IO宣言ヘッダの配列
void
ItemGen::instantiate_iodecl(ElbTask* task,
			    PtIOHeadArray pt_head_array)
{
  ymuint32 index = 0;
  for (ymuint32 i = 0; i < pt_head_array.size(); ++ i) {
    const PtIOHead* pt_head = pt_head_array[i];
    tVpiAuxType def_aux_type = pt_head->aux_type();
    bool sign = pt_head->is_signed();

    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(task, pt_left, pt_right,
			    left, right, left_val, right_val) ) {
      continue;
    }
    
    ElbIOHead* head = factory().new_IOHead(task, pt_head);

    for (ymuint32 j = 0; j < pt_head->item_num(); ++ j) {
      const PtIOItem* pt_item = pt_head->item(j);

      // IO定義と変数/ネット定義が一致しているか調べる．
      ElbObjHandle* handle = find_obj(task, pt_item->name());
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
	  continue;
	}
	decl = handle->decl();
	if ( decl ) {
	  tVpiObjType type = decl->type();
	  if ( type != kVpiReg &&
	       type != kVpiIntegerVar &&
	       type != kVpiTimeVar) {
	    decl = NULL;
	  }
	}
	if ( !decl ) {
	  ostringstream buf;
	  buf << handle->full_name() << ": Not a reg orinteger/time variable.";
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
	tVpiAuxType aux_type = def_aux_type;
	if ( aux_type == kVpiAuxNone ) {
	  // 型指定が無い場合は reg 型となる．
	  aux_type = kVpiAuxReg;
	}
	
	// task/function の IO 宣言には初期値はない．
	const PtExpr* pt_init = pt_item->init_value();
	assert_cond( pt_init == NULL, __FILE__, __LINE__);
	
	ElbDeclHead* head = factory().new_DeclHead(task, pt_head, aux_type,
						   left, right);
	decl = factory().new_Decl(head, pt_item);
	int tag = 0;
	switch ( aux_type ) {
	case kVpiAuxNet: tag = vpiNet; break;
	case kVpiAuxReg: tag = vpiReg; break;
	case kVpiAuxVar: tag = vpiVariables; break;
	default:
	  assert_not_reached(__FILE__, __LINE__);
	}
	reg_decl(tag, decl);
      }

      task->init_iodecl(index, head, pt_item, decl);

      ostringstream buf;
      buf << "IODecl(" << pt_item->name() << ")@"
	  << task->full_name() << " created.";
      put_msg(__FILE__, __LINE__,
	      pt_head->file_region(),
	      kMsgInfo,
	      "ELAB",
	      buf.str());
    }
  }
}

// @brief function 用のIO宣言要素をインスタンス化する．
// @brief IO宣言要素を実体化する．
// @param[in] func 親の function
// @param[in] pt_head_array IO宣言ヘッダの配列
void
ItemGen::instantiate_iodecl(ElbFunction* func,
			    PtIOHeadArray pt_head_array)
{
  ymuint32 index = 0;
  for (ymuint32 i = 0; i < pt_head_array.size(); ++ i) {
    const PtIOHead* pt_head = pt_head_array[i];
    tVpiAuxType def_aux_type = pt_head->aux_type();
    bool sign = pt_head->is_signed();

    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(func, pt_left, pt_right,
			    left, right, left_val, right_val) ) {
      continue;
    }
    
    ElbIOHead* head = factory().new_IOHead(func, pt_head);

    for (ymuint32 j = 0; j < pt_head->item_num(); ++ j) {
      const PtIOItem* pt_item = pt_head->item(j);

      // IO定義と変数/ネット定義が一致しているか調べる．
      ElbObjHandle* handle = find_obj(func, pt_item->name());
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
	  continue;
	}
	decl = handle->decl();
	if ( decl ) {
	  tVpiObjType type = decl->type();
	  if ( type != kVpiReg &&
	       type != kVpiIntegerVar &&
	       type != kVpiTimeVar) {
	    decl = NULL;
	  }
	}
	if ( !decl ) {
	  ostringstream buf;
	  buf << handle->full_name() << ": Not a reg orinteger/time variable.";
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
	tVpiAuxType aux_type = def_aux_type;
	if ( aux_type == kVpiAuxNone ) {
	  // 型指定が無い場合は reg 型となる．
	  aux_type = kVpiAuxReg;
	}
	
	// task/function の IO 宣言には初期値はない．
	const PtExpr* pt_init = pt_item->init_value();
	assert_cond( pt_init == NULL, __FILE__, __LINE__);
	
	ElbDeclHead* head = factory().new_DeclHead(func, pt_head, aux_type,
						   left, right);
	decl = factory().new_Decl(head, pt_item);
	int tag = 0;
	switch ( aux_type ) {
	case kVpiAuxNet: tag = vpiNet; break;
	case kVpiAuxReg: tag = vpiReg; break;
	case kVpiAuxVar: tag = vpiVariables; break;
	default:
	  assert_not_reached(__FILE__, __LINE__);
	}
	reg_decl(tag, decl);
      }

      func->init_iodecl(index, head, pt_item, decl);

      ostringstream buf;
      buf << "IODecl(" << pt_item->name() << ")@"
	  << func->full_name() << " created.";
      put_msg(__FILE__, __LINE__,
	      pt_head->file_region(),
	      kMsgInfo,
	      "ELAB",
	      buf.str());
    }
  }
}

END_NAMESPACE_YM_VERILOG
