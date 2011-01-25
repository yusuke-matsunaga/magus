
/// @file libym_verilog/elb_impl/EiPrimary.cc
/// @brief EiPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPrimary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPrimary.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
EiFactory::new_Primary(const PtExpr* pt_expr,
		       ElbDecl* obj)
{
  void* p = mAlloc.get_memory(sizeof(EiPrimary));
  EiPrimary* expr = new (p) EiPrimary(pt_expr, obj);

  return expr;
}

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
EiFactory::new_Primary(const PtExpr* pt_expr,
		       ElbParameter* obj)
{
  void* p = mAlloc.get_memory(sizeof(EiParamPrimary));
  EiParamPrimary* expr = new (p) EiParamPrimary(pt_expr, obj);

  return expr;
}

// @brief プライマリ式を生成する(配列要素版)．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
ElbExpr*
EiFactory::new_Primary(const PtExpr* pt_expr,
		       ElbDeclArray* obj,
		       const vector<ElbExpr*>& index_list)
{
  ymuint n = index_list.size();
  void* q = mAlloc.get_memory(sizeof(ElbExpr*) * n);
  ElbExpr** index_array = new (q) ElbExpr*[n];
  for (ymuint i = 0; i < n; ++ i) {
    index_array[i] = index_list[i];
  }
  void* p = mAlloc.get_memory(sizeof(EiArrayElemPrimary));
  ElbExpr* expr = new (p) EiArrayElemPrimary(pt_expr, obj, n, index_array);

  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
EiFactory::new_ArgHandle(const PtExpr* pt_expr,
			 const VlNamedObj* arg)
{
  void* p = mAlloc.get_memory(sizeof(EiScopePrimary));
  EiScopePrimary* expr = new (p) EiScopePrimary(pt_expr, arg);

  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
EiFactory::new_ArgHandle(const PtExpr* pt_expr,
			 ElbPrimitive* arg)
{
  void* p = mAlloc.get_memory(sizeof(EiPrimitivePrimary));
  EiPrimitivePrimary* expr = new (p) EiPrimitivePrimary(pt_expr, arg);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiPrimary::EiPrimary(const PtExpr* pt_expr,
		     ElbDecl* obj) :
  EiExprBase(pt_expr),
  mObj(obj)
{
}

// @brief デストラクタ
EiPrimary::~EiPrimary()
{
}

// @brief 型の取得
tVpiObjType
EiPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPrimary::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDecl*
EiPrimary::decl_obj() const
{
  return mObj;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiParamPrimary::EiParamPrimary(const PtExpr* pt_expr,
			       ElbParameter* obj) :
  EiExprBase(pt_expr),
  mObj(obj)
{
}

// @brief デストラクタ
EiParamPrimary::~EiParamPrimary()
{
}

// @brief 型の取得
tVpiObjType
EiParamPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiParamPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiParamPrimary::is_const() const
{
  return true;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiParamPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDecl*
EiParamPrimary::decl_obj() const
{
  return mObj;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiParamPrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiArrayElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] dim 配列の次元
// @param[in] index_list インデックスのリスト
EiArrayElemPrimary::EiArrayElemPrimary(const PtExpr* pt_expr,
				       ElbDeclArray* obj,
				       ymuint dim,
				       ElbExpr** index_list) :
  EiExprBase(pt_expr),
  mObj(obj),
  mDim(dim),
  mIndexList(index_list)
{
}

// @brief デストラクタ
EiArrayElemPrimary::~EiArrayElemPrimary()
{
}

// @brief 型の取得
tVpiObjType
EiArrayElemPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiArrayElemPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiArrayElemPrimary::is_const() const
{
  // 配列要素は定数ではない．
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiArrayElemPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDeclArray*
EiArrayElemPrimary::declarray_obj() const
{
  return mObj;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
// @note それ以外では 0 を返す．
ymuint
EiArrayElemPrimary::declarray_dimension() const
{
  return mDim;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
// @note それ以外では NULL を返す．
const VlExpr*
EiArrayElemPrimary::declarray_index(ymuint pos) const
{
  return mIndexList[pos];
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiArrayElemPrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiScopePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiScopePrimary::EiScopePrimary(const PtExpr* pt_expr,
			       const VlNamedObj* obj) :
  EiExprBase(pt_expr),
  mObj(obj)
{
}

// @brief デストラクタ
EiScopePrimary::~EiScopePrimary()
{
}

// @brief 型の取得
tVpiObjType
EiScopePrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiScopePrimary::value_type() const
{
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiScopePrimary::is_const() const
{
  return false;
}

// @brief 対象のオブジェクトを返す．
const VlNamedObj*
EiScopePrimary::scope_obj() const
{
  return mObj;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiScopePrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitivePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiPrimitivePrimary::EiPrimitivePrimary(const PtExpr* pt_expr,
				       ElbPrimitive* obj) :
  EiExprBase(pt_expr),
  mObj(obj)
{
}

// @brief デストラクタ
EiPrimitivePrimary::~EiPrimitivePrimary()
{
}

// @brief 型の取得
tVpiObjType
EiPrimitivePrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
tVpiValueType
EiPrimitivePrimary::value_type() const
{
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPrimitivePrimary::is_const() const
{
  return false;
}

// @brief 対象のオブジェクトを返す．
const VlPrimitive*
EiPrimitivePrimary::primitive_obj() const
{
  return mObj;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPrimitivePrimary::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
