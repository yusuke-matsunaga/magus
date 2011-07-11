#ifndef YM_VERILOG_VL_VLEXPR_H
#define YM_VERILOG_VL_VLEXPR_H

/// @file ym_verilog/vl/VlExpr.h
/// @brief VlExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlExpr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlExpr VlExpr.h <ym_verilog/vl/VlExpr.h>
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlExpr :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlExpr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  virtual
  tVpiValueType
  value_type() const = 0;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const = 0;

  /// @brief 式のビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

  /// @brief 定数式の時 true を返す．
  virtual
  bool
  is_const() const = 0;

  /// @brief 部分/ビット指定が定数の時 true を返す．
  /// @note kVpiPartSelect/kVpiBitSelect の時，意味を持つ．
  /// @note それ以外では常に false を返す．
  virtual
  bool
  is_constant_select() const = 0;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const = 0;

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const = 0;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const = 0;

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const = 0;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const = 0;

  /// @brief システム関数呼び出しの時に true を返す．
  virtual
  bool
  is_sysfunccall() const = 0;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDeclBase*
  decl_base() const = 0;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDecl*
  decl_obj() const = 0;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDeclArray*
  declarray_obj() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  virtual
  ymuint
  declarray_dimension() const = 0;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  declarray_index(ymuint pos) const = 0;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  /// @note 固定インデックスの場合のみ意味を持つ．
  virtual
  ymuint
  declarray_offset() const = 0;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では NULL を返す．
  /// @note スコープへの参照が式で用いられるのはシステム関数/タスクの引数だけ
  virtual
  const VlNamedObj*
  scope_obj() const = 0;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  /// @note primitiveへの参照が式で用いられるのはシステム関数/タスクの引数だけ
  virtual
  const VlPrimitive*
  primitive_obj() const = 0;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  parent_expr() const = 0;

  /// @brief インデックス式を返す．
  /// @note ビット選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  index() const = 0;

  /// @brief インデックス値を返す．
  /// @note 固定ビット選択の時，意味を持つ．
  /// @note それ以外では値は不定
  virtual
  int
  index_val() const = 0;

  /// @brief 範囲指定のモードを返す．
  /// @retval kVpiNoRange 範囲指定なし
  /// @retval kVpiConstRange 固定範囲
  /// @retval kVpiPlusRange +: の可動範囲
  /// @retval kVpiMinusRange -: の可動範囲
  /// @note is_partselect() == true の時のみ意味を持つ．
  virtual
  tVpiRangeMode
  range_mode() const = 0;

  /// @brief 範囲の MSB の式を返す．
  /// @note 範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  left_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @note それ以外では値は不定
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の式を返す．
  /// @note 範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  right_range() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @note それ以外では値は不定
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のベースを表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  base() const = 0;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では値は不定
  virtual
  int
  range_width() const = 0;

  /// @brief 演算子の型を返す．
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  tVpiOpType
  op_type() const = 0;

  /// @brief オペランド数を返す．
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  ymuint
  operand_num() const = 0;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  operand(ymuint pos) const = 0;

  /// @brief オペランドに要求されるデータ型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では kVpiValueNone を返す．
  /// 通常はオペランドの式の value_type() に一致するが，
  /// その式が self-typed の場合には異なることもある．
  virtual
  tVpiValueType
  operand_type(ymuint pos) const = 0;

  /// @brief 繰り返し数を返す．
  /// @note multiple concatenation の時のみ意味を持つ．
  virtual
  ymuint
  rep_num() const = 0;

  /// @brief 定数型を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  tVpiConstType
  constant_type() const = 0;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  VlValue
  constant_value() const = 0;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlTaskFunc*
  function() const = 0;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlUserSystf*
  user_systf() const = 0;

  /// @brief 引数の数を返す．
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  virtual
  ymuint
  argument_num() const = 0;

  /// @brief 引数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  argument(ymuint pos) const = 0;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  lhs_elem_num() const = 0;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLEXPR_H
