
/// @file libym_verilog/elaborator/ExprGen_primary.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_primary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym_verilog/pt/PtModule.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ym_verilog/vl/VlModule.h"

#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"
#include "ElbExpr.h"
#include "ElbGenvar.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtExpr(primary) から named_event を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_namedevent(const VlNamedObj* parent,
				const PtExpr* pt_expr)
{
  assert_cond(pt_expr->type() == kPtPrimaryExpr, __FILE__, __LINE__);
  assert_cond(pt_expr->left_range() == NULL, __FILE__, __LINE__);
  assert_cond(pt_expr->left_range() == NULL, __FILE__, __LINE__);
  
  const FileRegion& fr = pt_expr->file_region();

  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  // 識別子の名前
  const char* name = pt_expr->name();

  // 名前をキーにして要素を探索する．
  ElbObjHandle* handle = find_obj_up(parent, nb_array, name, NULL);
  if ( !handle ) {
    error_not_found(pt_expr);
    return NULL;
  }

  ElbDecl* decl = handle->decl();
  if ( decl ) {
    if ( decl->type() == kVpiNamedEvent ) {
      // 単独の named event オブジェクトの場合
      return instantiate_namedevent_primary1(parent, pt_expr, decl);
    }
  }

  ElbDeclArray* decl_array = handle->decl_array();
  if ( decl_array ) {
    if ( decl_array->elem_type() == kVpiNamedEvent ) {
      // 配列要素の named event オブジェクトの場合
      return instantiate_namedevent_primary2(parent, pt_expr, decl_array);
    }
  }

  // ここに来たら型が違うということ．
  error_not_a_namedevent(pt_expr);
  return NULL;
}
  
// @brief 単体の NamedEvent 用のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] decl 宣言要素 (NamedEvent であることを期待している)
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_namedevent_primary1(const VlNamedObj* parent,
					 const PtExpr* pt_expr,
					 ElbDecl* decl)
{
  // 添字の部分を実体化する．
  // といってもここでは添字がないことをチェックするだけ．
  bool has_bit_select;
  bool has_range_select;
  ElbExpr* index1;
  ElbExpr* index2;
  bool stat = resolve1(parent, pt_expr, decl, NULL,
		       has_range_select, has_bit_select,
		       index1, index2);
  if ( !stat ) {
    // エラーメッセージは resolve() 内で出力されている．
    return NULL;
  }
    
  return factory().new_Primary(pt_expr, decl);
}

// @brief 配列要素の NamedEvent 用のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] decl 宣言要素 (NamedEvent であることを期待している)
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_namedevent_primary2(const VlNamedObj* parent,
					 const PtExpr* pt_expr,
					 ElbDeclArray* decl_array)
{
  // 添字の部分を実体化する．
  vector<ElbExpr*> index_list;
  bool has_bit_select;
  bool has_range_select;
  ElbExpr* index1;
  ElbExpr* index2;
  bool stat = resolve2(parent, pt_expr, decl_array, NULL,
		       index_list,
		       has_range_select, has_bit_select,
		       index1, index2);
  if ( !stat ) {
    // エラーメッセージは resolve() 内で出力されている．
    return NULL;
  }

  if ( has_bit_select ) {
    // named event はビット選択できない．
    error_dimension_mismatch(pt_expr);
    return NULL;
  }

  return factory().new_Primary(pt_expr, decl_array, index_list);
}

// @brief PtPrimary から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_primary(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr)
{
  if ( env.is_constant() ) {
    // 定数式用のインスタンス化
    return instantiate_const_primary(parent, env, pt_expr);
  }
  if ( env.inside_constant_function() ) {
    // constant function 内の式用のインスタンス化
    return instantiate_cf_primary(parent, env, pt_expr, env.function());
  }
  // 通常のインスタンス化
  return instantiate_normal_primary(parent, env, pt_expr);
}

