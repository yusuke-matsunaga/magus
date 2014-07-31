
/// @file ExprGen_primary.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "YmVerilog/pt/PtModule.h"
#include "YmVerilog/pt/PtExpr.h"
#include "YmVerilog/pt/PtMisc.h"

#include "YmVerilog/vl/VlModule.h"
#include "YmVerilog/vl/VlDeclArray.h"
#include "YmVerilog/vl/VlRange.h"

#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"
#include "ElbExpr.h"
#include "ElbGenvar.h"

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtPrimary から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_primary(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  bool has_hname = (nb_array.size() > 0);
  if ( has_hname ) {
    if ( env.is_constant() ) {
      // 階層つき識別子はだめ
      error_hname_in_ce(pt_expr);
      return NULL;
    }
    if ( env.inside_constant_function() ) {
      // 階層つき識別子はだめ
      error_hname_in_cf(pt_expr);
    }
  }

  // 識別子の名前
  const char* name = pt_expr->name();

  // 識別子の添字の次元
  int isize = pt_expr->index_num();

  // 名前に対応したオブジェクトのハンドル
  ElbObjHandle* handle = NULL;

  if ( env.is_constant() ) {
    handle = find_const_handle(parent, pt_expr);
    if ( handle == NULL ) {
      // エラーが起こった．
      // メッセージは find_const_handle() 内で出力されている．
      return NULL;
    }
  }
  else if ( env.inside_constant_function() ) {
    // まず関数内の識別子を探索する．
    handle = find_obj_up(parent, nb_array, name, env.constant_function());
    if ( handle == NULL && !env.is_lhs() ) {
      // 右辺ならモジュール内の定数識別子を探索する．
      handle = find_const_handle(parent, pt_expr);
    }
    if ( handle == NULL ) {
      // エラーが起こった．
      // メッセージは find_const_handle() 内で出力されている．
      return NULL;
    }
  }
  else {
    // 通常のスコープで探索する．
    // たぶんモジュール内でいいはず．
    handle = find_obj_up(parent, nb_array, name,
			 parent->parent_module());
    if ( handle == NULL ) {
      // 見つからなくてもデフォルトネットタイプが kVpiNone でないかぎり
      // 暗黙の1ビットネット宣言を行う．
      // ただし識別子に添字がついていたらだめ
      const VlModule* parent_module = parent->parent_module();
      tVpiNetType def_nettype = parent_module->def_net_type();
      if ( pt_expr->is_simple() &&
	   !has_hname &&
	   isize == 0 &&
	   def_nettype != kVpiNone ) {
	ElbDecl* decl = factory().new_ImpNet(parent, pt_expr, def_nettype);
	reg_decl(vpiNet, decl);

	handle = find_obj(parent, name);
	assert_cond(handle, __FILE__, __LINE__);
      }
    }
    if ( handle == NULL ) {
      error_not_found(pt_expr);
      return NULL;
    }
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
	return factory().new_Primary(pt_expr, decl);
      }
      ElbDeclArray* declarray = handle->declarray();
      if ( declarray ) {
	return factory().new_ArgHandle(pt_expr, declarray);
      }
    }
    else if ( isize == 1 ) {
      const PtExpr* pt_expr1 = pt_expr->index(0);
      int index;
      bool stat = evaluate_int(parent, pt_expr1, index, true);
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

  if ( !env.is_lhs() ) {
    // 対象のオブジェクトが genvar の場合
    ElbGenvar* genvar = handle->genvar();
    if ( genvar ) {
      return instantiate_genvar(parent, pt_expr, genvar->value());
    }
  }

  // 添字には constant/constant function 以外の情報は引き継がない
  ElbEnv index_env;
  if ( pt_expr->is_const_index() ) {
    index_env = ElbConstantEnv();
  }
  else if ( env.inside_constant_function() ) {
    index_env = ElbConstantFunctionEnv(env.constant_function());
  }

  // 対象のオブジェクトが宣言要素だった場合
  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* primary = instantiate_primary_sub(handle, parent,
					     index_env, pt_expr,
					     is_array,
					     has_range_select,
					     has_bit_select);
  if ( primary == NULL ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return NULL;
  }

  const VlDeclBase* decl_base = primary->decl_base();
  assert_cond( decl_base != NULL, __FILE__, __LINE__);
  tVpiObjType decl_type = decl_base->type();

  if ( !check_decl(env, pt_expr, decl_type, is_array,
		   has_range_select | has_bit_select) ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return NULL;
  }

  if ( has_bit_select ) {
    // ビット指定付きの場合
    const PtExpr* pt_expr1 = pt_expr->index(isize - 1);
    int index_val;
    bool stat1 = evaluate_int(parent, pt_expr1, index_val, false);
    if ( stat1 ) {
      // 固定インデックスだった．
      ymuint offset;
      bool stat2 = decl_base->calc_bit_offset(index_val, offset);
      if ( !stat2 ) {
	// インデックスが範囲外
	MsgMgr::put_msg(__FILE__, __LINE__,
			pt_expr1->file_region(),
			kMsgWarning,
			"ELAB",
			"Bit-Select index is out of range.");
	// ただ値が X になるだけでエラーにはならないそうだ．
      }
      return factory().new_BitSelect(pt_expr, primary, pt_expr1, index_val);
    }
    ElbExpr* index = instantiate_expr(parent, index_env, pt_expr1);
    if ( !index ) {
      return NULL;
    }
    return factory().new_BitSelect(pt_expr, primary, index);
  }
  if ( has_range_select ) {
    // 範囲指定付きの場合
    switch ( pt_expr->range_mode() ) {
    case kVpiConstRange:
      {
	const PtExpr* pt_left = pt_expr->left_range();
	const PtExpr* pt_right = pt_expr->right_range();
	int index1_val;
	int index2_val;
	bool stat1 = evaluate_int(parent, pt_left, index1_val, true);
	bool stat2 = evaluate_int(parent, pt_right, index2_val, true);
	if ( !stat1 || !stat2 ) {
	  return NULL;
	}
	bool big = (index1_val >= index2_val);
	if ( big ^ decl_base->is_big_endian() ) {
	  // 範囲の順番が逆
	  error_range_order(pt_expr);
	  return NULL;
	}
	ymuint offset;
	bool stat3 = decl_base->calc_bit_offset(index1_val, offset);
	if ( !stat3 ) {
	  // 左のインデックスが範囲外
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_left->file_region(),
			  kMsgWarning,
			  "ELAB",
			  "Left index is out of range.");
	  // ただ値が X になるだけでエラーにはならないそうだ．
	}
	bool stat4 = decl_base->calc_bit_offset(index2_val, offset);
	if ( !stat4 ) {
	  // 右のインデックスが範囲外
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  pt_right->file_region(),
			  kMsgWarning,
			  "ELAB",
			  "Right index is out of range.");
	  // ただ値が X になるだけでエラーにはならないそうだ．
	}
	return factory().new_PartSelect(pt_expr, primary,
					pt_left, pt_right,
					index1_val, index2_val);
      }

    case kVpiPlusRange:
      {
	const PtExpr* pt_base = pt_expr->left_range();
	const PtExpr* pt_range = pt_expr->right_range();
	int range_val;
	bool stat1 = evaluate_int(parent, pt_range, range_val, true);
	if ( !stat1 ) {
	  // range は常に固定でなければならない．
	  return NULL;
	}
	int base_val;
	bool stat2 = evaluate_int(parent, pt_base, base_val, false);
	if ( stat2 ) {
	  // 固定インデックスだった．
	  int index1_val;
	  int index2_val;
	  if ( decl_base->is_big_endian() ) {
	    index1_val = base_val + range_val - 1;
	    index2_val = base_val;
	  }
	  else {
	    index1_val = base_val;
	    index2_val = base_val + range_val - 1;
	  }
	  ymuint offset;
	  bool stat3 = decl_base->calc_bit_offset(index1_val, offset);
	  bool stat4 = decl_base->calc_bit_offset(index2_val, offset);
	  if ( !stat3 || !stat4 ) {
	    // 左か右のインデックスが範囲外
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_expr->file_region(),
			    kMsgWarning,
			    "ELAB",
			    "Index is out of range.");
	    // ただ値が X になるだけでエラーにはならないそうだ．
	  }
	  return factory().new_PartSelect(pt_expr, primary,
					  pt_base, pt_range,
					  index1_val, index2_val);
	}
	else {
	  ElbExpr* base = instantiate_expr(parent, index_env, pt_base);
	  if ( !base ) {
	    return NULL;
	  }
	  return factory().new_PlusPartSelect(pt_expr, primary,
					      base, pt_range, range_val);
	}
      }

    case kVpiMinusRange:
      {
	const PtExpr* pt_base = pt_expr->left_range();
	const PtExpr* pt_range = pt_expr->right_range();
	int range_val;
	bool stat1 = evaluate_int(parent, pt_range, range_val, true);
	if ( !stat1 ) {
	  // range は常に固定でなければならない．
	  return NULL;
	}
	int base_val;
	bool stat2 = evaluate_int(parent, pt_base, base_val, false);
	if ( stat2 ) {
	  // 固定インデックスだった．
	  int index1_val;
	  int index2_val;
	  if ( decl_base->is_big_endian() ) {
	    index1_val = base_val;
	    index2_val = base_val - range_val + 1;
	  }
	  else {
	    index1_val = base_val - range_val + 1;
	    index2_val = base_val;
	  }
	  ymuint offset;
	  bool stat3 = decl_base->calc_bit_offset(index1_val, offset);
	  bool stat4 = decl_base->calc_bit_offset(index2_val, offset);
	  if ( !stat3 || !stat4 ) {
	    // 左か右のインデックスが範囲外
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    pt_expr->file_region(),
			    kMsgWarning,
			    "ELAB",
			    "Index is out of range.");
	    // ただ値が X になるだけでエラーにはならないそうだ．
	  }
	  return factory().new_PartSelect(pt_expr, primary,
					  pt_base, pt_range,
					  index1_val, index2_val);
	}
	else {
	  ElbExpr* base = instantiate_expr(parent, index_env, pt_base);
	  if ( !base ) {
	    return NULL;
	  }
	  return factory().new_MinusPartSelect(pt_expr, primary,
					       base, pt_range, range_val);
	}
      }

    case kVpiNoRange:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
  }
  return primary;
}

