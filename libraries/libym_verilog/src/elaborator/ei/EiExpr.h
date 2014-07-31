#ifndef EIEXPR_H
#define EIEXPR_H

/// @file EiExpr.h
/// @brief EiExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbExpr.h"
#include "YmVerilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiExpr EiExpr.h "EiExpr.h"
/// @brief ExprExpr の実装クラス
//////////////////////////////////////////////////////////////////////
class EiExpr :
  public ElbExpr
{
protected:

  /// @brief コンストラクタ
  EiExpr();

  /// @brief デストラクタ
  virtual
  ~EiExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_const() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_constant_select() const;

  /// @brief プライマリ(net/reg/variables/parameter)の時に true を返す．
  virtual
  bool
  is_primary() const;

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const;

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const;

  /// @brief システム関数よびあどい時に true を返す．
  virtual
  bool
  is_sysfunccall() const;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDeclBase*
  decl_base() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  decl_obj() const;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDeclArray*
  declarray_obj() const;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint
  declarray_dimension() const;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
 /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  declarray_index(ymuint pos) const;

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  /// @note 固定インデックスの場合のみ意味を持つ．
  virtual
  ymuint
  declarray_offset() const;

  /// @brief スコープへの参照の場合，対象のオブジェクトを返す．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlNamedObj*
  scope_obj() const;

  /// @brief primitive への参照の場合，対象のオブジェクトを返す．
  virtual
  const VlPrimitive*
  primitive_obj() const;

  /// @brief 親の式を返す．
  /// @note 式に対するビット選択/範囲選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  parent_expr() const;

  /// @brief インデックス式を返す．
  /// @note ビット選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief インデックス値を返す．
  /// @note 式に対するビット選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  int
  index_val() const;

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 範囲の MSB を返す．
  /// @note 部分選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB を返す．
  /// @note 部分選択の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のベースを表す式を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  base() const;

  /// @brief 範囲のビット幅を返す．
  /// @note 可変範囲選択の時，意味を持つ．
  /// @note それ以外では 0 を返す．
  virtual
  int
  range_width() const;

  /// @brief 演算子のタイプを返す．
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは kVlNullOp を返す．
  virtual
  tVlOpType
  op_type() const;

  /// @brief オペランド数を返す．
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  operand(ymuint pos) const;

  /// @brief 繰り返し数を返す．
  /// @note multiple concatenation の時のみ意味を持つ．
  virtual
  ymuint
  rep_num() const;

  /// @brief 定数の型を返す．
  /// @note 定数の時，意味を持つ．
  /// @note このクラスでは動作は不定
  virtual
  tVpiConstType
  constant_type() const;

  /// @brief 定数値を返す．
  /// @note kVpiConstant の時，意味を持つ．
  /// @note それ以外では動作は不定
  virtual
  VlValue
  constant_value() const;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlTaskFunc*
  function() const;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlUserSystf*
  user_systf() const;

  /// @brief 引数の数を返す．
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint
  argument_num() const;

  /// @brief 引数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  /// @note kVpiFuncCall/kVpiSysFuncCall の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  argument(ymuint pos) const;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint
  lhs_elem_num() const;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは NULL を返す．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を返す．
  virtual
  const PtBase*
  pt_obj() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class EiExprBase EiExpr.h "EiExpr.h"
/// @brief PtExpr を持つ EiExpr の派生クラス
//////////////////////////////////////////////////////////////////////
class EiExprBase :
  public EiExpr
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  EiExprBase(const PtExpr* pt_expr);

  /// @brief デストラクタ
  virtual
  ~EiExprBase();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EiExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の定義要素を返す．
  virtual
  const PtBase*
  pt_obj() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr を得る．
  const PtExpr*
  pt_expr() const { return mPtExpr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtExpr* mPtExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIEXPR_H
