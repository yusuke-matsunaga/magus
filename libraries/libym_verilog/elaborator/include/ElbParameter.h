#ifndef LIBYM_VERILOG_ELB_ELBPAREMETER_H
#define LIBYM_VERILOG_ELB_ELBPARAMETER_H

/// @file libym_verilog/elaborator/include/ElbParameter.h
/// @brief ElbParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlDecl.h"
#include "ElbFwd.h"


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
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  right_range() const = 0;

  /// @brief ビット幅を返す．
  /// @note このクラスでは 1 を返す．
  virtual
  ymuint32
  bit_size() const = 0;

  /// @brief LSB からのオフセット値の取得
  /// @param[in] index インデックス
  /// @retval index の LSB からのオフセット index が範囲内に入っている．
  /// @retval -1 index が範囲外
  virtual
  int
  bit_offset(int index) const = 0;

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
  
  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const = 0;
  
  /// @brief 定数値を持つ型のときに true を返す．
  /// @note このクラスは true を返す．
  virtual
  bool
  is_consttype() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  virtual
  const VlExpr*
  left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  virtual
  const VlExpr*
  right_range() const;

  /// @brief データ型の取得
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const = 0;
  
  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note クラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note クラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note クラスでは kVpiNoStrength を返す．
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

  /// @brief 配列型オブジェクトの場合の次元数の取得
  /// @note このクラスでは 0 を返す．
  virtual
  ymuint32
  dimension() const;
  
  /// @brief 範囲の取得
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  /// @note このクラスでは NULL を返す．
  virtual
  const VlRange*
  range(ymuint32 pos) const;

  
public:
  //////////////////////////////////////////////////////////////////////
  // ElbParameter の仮想関数
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  virtual
  void
  set_expr(ElbExpr* expr) = 0;
  
  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _left_range() const = 0;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _right_range() const = 0;

  /// @brief int 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return 整数値に変換できたら true を返す．
  virtual
  bool
  eval_int(int& val) const = 0;

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  eval_scalar() const = 0;

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  eval_logic() const = 0;

  /// @brief 論理値を返す．
  virtual
  bool
  eval_bool() const = 0;
  
  /// @brief real 型の値を返す．
  virtual
  double
  eval_real() const = 0;

  /// @brief VlTime 型の値を返す．
  /// @param[out] val 結果を格納する変数
  /// @return VlTime 値に変換できたら true を返す．
  virtual
  bool
  eval_time(VlTime& val) const = 0;

  /// @brief bitvector 型の値を返す．
  virtual
  void
  eval_bitvector(BitVector& bitvector,
		 tVpiValueType req_type = kVpiValueNone) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParameter の仮想関数
  //////////////////////////////////////////////////////////////////////

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
  
// @brief コンストラクタ
inline
ElbParamHead::ElbParamHead()
{
}

// @brief デストラクタ
inline
ElbParamHead::~ElbParamHead()
{
}
  
// @brief コンストラクタ
inline
ElbParameter::ElbParameter() :
  mNext(NULL)
{
}
  
// @brief デストラクタ
inline
ElbParameter::~ElbParameter()
{
}

// @brief 次の要素を返す．
inline
const ElbParameter*
ElbParameter::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBPARAMETER_H