// @brief 定数式用のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_const_primary(const VlNamedObj* parent,
				   const ElbEnv& env,
				   const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  // 識別子の名前
  const char* name = pt_expr->name();
  
  // 識別子の添字の次元
  int isize = pt_expr->index_num();

  if ( nb_array.size() > 0 ) {
    // 階層つき識別子はだめ
    error_hname_in_ce(pt_expr);
    return NULL;
  }

  // モジュール内の識別子を探索する．
  ElbObjHandle* handle = find_obj_up(parent, PtNameBranchArray(), name,
				     parent->parent_module());
  if ( !handle ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return NULL;
  }
    
  // そのオブジェクトが genvar の場合
  ElbGenvar* genvar = handle->genvar();
  if ( genvar ) {
    return instantiate_genvar(pt_expr, genvar->value());
  }
    
  // それ以外の宣言要素の場合
  // しかしこの場合には parameter でなければならない．
  ElbParameter* param = handle->parameter();
  if ( !param ) {
    error_not_a_parameter(pt_expr);
    return NULL;
  }
  return instantiate_param_primary(parent, env, pt_expr, param);
}

// @brief constant function 内で用いられるプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[in] func 親の関数
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_cf_primary(const VlNamedObj* parent,
				const ElbEnv& env,
				const PtExpr* pt_expr,
				const VlNamedObj* func)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  // 識別子の名前
  const char* name = pt_expr->name();
  
  // 識別子の添字の次元
  int isize = pt_expr->index_num();
  
  if ( nb_array.size() > 0 ) {
    // 階層つき識別子はだめ
    error_hname_in_cf(pt_expr);
    return NULL;
  }
  
  ElbDecl* decl = NULL;
  ElbParameter* param = NULL;
  
  // 名前をキーにして要素を探索する．
  // まず func 内の識別子を探索する．
  ElbObjHandle* handle = find_obj_up(parent, PtNameBranchArray(), name, func);
  if ( handle ) {
    decl = handle->decl();
    param = handle->parameter();
    if ( !decl || !param ) {
      error_illegal_object_cf(pt_expr);
      return NULL;
    }
  }
  else {
    // 関数内で見つからなかった場合，モジュール内の識別子を探索する．
    handle = find_obj_up(parent, PtNameBranchArray(), name,
			 parent->parent_module());
    if ( handle ) {
      // 対象が genvar だった場合
      ElbGenvar* genvar = handle->genvar();
      if ( genvar ) {
	return instantiate_genvar(pt_expr, genvar->value());
      }

      // それ以外の宣言要素の場合
      // しかしこの場合には paramter でなければならない．
      param = handle->parameter();
      if ( !param ) {
	error_not_a_parameter(pt_expr);
	return NULL;
      }
    }
    else {
      // その名前のオブジェクトが存在しなかった．
      error_not_found(pt_expr);
      return NULL;
    }
  }
  if ( decl ) {
    return instantiate_decl_primary(parent, env, pt_expr, decl);
  }
  else {
    return instantiate_param_primary(parent, env, pt_expr, param);
  }
}
  
// @brief 通常のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_normal_primary(const VlNamedObj* parent,
				    const ElbEnv& env,
				    const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  // 識別子の名前
  const char* name = pt_expr->name();
  
  // 識別子の添字の次元
  int isize = pt_expr->index_num();
  
  // 名前をキーにして要素を探索する．
  ElbObjHandle* handle = find_obj_up(parent, nb_array, name, NULL);
  if ( handle ) {
    // genvar の場合
    ElbGenvar* genvar = handle->genvar();
    if ( genvar ) {
      return instantiate_genvar(pt_expr, genvar->value());
    }
  }
  else {
    // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
    // 暗黙の1ビットネット宣言を行う．
    // ただし識別子に添字がついていたらだめ
    const VlModule* parent_module = parent->parent_module();
    tVpiNetType def_nettype = parent_module->def_net_type();
    if ( !pt_expr->is_simple() ||
	 nb_array.size() > 0 ||
	 isize != 0 ||
	 def_nettype == kVpiNone ) {
      error_not_found(pt_expr);
      return NULL;
    }
    ElbDecl* decl = factory().new_ImpNet(parent, pt_expr, def_nettype);
    reg_decl(vpiNet, decl);

    handle = find_obj(parent, name);
    assert_cond(handle, __FILE__, __LINE__);
  }

  if ( env.is_system_tf_arg() ) {
    // システム関数/タスクの引数の場合
    if ( isize == 0 ) {
      const VlNamedObj* scope = handle->obj();
      if ( scope ) {
	return factory().new_ArgHandle(pt_expr, scope);
      }
      ElbPrimitive* primitive = handle->primitive();
      if ( primitive ) {
	return factory().new_ArgHandle(pt_expr, primitive);
      }
      ElbDecl* decl = handle->decl();
      if ( decl ) {
	// net-array などもここに含まれる．
	return factory().new_Primary(pt_expr, decl);
      }
    }
    else if ( isize == 1 ) {
      const PtExpr* pt_expr1 = pt_expr->index(0);
      int index;
      bool stat = evaluate_expr_int(parent, pt_expr1, index);
      if ( !stat ) {
	return NULL;
      }
      const VlNamedObj* scope = handle->array_elem(index);
      if ( scope ) {
	return factory().new_ArgHandle(pt_expr, scope);
      }
      ElbPrimArray* prim_array = handle->prim_array();
      if ( prim_array ) {
	ElbPrimitive* primitive = prim_array->_primitive_by_index(index);
	if ( primitive ) {
	  return factory().new_ArgHandle(pt_expr, primitive);
	}
      }
    }
    error_illegal_object(pt_expr);
    return NULL;
  }
  
  ElbDecl* decl = handle->decl();
  if ( decl ) {
    return instantiate_decl_primary(parent, env, pt_expr, decl);
  }
  
  ElbDeclArray* decl_array = handle->decl_array();
  if ( decl_array ) {
    return instantiate_array_primary(parent, env, pt_expr, decl_array);
  }

  ElbParameter* param = handle->parameter();
  if ( param ) {
    return instantiate_param_primary(parent, env, pt_expr, param);
  }

  // ここまで来たら適切な型ではなかったということ．
  error_illegal_object(pt_expr);
  return NULL;
}