// @brief PtExpr(primary) から named_event を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ExprGen::instantiate_namedevent(const VlNamedObj* parent,
				const PtExpr* pt_expr)
{
  assert_cond(pt_expr->type() == kPtPrimaryExpr, __FILE__, __LINE__);
  assert_cond(pt_expr->left_range() == NULL, __FILE__, __LINE__);
  assert_cond(pt_expr->right_range() == NULL, __FILE__, __LINE__);

  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();

  // 識別子の名前
  const char* name = pt_expr->name();

  // 名前に対応したオブジェクトのハンドルを求める．
  ElbObjHandle* handle = find_obj_up(parent, nb_array, name, NULL);
  if ( handle == NULL ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return NULL;
  }

  // 配列要素などの処理を行う．
  ElbEnv env0;
  if ( pt_expr->is_const_index() ) {
    env0 = ElbConstantEnv();
  }

  bool is_array;
  bool has_range_select;
  bool has_bit_select;
  ElbExpr* primary = instantiate_primary_sub(handle, parent, env0, pt_expr,
					     is_array,
					     has_range_select,
					     has_bit_select);
  if ( primary == NULL ) {
    // エラー
    // メッセージは instantiate_decl() 内で出力されている．
    return NULL;
  }

  const VlDeclBase* decl_base = primary->decl_base();
  assert_cond( decl_base != NULL, __FILE__, __LINE__);
  tVpiObjType decl_type = decl_base->type();
  if ( decl_type != kVpiNamedEvent ) {
    // 型が違う
    error_not_a_namedevent(pt_expr);
    return NULL;
  }
  if ( has_range_select || has_bit_select ) {
    // 部分選択，ビット選択は使えない．
    error_select_for_namedevent(pt_expr);
    return NULL;
  }

  return primary;
}

