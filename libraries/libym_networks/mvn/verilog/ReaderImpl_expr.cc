
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
#include "ym_networks/MvnMgr.h"
#include "ym_networks/MvnModule.h"
#include "ym_networks/MvnNode.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlValue.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/vl/VlRange.h"

#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_VERILOG

// @brief 式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_expr(MvnModule* parent_module,
		     const VlExpr* expr,
		     const Env& env)
{
  vector<ymuint32> dummy;
  MvnNode* node = gen_expr(parent_module, expr, kVpiCaseExact, env, dummy);
  if ( node == NULL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    expr->file_region(),
		    kMsgError,
		    "MVN_VLXXX",
		    "'X' or 'Z' in constant expression");
  }
  return node;
}

// @brief case 文用の式に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[in] env 環境
// @param[out] xmask Xマスク
MvnNode*
ReaderImpl::gen_expr(MvnModule* parent_module,
		     const VlExpr* expr,
		     tVpiCaseType case_type,
		     const Env& env,
		     vector<ymuint32>& xmask)
{
  MvnNode* node = NULL;
  if ( expr->is_const() ) {
    node = gen_const(parent_module, expr, case_type, xmask);
  }
  else if ( expr->is_operation() ) {
    node = gen_opr(parent_module, expr, case_type, env, xmask);
  }
  else {
    node = gen_primary(expr, env);
    if ( expr->is_primary() ) {
      ; // なにもしない．
    }
    else if ( expr->is_bitselect() ) {
      if ( expr->is_constant_select() ) {
	const VlDeclBase* decl = expr->decl_base();
	ymuint bitpos;
	if ( !decl->calc_bit_offset(expr->index_val(), bitpos) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  expr->file_region(),
			  kMsgError,
			  "MVN_VL",
			  "Index is out of range.");
	  return NULL;
	}
	const MvnOutputPin* pin = node->output(0);
	MvnNode* node1 = mMvnMgr->new_constbitselect(parent_module,
						     bitpos,
						     pin->bit_width());
	mMvnMgr->connect(node, 0, node1, 0);
	node = node1;
      }
      else {
#warning "TODO-2011-07-07-01: [msb:lsb] のオフセット変換をしていない"
	MvnNode* node1 = gen_expr(parent_module, expr->index(), env);
	const MvnOutputPin* pin0 = node->output(0);
	const MvnOutputPin* pin1 = node1->output(0);
	MvnNode* node2 = mMvnMgr->new_bitselect(parent_module,
						pin0->bit_width(),
						pin1->bit_width());
	mMvnMgr->connect(node, 0, node2, 0);
	mMvnMgr->connect(node1, 0, node2, 1);
	node = node2;
      }
    }
    else if ( expr->is_partselect() ) {
      if ( expr->is_constant_select() ) {
	const VlDeclBase* decl = expr->decl_base();
	ymuint msb;
	if ( !decl->calc_bit_offset(expr->left_range_val(), msb) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  expr->left_range()->file_region(),
			  kMsgError,
			  "MVN_VL",
			  "Left range is out of range");
	  return NULL;
	}
	ymuint lsb;
	if ( !decl->calc_bit_offset(expr->right_range_val(), lsb) ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  expr->right_range()->file_region(),
			  kMsgError,
			  "MVN_VL",
			  "Right range is out of range");
	  return NULL;
	}
	const MvnOutputPin* pin = node->output(0);
	MvnNode* node1 = mMvnMgr->new_constpartselect(parent_module,
						      msb, lsb,
						      pin->bit_width());
	mMvnMgr->connect(node, 0, node1, 0);
	node = node1;
      }
      else {
#warning "TODO-2011-07-07-02: [msb:lsb] のオフセット変換をしていない"
	// まだできてない．
	// というか可変 part_select は VPI がおかしいと思う．
	assert_not_reached(__FILE__, __LINE__);
	return NULL;
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  assert_cond( node != NULL, __FILE__, __LINE__);

  return coerce_expr(parent_module, node, expr->value_type());
}

