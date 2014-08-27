#ifndef VERILOG_VLVALUETYPE_H
#define VERILOG_VLVALUETYPE_H

/// @file YmVerilog/VlValueType.h
/// @brief VlValueType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief integer 型のサイズ
const ymuint32 kVpiSizeInteger = 32U;

/// @brief real 型のサイズ
const ymuint32 kVpiSizeReal = 64U;

/// @brief time 型のサイズ
const ymuint32 kVpiSizeTime = 64U;

//////////////////////////////////////////////////////////////////////
/// @class VlValueType VlValueType.h "ym_YmVerilog/VlValueType.h"
/// @brief verilog の値の型を表すクラス
//////////////////////////////////////////////////////////////////////
class VlValueType
{
public:

  /// @brief 空のコンストラクタ
  /// @note 型の指定なしの意味になる．
  VlValueType();

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] has_sign 符号の有無
  /// @param[in] has_size サイズ指定の有無
  /// @param[in] size サイズ(ビット幅)
  VlValueType(bool has_sign,
	      bool has_size,
	      ymuint size);

  /// @brief 整数型を返す．
  static
  VlValueType
  int_type();

  /// @brief 実数型を返す．
  static
  VlValueType
  real_type();

  /// @brief time 型を返す．
  static
  VlValueType
  time_type();

  /// @brief デストラクタ
  ~VlValueType();


public:

  /// @brief 型の指定がない時に true を返す．
  bool
  is_no_type() const;

  /// @brief 整数型の時に true を返す．
  bool
  is_int_type() const;

  /// @brief time 型の時に true を返す．
  bool
  is_time_type() const;

  /// @brief ビットベクタ型の時に true を返す．
  /// @note 整数型/time型も含む．
  bool
  is_bitvector_type() const;

  /// @brief 実数型の時に true を返す．
  bool
  is_real_type() const;

  /// @brief 符号付きの時に true を返す．
  bool
  is_signed() const;

  /// @brief サイズ指定付きの時に true を返す．
  bool
  is_sized() const;

  /// @brief サイズを返す．
  ymuint
  size() const;

  /// @brief 等価比較演算子
  bool
  operator==(const VlValueType& right) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const VlValueType& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定するコンストラクタ
  /// @param[in] data 値
  VlValueType(ymuint32 data);


private:
  //////////////////////////////////////////////////////////////////////
  // mData の符号化に使われる定数
  //////////////////////////////////////////////////////////////////////

  // 符号ビット
  static
  const int kSignBit = 0;

  // サイズビット
  static
  const int kSizeBit = 1;

  // ビットベクタビット
  static
  const int kBvBit = 2;

  // 実数ビット
  static
  const int kRealBit = 3;

  // サイズシフト
  static
  const int kSizeShift = 4;

  // 符号マスク
  static
  const ymuint32 kSignMask = 1U << kSignBit;

  // サイズマスク
  static
  const ymuint32 kSizeMask = 1U << kSizeBit;

  // ビットベクタマスク
  static
  const ymuint32 kBvMask = 1U << kBvBit;

  // 実数マスク
  static
  const ymuint32 kRealMask = 1U << kRealBit;

  // 整数を表すパタン
  static
  const ymuint32 kIntData = kSignMask | kSizeMask | kBvMask |
			   (kVpiSizeInteger << kSizeShift);

  // time 型を表すパタン
  static
  const ymuint32 kTimeData = kSizeMask | kBvMask |
			    (kVpiSizeTime << kSizeShift);

  // 実数を表すパタン
  static
  const ymuint32 kRealData = kSignMask | kSizeMask | kRealMask |
                            (kVpiSizeReal << kSizeShift);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型を表す実体
  ymuint32 mData;

};

/// @brief 型の内容をストリーム出力する関数
/// @relates VlValueType
/// @param[in] s 出力先のストリーム
/// @param[in] type 型
ostream&
operator<<(ostream& s,
	   const VlValueType& type);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 型の指定なしの意味になる．
inline
VlValueType::VlValueType() :
  mData(0U)
{
}

// @brief 内容を指定するコンストラクタ
// @param[in] has_sign 符号の有無
// @param[in] has_size サイズ指定の有無
// @param[in] size サイズ(ビット幅)
inline
VlValueType::VlValueType(bool has_sign,
			 bool has_size,
			 ymuint size) :
  mData(kSignMask * has_sign |
	kSizeMask * has_size |
	kBvMask |
	(size << kSizeShift))
{
}

// @brief 値を直接指定するコンストラクタ
// @param[in] data 値
inline
VlValueType::VlValueType(ymuint32 data) :
  mData(data)
{
}

// @brief 整数型を返す．
inline
VlValueType
VlValueType::int_type()
{
  return VlValueType(kIntData);
}

// @brief 実数型を返す．
inline
VlValueType
VlValueType::real_type()
{
  return VlValueType(kRealData);
}

// @brief time 型を返す．
inline
VlValueType
VlValueType::time_type()
{
  return VlValueType(kTimeData);
}

// @brief デストラクタ
inline
VlValueType::~VlValueType()
{
}

// @brief 型の指定がない時に true を返す．
inline
bool
VlValueType::is_no_type() const
{
  return (mData & (kBvMask | kRealMask)) == 0U;
}

// @brief 整数型の時に true を返す．
inline
bool
VlValueType::is_int_type() const
{
  return mData == kIntData;
}

// @brief time 型の時に true を返す．
inline
bool
VlValueType::is_time_type() const
{
  return mData == kTimeData;
}

// @brief ビットベクタ型の時に true を返す．
// @note 整数型/time型も含む．
inline
bool
VlValueType::is_bitvector_type() const
{
  return static_cast<bool>((mData >> kBvBit) & 1U);
}

// @brief 実数型の時に true を返す．
inline
bool
VlValueType::is_real_type() const
{
  return mData == kRealData;
}

// @brief 符号付きの時に true を返す．
inline
bool
VlValueType::is_signed() const
{
  return static_cast<bool>((mData >> kSignBit) & 1U);
}

// @brief サイズ指定付きの時に true を返す．
inline
bool
VlValueType::is_sized() const
{
  return static_cast<bool>((mData >> kSizeBit) & 1U);
}

// @brief サイズを返す．
inline
ymuint
VlValueType::size() const
{
  return (mData >> kSizeShift);
}

// @brief 等価比較演算子
inline
bool
VlValueType::operator==(const VlValueType& right) const
{
  return mData == right.mData;
}

// @brief 非等価比較演算子
inline
bool
VlValueType::operator!=(const VlValueType& right) const
{
  return mData != right.mData;
}

// @brief 型の内容をストリーム出力する関数
// @relates VlValueType
// @param[in] s 出力先のストリーム
// @param[in] type 型
inline
ostream&
operator<<(ostream& s,
	   const VlValueType& type)
{
  if ( type.is_no_type() ) {
    s << "NO TYPE";
  }
  else if ( type.is_int_type() ) {
    s << "INT TYPE";
  }
  else if ( type.is_real_type() ) {
    s << "REAL TYPE";
  }
  else if ( type.is_time_type() ) {
    s << "TIME TYPE";
  }
  else if ( type.is_bitvector_type() ) {
    if ( type.is_signed() ) {
      s << "SIGNED ";
    }
    else {
      s << "UNSIGNED ";
    }
    if ( type.is_sized() ) {
      s << "SIZED ";
    }
    else {
      s << "UNSIZED ";
    }
    s << type.size() << " BITS TYPE";
  }

  return s;
}

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VLVALUETYPE_H
