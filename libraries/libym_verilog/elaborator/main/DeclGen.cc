
/// @file libym_verilog/elaborator/DeclGen.cc
/// @brief ElbMgr の実装ファイル(宣言要素の elaboration)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DeclGen.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "DeclGen.h"
#include "ElbEnv.h"

#include <ym_verilog/pt/PtDecl.h>
#include <ym_verilog/pt/PtExpr.h>
#include <ym_verilog/pt/PtMisc.h>
#include <ym_verilog/pt/PtArray.h>

#include <ym_verilog/vl/VlModule.h>

#include "ElbParamAssign.h"
#include "ElbContAssign.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbGenvar.h"
#include "ElbRange.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス DeclGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
DeclGen::DeclGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
DeclGen::~DeclGen()
{
}

// 宣言要素をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
void
DeclGen::instantiate_decl(const VlNamedObj* parent,
			  PtDeclHeadArray pt_head_array)
{
  for (ymuint32 i = 0; i < pt_head_array.size(); ++ i) {
    const PtDeclHead* pt_head = pt_head_array[i];
    switch ( pt_head->type() ) {
    case kPtDecl_Param:
    case kPtDecl_LocalParam:
      assert_not_reached(__FILE__, __LINE__);
      break;
    
    case kPtDecl_Reg:
      instantiate_reg_head(parent, pt_head);
      break;
    
    case kPtDecl_Var:
      instantiate_var_head(parent, pt_head);
      break;
    
    case kPtDecl_Genvar:
      instantiate_genvar_head(parent, pt_head);
      break;
    
    case kPtDecl_Net:
      instantiate_net_head(parent, pt_head);
      break;
    
    case kPtDecl_Event:
      instantiate_event_head(parent, pt_head);
      break;
    
    case kPtDecl_SpecParam:
      // 未対応
      break;

    default:
      break;
    }
  }
}

// @brief パラメータ用の instantiate 関数
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] is_local local_param の時 true
void
DeclGen::instantiate_param(const VlNamedObj* parent,
			   PtDeclHeadArray pt_head_array,
			   bool is_local)
{
  const VlModule* module = parent->parent_module();
  
  for (ymuint32 i = 0; i < pt_head_array.size(); ++ i) {
    const PtDeclHead* pt_head = pt_head_array[i];
    
    const PtExpr* pt_left = pt_head->left_range();
    const PtExpr* pt_right = pt_head->right_range();
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(parent, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      continue;
    }
    ElbParamHead* param_head = factory().new_ParamHead(module, pt_head,
						       left, right,
						       left_val, right_val);

    for (ymuint32 i = 0; i < pt_head->item_num(); ++ i) {
      const PtDeclItem* pt_item = pt_head->item(i);
      const FileRegion& file_region = pt_item->file_region();
      
      // 右辺の式は constant expression のはずなので今つくる．
      const PtExpr* pt_init_expr = pt_item->init_value();
      ElbExpr* init = instantiate_constant_expr(parent, pt_init_expr);
      if ( !init ) {
	continue;
      }

      ElbParameter* param = factory().new_Parameter(param_head,
						    pt_item,
						    is_local);
      assert_cond(param, __FILE__, __LINE__);
      reg_parameter(param);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, param);
    
      ostringstream buf;
      buf << "Parameter(" << param->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			file_region,
			kMsgInfo,
			"ELAB",
			buf.str());
    
      param->set_expr(init);
      
      // ダブっている感じがするけど同じことを表す parameter assign 文
      // をつくってモジュールに追加する．
      ElbParamAssign* pa = factory().new_ParamAssign(module, pt_item,
						     param, init,
						     true);
      reg_paramassign(pa);
    }
  }
}

