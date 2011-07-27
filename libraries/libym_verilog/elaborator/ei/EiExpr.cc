
/// @file libym_verilog/elb_impl/EiExpr.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"
#include "EiOperation.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"
#include "ym_verilog/VlValue.h"
#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiExpr::EiExpr()
{
}

// @brief デストラクタ
EiExpr::~EiExpr()
{
}

// @brief ファイル位置を返す．
FileRegion
EiExpr::file_region() const
{
  return pt_obj()->file_region();
}

// @brief 定数の時 true を返す．
// @note このクラスは false を返す．
bool
EiExpr::is_const() const
{
  return false;
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
// @note このクラスでは false を返す．
bool
EiExpr::is_constant_select() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiExpr::is_primary() const
{
  return false;
}

// @brief ビット指定の時に true を返す．
bool
EiExpr::is_bitselect() const
{
  return false;
}

// @brief 範囲指定の時に true を返す．
bool
EiExpr::is_partselect() const
{
  return false;
}

// @brief 演算子の時に true を返す．
bool
EiExpr::is_operation() const
{
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiExpr::is_funccall() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiExpr::is_sysfunccall() const
{
  return false;
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
// @note それ以外では NULL を返す．
const VlDeclBase*
EiExpr::decl_base() const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note プライマリ，ビット選択，部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlDecl*
EiExpr::decl_obj() const
{
  return NULL;
}

// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
// @note それ以外では NULL を返す．
const VlDeclArray*
EiExpr::declarray_obj() const
{
  return NULL;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
// @note このクラスでは 0 を返す．
ymuint
EiExpr::declarray_dimension() const
{
  return 0;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::declarray_index(ymuint pos) const
{
  return NULL;
}

// @brief 配列型宣言要素への参照のオフセットを返す．
// @note 固定インデックスの場合のみ意味を持つ．
ymuint
EiExpr::declarray_offset() const
{
  return 0;
}

// @brief 対象のオブジェクトを返す．
// @note 対象がスコープの時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlNamedObj*
EiExpr::scope_obj() const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note 対象が ElbPrimitive の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlPrimitive*
EiExpr::primitive_obj() const
{
  return NULL;
}

// @brief インデックス式を返す．
// @note ビット選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::index() const
{
  return NULL;
}

// @brief インデックス値を返す．
// @note 式に対するビット選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
EiExpr::index_val() const
{
  return 0;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiExpr::range_mode() const
{
  return kVpiNoRange;
}

// @brief 範囲の MSB を返す．
// @note 部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::left_range() const
{
  return NULL;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
EiExpr::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB を返す．
// @note 部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::right_range() const
{
  return NULL;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
EiExpr::right_range_val() const
{
  return 0;
}

// @brief 範囲のベースを表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiExpr::base() const
{
  return NULL;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiExpr::range_width() const
{
  return 0;
}

// @brief 親の式を返す．
// @note 式に対するビット選択/範囲選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::parent_expr() const
{
  return NULL;
}

// @brief 演算子のタイプを返す．
// @note 演算子の時，意味を持つ．
// @note このクラスでは kVlNullOp を返す．
tVlOpType
EiExpr::op_type() const
{
  return kVlNullOp;
}

// @brief オペランド数を返す．
// @note 演算子の時，意味を持つ．
// @note このクラスでは 0 を返す．
ymuint
EiExpr::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
const VlExpr*
EiExpr::operand(ymuint pos) const
{
  return _operand(pos);
}

// @brief オペランドに要求されるデータ型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
// @note このクラスでは kVpiValueNone を返す．
VlValueType
EiExpr::operand_type(ymuint pos) const
{
  return VlValueType();
}

// @brief 繰り返し数を返す．
// @note multiple concatenation の時のみ意味を持つ．
ymuint
EiExpr::rep_num() const
{
  return 0;
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
// @note このクラスでは動作は不定
tVpiConstType
EiExpr::constant_type() const
{
  // とは言っても何か返さなければならない．
  return kVpiDecConst;
}

// @brief 定数値を返す．
// @note kVpiConstant の時，意味を持つ．
// @note それ以外では動作は不定
VlValue
EiExpr::constant_value() const
{
  return VlValue();
}

// @brief 対象の関数を返す．
// @note function call の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlTaskFunc*
EiExpr::function() const
{
  return NULL;
}

// @brief 対象のシステム関数を返す．
// @note system function call の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlUserSystf*
EiExpr::user_systf() const
{
  return NULL;
}

// @brief 引数の数を返す．
// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
// @note このクラスでは 0 を返す．
ymuint
EiExpr::argument_num() const
{
  return 0;
}

// @brief 引数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::argument(ymuint pos) const
{
  return NULL;
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
// @note このクラスでは 0 を返す．
ymuint
EiExpr::lhs_elem_num() const
{
  return 0;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExpr::lhs_elem(ymuint pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiExprBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiExprBase::EiExprBase(const PtExpr* pt_expr) :
  mPtExpr(pt_expr)
{
}

// @brief デストラクタ
EiExprBase::~EiExprBase()
{
}

// @brief Verilog-HDL の文字列を得る．
string
EiExprBase::decompile() const
{
  return pt_expr()->decompile();
}

/// @brief パース木の定義要素を返す．
const PtBase*
EiExprBase::pt_obj() const
{
  return pt_expr();
}


//////////////////////////////////////////////////////////////////////
// クラス EiOperation
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiOperation::EiOperation(const PtExpr* pt_expr) :
  EiExprBase(pt_expr)
{
}

// @brief デストラクタ
EiOperation::~EiOperation()
{
}

// @brief 型の取得
tVpiObjType
EiOperation::type() const
{
  return kVpiOperation;
}

// @brief 演算子の時に true を返す．
bool
EiOperation::is_operation() const
{
  return true;
}

// @brief 演算子のタイプを返す．
tVlOpType
EiOperation::op_type() const
{
  return pt_expr()->op_type();
}

END_NAMESPACE_YM_VERILOG
