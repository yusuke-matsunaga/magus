#ifndef ELBPAREMETER_H
#define ELBPARAMETER_H

/// @file ElbParameter.h
/// @brief ElbParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"

#include "YmVerilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbParamHead.h ElbParameter.h "ElbParameter.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbParamHead
{
protected:

  /// @brief コンストラクタ
  ElbParamHead();

  /// @brief デストラクタ
  virtual
  ~ElbParamHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const = 0;

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const = 0;

  /// @brief 符号の取得
  /// @param[in] val 値
  /// @retval true 符号つき
  /// @retval false 符号なし
  /// @note ヘッダに型指定がない時は値から情報を得る．
  virtual
  bool
  is_signed(const VlValue& val) const = 0;

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
  /// @param[in] val 値
  /// @note ヘッダが型指定を持たない時には値から情報を得る．
  virtual
  ymuint
  bit_size(const VlValue& val) const = 0;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @param[in] val 値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  /// @note ヘッダが型指定を持たない時には値から情報を得る．
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset,
		  const VlValue& val) const = 0;

  /// @breif 値の型を返す．
  /// @param[in] val 値
  /// @note ヘッダが型指定を持たない時には値から情報を得る．
  virtual
  VlValueType
  value_type(const VlValue& val) const = 0;

  /// @brief データ型の取得
  virtual
  tVpiVarType
  data_type() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbParameter ElbParameter.h "ElbParameter.h"
/// @brief parameter を表すクラス
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbParameter :
  public VlDecl
{
  friend class CellParam;

protected:

  /// @brief コンストラクタ
  ElbParameter();

  /// @brief デストラクタ
  virtual
  ~ElbParameter();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数値を持つ型のときに true を返す．
  /// @note このクラスは true を返す．
  virtual
  bool
  is_consttype() const;

  /// @brief ビット要素を返す．
  /// @param[in] index インデックス
  /// @retval ビット要素 index が範囲内の場合
  /// @retval NULL index が範囲外の場合．またはビット展開されていない場合
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  elem_by_index(int index) const;

  /// @brief ビット要素を返す．
  /// @param[in] offset オフセット
  /// @retval ビット要素 offset が範囲内の場合
  /// @retval NULL offset が範囲外の場合．またはビット展開されていない場合
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDecl*
  elem_by_offset(ymuint offset) const;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note このクラスでは NULL を返す．
  virtual
  const VlDelay*
  delay() const;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const VlExpr*
  init_value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParameter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  virtual
  VlValue
  get_value() const = 0;

  /// @brief parameter の値の設定
  /// @param[in] expr 値を表す式
  /// @param[in] val 値
  virtual
  void
  set_expr(const PtExpr* expr,
	   const VlValue& val) = 0;

  /// @brief 次の要素を返す．
  const ElbParameter*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbParameter* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を返す．
inline
const ElbParameter*
ElbParameter::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // ELBPARAMETER_H
