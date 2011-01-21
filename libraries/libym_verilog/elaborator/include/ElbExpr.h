#ifndef LIBYM_VERILOG_ELB_ELBEXPR_H
#define LIBYM_VERILOG_ELB_ELBEXPR_H

/// @file libym_verilog/elaborator/include/ElbExpr.h
/// @brief ElbExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbExpr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlExpr.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbExpr ElbExpr.h <ym_verilog/vl/VlExpr.h>
/// @brief エラボレーション中の expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class ElbExpr :
  public VlExpr
{
protected:

  /// @brief コンストラクタ
  ElbExpr() { }

  /// @brief デストラクタ
  virtual
  ~ElbExpr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  string
  decompile() const;

#if 0
  /// @brief int 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return 整数値に変換できたら true を返す．
  /// @note eval_bitvector() の結果から変換する．
  virtual
  bool
  eval_int(int& val) const;

  /// @brief 論理値を返す．
  /// @note eval_scalar() の結果から変換する．
  virtual
  bool
  eval_bool() const;

  /// @brief VlTime 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return VlTime 値に変換できたら true を返す．
  /// @note eval_bitvector() の結果から変換する．
  virtual
  bool
  eval_time(VlTime& val) const;
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
  set_reqsize(tVpiValueType type) = 0;

  /// @brief 要求される式のサイズを自分で決めてセットする．
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  void
  set_selfsize();

  /// @brief decompile() の実装関数
  /// @param[in] pprim 親の演算子の優先順位
  virtual
  string
  decompile_impl(int ppri) const = 0;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _operand(ymuint pos) const;

#if 0
  /// @brief スカラー値を書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_scalar(tVpiScalarVal v) = 0;

  /// @brief 実数値を書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_real(double v) = 0;

  /// @brief ビットベクタを書き込む．
  /// @param[in] v 書き込む値
  /// @note 左辺式の時のみ意味を持つ．
  virtual
  void
  set_bitvector(const BitVector& v) = 0;
#endif


public:
  //////////////////////////////////////////////////////////////////////
  // 便利関数
  //////////////////////////////////////////////////////////////////////

  // 二項演算のタイプとサイズを決める．
  static
  tVpiValueType
  calc_type(tVpiValueType type0,
	    tVpiValueType type1);


  // 巾乗演算のタイプとサイズを決める．
  static
  tVpiValueType
  calc_type2(tVpiValueType type0,
	     tVpiValueType type1);

  // 出力に要求されているサイズから自分のサイズを決める．
  static
  tVpiValueType
  update_size(tVpiValueType type,
	      tVpiValueType req_type);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 要求される式のサイズを自分で決めてセットする．
// @note 必要であればオペランドに対して再帰的に処理を行なう．
inline
void
ElbExpr::set_selfsize()
{
  set_reqsize(value_type());
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBEXPR_H
