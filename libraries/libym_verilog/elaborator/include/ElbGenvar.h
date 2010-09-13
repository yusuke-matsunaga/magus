#ifndef LIBYM_VERILOG_ELB_ELBGENVAR_H
#define LIBYM_VERILOG_ELB_ELBGENVAR_H

/// @file libym_verilog/elabotor/include/ElbGenvar.h
/// @brief EiGenvar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbGenvar.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "ym_verilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbGenvar ElbGenvar.h "ElbGenvar.h"
/// @brief genvar を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbGenvar :
  public ElbDecl
{
protected:

  /// @brief コンストラクタ
  ElbGenvar() { }

  /// @brief デストラクタ
  virtual
  ~ElbGenvar() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 定数値を持つ型のときに true を返す．
  virtual
  bool
  is_consttype() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief MSB の値を返す．
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  virtual
  int
  right_range_const() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint32
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @retval index のオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note デフォルト値としてこのクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note デフォルト値としてこのクラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note デフォルト値としてこのクラスでは NULL を返す．
  virtual
  const VlDelay*
  delay() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  /// @note このクラスではなにもしない．
  virtual
  void
  set_signed();

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _right_range() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbGenvar に固有の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用中の時 true を返す．
  virtual
  bool
  is_inuse() const = 0;

  /// @brief 使用中にする．
  virtual
  void
  set_inuse() = 0;

  /// @brief 使用を終える．
  virtual
  void
  reset_inuse() = 0;

  /// @brief 現在の値を返す．
  virtual
  int
  value() const = 0;

  /// @brief 値を設定する．
  virtual
  void
  set_value(int value) = 0;

  /// @brief 元となったパース木の定義要素を返す．
  virtual
  const PtDeclItem*
  pt_item() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBGENVAR_H