// @brief 単体の宣言要素用のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[in] decl 対象の宣言要素
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_decl_primary(const VlNamedObj* parent,
				  const ElbEnv& env,
				  const PtExpr* pt_expr,
				  ElbDecl* decl)
{
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* index1;
  ElbExpr* index2;
  bool stat = resolve1(parent, pt_expr, decl, NULL,
		       has_range_select, has_bit_select,
		       index1, index2);
  if ( !stat ) {
    // エラーメッセージは resolve() 内で出力されている．
    return NULL;
  }
    
  tVpiObjType type = decl->type();
  if ( !env.is_valid_primary(type, has_bit_select | has_range_select) ) {
    error_illegal_object(pt_expr);
    return NULL;
  }
  
  if ( has_bit_select ) {
    return factory().new_BitSelect(pt_expr, decl, index1);
  }
  if ( has_range_select ) {
    int index1_val;
    if ( !expr_to_int(index1, index1_val) ) {
      return NULL;
    }
    int index2_val;
    if ( !expr_to_int(index2, index2_val) ) {
      return NULL;
    }
    return factory().new_PartSelect(pt_expr, decl,
				    index1, index2,
				    index1_val, index2_val);
  }
  return factory().new_Primary(pt_expr, decl);
}
  
// @brief 配列要素用のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[in] decl_array 対象の配列
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_array_primary(const VlNamedObj* parent,
				   const ElbEnv& env,
				   const PtExpr* pt_expr,
				   ElbDeclArray* decl_array)
{
  vector<ElbExpr*> index_list;
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* index1;
  ElbExpr* index2;
  bool stat = resolve2(parent, pt_expr, decl_array, NULL,
		       index_list,
		       has_range_select, has_bit_select,
		       index1, index2);
  if ( !stat ) {
    // エラーメッセージは resolve() 内で出力されている．
    return NULL;
  }
  
  tVpiObjType type = decl_array->elem_type();
  if ( !env.is_valid_primary(type, has_bit_select | has_range_select) ) {
    error_illegal_object(pt_expr);
    return NULL;
  }
  
  if ( has_bit_select ) {
    return factory().new_BitSelect(pt_expr, decl_array, index_list,
				   index1);
  }
  if ( has_range_select ) {
    int index1_val;
    if ( !expr_to_int(index1, index1_val) ) {
      return NULL;
    }
    int index2_val;
    if ( !expr_to_int(index2, index2_val) ) {
      return NULL;
    }
    return factory().new_PartSelect(pt_expr, decl_array, index_list,
				    index1, index2,
				    index1_val, index2_val);
  }
  return factory().new_Primary(pt_expr, decl_array, index_list);
}

