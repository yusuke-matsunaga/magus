
/// @file EiPrimary.cc
/// @brief EiPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPrimary.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"

#include "verilog/VlValue.h"
#include "verilog/pt/PtDecl.h"


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
  return new (p) EiPrimary(pt_expr, obj);
}

// @brief プライマリ式を生成する．
// @param[in] pt_item パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
EiFactory::new_Primary(const PtDeclItem* pt_item,
		       ElbDecl* obj)
{
  void* p = mAlloc.get_memory(sizeof(EiDeclPrimary));
  return new (p) EiDeclPrimary(pt_item, obj);
}

// @brief プライマリ式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
ElbExpr*
EiFactory::new_Primary(const PtExpr* pt_expr,
		       ElbParameter* obj)
{
  void* p = mAlloc.get_memory(sizeof(EiParamPrimary));
  return new (p) EiParamPrimary(pt_expr, obj);
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
  return new (p) EiArrayElemPrimary(pt_expr, obj, n, index_array);
}

// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] offset オフセット
ElbExpr*
EiFactory::new_Primary(const PtExpr* pt_expr,
		       ElbDeclArray* obj,
		       ymuint offset)
{
  void* p = mAlloc.get_memory(sizeof(EiConstArrayElemPrimary));
  return new (p) EiConstArrayElemPrimary(pt_expr, obj, offset);
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
EiFactory::new_ArgHandle(const PtExpr* pt_expr,
			 const VlNamedObj* arg)
{
  void* p = mAlloc.get_memory(sizeof(EiScopePrimary));
  return new (p) EiScopePrimary(pt_expr, arg);
}

// @brief システム関数/システムタスクの引数を生成する．
// @param[in] pt_expr パース木中で参照している要素
// @param[in] arg 引数本体
ElbExpr*
EiFactory::new_ArgHandle(const PtExpr* pt_expr,
			 ElbPrimitive* arg)
{
  void* p = mAlloc.get_memory(sizeof(EiPrimitivePrimary));
  return new (p) EiPrimitivePrimary(pt_expr, arg);
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimaryBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiPrimaryBase::EiPrimaryBase(const PtExpr* pt_expr) :
  EiExprBase(pt_expr)
{
}

// @brief デストラクタ
EiPrimaryBase::~EiPrimaryBase()
{
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiPrimaryBase::is_primary() const
{
  return true;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPrimaryBase::_set_reqsize(const VlValueType& type)
{
  // なにもしない．
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
// @note このクラスでは NULL を返す．
ElbExpr*
EiPrimaryBase::_operand(ymuint pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiPrimary::EiPrimary(const PtExpr* pt_expr,
		     ElbDecl* obj) :
  EiPrimaryBase(pt_expr),
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
VlValueType
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

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
// @note それ以外では NULL を返す．
const VlDeclBase*
EiPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDecl*
EiPrimary::decl_obj() const
{
  return mObj;
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
// @note このクラスでは 1 を返す．
ymuint
EiPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
// @note このクラスでは pos = 0 の時，自分自身を返す．
const VlExpr*
EiPrimary::lhs_elem(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return this;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_item パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiDeclPrimary::EiDeclPrimary(const PtDeclItem* pt_item,
			     ElbDecl* obj) :
  mPtObj(pt_item),
  mObj(obj)
{
}

// @brief デストラクタ
EiDeclPrimary::~EiDeclPrimary()
{
}

// @brief 型の取得
tVpiObjType
EiDeclPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiDeclPrimary::is_const() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiDeclPrimary::is_primary() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
// @note それ以外では NULL を返す．
const VlDeclBase*
EiDeclPrimary::decl_base() const
{
  return mObj;;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDecl*
EiDeclPrimary::decl_obj() const
{
  return mObj;
}

// @brief Verilog-HDL の文字列を得る．
string
EiDeclPrimary::decompile() const
{
  return mPtObj->name();
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
// @note このクラスでは 1 を返す．
ymuint
EiDeclPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
// @note このクラスでは pos = 0 の時，自分自身を返す．
const VlExpr*
EiDeclPrimary::lhs_elem(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return this;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiDeclPrimary::_set_reqsize(const VlValueType& type)
{
  // なにもしない．
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
// @note このクラスでは NULL を返す．
ElbExpr*
EiDeclPrimary::_operand(ymuint pos) const
{
  return NULL;
}

// @brief パース木の定義要素を返す．
const PtBase*
EiDeclPrimary::pt_obj() const
{
  return mPtObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiParamPrimary::EiParamPrimary(const PtExpr* pt_expr,
			       ElbParameter* obj) :
  EiPrimaryBase(pt_expr),
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
VlValueType
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

// @brief 定数値を返す．
// @note kVpiConstant の時，意味を持つ．
// @note それ以外では動作は不定
VlValue
EiParamPrimary::constant_value() const
{
  return mObj->get_value();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
// @note それ以外では NULL を返す．
const VlDeclBase*
EiParamPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDecl*
EiParamPrimary::decl_obj() const
{
  return mObj;
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
  EiPrimaryBase(pt_expr),
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
VlValueType
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

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
// @note それ以外では NULL を返す．
const VlDeclBase*
EiArrayElemPrimary::decl_base() const
{
  return mObj;
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

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
// @note このクラスでは 1 を返す．
ymuint
EiArrayElemPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
// @note このクラスでは pos = 0 の時，自分自身を返す．
const VlExpr*
EiArrayElemPrimary::lhs_elem(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return this;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstArrayElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] offset オフセット
EiConstArrayElemPrimary::EiConstArrayElemPrimary(const PtExpr* pt_expr,
						 ElbDeclArray* obj,
						 ymuint offset) :
  EiPrimaryBase(pt_expr),
  mObj(obj),
  mOffset(offset)
{
}

// @brief デストラクタ
EiConstArrayElemPrimary::~EiConstArrayElemPrimary()
{
}

// @brief 型の取得
tVpiObjType
EiConstArrayElemPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiConstArrayElemPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiConstArrayElemPrimary::is_const() const
{
  // 配列要素は定数ではない．
  return false;
}

// @brief 部分/ビット指定が定数の時 true を返す．
// @note kVpiPartSelect/kVpiBitSelect の時，意味を持つ．
// @note それ以外では常に false を返す．
bool
EiConstArrayElemPrimary::is_constant_select() const
{
  return true;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
// @note それ以外では NULL を返す．
const VlDeclBase*
EiConstArrayElemPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
const VlDeclArray*
EiConstArrayElemPrimary::declarray_obj() const
{
  return mObj;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
// @note それ以外では 0 を返す．
ymuint
EiConstArrayElemPrimary::declarray_dimension() const
{
  return 0;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
// @note それ以外では NULL を返す．
const VlExpr*
EiConstArrayElemPrimary::declarray_index(ymuint pos) const
{
  return NULL;
}

// @brief 配列型宣言要素への参照のオフセットを返す．
// @note 固定インデックスの場合のみ意味を持つ．
ymuint
EiConstArrayElemPrimary::declarray_offset() const
{
  return mOffset;
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
// @note このクラスでは 1 を返す．
ymuint
EiConstArrayElemPrimary::lhs_elem_num() const
{
  return 1;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
// @note このクラスでは pos = 0 の時，自分自身を返す．
const VlExpr*
EiConstArrayElemPrimary::lhs_elem(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return this;
}


//////////////////////////////////////////////////////////////////////
// クラス EiScopePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiScopePrimary::EiScopePrimary(const PtExpr* pt_expr,
			       const VlNamedObj* obj) :
  EiPrimaryBase(pt_expr),
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
VlValueType
EiScopePrimary::value_type() const
{
  return VlValueType();
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


//////////////////////////////////////////////////////////////////////
// クラス EiPrimitivePrimary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
EiPrimitivePrimary::EiPrimitivePrimary(const PtExpr* pt_expr,
				       ElbPrimitive* obj) :
  EiPrimaryBase(pt_expr),
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
VlValueType
EiPrimitivePrimary::value_type() const
{
  return VlValueType();
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

END_NAMESPACE_YM_VERILOG