// @brief 定数識別子を探す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbObjHandle*
ExprGen::find_const_handle(const VlNamedObj* parent,
			   const PtExpr* pt_expr)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();

  // 識別子の名前
  const char* name = pt_expr->name();

  // モジュール内の識別子を探索する．
  ElbObjHandle* handle = find_obj_up(parent, nb_array, name,
				     parent->parent_module());
  if ( handle == NULL ) {
    // 見つからなかった．
    error_not_found(pt_expr);
    return NULL;
  }
  // handle が持つオブジェクトは genvar か parameter でなければならない．
  if ( handle->genvar() == NULL && handle->parameter() == NULL ) {
    error_not_a_parameter(pt_expr);
    return NULL;
  }

  return handle;
}

// @brief genvar に対応した定数を生成する．
// @param[in] pt_expr 式を表すパース木
// @param[in] val 値
// @note pt_expr に添字が付いていたらエラーとなる．
ElbExpr*
ExprGen::instantiate_genvar(const VlNamedObj* parent,
			    const PtExpr* pt_expr,
			    int val)
{
  bool has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  ymuint isize = pt_expr->index_num();
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    error_dimension_mismatch(pt_expr);
    return NULL;
  }
  bool has_bit_select = (isize == 1);

  if ( has_bit_select ) {
    int index1 = 0;
    if ( !evaluate_int(parent, pt_expr->index(0), index1, true) ) {
      return NULL;
    }
    val >>= index1;
    val &= 1;
  }
  else if ( has_range_select ) {
    int index1 = 0;
    int index2 = 0;
    bool stat1 = evaluate_int(parent, pt_expr->left_range(), index1, true);
    bool stat2 = evaluate_int(parent, pt_expr->right_range(), index2, true);
    if ( !stat1 || !stat2 ) {
      return NULL;
    }
    val >>= index2;
    val &= ((1 << (index1 - index2 + 1)) - 1);
  }

  // genvar の値に対応した定数式を生成
  return factory().new_GenvarConstant(pt_expr, val);
}