// @brief parameter 宣言用のプライマリ式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[in] param 対象の parameter 宣言
// @return 生成された式を返す．
// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
ElbExpr*
ExprGen::instantiate_param_primary(const VlNamedObj* parent,
				   const ElbEnv& env,
				   const PtExpr* pt_expr,
				   ElbParameter* param)
{
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* index1;
  ElbExpr* index2;
  bool stat = resolve1(parent, pt_expr, param, NULL,
		       has_range_select, has_bit_select,
		       index1, index2);
  if ( !stat ) {
    // エラーメッセージは resolve() 内で出力されている．
    return NULL;
  }
    
  tVpiObjType type = param->type();
  if ( !env.is_valid_primary(type, has_bit_select | has_range_select) ) {
    error_illegal_object(pt_expr);
    return NULL;
  }
  
  if ( has_bit_select ) {
    return factory().new_BitSelect(pt_expr, param, index1);
  }
  if ( has_range_select ) {
    int index1_val;
    if ( !expr_to_int(index1, index1_val) ) {
      return NULL;
    }
    int index2_val;
    if ( !expr_to_int(index2, index2_val) ) {
      return NULL;
    }
    return factory().new_PartSelect(pt_expr, param,
				    index1, index2,
				    index1_val, index2_val);
  }
  return factory().new_Primary(pt_expr, param);
}

