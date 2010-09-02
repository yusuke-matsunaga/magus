
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

#include "ym_verilog/pt/PtItem.h"

#include "ElbTaskFunc.h"

#include "ElbStub.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;
#define dout cout

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// task/function 関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief task/function の生成を行う．
// @param[in] parent 親のヘッダ
// @param[in] pt_item タスク/関数定義
void
ItemGen::phase1_tf(const VlNamedObj* parent,
		   const PtItem* pt_item)
{
  if ( debug ) {
    dout << endl
	 << "phase1_tf( "
	 << parent->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(parent) << dec
	 << "], "
	 << pt_item->name() << " )" << endl
	 << endl;
  }
  
  ElbTask* task = NULL;
  ElbFunction* func = NULL;
  VlNamedObj* namedobj = NULL;

  if ( pt_item->type() == kPtItem_Task ) {
    task = factory().new_Task(parent, pt_item);
    reg_task(task);
    namedobj = task;
  }
  else {
    assert_cond( pt_item->type() == kPtItem_Func, __FILE__, __LINE__);

    const PtExpr* pt_left = pt_item->left_range();
    const PtExpr* pt_right = pt_item->right_range();

    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;

    if ( !instantiate_range(parent,
			    pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      return;
    }
    func = factory().new_Function(parent,
				  pt_item,
				  left, right,
				  left_val, right_val);
    reg_function(func);
    namedobj = func;
  }

  // parameter の生成
  instantiate_param(namedobj, pt_item->paramhead_array(), false);

  // localparam の生成
  instantiate_param(namedobj, pt_item->localparamhead_array(), true);
  
  // attribute instance の生成
  //instantiate_attribute(pt_item->attr_top(), false, namedobj);

  ostringstream buf;
  buf << "instantiating task/func : " << namedobj->full_name() << ".";
  put_msg(__FILE__, __LINE__,
	  pt_item->file_region(),
	  kMsgInfo,
	  "ELAB",
	  buf.str());

  // 本体のステートメント内部のスコープの生成
  const PtStmt* pt_body = pt_item->body();
  phase1_stmt(namedobj, pt_body);

  // 残りの仕事は phase2, phase3 で行う．
  add_phase2stub(make_stub(this, &ItemGen::phase2_tf,
			   task, func, pt_item));
  add_phase3stub(make_stub(this, &ItemGen::phase3_tf,
			   task, func, pt_item));

  if ( debug ) {
    dout << "phase1_tf end" << endl
	 << endl;
  }
}

// @param[in] task/function 内の宣言要素の生成を行う．
// @param[in] task タスク本体
// @param[in] func 関数本体
// @param[in] pt_item パース木のタスク/関数定義
void
ItemGen::phase2_tf(ElbTask* task,
		   ElbFunction* func,
		   const PtItem* pt_item)
{
  assert_cond( task == NULL || func == NULL, __FILE__, __LINE__);
  assert_cond( task != NULL || func != NULL, __FILE__, __LINE__);

  VlNamedObj* namedobj = task;
  if ( namedobj == NULL ) {
    namedobj = func;
  }

  if ( debug ) {
    dout << endl
	 << "phase2_tf( "
	 << namedobj->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(namedobj) << dec
	 << "] )" << endl
	 << endl;
  }

  // 宣言要素の生成
  instantiate_decl(namedobj, pt_item->declhead_array());

  // 入出力の生成
  instantiate_iodecl(NULL, task, func, pt_item->iohead_array());

  if ( func ) {
    // 関数名と同名の変数の生成
    ElbExpr* left = func->_left_range();
    ElbExpr* right = func->_right_range();
    int left_val = func->left_range_const();
    int right_val = func->right_range_const();
    ElbDeclHead* head = factory().new_DeclHead(func, pt_item,
					       left, right,
					       left_val, right_val);
    ElbDecl* decl = factory().new_Decl(head, pt_item);
    int tag = vpiVariables;
    if ( pt_item->data_type() == kVpiVarNone ) {
      tag = vpiReg;
    }
    reg_decl(tag, decl);

    func->set_ovar(decl);
  }

  if ( debug ) {
    dout << "phase2_tf end" << endl
	 << endl;
  }
}

// @param[in] task/function 内のステートメントの生成を行う．
// @param[in] task task 本体
// @param[in] func 関数本体
// @param[in] pt_item パース木のタスク/関数定義
void
ItemGen::phase3_tf(ElbTask* task,
		   ElbFunction* func,
		   const PtItem* pt_item)
{
  assert_cond( task == NULL || func == NULL, __FILE__, __LINE__);
  assert_cond( task != NULL || func != NULL, __FILE__, __LINE__);

  VlNamedObj* namedobj = task;
  if ( namedobj == NULL ) {
    namedobj = func;
  }

  if ( debug ) {
    dout << endl
	 << "phase3_tf( "
	 << namedobj->full_name()
	 << " ["
	 << hex << reinterpret_cast<ympuint>(namedobj) << dec
	 << "] )" << endl
	 << endl;
  }

  // 本体のステートメントの生成
  ElbTfEnv env(task, func);
  const PtStmt* pt_body = pt_item->body();
  ElbStmt* body = instantiate_stmt(namedobj, NULL, env, pt_body);
  if ( body ) {
    if ( task ) {
      task->set_stmt(body);
    }
    else {
      func->set_stmt(body);
    }
  }

  if ( debug ) {
    dout << "phase3_tf end" << endl
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
  // 基本的には phase1_tf(), phase2_tf(), phase3_tf() と同じことを
  // やっているが，ElbConstantFunctionEnv を用いているところが異なる．
  if ( debug ) {
    dout << endl
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
  instantiate_iodecl(NULL, NULL, func, pt_function->iohead_array());

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
    dout << "instantiate_constant_function end" << endl
	 << endl;
  }

  return func;
}

END_NAMESPACE_YM_VERILOG
