#ifndef LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBEXPR_H
#define LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBEXPR_H

/// @file libym_verilog/elaborator/include/ElbExpr.h
/// @brief ElbExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlExpr.h"
#include "ym_verilog/VlValueType.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbExpr ElbExpr.h "ElbExpr.h"
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class ElbExpr :
  public VlExpr
{
protected:

  /// @brief コンストラクタ
  ElbExpr();

  /// @brief デストラクタ
  virtual
  ~ElbExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求された値のタイプを返す．
  virtual
  VlValueType
  req_type() const;

  /// @brief 式のビット幅を返す．
  virtual
  ymuint
  bit_size() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_reqsize(const VlValueType& type);

  /// @brief 要求される式のサイズを自分で決めてセットする．
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_selfsize();

  /// @brief set_reqsize() の下請け関数
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  _set_reqsize(const VlValueType& type) = 0;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  virtual
  ElbExpr*
  _operand(ymuint pos) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////

  // 二項演算のタイプとサイズを決める．
  static
  VlValueType
  calc_type(const VlValueType& type0,
	    const VlValueType& type1);


  // 巾乗演算のタイプとサイズを決める．
  static
  VlValueType
  calc_type2(const VlValueType& type0,
	     const VlValueType& type1);

  // 出力に要求されているサイズから自分のサイズを決める．
  static
  VlValueType
  update_size(const VlValueType& type,
	      const VlValueType& req_type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要求された値のタイプ
  VlValueType mReqType;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
inline
void
ElbExpr::set_reqsize(const VlValueType& type)
{
  mReqType = type;
  _set_reqsize(type);
}

// @brief 要求される式のサイズを自分で決めてセットする．
// @note 必要であればオペランドに対して再帰的に処理を行なう．
inline
void
ElbExpr::set_selfsize()
{
  set_reqsize(value_type());
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBEXPR_H
