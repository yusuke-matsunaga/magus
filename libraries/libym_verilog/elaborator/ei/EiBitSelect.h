#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIBITSELECT_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIBITSELECT_H

/// @file libym_verilog/elaborator/ei/EiBitSelect.h
/// @brief EiBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBitSelect.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 可変ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBitSelect :
  public EiExprBase1
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  EiBitSelect(const PtBase* pt_expr,
	      ElbExpr* base_expr,
	      ElbExpr* index_expr);

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
  tVpiValueType
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

  /// @brief 親の式を返す．
  virtual
  const VlExpr*
  parent_expr() const;

#if 0
  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  eval_real() const;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;
#endif

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

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  ElbExpr* mBaseExpr;

  // インデックス式
  ElbExpr* mIndexExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 固定ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstBitSelect :
  public EiExprBase1
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] base_expr 対象の式
  /// @param[in] index_expr ビット選択式
  /// @param[in] index_val ビット選択式の値
  EiConstBitSelect(const PtBase* pt_expr,
		   ElbExpr* base_expr,
		   const PtExpr* index_expr,
		   int index_val);

  /// @brief デストラクタ
  virtual
  ~EiConstBitSelect();


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
  tVpiValueType
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

  /// @brief 親の式を返す．
  virtual
  const VlExpr*
  parent_expr() const;

#if 0
  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  eval_real() const;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const;
#endif

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

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  ElbExpr* mBaseExpr;

  // インデックス式
  const PtExpr* mIndexExpr;

  // インデックス値
  int mIndexVal;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIBITSELECT_H