// @brief 定数値に対応したノードを作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[out] xmask Xマスク
MvnNode*
ReaderImpl::gen_const(MvnModule* parent_module,
		      const VlExpr* expr,
		      tVpiCaseType case_type,
		      vector<ymuint32>& xmask)
{
  VlValue value = expr->constant_value();
  assert_cond( expr->value_type().is_bitvector_type(), __FILE__, __LINE__);
  BitVector bv = value.bitvector_value();
  ymuint bit_size = bv.size();
  ymuint blk_size = (bit_size + 31) / 32;
  vector<ymuint32> tmp(blk_size, 0U);
  xmask.resize(blk_size, 0U);
  for (ymuint i = 0; i < bit_size; ++ i) {
    ymuint blk = i / 32;
    ymuint pos = i % 32;
    ymuint32 bit = (1U << pos);
    VlScalarVal v = bv.value(i);
    if ( v.is_one() ) {
      tmp[blk] |= bit;
    }
    else if ( v.is_zero() ) {
      ; // なにもしない．
    }
    else if ( v.is_x() ) {
      if ( case_type == kVpiCaseX ) {
	xmask[blk] |= bit;
      }
      else {
	return NULL;
      }
    }
    else if ( v.is_z() ) {
      if ( case_type == kVpiCaseX || case_type == kVpiCaseZ ) {
	xmask[blk] |= bit;
      }
      else {
	return NULL;
      }
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
      return NULL;
    }
  }
  return mMvnMgr->new_const(parent_module, bit_size, tmp);
}

BEGIN_NONAMESPACE

// xmask の各ビットの OR を返す．
bool
or_xmask(ymuint blk_size,
	 const vector<ymuint32>& xmask)
{
  for (ymuint i = 0; i < blk_size; ++ i) {
    if ( xmask[i] != 0U ) {
      return true;
    }
  }
  return false;
}

// xmask の各ビットに val を書き込む．
void
fill_xmask(ymuint blk_size,
	   bool val,
	   vector<ymuint32>& xmask)
{
  ymuint32 v = val ? 0xFFFFFFFF : 0x0;
  for (ymuint i = 0; i < blk_size; ++ i) {
    xmask[i] = v;
  }
}

// xmask_array[0] を xmask にコピーする．
void
copy_xmask1(ymuint blk_size,
	    const vector<vector<ymuint32> >& xmask_array,
	    vector<ymuint32>& xmask)
{
  for (ymuint i = 0; i < blk_size; ++ i) {
    xmask[i] = xmask_array[0][i];
  }
}

// xmask_array[0] | xmask_array[1] を xmask にコピーする．
void
copy_xmask2(ymuint blk_size,
	    const vector<vector<ymuint32> >& xmask_array,
	    vector<ymuint32>& xmask)
{
  for (ymuint i = 0; i < blk_size; ++ i) {
    xmask[i] = xmask_array[0][i] | xmask_array[1][i];
  }
}

void
copy_xmask3(ymuint blk_size,
	    const vector<vector<ymuint32> >& xmask_array,
	    vector<ymuint32>& xmask)
{
  for (ymuint i = 0; i < blk_size; ++ i) {
    xmask[i] = xmask_array[0][i] | xmask_array[1][i] | xmask_array[2][i];
  }
}

END_NONAMESPACE