// @brief PtPrimary から左辺用の ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_lhs_primary(const VlNamedObj* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  // 識別子の名前
  const char* name = pt_expr->name();
  
  // 識別子の添字の次元
  ymuint32 isize = pt_expr->index_num();
  
  ElbDecl* decl = NULL;
  ElbDeclArray* decl_array = NULL;
  
  if ( env.inside_constant_function() ) {
    // constant function 中の式の場合
    if ( nb_array.size() > 0 ) {
      // 階層つき識別子はだめ
      error_hname_in_cf(pt_expr);
      return NULL;
    }
  
    // 親の関数
    const VlNamedObj* func = env.function();
    assert_cond(func, __FILE__, __LINE__);
  
    // 名前をキーにして func 内の要素を探索する．
    ElbObjHandle* handle = find_obj_up(parent, PtNameBranchArray(), name, func);
    if ( !handle ) {
      error_not_found(pt_expr);
      return NULL;
    }
    decl = handle->decl();
    decl_array = handle->decl_array();
  }
  else {
    // 名前をキーにして要素を探索する．
    ElbObjHandle* handle = find_obj_up(parent, nb_array, name, NULL);
    if ( handle  ) {
      decl = handle->decl();
      decl_array = handle->decl_array();
    }
    else {
      // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
      // 暗黙の1ビットネット宣言を行う．
      // ただし識別子に添字がついていたらだめ
      const VlModule* parent_module = parent->parent_module();
      tVpiNetType def_nettype = parent_module->def_net_type();
      if ( !pt_expr->is_simple() ||
	   nb_array.size() > 0 ||
	   isize != 0 ||
	   def_nettype == kVpiNone ) {
	error_not_found(pt_expr);
	return NULL;
      }
      decl = factory().new_ImpNet(parent, pt_expr, def_nettype);
      reg_decl(vpiNet, decl);
    }
  }
  
  if ( decl ) {
    // 添字の部分を実体化する．
    bool has_range_select;
    bool has_bit_select;
    ElbExpr* index1;
    ElbExpr* index2;
    bool stat = resolve1(parent, pt_expr, decl, NULL,
			 has_range_select, has_bit_select,
			 index1, index2);
    if ( !stat ) {
      // エラーメッセージは resolve() 内で出力されている．
      return NULL;
    }
    
    tVpiObjType type = decl->type();
    
    // かなり野暮ったいコード
    // でも変に凝るよりシンプルのほうがいいでしょ．
    if ( env.is_net_lhs() ) {
      // net/net-array 型のみが有効
      if ( type != kVpiNet ) {
	error_illegal_object(pt_expr);
	return NULL;
      }
    }
    else if ( env.is_var_lhs() ) {
      // reg/reg-array/var 型の宣言要素のみが有効
      if ( type != kVpiReg &&
	   type != kVpiIntegerVar &&
	   type != kVpiRealVar &&
	   type != kVpiTimeVar ) {
	error_illegal_object(pt_expr);
	return NULL;
      }
    }
    else if ( env.is_pca_lhs() ) {
      // reg/var 型の宣言要素のみが有効
      if ( type != kVpiReg &&
	   type != kVpiIntegerVar &&
	   type != kVpiRealVar &&
	   type != kVpiTimeVar ) {
	error_illegal_object(pt_expr);
	return NULL;
      }
      if ( has_range_select || has_bit_select ) {
	error_select_in_pca(pt_expr);
	return NULL;
      }
    }
    else if ( env.is_force_lhs() ) {
      // net/reg/var 型の宣言要素のみが有効
      if ( type != kVpiNet &&
	   type != kVpiReg &&
	   type != kVpiIntegerVar &&
	   type != kVpiRealVar &&
	   type != kVpiTimeVar ) {
	error_illegal_object(pt_expr);
	return NULL;
      }
      if ( has_range_select || has_bit_select ) {
	error_select_in_force(pt_expr);
	return NULL;
      }
    }

    if ( has_bit_select ) {
      return factory().new_BitSelect(pt_expr, decl, index1);
    }
    if ( has_range_select ) {
      int index1_val;
      if ( !expr_to_int(index1, index1_val) ) {
	return NULL;
      }
      int index2_val;
      if ( !expr_to_int(index2, index2_val) ) {
	return NULL;
      }
      return factory().new_PartSelect(pt_expr, decl, index1, index2,
				      index1_val, index2_val);
    }
    return factory().new_Primary(pt_expr, decl);
  }
  else if ( decl_array ) {
    if ( env.is_pca_lhs() ) {
      error_array_in_pca(pt_expr);
      return NULL;
    }
    if ( env.is_force_lhs() ) {
      error_array_in_force(pt_expr);
      return NULL;
    }
    if ( decl_array->dimension() != isize ) {
      error_dimension_mismatch(pt_expr);
      return NULL;
    }

    // 添字の部分を実体化する．
    vector<ElbExpr*> index_list;
    bool has_range_select;
    bool has_bit_select;
    ElbExpr* index1;
    ElbExpr* index2;
    bool stat = resolve2(parent, pt_expr, decl_array, NULL,
			 index_list,
			 has_range_select, has_bit_select,
			 index1, index2);
    if ( !stat ) {
      // エラーメッセージは resolve() 内で出力されている．
      return NULL;
    }

    tVpiObjType type = decl_array->elem_type();
    // かなり野暮ったいコード
    // でも変に凝るよりシンプルのほうがいいでしょ．
    if ( env.is_net_lhs() ) {
      // net/net-array 型のみが有効
      if ( type != kVpiNet ) {
	error_illegal_object(pt_expr);
	return NULL;
      }
    }
    else if ( env.is_var_lhs() ) {
      // reg/reg-array/var 型の宣言要素のみが有効
      if ( type != kVpiReg &&
	   type != kVpiIntegerVar &&
	   type != kVpiRealVar &&
	   type != kVpiTimeVar ) {
	error_illegal_object(pt_expr);
	return NULL;
      }
    }

    if ( has_bit_select ) {
      return factory().new_BitSelect(pt_expr, decl_array, index_list,
				     index1);
    }
    else if ( has_range_select ) {
      int index1_val;
      if ( !expr_to_int(index1, index1_val) ) {
	return NULL;
      }
      int index2_val;
      if ( !expr_to_int(index2, index2_val) ) {
	return NULL;
      }
      return factory().new_PartSelect(pt_expr, decl_array, index_list,
				      index1, index2,
				      index1_val, index2_val);
    }
    else {
      return factory().new_Primary(pt_expr, decl_array, index_list);
    }
  }

  // ここに来たら decl も decl_array も NULL だったということ．
  error_illegal_object(pt_expr);
  return NULL;
}

// @brief genvar に対応した定数を生成する．
// @param[in] pt_expr 式を表すパース木
// @param[in] val 値
// @note pt_expr に添字が付いていたらエラーとなる．
ElbExpr*
ExprGen::instantiate_genvar(const PtExpr* pt_expr,
			    int val)
{
  if ( pt_expr->index_num() != 0 ) {
    // genvar は配列型ではない．
    error_dimension_mismatch(pt_expr);
    return NULL;
  }
  // genvar に対応した定数式を生成
  return factory().new_GenvarConstant(pt_expr, val);
}

