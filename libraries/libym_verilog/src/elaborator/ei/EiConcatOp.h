#ifndef EICONCATOP_H
#define EICONCATOP_H

/// @file EiConcatOp.h
/// @brief 結合演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConcatOp EiConcatOp.h "EiConcatOp.h"
/// @brief 結合演算子
//////////////////////////////////////////////////////////////////////
class EiConcatOp :
  public EiOperation
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr_size オペランド数
  /// @param[in] opr_array オペランドを格納する配列
  EiConcatOp(const PtExpr* pt_expr,
	     ymuint opr_size,
	     ElbExpr** opr_array);

  /// @brief デストラクタ
  virtual
  ~EiConcatOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note オペランドが定数ならこの式も定数となる．
  virtual
  bool
  is_const() const;

  /// @brief オペランド数を返す．
  virtual
  ymuint
  operand_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  _set_reqsize(const VlValueType& type);


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランドを返す．
  virtual
  ElbExpr*
  _operand(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット幅を返す．
  ymuint
  bit_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド数
  ymuint32 mOprNum;

  // オペランドの配列
  ElbExpr** mOprList;

  // 結果のビット幅
  ymuint32 mSize;

};


//////////////////////////////////////////////////////////////////////
/// @class EiMultiConcatOp EiConcatOp.h "EiConcatOp.h"
/// @brief マルチ結合演算子
//////////////////////////////////////////////////////////////////////
class EiMultiConcatOp :
  public EiConcatOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] rep_num 繰り返し数
  /// @param[in] rep_expr 繰り返し数を表す式
  /// @param[in] opr_size オペランド数
  /// @param[in] opr_array オペランドを格納する配列
  /// @note は opr_size は繰り返し数のオペランドは含まない．
  EiMultiConcatOp(const PtExpr* pt_expr,
		  int rep_num,
		  ElbExpr* rep_expr,
		  ymuint opr_size,
		  ElbExpr** opr_array);

  /// @brief デストラクタ
  virtual
  ~EiMultiConcatOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief オペランド数を返す．
  virtual
  ymuint
  operand_num() const;

  /// @brief 繰り返し数を返す．
  /// @note multiple concatenation の時のみ意味を持つ．
  virtual
  ymuint
  rep_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  virtual
  ElbExpr*
  _operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数
  int mRepNum;

  // 繰り返し数を表す式
  ElbExpr* mRepExpr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ビット幅を返す．
inline
ymuint
EiConcatOp::bit_size() const
{
  return mSize;
}

END_NAMESPACE_YM_VERILOG

#endif // EICONCATOP_H
