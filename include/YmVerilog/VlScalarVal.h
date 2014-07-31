#ifndef VERILOG_VLSCALARVAL_H
#define VERILOG_VLSCALARVAL_H

/// @file YmVerilog/VlScalarVal.h
/// @brief VlScalarVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlScalarVal VlScalarVal.h "ym_YmVerilog/VlScalarVal.h"
/// @brief 1ビットの値を表すクラス
/// @note 具体的には
///  - 0
///  - 1
///  - X ( 0 か 1 か不明 )
///  - Z ( high-impedance )
///  の4種類の値を持つ．
//////////////////////////////////////////////////////////////////////
class VlScalarVal
{
public:

  /// @brief 空のコンストラクタ
  /// @note 不定値となる．
  VlScalarVal();

  /// @brief 整数値からの変換コンストラクタ
  /// @param[in] val 値
  /// @note val が 0 の時のみ 0 に，それ以外は 1 にする．
  explicit
  VlScalarVal(int val);

  /// @brief 符号なし整数値からの変換コンストラクタ
  /// @param[in] val 値
  /// @note val が 0 の時のみ 0 に，それ以外は 1 にする．
  explicit
  VlScalarVal(ymuint val);

  /// @brief 実数値からの変換コンストラクタ
  /// @param[in] val 値
  /// @note val が 0.0 の時のみ 0 に，それ以外は 1 にする．
  explicit
  VlScalarVal(double val);

  /// @brief ブール値からの変換コンストラクタ
  /// @param[in] val 値
  /// @note val が false なら 0 に，true なら 1 にする．
  explicit
  VlScalarVal(bool val);

  /// @brief デストラクタ
  ~VlScalarVal();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 0 を作る．
  static
  VlScalarVal
  zero();

  /// @brief 1 を作る．
  static
  VlScalarVal
  one();

  /// @brief X を作る．
  static
  VlScalarVal
  x();

  /// @brief Z を作る．
  static
  VlScalarVal
  z();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を調べる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 0 の時に true を返す．
  bool
  is_zero() const;

  /// @brief 1 の時に true を返す．
  bool
  is_one() const;

  /// @brief X の時に true を返す．
  bool
  is_x() const;

  /// @brief Z の時に true を返す．
  bool
  is_z() const;

  /// @brief X/Z の時に true を返す．
  bool
  is_xz() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 変換用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブール値に変換する．
  /// @retval true 値が 1 の時
  /// @retval false それ以外
  bool
  to_bool() const;

  /// @brief 論理値に変換する．
  /// @retval 0 値が 0 の時
  /// @retval 1 値が 1 の時
  /// @retval X 値が X か Z の時
  VlScalarVal
  to_logic() const;

  /// @brief 整数値に変換する．
  /// @retval 1 値が 1 の時
  /// @retval 0 それ以外
  int
  to_int() const;

  /// @brief 実数値に変換する．
  /// @retval 1.0 値が 1 の時
  /// @retval 0.0 それ以外
  double
  to_real() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定
  VlScalarVal
  operator!() const;

  /// @brief 選言(Conjunction)
  /// @param[in] right オペランド
  VlScalarVal
  operator&&(const VlScalarVal& right) const;

  /// @brief 連言(Disjunction)
  /// @param[in] right オペランド
  VlScalarVal
  operator||(const VlScalarVal& right) const;

  /// @brief 値が等しいときに true を返す．
  /// @param[in] right オペランド
  bool
  operator==(const VlScalarVal& right) const;

  /// @brief 値が等しくないときに true を返す．
  /// @param[in] right オペランド
  bool
  operator!=(const VlScalarVal& right) const;


public:
  //////////////////////////////////////////////////////////////////////
  // フレンド関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較
  /// @param[in] left, right オペランド
  /// @note どちらか一方に X/Z を含む時，答も X になる．
  friend
  VlScalarVal
  eq(const VlScalarVal& left,
     const VlScalarVal& right);