// @brief net をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_net_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  const PtExpr* pt_left = pt_head->left_range();
  const PtExpr* pt_right = pt_head->right_range();
  const PtDelay* pt_delay = pt_head->delay();
  
  bool has_delay = (pt_delay != NULL);
  ElbExpr* left = NULL;
  ElbExpr* right = NULL;
  int left_val = 0;
  int right_val = 0;
  if ( !instantiate_range(parent, pt_left, pt_right,
			  left, right,
			  left_val, right_val) ) {
    return;
  }
  ElbDeclHead* net_head = factory().new_DeclHead(parent, pt_head,
						 left, right,
						 left_val, right_val,
						 has_delay);
  if ( pt_delay ) {
    add_phase3stub(make_stub(this, &DeclGen::link_net_delay,
			     net_head, pt_delay));
  }

  for (ymuint32 i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    // init_value() が 0 でなければ初期割り当てを持つということ．
    const PtExpr* pt_init = pt_item->init_value();

    ymuint32 dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列

      // 初期割り当ては構文規則上持てないはず
      assert_cond( !pt_init, __FILE__, __LINE__);

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }
      
      ElbDeclArray* net_array = factory().new_DeclArray(net_head,
							pt_item,
							range_src);
      reg_declarray(vpiNetArray, net_array);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, net_array);
      
      ostringstream buf;
      buf << "NetArray(" << net_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単一の要素
      ElbDecl* net = factory().new_Decl(net_head,
					pt_item);
      reg_decl(vpiNet, net);

      if ( pt_init ) {
	// 初期割り当てつき
	add_phase3stub(make_stub(this, &DeclGen::link_net_assign,
				 net, pt_item));
      }
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, net);
      
      ostringstream buf;
      buf << "Net(" << net->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief net の遅延値を生成する．
// @param[in] net_head ネットのヘッダ
// @param[in] pt_delay パース木の遅延式定義
void
DeclGen::link_net_delay(ElbDeclHead* net_head,
			const PtDelay* pt_delay)
{
  const VlNamedObj* parent = net_head->parent();
  ElbDelay* delay = instantiate_delay(parent, pt_delay);
  if ( delay ) {
    net_head->set_delay(delay);
  }
}

// @brief net の初期値を生成する．
// @param[in] net ネット
// @param[in] pt_item パース木のネット定義要素
void
DeclGen::link_net_assign(ElbDecl* net,
			 const PtDeclItem* pt_item)
{
  // 実際には対応する continuous assign 文を作る．
  
  ElbExpr* lhs = factory().new_Primary(pt_item, net);
  
  const VlNamedObj* parent = net->parent();
  const PtExpr* pt_init = pt_item->init_value();
  ElbExpr* rhs = instantiate_rhs(parent, ElbEnv(), pt_init, lhs);
  if ( !rhs ) {
    return;
  }

  const VlModule* module = parent->parent_module();
  ElbContAssign* ca = factory().new_ContAssign(module, pt_item,
					       lhs, rhs);
  reg_contassign(ca);
}

