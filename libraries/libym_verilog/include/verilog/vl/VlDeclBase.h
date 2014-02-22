#ifndef VERILOG_VL_VLDECLBASE_H
#define VERILOG_VL_VLDECLBASE_H

/// @file verilog/vl/VlDeclBase.h
/// @brief VlDeclBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlNamedObj.h"
#include "verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlDeclBase VlDeclBase.h "ym_verilog/vl/VlDeclBase.h"
/// @brief エラボレーション中の名前付きオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlDeclBase :
  public VlNamedObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDeclBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDeclBase の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  VlValueType
  value_type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const = 0;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const = 0;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const = 0;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const = 0;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const = 0;

  /// @brief データ型の取得
  /// @retval kVpiVarInteger 整数型
  /// @retval kVpiVarReal 実数型
  /// @retval kVpiVarTime time型
  /// @retval kVpiVarRealTime realtime型
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;

  /// @brief net 型の取得
  /// @retval net型 net型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const = 0;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const VlDelay*
  delay() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLDECL_H
