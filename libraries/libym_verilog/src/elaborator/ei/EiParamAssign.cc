
/// @file EiParamAssign.cc
/// @brief EiParamAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiParamAssign.h"
#include "ElbParameter.h"
#include "ElbExpr.h"

#include "verilog/pt/PtItem.h"
#include "verilog/pt/PtExpr.h"
#include "verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
// @param[in] named_con 名前による割り当ての時 true
ElbParamAssign*
EiFactory::new_ParamAssign(const VlModule* module,
			   const PtBase* pt_obj,
			   ElbParameter* param,
			   const PtExpr* rhs_expr,
			   const VlValue& rhs_value)
{
  void* p = mAlloc.get_memory(sizeof(EiParamAssign2));
  return new (p) EiParamAssign(module, pt_obj, param, rhs_expr, rhs_value);
}

// @brief 名前によるパラメータ割り当て文を生成する．
// @param[in] module 親のモジュール
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
// @param[in] named_con 名前による割り当ての時 true
ElbParamAssign*
EiFactory::new_NamedParamAssign(const VlModule* module,
				const PtBase* pt_obj,
				ElbParameter* param,
				const PtExpr* rhs_expr,
				const VlValue& rhs_value)
{
  void* p = mAlloc.get_memory(sizeof(EiParamAssign));
  return new (p) EiParamAssign2(module, pt_obj, param, rhs_expr, rhs_value);
}

// @brief defparam 文を生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木の defparam ヘッダ
// @param[in] pt_item パース木の defparam 定義
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
ElbDefParam*
EiFactory::new_DefParam(const VlModule* module,
			const PtItem* pt_header,
			const PtDefParam* pt_defparam,
			ElbParameter* param,
			const PtExpr* rhs_expr,
			const VlValue& rhs_value)
{
  void* p = mAlloc.get_memory(sizeof(EiDefParam));
  return new (p) EiDefParam(module, pt_header, pt_defparam,
			    param, rhs_expr, rhs_value);
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のモジュール
// @param[in] pt_obj パース木中の対応する要素
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
EiParamAssign::EiParamAssign(const VlModule* parent,
			     const PtBase* pt_obj,
			     ElbParameter* param,
			     const PtExpr* rhs_expr,
			     const VlValue& rhs_value) :
  mModule(parent),
  mPtObj(pt_obj),
  mLhs(param),
  mRhsExpr(rhs_expr),
  mRhsValue(rhs_value)
{
}

// @brief デストラクタ
EiParamAssign::~EiParamAssign()
{
}

// @brief 型の取得
tVpiObjType
EiParamAssign::type() const
{
  return kVpiParamAssign;
}

// @brief ファイル位置を返す．
FileRegion
EiParamAssign::file_region() const
{
  return mPtObj->file_region();
}

// @brief 親のモジュールを返す．
const VlModule*
EiParamAssign::parent() const
{
  return mModule;
}

// @brief 左辺値を返す．
const VlDecl*
EiParamAssign::lhs() const
{
  return mLhs;
}

// @brief 右辺値を返す．
VlValue
EiParamAssign::rhs_value() const
{
  return mRhsValue;
}

// @brief 右辺の式を表す文字列を返す．
string
EiParamAssign::rhs_string() const
{
  return mRhsExpr->decompile();
}

// @brief 名前による接続の場合に true を返す．
bool
EiParamAssign::is_conn_by_name() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamAssign2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のモジュール
// @param[in] pt_obj パース木中の対応する要素
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
EiParamAssign2::EiParamAssign2(const VlModule* parent,
			       const PtBase* pt_obj,
			       ElbParameter* param,
			       const PtExpr* rhs_expr,
			       const VlValue& rhs_value) :
  EiParamAssign(parent, pt_obj, param, rhs_expr, rhs_value)
{
}

// @brief デストラクタ
EiParamAssign2::~EiParamAssign2()
{
}

// @brief 名前による接続の場合に true を返す．
bool
EiParamAssign2::is_conn_by_name() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDefParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のモジュール
// @param[in] pt_header パース木の defparam ヘッダ
// @param[in] pt_item パース木の defparam 定義
// @param[in] param 対象の parameter
// @param[in] rhs_expr 割り当て式の右辺
// @param[in] rhs_value 右辺の値
EiDefParam::EiDefParam(const VlModule* parent,
		       const PtItem* pt_header,
		       const PtDefParam* pt_defparam,
		       ElbParameter* param,
		       const PtExpr* rhs_expr,
		       const VlValue& rhs_value) :
  mModule(parent),
  mPtHead(pt_header),
  mPtDefParam(pt_defparam),
  mLhs(param),
  mRhsExpr(rhs_expr),
  mRhsValue(rhs_value)
{
}

// @brief デストラクタ
EiDefParam::~EiDefParam()
{
}

// @brief 型の取得
tVpiObjType
EiDefParam::type() const
{
  return kVpiDefParam;
}

// @brief ファイル位置を返す．
FileRegion
EiDefParam::file_region() const
{
  return mPtDefParam->file_region();
}

// @brief 親のモジュールを返す．
const VlModule*
EiDefParam::parent() const
{
  return mModule;
}

// @brief 左辺値を返す．
const VlDecl*
EiDefParam::lhs() const
{
  return mLhs;
}

// @brief 右辺値を返す．
VlValue
EiDefParam::rhs_value() const
{
  return mRhsValue;
}

// @brief 右辺の式を表す文字列を返す．
string
EiDefParam::rhs_string() const
{
  return mRhsExpr->decompile();
}

END_NAMESPACE_YM_VERILOG