// @brief 添字の部分を実体化する．(単体のオブジェクト用)
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] obj 対象のオブジェクト
// @param[in] func 親の function
// @param[out] has_range_select 範囲指定を持っていたら true を返す．
// @param[out] has_bit_select ビット指定を持っていたら true を返す．
// @param[out] index1, index2 範囲指定/ビット指定の式を返す．
bool
ExprGen::resolve1(const VlNamedObj* parent,
		  const PtExpr* pt_expr,
		  VlDecl* decl,
		  const VlNamedObj* func,
		  bool& has_range_select,
		  bool& has_bit_select,
		  ElbExpr*& index1,
		  ElbExpr*& index2)
{
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  ymuint32 isize = pt_expr->index_num();

  // 範囲指定があるとき true となるフラグ
  has_range_select = (pt_expr->left_range() && pt_expr->right_range());
  
  // ビット指定があるとき true となるフラグ
  has_bit_select = false;
  
  if ( isize == 1 && !has_range_select ) {
    // 識別子の添字の次元と decl_obj の配列の次元が 1 違いで
    // 範囲がなければ識別子の最後の添字はビット指定と見なす．
    has_bit_select = true;
    -- isize;
  }
  
  if ( isize != 0 ) {
    error_dimension_mismatch(pt_expr);
    return false;
  }
  
  index1 = NULL;
  index2 = NULL;
  
  bool const_mode = pt_expr->is_const_index() || decl->is_consttype();
  if ( has_bit_select ) {
    if ( decl->value_type() == kVpiValueReal ) {
      error_select_for_real(pt_expr);
      return false;
    }
    const PtExpr* pt_expr1 = pt_expr->index(0);
    if ( const_mode ) {
      index1 = instantiate_constant_expr(parent, pt_expr1);
    }
    else if ( func ) {
      ElbConstantFunctionEnv env(func);
      index1 = instantiate_expr(parent, env, pt_expr1);
    }
    else {
      index1 = instantiate_expr(parent, ElbEnv(), pt_expr1);
    }
    if ( !index1 ) {
      return false;
    }
  }
  else if ( has_range_select ) {
    if ( decl->value_type() == kVpiValueReal ) {
      error_select_for_real(pt_expr);
      return false;
    }
    if ( const_mode || ( pt_expr->range_mode() == ':' ) ) {
      index1 = instantiate_constant_expr(parent, pt_expr->left_range());
    }
    else if ( func ) {
      ElbConstantFunctionEnv env(func);
      index1 = instantiate_expr(parent, env, pt_expr->left_range());
    }
    else {
      index1 = instantiate_expr(parent, ElbEnv(), pt_expr->left_range());
    }
    if ( !index1 ) {
      return false;
    }
    
    // 範囲の2番目の式は常に定数でなければならない．
    index2 = instantiate_constant_expr(parent, pt_expr->right_range());
    if ( !index2 ) {
      return false;
    }
  }

  return true;
}

// @brief 添字の部分を実体化する．(配列要素のオブジェクト用)
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] obj 対象の配列型オブジェクト
// @param[in] func 親の function
// @param[out] index_list インデックス式のリスト
// @param[out] has_range_select 範囲指定を持っていたら true を返す．
// @param[out] has_bit_select ビット指定を持っていたら true を返す．
// @param[out] index1, index2 範囲指定/ビット指定の式を返す．
bool
ExprGen::resolve2(const VlNamedObj* parent,
		  const PtExpr* pt_expr,
		  VlDecl* decl,
		  const VlNamedObj* func,
		  vector<ElbExpr*>& index_list,
		  bool& has_range_select,
		  bool& has_bit_select,
		  ElbExpr*& index1,
		  ElbExpr*& index2)
{
  // 配列の次元
  ymuint32 dsize = decl->dimension();
  
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  ymuint32 isize = pt_expr->index_num();

  // 範囲指定があるとき true となるフラグ
  has_range_select = (pt_expr->left_range() && pt_expr->right_range());
  
  // ビット指定があるとき true となるフラグ
  has_bit_select = false;
  
  if ( isize == dsize + 1 && !has_range_select ) {
    // 識別子の添字の次元と decl_obj の配列の次元が 1 違いで
    // 範囲がなければ識別子の最後の添字はビット指定と見なす．
    has_bit_select = true;
  }
  else if ( isize != dsize ) {
    error_dimension_mismatch(pt_expr);
    return false;
  }
  
  index_list.clear();
  index_list.reserve(dsize);
  
  index1 = NULL;
  index2 = NULL;
  
  bool const_mode = pt_expr->is_const_index() || decl->is_consttype();
  for (ymuint32 i = 0; i < isize; ++ i) {
    const PtExpr* pt_expr1 = pt_expr->index(i);
    ElbExpr* expr1;
    if ( const_mode ) {
      expr1 = instantiate_constant_expr(parent, pt_expr1);
    }
    else if ( func ) {
      ElbConstantFunctionEnv env(func);
      expr1 = instantiate_expr(parent, env, pt_expr1);
    }
    else {
      expr1 = instantiate_expr(parent, ElbEnv(), pt_expr1);
    }
    if ( !expr1 ) {
      return false;
    }
    // ビット指定の式は除いて考える．
    if ( i < dsize ) {
      index_list.push_back(expr1);
    }
    else {
      index1 = expr1;
    }
  }

  if ( has_bit_select ) {
    if ( decl->value_type() == kVpiValueReal ) {
      error_select_for_real(pt_expr);
      return false;
    }
  }
  else if ( has_range_select ) {
    if ( decl->value_type() == kVpiValueReal ) {
      error_select_for_real(pt_expr);
      return false;
    }
    if ( const_mode || ( pt_expr->range_mode() == ':' ) ) {
      index1 = instantiate_constant_expr(parent, pt_expr->left_range());
    }
    else if ( func ) {
      ElbConstantFunctionEnv env(func);
      index1 = instantiate_expr(parent, env, pt_expr->left_range());
    }
    else {
      index1 = instantiate_expr(parent, ElbEnv(), pt_expr->left_range());
    }
    if ( !index1 ) {
      return false;
    }
    
    // 範囲の2番目の式は常に定数でなければならない．
    index2 = instantiate_constant_expr(parent, pt_expr->right_range());
    if ( !index2 ) {
      return false;
    }
  }

  return true;
}

