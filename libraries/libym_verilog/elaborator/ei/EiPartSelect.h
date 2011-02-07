#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPARTSELECT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPARTSELECT_H

/// @file libym_verilog/elaborator/ei/EiPartSelect.h
/// @brief EiPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPartSelect.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief パート選択式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiPartSelect :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  EiPartSelect(const PtExpr* pt_expr,
	       ElbExpr* parent_expr);

  /// @brief デストラクタ
  virtual
  ~EiPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief 範囲指定の時に true を返す．
  virtual
  bool
  is_partselect() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  virtual
  const VlDecl*
  decl_obj() const;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  /// @note 宣言要素に対するビット選択，部分選択の場合にも意味を持つ．
  virtual
  const VlDeclArray*
  declarray_obj() const;

  /// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
  /// @note それ以外では 0 を返す．
  virtual
  ymuint
  declarray_dimension() const;

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
  /// @note それ以外では NULL を返す．
  virtual
  const VlExpr*
  declarray_index(ymuint pos) const;

  /// @brief 親の式を返す．
  virtual
  const VlExpr*
  parent_expr() const;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  /// @note このクラスでは 1 を返す．
  virtual
  ymuint
  lhs_elem_num() const;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  /// @note このクラスでは pos = 0 の時，自分自身を返す．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  ElbExpr* mParentExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief 固定パート選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstPartSelect :
  public EiPartSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] index1, index2 パート選択式
  /// @param[in] index1_val, index2_val パート選択式の値
  EiConstPartSelect(const PtExpr* pt_expr,
		    ElbExpr* parent_expr,
		    const PtExpr* index1,
		    const PtExpr* index2,
		    int index1_val,
		    int index2_val);

  /// @brief デストラクタ
  virtual
  ~EiConstPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

  /// @brief 範囲の MSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲の LSB の式を返す．
  /// @note 通常の範囲選択の時，意味を持つ．
  virtual
  const VlExpr*
  right_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 式に対する範囲選択の時，意味を持つ．
  virtual
  int
  right_range_val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲選択の MSB
  const PtExpr* mLeftRange;

  // 範囲選択の LSB
  const PtExpr* mRightRange;

  // 範囲選択の MSB の値
  int mLeftVal;

  // 範囲選択の LSB の値
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiVarPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarPartSelect :
  public EiPartSelect
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiVarPartSelect(const PtExpr* pt_expr,
		  ElbExpr* parent_expr,
		  ElbExpr* base,
		  const PtExpr* range,
		  int range_val);

  /// @brief デストラクタ
  virtual
  ~EiVarPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

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


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(tVpiValueType type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲のベースの式
  ElbExpr* mBaseExpr;

  // 範囲
  const PtExpr* mRangeExpr;

  // 範囲の値
  int mRangeVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPlusPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択式([ +: ])を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPlusPartSelect :
  public EiVarPartSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] obj 本体の式
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiPlusPartSelect(const PtExpr* pt_expr,
		   ElbExpr* parent_expr,
		   ElbExpr* base,
		   const PtExpr* range,
		   int range_val);

  /// @brief デストラクタ
  virtual
  ~EiPlusPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMinusPartSelect EiVarPartSelect.h "EiVarPartSelect.h"
/// @brief 可変パート選択式([ -: ])を表すクラス
//////////////////////////////////////////////////////////////////////
class EiMinusPartSelect :
  public EiVarPartSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_expr 対象の式
  /// @param[in] base 範囲のベースアドレスを表す式
  /// @param[in] range 範囲を表す式
  /// @param[in] range_val 範囲の値
  EiMinusPartSelect(const PtExpr* pt_expr,
		    ElbExpr* parent_expr,
		    ElbExpr* base,
		    const PtExpr* range,
		    int range_val);

  /// @brief デストラクタ
  virtual
  ~EiMinusPartSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  virtual
  tVpiRangeMode
  range_mode() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPARTSELECT_H
