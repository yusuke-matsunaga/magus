
/// @file libym_verilog/elb_impl/EiExpr.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"
#include "EiOperation.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"
#include "ym_verilog/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiExprBase1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiExprBase1::EiExprBase1(const PtBase* pt_obj) :
  mPtObj(pt_obj)
{
}

// @brief デストラクタ
EiExprBase1::~EiExprBase1()
{
}

// @brief ファイル位置を返す．
FileRegion
EiExprBase1::file_region() const
{
  return mPtObj->file_region();
}

// @brief 式のビット幅を返す．
ymuint
EiExprBase1::bit_size() const
{
  return unpack_size(value_type());
}

// @brief 定数の時 true を返す．
// @note このクラスは false を返す．
bool
EiExprBase1::is_const() const
{
  return false;
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
// @note このクラスでは false を返す．
bool
EiExprBase1::is_constant_select() const
{
  return false;
}

// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
bool
EiExprBase1::is_primary() const
{
  return false;
}

// @brief ビット指定の時に true を返す．
bool
EiExprBase1::is_bitselect() const
{
  return false;
}

// @brief 範囲指定の時に true を返す．
bool
EiExprBase1::is_partselect() const
{
  return false;
}

// @brief 演算子の時に true を返す．
bool
EiExprBase1::is_operation() const
{
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiExprBase1::is_funccall() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiExprBase1::is_sysfunccall() const
{
  return false;
}

// @brief 対象のオブジェクトを返す．
// @note プライマリ，ビット選択，部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlDecl*
EiExprBase1::decl_obj() const
{
  return NULL;
}

// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
// @note それ以外では NULL を返す．
const VlDeclArray*
EiExprBase1::declarray_obj() const
{
  return NULL;
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
// @note このクラスでは 0 を返す．
ymuint
EiExprBase1::declarray_dimension() const
{
  return 0;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
// @note このクラスでは NULL を返す．
const VlExpr*
EiExprBase1::declarray_index(ymuint pos) const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note 対象がスコープの時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlNamedObj*
EiExprBase1::scope_obj() const
{
  return NULL;
}

// @brief 対象のオブジェクトを返す．
// @note 対象が ElbPrimitive の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlPrimitive*
EiExprBase1::primitive_obj() const
{
  return NULL;
}

// @brief インデックス式を返す．
// @note ビット選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExprBase1::index() const
{
  return NULL;
}

// @brief インデックス値を返す．
// @note 式に対するビット選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
EiExprBase1::index_val() const
{
  return 0;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiExprBase1::range_mode() const
{
  return kVpiNoRange;
}

// @brief 範囲の MSB を返す．
// @note 部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExprBase1::left_range() const
{
  return NULL;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
EiExprBase1::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB を返す．
// @note 部分選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExprBase1::right_range() const
{
  return NULL;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
// @note このクラスでは 0 を返す．
int
EiExprBase1::right_range_val() const
{
  return 0;
}

// @brief 範囲のベースを表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiExprBase1::base() const
{
  return NULL;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiExprBase1::range_width() const
{
  return 0;
}

// @brief 親の式を返す．
// @note 式に対するビット選択/範囲選択の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExprBase1::parent_expr() const
{
  return NULL;
}

// @brief 演算子のタイプを返す．
// @note 演算子の時，意味を持つ．
// @note このクラスでは kVpiNullOp を返す．
tVpiOpType
EiExprBase1::op_type() const
{
  return kVpiNullOp;
}

// @brief オペランド数を返す．
// @note 演算子の時，意味を持つ．
// @note このクラスでは 0 を返す．
ymuint
EiExprBase1::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
const VlExpr*
EiExprBase1::operand(ymuint pos) const
{
  return _operand(pos);
}

// @brief 定数の型を返す．
// @note 定数の時，意味を持つ．
// @note このクラスでは動作は不定
tVpiConstType
EiExprBase1::constant_type() const
{
  // とは言っても何か返さなければならない．
  return kVpiDecConst;
}

// @brief 対象の関数を返す．
// @note function call の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlTaskFunc*
EiExprBase1::function() const
{
  return NULL;
}

// @brief 対象のシステム関数を返す．
// @note system function call の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlUserSystf*
EiExprBase1::user_systf() const
{
  return NULL;
}

// @brief 引数の数を返す．
// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
// @note このクラスでは 0 を返す．
ymuint
EiExprBase1::argument_num() const
{
  return 0;
}

// @brief 引数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
// @note このクラスでは NULL を返す．
const VlExpr*
EiExprBase1::argument(ymuint pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiOperation
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
EiOperation::EiOperation(const PtBase* pt_expr) :
  EiExprBase1(pt_expr)
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

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiOperation::decompile_impl(int ppri) const
{
  // 各演算子を表す文字列のテーブル
  static const char* sym_table[] = {
    "",               // dummy
    "-",              // vpiMinusOp
    "+",              // vpiPlusOp
    "!",              // vpiNotOp
    "~",              // vpiBitNeg
    "&",              // vpiUnaryAndOp
    "~&",             // vpiUnaryNandOp
    "|",              // vpiUnaryOrOp
    "~|",             // vpiUnaryNorOp
    "^",              // vpiUnaryXorOp
    "~^",             // vpiUnaryXNorOp
    "-",              // vpiSubOp
    "/",              // vpiDivOp
    "%",              // vpiModOp
    "==",             // vpiEqOp
    "!=",             // vpiNeqOp
    "===",            // vpiCaseEqOp
    "!==",            // vpiCaseNeqOp
    ">",              // vpiGtOp
    ">=",             // vpiGeOp
    "<",              // vpiLtOp
    "<=",             // vpiLeOp
    "<<",             // vpiLShiftOp
    ">>",             // vpiRShiftOp
    "+",              // vpiAddOp
    "*",              // vpiMultOp
    "&&",             // vpiLogAndOp
    "||",             // vpiLogOrOp
    "&",              // vpiBitAndOp
    "|",              // vpiBitOrOp
    "^",              // vpiBitXorOp
    "~^",             // vpiBitXNorOp
    "?:",             // vpiConditionOp(dummy)
    "{}",             // vpiConcatOp(dummy)
    "{{}}",           // vpiMultiConcatOp(dummy)
    "or",             // vpiEventOrOp(dummy)
    ";",              // vpiNullOp(dummy)
    ",",              // vpiListOp(dummy)
    ":",              // vpiMinTypMaxOp(dummy)
    "posedge ",       // vpiPosedgeOp
    "negedge ",       // vpiNegedgeOp
    "<<<",            // vpiArithLShiftOp
    ">>>",            // vpiArithRShiftOp
    "**"              // vpiPowerOp
  };

  // 優先順位のテーブル
  static int pri_table[] = {
    0,                // dummy
    13,               // vpiMinusOp
    13,               // vpiPlusOp
    13,               // vpiNotOp
    13,               // vpiBitNegOp
    13,               // vpiUnaryAndOp
    13,               // vpiUnaryNandOp
    13,               // vpiUnaryOrOp
    13,               // vpiUnaryNorOp
    13,               // vpiUnaryXorOp
    13,               // vpiUnaryXNorOp
    10,               // vpiSubOp
    11,               // vpiDivOp
    11,               // vpiModOp
    7,                // vpiEqOp
    7,                // vpiNeqOp
    7,                // vpiCaseEqOp
    7,                // vpiCaseNeqOp
    8,                // vpiGtOp
    8,                // vpiGeOp
    8,                // vpiLtOp
    8,                // vpiLeOp
    9,                // vpiLShiftOp
    9,                // vpiRShiftOp
    10,               // vpiAddOp
    11,               // vpiMultOp
    3,                // vpiLogAndOp
    2,                // vpiLogOrOp
    6,                // vpiBitAndOp
    4,                // vpiBitOrOp
    5,                // vpiBitXorOp
    5,                // vpiBitXNorOp
    1,                // vpiConditionOp
    0,                // vpiConcatOp
    0,                // vpiMultiConcatOp
    0,                // vpiEventOrOp
    0,                // vpiNullOp
    0,                // vpiListOp
    0,                // vpiMinTypMaxOp
    0,                // vpiPosedgeOp
    0,                // vpiNegedgeOp
    9,                // vpiArithLShiftOp
    9,                // vpiArithRShiftOp
    12                // vpiPowerOp
  };

  string ans;

  tVpiOpType optype = op_type();
  // parent_optype の優先順位が自分の優先順位よりも高ければ括弧が必要
  bool need_par = false;
  int pri = pri_table[optype];
  if ( ppri > pri ) {
    need_par = true;
    ans += "(";
  }

  switch ( optype ) {

    // 空
  case kVpiNullOp:
    break;

    // 単項演算子
  case kVpiMinusOp:
  case kVpiNotOp:
  case kVpiBitNegOp:
  case kVpiPlusOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryXNorOp:
  case kVpiUnaryXorOp:
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    ans += sym_table[optype] + _operand(0)->decompile_impl(pri);
    break;

    // 二項演算子
  case kVpiAddOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp:
  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiDivOp:
  case kVpiEqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLShiftOp:
  case kVpiLeOp:
  case kVpiLogAndOp:
  case kVpiLogOrOp:
  case kVpiLtOp:
  case kVpiModOp:
  case kVpiMultOp:
  case kVpiNeqOp:
  case kVpiPowerOp:
  case kVpiRShiftOp:
  case kVpiSubOp:
    ans += _operand(0)->decompile_impl(pri) + sym_table[optype] +
      _operand(1)->decompile_impl(pri);
    break;

    // 三項演算子
  case kVpiConditionOp:
    ans += _operand(0)->decompile_impl(pri) + "?" +
      _operand(1)->decompile_impl(pri) + ":" +
      _operand(2)->decompile_impl(pri);
    break;

  case kVpiMinTypMaxOp:
    ans += _operand(0)->decompile_impl(pri) + ":" +
      _operand(1)->decompile_impl(pri) + ":" +
      _operand(2)->decompile_impl(pri);
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( need_par ) {
    ans += ")";
  }
  return ans;
}

END_NAMESPACE_YM_VERILOG