// @brief オブジェクトが存在しない場合のエラーメッセージを生成する．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_found(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX", "Not found.");
}

// @brief オブジェクトの型が不適切だった場合のエラーメッセージを生成する．
void
ExprGen::error_illegal_object(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX", "Illegal object type.");
}
  
// @brief オブジェクトの型が constant function 用として不適切だった場合のエラー生成．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_illegal_object_cf(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Illegal object type inside constant function.");
}

// @brief 階層名が constant expression 中にあった場合のエラーメッセージを生成する．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_hname_in_ce(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Hierarchical name shall not be used in constant expression.");
}

// @brief 階層名が constant function 中にあった場合のエラーメッセージを生成する．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_hname_in_cf(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Hierarchical name shall not be used inside constant function.");
}

// @brief オブジェクトが parameter でなかった場合のエラーメッセージを生成する．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_a_parameter(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX", "Not a parameter.");
}
  
// @brief オブジェクトが named-event でなかった場合のエラー生成．
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_not_a_namedevent(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX", "Not a named-event.");
}

// @brief assign/deassign に不適切なビット/範囲指定のエラー生成
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_select_in_pca(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Bit/part-select shall not be used"
	       "in LHS of assign/deassign statement.");
}

// @brief force/release に不適切なビット/範囲指定のエラー生成
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_select_in_force(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Bit/part-select shall not be used"
	       "in LHS of force/release statement.");
}

// @brief assign/deassign に不適切な配列要素のエラー生成
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_array_in_pca(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Array element shall not be used"
	       "in LHS of assign/deassign statement.");
}

// @brief force/release に不適切な配列要素のエラー生成
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_array_in_force(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Array element shall not be used"
	       "in LHS of force/release statement.");
}

// @brief 配列の次元が合わない場合のエラーメッセージを生成する．
void
ExprGen::error_dimension_mismatch(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX", "Dimension mismatch.");
}

// @brief real 型に対するビット選択あるいは部分選択があった場合のエラー生成
// @param[in] pt_expr 式を表すパース木
void
ExprGen::error_select_for_real(const PtExpr* pt_expr)
{
  error_common(pt_expr, "ELABXXX",
	       "Bit-select/Part-select for real.");
}

// @brief エラーメッセージを生成する共通部分
// @param[in] pt_expr 式を表すパース木
// @param[in] label エラーラベル
// @param[in] msg エラーメッセージ
void
ExprGen::error_common(const PtExpr* pt_expr,
		      const char* label,
		      const char* msg)
{
  ostringstream buf;
  buf << expand_full_name(pt_expr->namebranch_array(), pt_expr->name())
      << " : " << msg;
  put_msg(__FILE__, __LINE__,
	  pt_expr->file_region(),
	  kMsgError,
	  label,
	  buf.str());
}

END_NAMESPACE_YM_VERILOG