// @brief 宣言要素のインスタンス化を行う．
// @param[in] handle オブジェクトハンドル
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] is_array 対象が配列の時 true を返す．
// @param[out] has_range_select 範囲指定を持っていたら true を返す．
// @param[out] has_bit_select ビット指定を持っていたら true を返す．
ElbExpr*
ExprGen::instantiate_primary_sub(ElbObjHandle* handle,
				 const VlNamedObj* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr,
				 bool& is_array,
				 bool& has_range_select,
				 bool& has_bit_select)
{
  // 配列の次元
  ymuint dsize = 0;
  // プライマリ式の次元 (ビット指定を含んでいる可能性あり)
  ymuint isize = pt_expr->index_num();

  // 範囲指定があるとき true となるフラグ
  has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  // 答え
  ElbExpr* primary = NULL;

  ElbParameter* param = handle->parameter();
  ElbDecl* decl = handle->decl();
  ElbDeclArray* declarray = handle->declarray();
  VlValueType value_type;
  if ( param != NULL ) {
    primary = factory().new_Primary(pt_expr, param);
    is_array = false;
    value_type = param->value_type();
  }
  else if ( decl != NULL ) {
    primary = factory().new_Primary(pt_expr, decl);
    is_array = false;
    value_type = decl->value_type();
  }
  else if ( declarray != NULL ) {
    // 配列の次元
    dsize = declarray->dimension();
    if ( isize != dsize && (isize != dsize + 1 || has_range_select) ) {
      // 次元が合わない．
      error_dimension_mismatch(pt_expr);
      return NULL;
    }

    is_array = true;
    value_type = declarray->value_type();

    // 添字が定数ならオフセットを計算する．
    ymuint offset = 0;
    ymuint mlt = 1;
    bool const_index = true;
    for (ymuint i = 0; i < dsize; ++ i) {
      ymuint j = dsize - i - 1;
      const PtExpr* pt_expr1 = pt_expr->index(j);
      int index_val;
      bool stat = evaluate_int(parent, pt_expr1, index_val, false);
      if ( !stat ) {
	const_index = false;
	break;
      }
      offset += index_val * mlt;
      mlt *= declarray->range(j)->size();
    }
    if ( const_index ) {
      primary = factory().new_Primary(pt_expr, declarray, offset);
    }
    else {
      // 添字の式を生成する．
      vector<ElbExpr*> index_list;
      index_list.reserve(dsize);
      for (ymuint i = 0; i < dsize; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->index(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	index_list.push_back(expr1);
      }

      primary = factory().new_Primary(pt_expr, declarray, index_list);
    }
  }
  if ( primary == NULL ) {
    // 適切な型ではなかった．
    error_illegal_object(pt_expr);
    return NULL;
  }

  // ビット指定があるとき true となるフラグ
  has_bit_select = false;

  if ( isize == dsize + 1 && !has_range_select ) {
    // 識別子の添字の次元と配列の次元が 1 違いで
    // 範囲がなければ識別子の最後の添字はビット指定と見なす．
    has_bit_select = true;
    -- isize;
  }

  if ( isize != dsize ) {
    // 次元が会わない．
    error_dimension_mismatch(pt_expr);
    return NULL;
  }

  if ( has_range_select || has_bit_select ) {
    if ( value_type.is_real_type() ) {
      error_select_for_real(pt_expr);
      return NULL;
    }
  }

  return primary;
}

// @brief decl の型が適切がチェックする．
bool
ExprGen::check_decl(const ElbEnv& env,
		    const PtExpr* pt_expr,
		    tVpiObjType decl_type,
		    bool is_array,
		    bool has_select)
{
  if ( env.is_pca_lhs() ) {
    // procedural continuous assignment 文の左辺式
    if ( is_array ) {
      // 配列要素はダメ
      error_array_in_pca(pt_expr);
      return false;
    }
    if ( has_select ) {
      // 部分指定はダメ
      error_select_in_pca(pt_expr);
      return false;
    }
    if ( decl_type != kVpiReg &&
	 decl_type != kVpiIntegerVar &&
	 decl_type != kVpiRealVar &&
	 decl_type != kVpiTimeVar) {
      // reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_force_lhs() ) {
    // force 文の左辺式
    if ( is_array ) {
      // 配列要素はダメ
      error_array_in_force(pt_expr);
      return false;
    }
    if ( has_select ) {
      // 部分指定はダメ
      error_select_in_force(pt_expr);
      return false;
    }
    if ( decl_type != kVpiNet &&
	 decl_type != kVpiReg &&
	 decl_type != kVpiIntegerVar &&
	 decl_type != kVpiRealVar &&
	 decl_type != kVpiTimeVar) {
      // net/reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_net_lhs() ) {
    if ( decl_type != kVpiNet &&
	 (decl_type != kVpiNetArray || !is_array) ) {
      // net 以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else if ( env.is_var_lhs() ) {
    if ( decl_type != kVpiReg &&
	 (decl_type != kVpiRegArray || !is_array) &&
	 decl_type != kVpiIntegerVar &&
	 decl_type != kVpiRealVar &&
	 decl_type != kVpiTimeVar &&
	 decl_type != kVpiVarSelect ) {
      // reg/変数以外はダメ
      error_illegal_object(pt_expr);
      return false;
    }
  }
  else {
    // 右辺系の環境
    if ( env.is_constant() ) {
      // 定数式
      if ( decl_type != kVpiParameter &&
	   decl_type != kVpiSpecParam ) {
	// 定数(parameter)でないのでダメ
	error_illegal_object(pt_expr);
	return false;
      }
    }

    // あとは個別の型ごとにチェックする．
    if ( decl_type == kVpiRealVar && has_select ) {
      // real の部分選択は無効
      error_select_for_real(pt_expr);
      return false;
    }
    if ( decl_type == kVpiNamedEvent && !env.is_event_expr() ) {
      // イベント式以外では名前つきイベントは使えない．
      error_illegal_object(pt_expr);
      return false;
    }
  }
  return true;
}

// @brief プライマリに対して式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] put_error エラーを出力する時，true にする．
VlValue
ExprGen::evaluate_primary(const VlNamedObj* parent,
			  const PtExpr* pt_expr,
			  bool put_error)
{
  // 識別子の階層
  PtNameBranchArray nb_array = pt_expr->namebranch_array();
  if ( nb_array.size() > 0 ) {
    // 階層つき識別子はだめ
    if ( put_error ) {
      error_hname_in_ce(pt_expr);
    }
    return VlValue();
  }

  bool has_range_select = (pt_expr->left_range() && pt_expr->right_range());

  ymuint isize = pt_expr->index_num();
  if (  isize > 1 || (isize == 1 && has_range_select) ) {
    // 配列型ではない．
    if ( put_error ) {
      error_dimension_mismatch(pt_expr);
    }
    return VlValue();
  }
  bool has_bit_select = (isize == 1);

  int index1 = 0;
  int index2 = 0;
  if ( has_bit_select ) {
    if ( !evaluate_int(parent, pt_expr->index(0), index1, put_error) ) {
      return VlValue();
    }
  }
  if ( has_range_select ) {
    const PtExpr* pt_left = pt_expr->left_range();
    bool stat1 = evaluate_int(parent, pt_left, index1, put_error);
    const PtExpr* pt_right = pt_expr->right_range();
    bool stat2 = evaluate_int(parent, pt_right, index2, put_error);
    if ( !stat1 || !stat2 ) {
      return VlValue();
    }
  }

  // 識別子の名前
  const char* name = pt_expr->name();

  // モジュール内の識別子を探索する．
  ElbObjHandle* handle = find_obj_up(parent, PtNameBranchArray(), name,
				     parent->parent_module());
  if ( !handle ) {
    // 見つからなかった．
    if ( put_error ) {
      error_not_found(pt_expr);
    }
    return VlValue();
  }

  // そのオブジェクトが genvar の場合
  ElbGenvar* genvar = handle->genvar();
  if ( genvar ) {
    if ( has_bit_select ) {
      // ビット選択
      BitVector bv(genvar->value());
      return VlValue(bv.bit_select(index1));
    }
    else if ( has_range_select ) {
      // 範囲選択
      BitVector bv(genvar->value());
      if ( index1 < index2 ) {
	if ( put_error ) {
	  error_range_order(pt_expr);
	}
	return VlValue();
      }
      return VlValue(bv.part_select(index1, index2));
    }
    else {
      return VlValue(genvar->value());
    }
  }

  // それ以外の宣言要素の場合
  // しかしこの場合には parameter でなければならない．
  ElbParameter* param = handle->parameter();
  if ( !param ) {
    if ( put_error ) {
      error_not_a_parameter(pt_expr);
    }
    return VlValue();
  }
  VlValue val = param->get_value();
  if ( param->value_type().is_real_type() ) {
    if ( has_bit_select || has_range_select ) {
      if ( put_error ) {
	error_illegal_real_type(pt_expr);
      }
      return VlValue();
    }
  }
  else {
    if ( has_bit_select ) {
      // ビット選択
      if ( !val.is_bitvector_conv() ) {
	if ( put_error ) {
	  error_illegal_real_type(pt_expr);
	}
	return VlValue();
      }
      ymuint offset;
      if ( !param->calc_bit_offset(index1, offset) ) {
	// インデックスが範囲外だった．
	// エラーではなく X になる．
	return VlValue(VlScalarVal::x());
      }
      return VlValue(val.bitvector_value().bit_select(offset));
    }
    else if ( has_range_select ) {
      if ( !val.is_bitvector_conv() ) {
	if ( put_error ) {
	  error_illegal_real_type(pt_expr);
	}
	return VlValue();
      }
      switch ( pt_expr->range_mode() ) {
      case kVpiConstRange:
	{
	  bool big = (index1 >= index2);
	  if ( big ^ param->is_big_endian() ) {
	    if ( put_error ) {
	      error_range_order(pt_expr);
	    }
	    return VlValue();
	  }
	}
	break;

      case kVpiPlusRange:
	if ( param->is_big_endian() ) {
	  int range = index2;
	  index2 = index1;
	  index1 = index1 + range - 1;
	}
	else {
	  index2 = index1 + index2 - 1;
	}
	break;

      case kVpiMinusRange:
	if ( param->is_big_endian() ) {
	  index2 = index1 - index2 + 1;
	}
	else {
	  int range = index2;
	  index2 = index1;
	  index1 = index1 - range + 1;
	}
	assert_not_reached(__FILE__, __LINE__);
	break;

      case kVpiNoRange:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
      ymuint msb_offset;
      bool stat1 = param->calc_bit_offset(index1, msb_offset);
      ymuint lsb_offset;
      bool stat2 = param->calc_bit_offset(index2, lsb_offset);
      if ( stat1 && stat2 ) {
	return VlValue(val.bitvector_value().part_select(msb_offset, lsb_offset));
      }
      ymuint bw;
      if ( index1 < index2 ) {
	bw = index2 - index1 + 1;
      }
      else {
	bw = index1 - index2 + 1;
      }
      return VlValue(BitVector(VlScalarVal::x(), bw));
    }
  }

  return val;
}

END_NAMESPACE_YM_VERILOG
