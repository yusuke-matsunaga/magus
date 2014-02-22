#ifndef EIBITSELECT_H
#define EIBITSELECT_H

/// @file EiBitSelect.h
/// @brief EiBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief ビット選択式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiBitSelect :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  EiBitSelect(const PtExpr* pt_expr,
	      ElbExpr* base_expr);

  /// @brief デストラクタ
  virtual
  ~EiBitSelect();


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

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note 参照している要素の型によって決まる．
  virtual
  bool
  is_const() const;

  /// @brief ビット指定の時に true を返す．
  virtual
  bool
  is_bitselect() const;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  /// @note それ以外では NULL を返す．
  virtual
  const VlDeclBase*
  decl_base() const;

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


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  _set_reqsize(const VlValueType& type);

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  ElbExpr* mBaseExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 固定ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstBitSelect :
  public EiBitSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  /// @param[in] index_val ビット選択式の値
  EiConstBitSelect(const PtExpr* pt_expr,
		   ElbExpr* base_expr,
		   const PtExpr* index_expr,
		   int index_val);

  /// @brief デストラクタ
  virtual
  ~EiConstBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

  /// @brief インデックス式を返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief インデックス値を返す．
  virtual
  int
  index_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス式
  const PtExpr* mIndexExpr;

  // インデックス値
  int mIndexVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiVarBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 可変ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarBitSelect :
  public EiBitSelect
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  EiVarBitSelect(const PtExpr* pt_expr,
		 ElbExpr* base_expr,
		 ElbExpr* index_expr);

  /// @brief デストラクタ
  virtual
  ~EiVarBitSelect();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 固定選択子の時 true を返す．
  /// @note ビット選択，部分選択の時，意味を持つ．
  virtual
  bool
  is_constant_select() const;

  /// @brief インデックス式を返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief インデックス値を返す．
  virtual
  int
  index_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス式
  ElbExpr* mIndexExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBITSELECT_H