  /// @brief  非等価比較
  /// @param[in] left, right オペランド
  /// @note どちらか一方に X/Z を含む時，答も X になる．
  friend
  VlScalarVal
  neq(const VlScalarVal& left,
      const VlScalarVal& right);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  explicit
  VlScalarVal(ymuint8 val);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 0 を表す値
  static
  const ymuint8 kScalar0 = 0U;

  /// @brief 1 を表す値
  static
  const ymuint8 kScalar1 = 1U;

  /// @brief X を表す値
  static
  const ymuint8 kScalarX = 2U;

  /// @brief high-impedance を表す値
  static
  const ymuint8 kScalarZ = 3U;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ymuint8 mData;

};

/// @brief ストリーム出力
/// @relates VlScalarVal
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
ostream&
operator<<(ostream& s,
	   const VlScalarVal& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 不定値となる．
inline
VlScalarVal::VlScalarVal() : mData(kScalarX)
{
}

// @brief 整数値からの変換コンストラクタ
// @param[in] val 値
// @note val が 0 の時のみ 0 に，それ以外は 1 にする．
inline
VlScalarVal::VlScalarVal(int val)
{
  if ( val == 0 ) {
    mData = kScalar0;
  }
  else {
    mData = kScalar1;
  }
}

// @brief 符号なし整数値からの変換コンストラクタ
// @param[in] val 値
// @note val が 0 の時のみ 0 に，それ以外は 1 にする．
inline
VlScalarVal::VlScalarVal(ymuint val)
{
  if ( val == 0U ) {
    mData = kScalar0;
  }
  else {
    mData = kScalar1;
  }
}

// @brief 実数値からの変換コンストラクタ
// @param[in] val 値
// @note val が 0.0 の時のみ 0 に，それ以外は 1 にする．
inline
VlScalarVal::VlScalarVal(double val)
{
  if ( val == 0.0 ) {
    mData = kScalar0;
  }
  else {
    mData = kScalar1;
  }
}

// @brief ブール値からの変換コンストラクタ
// @param[in] val 値
// @note val が false なら 0 に，true なら 1 にする．
inline
VlScalarVal::VlScalarVal(bool val)
{
  if ( val ) {
    mData = kScalar1;
  }
  else {
    mData = kScalar0;
  }
}

// @brief 値を直接指定するコンストラクタ
inline
VlScalarVal::VlScalarVal(ymuint8 val) :
  mData(val)
{
}

// @brief デストラクタ
inline
VlScalarVal::~VlScalarVal()
{
}

// @brief 0 を作る．
inline
VlScalarVal
VlScalarVal::zero()
{
  return VlScalarVal(kScalar0);
}

// @brief 1 を作る．
inline
VlScalarVal
VlScalarVal::one()
{
  return VlScalarVal(kScalar1);
}

// @brief X を作る．
inline
VlScalarVal
VlScalarVal::x()
{
  return VlScalarVal(kScalarX);
}

// @brief Z を作る．
inline
VlScalarVal
VlScalarVal::z()
{
  return VlScalarVal(kScalarZ);
}

// @brief 0 の時に true を返す．
inline
bool
VlScalarVal::is_zero() const
{
  return mData == kScalar0;
}

// @brief 1 の時に true を返す．
inline
bool
VlScalarVal::is_one() const
{
  return mData == kScalar1;
}

// @brief X の時に true を返す．
inline
bool
VlScalarVal::is_x() const
{
  return mData == kScalarX;
}

// @biref Z の時に true を返す．
inline
bool
VlScalarVal::is_z() const
{
  return mData == kScalarZ;
}

// @brief X/Z の時に true を返す．
inline
bool
VlScalarVal::is_xz() const
{
  // kScalar? のコーディングに注意
  return mData >= kScalarX;
}

// @brief ブール値に変換する．
// @retval true 値が 1 の時
// @retval false それ以外
inline
bool
VlScalarVal::to_bool() const
{
  return mData == kScalar1;
}

// @brief 論理値に変換する．
// @retval 0 値が 0 の時
// @retval 1 値が 1 の時
// @retval X 値が X か Z の時
inline
VlScalarVal
VlScalarVal::to_logic() const
{
  switch ( mData ) {
  case kScalar0: return VlScalarVal(kScalar0);
  case kScalar1: return VlScalarVal(kScalar1);
  case kScalarX:
  case kScalarZ: return VlScalarVal(kScalarX);
  }
  assert_not_reached(__FILE__, __LINE__);
  // ダミー
  return VlScalarVal::x();
}

// @brief 整数値に変換する．
// @retval 1 値が 1 の時
// @retval 0 それ以外
inline
int
VlScalarVal::to_int() const
{
  if ( mData == kScalar1 ) {
    return 1;
  }
  else {
    return 0;
  }
}

// @brief 実数値に変換する．
// @retval 1.0 値が 1 の時
// @retval 0.0 それ以外
inline
double
VlScalarVal::to_real() const
{
  if ( mData == kScalar1 ) {
    return 1.0;
  }
  else {
    return 0.0;
  }
}

// @brief 否定
inline
VlScalarVal
VlScalarVal::operator!() const
{
  switch ( mData ) {
  case kScalar0: return VlScalarVal(kScalar1);
  case kScalar1: return VlScalarVal(kScalar0);
  case kScalarX:
  case kScalarZ: return VlScalarVal(kScalarX);
  }
  assert_not_reached(__FILE__, __LINE__);
  // ダミー
  return VlScalarVal::x();
}

// @brief 選言(Conjunction)
inline
VlScalarVal
VlScalarVal::operator&&(const VlScalarVal& right) const
{
  if ( is_zero() || right.is_zero() ) {
    return VlScalarVal::zero();
  }
  if ( is_one() && right.is_one() ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::x();
}

// @brief 連言(Disjunction)
inline
VlScalarVal
VlScalarVal::operator||(const VlScalarVal& right) const
{
  if ( is_one() || right.is_one() ) {
    return VlScalarVal::one();
  }
  if ( is_zero() && right.is_zero() ) {
    return VlScalarVal::zero();
  }
  return VlScalarVal::x();
}

// @brief 値が等しいときに true を返す．
// @param[in] right オペランド
inline
bool
VlScalarVal::operator==(const VlScalarVal& right) const
{
  return mData == right.mData;
}

// @brief 値が等しくないときに true を返す．
// @param[in] right オペランド
inline
bool
VlScalarVal::operator!=(const VlScalarVal& right) const
{
  return mData != right.mData;
}

// @brief 等価比較
// @note どちらか一方に X/Z を含む時，答も X になる．
inline
VlScalarVal
eq(const VlScalarVal& left,
   const VlScalarVal& right)
{
  if ( left.is_xz() || right.is_xz() ) {
    return VlScalarVal::x();
  }
  if ( left.mData == right.mData ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief  非等価比較
// @note どちらか一方に X/Z を含む時，答も X になる．
inline
VlScalarVal
neq(const VlScalarVal& left,
    const VlScalarVal& right)
{
  if ( left.is_xz() || right.is_xz() ) {
    return VlScalarVal::x();
  }
  if ( left.mData != right.mData ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief ストリーム出力
// @relates VlScalarVal
// @param[in] s 出力先のストリーム
// @param[in] val 値
inline
ostream&
operator<<(ostream& s,
	   const VlScalarVal& val)
{
  if ( val.is_zero() ) {
    s << "0";
  }
  else if ( val.is_one() ) {
    s << "1";
  }
  else if ( val.is_x() ) {
    s << "X";
  }
  else if ( val.is_z() ) {
    s << "Z";
  }
  else {
    s << "?";
  }
  return s;
}

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VLSCALARVAL_H