// @brief reg をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_reg_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  const PtExpr* pt_left = pt_head->left_range();
  const PtExpr* pt_right = pt_head->right_range();

  ElbExpr* left = NULL;
  ElbExpr* right = NULL;
  int left_val = 0;
  int right_val = 0;
  if ( !instantiate_range(parent, pt_left, pt_right,
			  left, right,
			  left_val, right_val) ) {
    return;
  }
  ElbDeclHead* reg_head = factory().new_DeclHead(parent, pt_head,
						 left, right,
						 left_val, right_val);
  
  for (ymuint32 i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    const PtExpr* pt_init = pt_item->init_value();
    ymuint32 dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      assert_cond( !pt_init, __FILE__, __LINE__);

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }
      
      ElbDeclArray* reg_array = factory().new_DeclArray(reg_head,
							pt_item,
							range_src);
      reg_declarray(vpiRegArray, reg_array);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, reg_array);
      
      ostringstream buf;
      buf << "RegArray(" << reg_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単独の要素
      ElbExpr* init = NULL;
      if ( pt_init ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(parent, pt_init);
	// エラーの時には init = NULL となるがそれでも処理は続ける．
	// もちろんエラーは記録されている．
      }
      
      ElbDecl* reg = factory().new_Decl(reg_head, pt_item, init);
      reg_decl(vpiReg, reg);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, reg);
      
      ostringstream buf;
      buf << "Reg(" << reg->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief variable header をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_var_head(const VlNamedObj* parent,
			      const PtDeclHead* pt_head)
{
  assert_cond(pt_head->data_type() != kVpiVarNone, __FILE__, __LINE__);

  ElbDeclHead* var_head = factory().new_DeclHead(parent, pt_head);

  for (ymuint32 i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    const PtExpr* pt_init = pt_item->init_value();
    ymuint32 dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列の場合

      // 配列は初期値を持たない．
      assert_cond( !pt_init, __FILE__, __LINE__);

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }
      
      ElbDeclArray* var_array = factory().new_DeclArray(var_head,
							pt_item,
							range_src);
      reg_declarray(vpiVariables, var_array);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, var_array);

      ostringstream buf;
      buf << "VarArray(" << var_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単独の変数
      ElbExpr* init = NULL;
      if ( pt_init ) {
	// 初期値を持つ場合
	// 初期値は constant_expression なので今作る．
	init = instantiate_constant_expr(parent, pt_init);
	// エラーの時には init = NULL となるがそれでも処理は続ける．
	// もちろんエラーは記録されている．
      }
      
      ElbDecl* var = factory().new_Decl(var_head, pt_item, init);
      reg_decl(vpiVariables, var);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, var);

      ostringstream buf;
      buf << "Var(" << var->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief named_event をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_event_head(const VlNamedObj* parent,
				const PtDeclHead* pt_head)
{
  ElbDeclHead* event_head = factory().new_DeclHead(parent, pt_head);

  for (ymuint32 i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    ymuint32 dim_size = pt_item->dimension_list_size();
    if ( dim_size > 0 ) {
      // 配列

      // 範囲の配列を作る．
      vector<ElbRangeSrc> range_src;
      if ( !instantiate_dimension_list(parent, pt_item, range_src) ) {
	continue;
      }
      
      ElbDeclArray* ne_array = factory().new_DeclArray(event_head,
						       pt_item,
						       range_src);
      reg_declarray(vpiNamedEventArray, ne_array);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, ne_array);
      
      ostringstream buf;
      buf << "NamedEventArray(" << ne_array->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
    else {
      // 単一の要素
      ElbDecl* named_event = factory().new_Decl(event_head,
						pt_item);
      reg_decl(vpiNamedEvent, named_event);
  
      // attribute instance の生成
      //instantiate_attribute(pt_head->attr_top(), false, named_event);
      
      ostringstream buf;
      buf << "NamedEvent(" << named_event->full_name() << ") created.";
      msg_mgr().put_msg(__FILE__, __LINE__,
			pt_item->file_region(),
			kMsgInfo,
			"ELAB",
			buf.str());
    }
  }
}

// @brief genvar をインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 宣言のヘッダ
void
DeclGen::instantiate_genvar_head(const VlNamedObj* parent,
				 const PtDeclHead* pt_head)
{
  for (ymuint32 i = 0; i < pt_head->item_num(); ++ i) {
    const PtDeclItem* pt_item = pt_head->item(i);
    ElbGenvar* genvar = factory().new_Genvar(parent, pt_item, 0);
    reg_genvar(genvar);
    
    ostringstream buf;
    buf << "Getnvar(" << genvar->full_name() << ") created.";
    msg_mgr().put_msg(__FILE__, __LINE__,
		      pt_item->file_region(),
		      kMsgInfo,
		      "ELAB",
		      buf.str());
  }
}

// @brief 配列の次元リストを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item 要素定義
// @param[in] range_src 範囲の情報を設定する配列
bool
DeclGen::instantiate_dimension_list(const VlNamedObj*  parent,
				    const PtDeclItem* pt_item,
				    vector<ElbRangeSrc>& range_src)
{
  ymuint32 n = pt_item->dimension_list_size();
  range_src.reserve(n);
  
  bool ok = true;
  for (ymuint32 i = 0; i < n; ++ i) {
    const PtRange* pt_range = pt_item->range(i);
    const PtExpr* pt_left = pt_range->left();
    const PtExpr* pt_right = pt_range->right();
    ElbExpr* left = NULL;
    ElbExpr* right = NULL;
    int left_val = 0;
    int right_val = 0;
    if ( !instantiate_range(parent, pt_left, pt_right,
			    left, right,
			    left_val, right_val) ) {
      ok = false;
      break;
    }
    range_src.push_back(ElbRangeSrc(pt_range, left, right,
				    left_val, right_val));
  }
  
  return ok;
}

END_NAMESPACE_YM_VERILOG