// @brief 演算に対応したノードの木を作る．
// @param[in] parent_module 親のモジュール
// @param[in] expr 式
// @param[in] case_type case 文の種類
// @param[in] env 環境
// @param[out] xmask Xマスク
MvnNode*
ReaderImpl::gen_opr(MvnModule* parent_module,
		    const VlExpr* expr,
		    tVpiCaseType case_type,
		    const Env& env,
		    vector<ymuint32>& xmask)
{
  ymuint out_bw = expr->bit_size();
  ymuint blk_size = (out_bw + 31) / 32;
  xmask.resize(blk_size, 0U);

  tVlOpType op_type = expr->op_type();
  ymuint n = expr->operand_num();
  vector<MvnNode*> operand_array(n);
  vector<vector<ymuint32> > xmask_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    MvnNode* node1 = gen_expr(parent_module, expr->operand(i), case_type,
			      env, xmask_array[i]);
    VlValueType opr_type = expr->operand_type(i);
    operand_array[i] = coerce_expr(parent_module, node1, opr_type);
  }

  switch ( op_type ) {
  case kVlNullOp:
    copy_xmask1(blk_size, xmask_array, xmask);
    return operand_array[0];

  case kVlMinusOp:
    {
      ymuint bw = operand_array[0]->output(0)->bit_width();
      assert_cond( bw == out_bw, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_cmpl(parent_module, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val = or_xmask(blk_size, xmask_array[0]);
      fill_xmask(blk_size, val, xmask);
      return node;
    }

  case kVlNotOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__);
      ymuint bw = operand_array[0]->output(0)->bit_width();
      assert_cond( bw == 1, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      copy_xmask1(1, xmask_array, xmask);
      return node;
    }

  case kVlBitNegOp:
    {
      ymuint bw = operand_array[0]->output(0)->bit_width();
      assert_cond( bw == out_bw, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_not(parent_module, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlPlusOp:
    {
      ymuint bw = operand_array[0]->output(0)->bit_width();
      assert_cond( bw == out_bw, __FILE__, __LINE__);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val = or_xmask(blk_size, xmask_array[0]);
      fill_xmask(blk_size, val, xmask);
      return operand_array[0];
    }

  case kVlUnaryAndOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__ );
      ymuint bw = operand_array[0]->output(0)->bit_width();
      MvnNode* node = mMvnMgr->new_rand(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlUnaryNandOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__ );
      ymuint bw = operand_array[0]->output(0)->bit_width();
      MvnNode* node = mMvnMgr->new_rand(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node1;
    }

  case kVlUnaryOrOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__ );
      ymuint bw = operand_array[0]->output(0)->bit_width();
      MvnNode* node = mMvnMgr->new_ror(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlUnaryNorOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__ );
      ymuint bw = operand_array[0]->output(0)->bit_width();
      MvnNode* node = mMvnMgr->new_ror(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node1;
    }

  case kVlUnaryXorOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__ );
      ymuint bw = operand_array[0]->output(0)->bit_width();
      MvnNode* node = mMvnMgr->new_rxor(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlUnaryXNorOp:
    {
      assert_cond( out_bw == 1, __FILE__, __LINE__ );
      ymuint bw = operand_array[0]->output(0)->bit_width();
      MvnNode* node = mMvnMgr->new_rxor(parent_module, bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      // ビットごとにコピーする．
      copy_xmask1(blk_size, xmask_array, xmask);
      return node1;
    }

  case kVlPosedgeOp:
  case kVlNegedgeOp:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kVlAddOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_add(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlSubOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_sub(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlMultOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_mult(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlDivOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_div(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlModOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_mod(parent_module, out_bw, out_bw, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlPowerOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      ymuint bw3 = out_bw;
      MvnNode* node = mMvnMgr->new_pow(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlLShiftOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      ymuint bw3 = out_bw;
      MvnNode* node = mMvnMgr->new_sll(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlRShiftOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      ymuint bw3 = out_bw;
      MvnNode* node = mMvnMgr->new_srl(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlArithLShiftOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      ymuint bw3 = out_bw;
      MvnNode* node = mMvnMgr->new_sla(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlArithRShiftOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      ymuint bw3 = out_bw;
      MvnNode* node = mMvnMgr->new_sra(parent_module, bw1, bw2, bw3);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(blk_size, val1 | val2, xmask);
      return node;
    }

  case kVlBitAndOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_and(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // ビットごとにコピーする．
      copy_xmask2(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlBitOrOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_or(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // ビットごとにコピーする．
      copy_xmask2(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlBitXNorOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_xor(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, out_bw);
      mMvnMgr->connect(node, 0, node1, 0);
      // ビットごとにコピーする．
      copy_xmask2(blk_size, xmask_array, xmask);
      return node1;
    }

  case kVlBitXorOp:
    {
      assert_cond( operand_array[0]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      assert_cond( operand_array[1]->output(0)->bit_width() == out_bw,
		   __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_xor(parent_module, 2, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // ビットごとにコピーする．
      copy_xmask2(blk_size, xmask_array, xmask);
      return node;
    }

  case kVlLogAndOp:
    {
      MvnNode* node = mMvnMgr->new_and(parent_module, 2, 1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // ビットごとにコピーする．
      copy_xmask2(1, xmask_array, xmask);
      return node;
    }

  case kVlLogOrOp:
    {
      MvnNode* node = mMvnMgr->new_or(parent_module, 2, 1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // ビットごとにコピーする．
      copy_xmask2(1, xmask_array, xmask);
      return node;
    }

  case kVlCaseEqOp:
  case kVlCaseNeqOp:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kVlEqOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      assert_cond( bw1 == bw2, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_equal(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(1, val1 | val2, xmask);
      return node;
    }

  case kVlNeqOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      assert_cond( bw1 == bw2, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_equal(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(1, val1 | val2, xmask);
      return node1;
    }

  case kVlLtOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      assert_cond( bw1 == bw2, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(1, val1 | val2, xmask);
      return node;
    }

  case kVlGeOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      assert_cond( bw1 == bw2, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(1, val1 | val2, xmask);
      return node1;
    }

  case kVlGtOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      assert_cond( bw1 == bw2, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[1], 0, node, 0);
      mMvnMgr->connect(operand_array[0], 0, node, 1);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(1, val1 | val2, xmask);
      return node;
    }

  case kVlLeOp:
    {
      ymuint bw1 = operand_array[0]->output(0)->bit_width();
      ymuint bw2 = operand_array[1]->output(0)->bit_width();
      assert_cond( bw1 == bw2, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_lt(parent_module, bw1);
      mMvnMgr->connect(operand_array[1], 0, node, 0);
      mMvnMgr->connect(operand_array[0], 0, node, 1);
      MvnNode* node1 = mMvnMgr->new_not(parent_module, 1);
      mMvnMgr->connect(node, 0, node1, 0);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(blk_size, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      fill_xmask(1, val1 | val2, xmask);
      return node1;
    }

  case kVlConditionOp:
    {
      ymuint bw1 = operand_array[1]->output(0)->bit_width();
      ymuint bw2 = operand_array[2]->output(0)->bit_width();
      assert_cond( bw1 == out_bw, __FILE__, __LINE__);
      assert_cond( bw2 == out_bw, __FILE__, __LINE__);
      MvnNode* node = mMvnMgr->new_ite(parent_module, out_bw);
      mMvnMgr->connect(operand_array[0], 0, node, 0);
      mMvnMgr->connect(operand_array[1], 0, node, 1);
      mMvnMgr->connect(operand_array[2], 0, node, 2);
      // 算術演算の場合は1ビットでも X があれば全部 X になる．
      bool val1 = or_xmask(1, xmask_array[0]);
      bool val2 = or_xmask(blk_size, xmask_array[1]);
      bool val3 = or_xmask(blk_size, xmask_array[2]);
      fill_xmask(blk_size, val1 | val2 | val3, xmask);
      return node;
    }

  case kVlMinTypMaxOp:
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kVlConcatOp:
    {
      vector<ymuint> bw_array(n);
      for (ymuint i = 0; i < n; ++ i) {
	bw_array[i] = operand_array[i]->output(0)->bit_width();
      }
      MvnNode* node = mMvnMgr->new_concat(parent_module, bw_array);
      for (ymuint i = 0; i < n; ++ i) {
	mMvnMgr->connect(operand_array[i], 0, node, i);
      }
      ymuint offset = 0;
      for (ymuint i = 0; i < n; ++ i) {
	ymuint bw1 = bw_array[i];
	for (ymuint j = 0; j < bw1; ++ j) {
	  xmask[offset + j] = xmask_array[i][j];
	}
	offset += bw1;
      }
      return node;
    }

  case kVlMultiConcatOp:
    {
      ymuint r = expr->rep_num();
      ymuint n1 = n - 1;
      vector<ymuint> bw_array(n1 * r);
      for (ymuint j = 0; j < r; ++ j) {
	ymuint base = j * n1;
	for (ymuint i = 0; i < n1; ++ i) {
	  bw_array[base + i] = operand_array[i + 1]->output(0)->bit_width();
	}
      }
      MvnNode* node = mMvnMgr->new_concat(parent_module, bw_array);
      for (ymuint j = 0; j < r; ++ j) {
	ymuint base = j * n1;
	for (ymuint i = 0; i < n1; ++ i) {
	  mMvnMgr->connect(operand_array[i + 1], 0, node, base + i);
	}
      }
      ymuint offset = 0;
      for (ymuint j = 0; j < r; ++ j) {
	ymuint base = j * n1;
	for (ymuint i = 0; i < n; ++ i) {
	  ymuint bw1 = bw_array[base + i];
	  for (ymuint k = 0; k < bw1; ++ k) {
	    xmask[offset + k] = xmask_array[i][k];
	  }
	  offset += bw1;
	}
      }
      return node;
    }

  default:
    break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 宣言要素への参照に対応するノードを作る．
// @param[in] expr 式
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_primary(const VlExpr* expr,
			const Env& env)
{
  const VlDecl* decl = expr->decl_obj();
  const VlDeclArray* declarray = expr->declarray_obj();
  if ( decl ) {
    assert_cond(expr->declarray_dimension() == 0, __FILE__, __LINE__);
    MvnNode* node = env.get(decl);
#if defined(YM_DEBUG)
    if ( node == NULL ) {
      cerr << decl->name() << " is not found in mGlobalEnv" << endl;
    }
#endif
    assert_cond( node != NULL, __FILE__, __LINE__);
    return node;
  }
  else if ( declarray ) {
    if ( expr->is_constant_select() ) {
      // インデックス固定の配列要素
      ymuint offset = expr->declarray_offset();
      MvnNode* node = env.get(declarray, offset);
      if ( node == NULL ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			expr->file_region(),
			kMsgError,
			"MVN_VL",
			"Index is out of range.");
	return NULL;
      }
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
      MvnNode* node = env.get(declarray, offset);
      if ( node == NULL ) {
	cerr << decl->name() << " is not found in mGlobalEnv" << endl;
      }
      assert_cond( node != NULL, __FILE__, __LINE__);
      return node;
#else
#warning "TODO-2011-07-07-03: 可変インデックスの配列要素"
      assert_not_reached(__FILE__, __LINE__);
      return NULL;
#endif
    }
  }
#if defined(YM_DEBUG)
  cerr << "Error in " << expr->decompile() << endl;
  cerr << "  " << expr->file_region() << endl;
#endif
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @bief 右辺式に対応するノードを作る．
// @param[in] parent_module 親のモジュール
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
// @param[in] env 環境
MvnNode*
ReaderImpl::gen_rhs(MvnModule* parent_module,
		    const VlExpr* lhs,
		    const VlExpr* rhs,
		    const Env& env)
{
  MvnNode* node_orig = gen_expr(parent_module, rhs, env);

  VlValueType lhs_value_type = lhs->value_type();

  return coerce_expr(parent_module, node_orig, lhs_value_type);
}

// @brief 式の型を補正する．
// @param[in] parent_module 親のモジュール
// @param[in] src_node 元のノード
// @param[in] value_type 要求されるデータ型
MvnNode*
ReaderImpl::coerce_expr(MvnModule* parent_module,
			MvnNode* src_node,
			VlValueType value_type)
{
  ymuint bit_width = value_type.size();
  ymuint src_bw = src_node->output(0)->bit_width();
  MvnNode* node = src_node;
  if ( bit_width > src_bw ) {
    // 左辺のビット幅が大きいとき
    // 上位ビットをパディングする．
    ymuint np = bit_width - src_bw;
    if ( value_type.is_signed() ) {
      // 符号付きの場合は再上位ビットをコピーする．
      vector<ymuint> ibw_array(np + 1);
      for (ymuint i = 0; i < np; ++ i) {
	ibw_array[i] = 1;
      }
      ibw_array[np] = src_bw;
      node = mMvnMgr->new_concat(parent_module, ibw_array);
      MvnNode* msb_node = mMvnMgr->new_constbitselect(parent_module,
						      src_bw - 1, src_bw);
      mMvnMgr->connect(src_node, 0, msb_node, 0);
      for (ymuint i = 0; i < np; ++ i) {
	mMvnMgr->connect(msb_node, 0, node, i);
      }
      mMvnMgr->connect(src_node, 0, node, np);
    }
    else {
      // 符号なしの場合は0を入れる．
      vector<ymuint> ibw_array(2);
      ibw_array[0] = np;
      ibw_array[1] = src_bw;
      node = mMvnMgr->new_concat(parent_module, ibw_array);
      ymuint nblk = (np + 31) / 32;
      vector<ymuint32> val(nblk);
      for (ymuint i = 0; i < nblk; ++ i) {
	val[i] = 0U;
      }
      MvnNode* zero = mMvnMgr->new_const(parent_module, np, val);
      mMvnMgr->connect(zero, 0, node, 0);
      mMvnMgr->connect(src_node, 0, node, 1);
    }
  }
  else if ( bit_width < src_bw ) {
    // 左辺のビット幅が小さいとき
    // ただ単に下位ビットを取り出す．
    node = mMvnMgr->new_constpartselect(parent_module, bit_width - 1, 0, src_bw);
    mMvnMgr->connect(src_node, 0, node, 0);
  }
  assert_cond( node != NULL, __FILE__, __LINE__);
  return node;
}

// @brief 右辺式から該当する部分を切り出す．
// @param[in] parent_module 親のモジュール
// @param[in] rhs_node 右辺式のノード
// @param[in] offset オフセット
// @param[in] bit_width ビット幅
// @note rhs_node から [offset: offset + bit_width - 1] の選択するノードを返す．
// @note 全範囲を選択する場合には node を返す．
// @note 範囲が合わなかったら NULL を返す．
MvnNode*
ReaderImpl::splice_rhs(MvnModule* parent_module,
		       MvnNode* rhs_node,
		       ymuint offset,
		       ymuint bit_width)
{
  ymuint src_bw = rhs_node->output(0)->bit_width();
  assert_cond( offset + bit_width <= src_bw, __FILE__, __LINE__);

  MvnNode* src_node = NULL;
  if ( offset == 0 && bit_width == src_bw ) {
    // 全範囲の選択
    src_node = rhs_node;
  }
  else if ( bit_width == 1 ) {
    src_node = mMvnMgr->new_constbitselect(parent_module,
					   offset,
					   src_bw);
    mMvnMgr->connect(rhs_node, 0, src_node, 0);
  }
  else {
    src_node = mMvnMgr->new_constpartselect(parent_module,
					    offset,
					    offset + bit_width - 1,
					    src_bw);
    mMvnMgr->connect(rhs_node, 0, src_node, 0);
  }
  return src_node;
}

END_NAMESPACE_YM_NETWORKS_VERILOG
