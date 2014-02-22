
/// @file VlValue.cc
/// @brief VlValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/VlValue.h"
#include "VlValueRep.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

VlValueRep* error_rep = NULL;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス VlValue
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @param[in] 値は不定
VlValue::VlValue()
{
  if ( error_rep == NULL ) {
    error_rep = new VlValueError();
    error_rep->inc();
  }
  mRep = error_rep;
}

// @brief コピーコンストラクタ
VlValue::VlValue(const VlValue& src) :
  mRep(src.mRep)
{
}

// @brief 整数値からのコンストラクタ
VlValue::VlValue(int val) :
  mRep(new VlValueInt(val))
{
}

// @brief ymuint からのコンストラクタ
VlValue::VlValue(ymuint val) :
  mRep(new VlValueUint(val))
{
}

// @brief スカラー値からのコンストラクタ
VlValue::VlValue(const VlScalarVal& val) :
  mRep(new VlValueScalar(val))
{
}

// @brief time からのコンストラクタ
VlValue::VlValue(VlTime val) :
  mRep(new VlValueTime(val))
{
}

// @brief 実数からのコンストラクタ
VlValue::VlValue(double val) :
  mRep(new VlValueReal(val))
{
}

// @brief ビットベクタからのコンストラクタ
VlValue::VlValue(const BitVector& val) :
  mRep(new VlValueBitVector(val))
{
}

// @brief 型変換を伴うコンストラクタ
VlValue::VlValue(const VlValue& src,
		 const VlValueType& value_type)
{
  if ( value_type.is_int_type() ) {
    mRep = new VlValueInt(src.int_value());
  }
  else if ( value_type.is_real_type() ) {
    mRep = new VlValueReal(src.real_value());
  }
  else if ( value_type.is_time_type() ) {
    mRep = new VlValueTime(src.time_value());
  }
  else if ( value_type.is_no_type() ) {
    mRep = src.mRep;
  }
  else if ( value_type.is_bitvector_type() ) {
    const BitVector& src_bv = src.bitvector_value();
    mRep = new VlValueBitVector(BitVector(src_bv,
					  value_type.size(),
					  value_type.is_sized(),
					  value_type.is_signed(),
					  src_bv.base()));
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief デストラクタ
VlValue::~VlValue()
{
  // SmartPtr<VlValueRep> が仕事をしてくれている．
}

// @brief 整数値を設定する．
void
VlValue::set(int val)
{
  mRep = new VlValueInt(val);
}

// @brief ymuint の値をセットする．
void
VlValue::set(ymuint val)
{
  mRep = new VlValueUint(val);
}

// @brief スカラー値をセットする．
void
VlValue::set(const VlScalarVal& val)
{
  mRep = new VlValueScalar(val);
}

// @brief time の値をセットする．
void
VlValue::set(VlTime val)
{
  mRep = new VlValueTime(val);
}

// @brief 実数値をセットする．
void
VlValue::set(double val)
{
  mRep = new VlValueReal(val);
}

// @brief ビットベクタの値をセットする．
void
VlValue::set(const BitVector& val)
{
  mRep = new VlValueBitVector(val);
}

// @relates VlValue
// @brief 単項のマイナス(complement)演算子
// @return 2の補数を返す．
//
// 2の補数を求める．
// パタンとして2の補数を求めるだけで
// 符号の有無は変わらない．
VlValue
operator-(const VlValue& src)
{
  switch ( src.type() ) {
  case VlValue::kIntType:
    return VlValue( - src.int_value() );

  case VlValue::kUintType:
  case VlValue::kScalarType:
    return VlValue( static_cast<ymuint>(- src.int_value()) );

  case VlValue::kRealType:
    return VlValue( - src.real_value() );

  case VlValue::kTimeType:
    // これは意味があるとは思えないのでビットベクタに変換する．
  case VlValue::kBitVectorType:
    return VlValue( - src.bitvector_value() );

  case VlValue::kErrorType:
    return src;
  }
  assert_not_reached(__FILE__, __LINE__);
  return VlValue();
}

// @relates VlValue
// @brief 加算
// @param[in] src1, src2 オペランド
// @return src1 + src2 を返す
VlValue
operator+(const VlValue& src1,
	  const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_uint_type() ) {
    if ( src2.is_uint_type() ) {
      return VlValue( src1.uint_value() + src2.uint_value() );
    }
    if ( src2.is_int_type() ) {
      return VlValue( src1.int_value() + src2.int_value() );
    }
  }
  if ( src1.is_int_type() ) {
    if ( src2.is_int_type() || src2.is_int_type() ) {
      return VlValue( src1.int_value() + src2.int_value() );
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    return VlValue( src1.real_value() + src2.real_value() );
  }
  if ( src1.is_time_type() && src2.is_time_type() ) {
    return VlValue( src1.time_value() + src2.time_value() );
  }
  return VlValue( src1.bitvector_value() + src2.bitvector_value() );
}

// @relates VlValue
// @brief 減算
// @param[in] src1, src2 オペランド
// @return src1 - src2 を返す
VlValue
operator-(const VlValue& src1,
	  const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_uint_type() ) {
    if ( src2.is_uint_type() ) {
      return VlValue( src1.uint_value() - src2.uint_value() );
    }
    if ( src2.is_int_type() ) {
      return VlValue( src1.int_value() - src2.int_value() );
    }
  }
  if ( src1.is_int_type() ) {
    if ( src2.is_int_type() || src2.is_int_type() ) {
      return VlValue( src1.int_value() - src2.int_value() );
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    return VlValue( src1.real_value() - src2.real_value() );
  }
  return VlValue( src1.bitvector_value() - src2.bitvector_value() );
}

// @relates VlValue
// @brief 乗算
// @param[in] src1, src2 オペランド
// @return src1 * src2 を返す
VlValue
operator*(const VlValue& src1,
	  const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_uint_type() ) {
    if ( src2.is_uint_type() ) {
      return VlValue( src1.uint_value() * src2.uint_value() );
    }
    if ( src2.is_int_type() ) {
      return VlValue( src1.int_value() * src2.int_value() );
    }
  }
  if ( src1.is_int_type() ) {
    if ( src2.is_int_type() || src2.is_int_type() ) {
      return VlValue( src1.int_value() * src2.int_value() );
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    return VlValue( src1.real_value() * src2.real_value() );
  }
  return VlValue( src1.bitvector_value() * src2.bitvector_value() );
}

// @relates VlValue
// @brief 除算
// @param[in] src1, src2 オペランド
// @return src1 / src2 を返す
VlValue
operator/(const VlValue& src1,
	  const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_uint_type() ) {
    if ( src2.is_uint_type() ) {
      return VlValue( src1.uint_value() / src2.uint_value() );
    }
    if ( src2.is_int_type() ) {
      return VlValue( src1.int_value() / src2.int_value() );
    }
  }
  if ( src1.is_int_type() ) {
    if ( src2.is_int_type() || src2.is_int_type() ) {
      return VlValue( src1.int_value() / src2.int_value() );
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    return VlValue( src1.real_value() / src2.real_value() );
  }
  return VlValue( src1.bitvector_value() / src2.bitvector_value() );
}

// @relates VlValue
// @brief 剰余算
// @param[in] src1, src2 オペランド
// @return src1 % src2 を返す
VlValue
operator%(const VlValue& src1,
	  const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_uint_type() ) {
    if ( src2.is_uint_type() ) {
      return VlValue( src1.uint_value() % src2.uint_value() );
    }
    if ( src2.is_int_type() ) {
      return VlValue( src1.int_value() % src2.int_value() );
    }
  }
  if ( src1.is_int_type() ) {
    if ( src2.is_int_type() || src2.is_int_type() ) {
      return VlValue( src1.int_value() % src2.int_value() );
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() % src2.bitvector_value() );
}

// @relates VlValue
// @brief 巾乗
// @param[in] src1, src2 オペランド
// @return src1 の src2 乗を返す
VlValue
power(const VlValue& src1,
      const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_real_type() || src1.is_int_type() ||
       src2.is_real_type() || src2.is_int_type() ) {
    double v1 = src1.real_value();
    double v2 = src2.real_value();
    if ( (v1 == 0.0 && v2 <= 0.0) || (v1 < 0.0 && rint(v2) != v2) ) {
      return VlValue(VlScalarVal::x());
    }
    return VlValue( pow(v1, v2) );
  }
  return VlValue( power(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief less than 比較演算
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::one() src1 < src2 の時
// @retval VlScalarVal::zero() src1 >= src2 の時
// @retval VlScalarVal::x() 比較不能の時
VlValue
lt(const VlValue& src1,
   const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( src1.is_int_type() ) {
    if ( src2.is_int_type() || src2.is_uint_type() ) {
      if ( src1.int_value() < src2.int_value() ) {
	return VlValue(VlScalarVal::one());
      }
      else {
	return VlValue(VlScalarVal::zero());
      }
    }
  }
  else if ( src1.is_uint_type() ) {
    if ( src2.is_int_type() ) {
      if ( src1.int_value() < src2.int_value() ) {
	return VlValue(VlScalarVal::one());
      }
      else {
	return VlValue(VlScalarVal::zero());
      }
    }
    else if ( src2.is_uint_type() ) {
      if ( src1.uint_value() < src2.uint_value() ) {
	return VlValue(VlScalarVal::one());
      }
      else {
	return VlValue(VlScalarVal::zero());
      }
    }
  }
  else if ( src1.is_real_type() || src2.is_real_type() ) {
    if ( src1.real_value() < src2.real_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  return VlValue( lt(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief greater than 比較演算
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::one() src1 > src2 の時
// @retval VlScalarVal::zero() src1 <= src2 の時
// @retval VlScalarVal::x() 比較不能の時
VlValue
gt(const VlValue& src1,
   const VlValue& src2)
{
  return lt(src2, src1);
}

// @relates VlValue
// @brief less than or equal 比較演算
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::one() src1 <= src2 の時
// @retval VlScalarVal::zero() src1 > src2 の時
// @retval VlScalarVal::x() 比較不能の時
VlValue
le(const VlValue& src1,
   const VlValue& src2)
{
  return log_not(lt(src2, src1));
}

// @relates VlValue
// @brief greater than or equal 比較演算
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::one() src1 >= src2 の時
// @retval VlScalarVal::zero() src1 < src2 の時
// @retval kVpiscalarX 比較不能の時
VlValue
ge(const VlValue& src1,
   const VlValue& src2)
{
  return log_not(lt(src1, src2));
}

// @relates VlValue
// @brief 等価比較演算子
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::one() src1 == src2 の時
// @retval VlScalarVal::zero() src1 != src2 の時
// @retval VlScalarVal::x() 比較不能の時
VlValue
eq(const VlValue& src1,
   const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( (src1.is_int_type() || src1.is_uint_type()) &&
       (src2.is_int_type() || src2.is_uint_type()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  return VlValue( eq(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
// @param[in] src1, src2 オペランド
// @return 等価と見なせるとき true を返す．
VlValue
eq_with_x(const VlValue& src1,
	  const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( (src1.is_int_type() || src1.is_uint_type()) &&
       (src2.is_int_type() || src2.is_uint_type()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  return VlValue( eq_with_x(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
// @param[in] src1, src2 オペランド
// @return 等価と見なせるとき true を返す．
VlValue
eq_with_xz(const VlValue& src1,
	   const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( (src1.is_int_type() || src1.is_uint_type()) &&
       (src2.is_int_type() || src2.is_uint_type()) ) {
    if ( src1.int_value() == src2.int_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  if ( src1.is_real_type() || src2.is_real_type() ) {
    if ( src1.real_value() == src2.real_value() ) {
      return VlValue(VlScalarVal::one());
    }
    else {
      return VlValue(VlScalarVal::zero());
    }
  }
  return VlValue( eq_with_xz(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief 非等価比較演算子
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::one() src1 != src2 の時
// @retval VlScalarVal::zero() src1 == src2 の時
// @retval VlScalarVal::x() 比較不能の時
VlValue
ne(const VlValue& src1,
   const VlValue& src2)
{
  return log_not(eq(src1, src2));
}

// @relates VlValue
// @brief NOT演算
// @param[in] src オペランド
// @retval VlScalarVal::zero() src が真の時
// @retval VlScalarVal::one() src が偽の時
// @retval VlScalarVal::x() 計算不能の時
VlValue
log_not(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  return VlValue( !src.logic_value() );
}

// @relates VlValue
// @brief AND演算
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::zero() src1 と src2 のどちらか一方が偽の時
// @retval VlScalarVal::one() src1 と src2 がともに真の時
// @retval VlScalarVal::x() 計算不能の時
VlValue
log_and(const VlValue& src1,
	const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  return VlValue( src1.logic_value() && src2.logic_value() );
}

// @relates VlValue
// @brief OR演算
// @param[in] src1, src2 オペランド
// @retval VlScalarVal::zero() src1 と src2 がともに偽の時
// @retval VlScalarVal::one() src1 と src2 のどちらか一方が真の時
// @retval VlScalarVal::x() 計算不能の時
VlValue
log_or(const VlValue& src1,
       const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  return VlValue( src1.logic_value() || src2.logic_value() );
}

// @relates VlValue
// @brief bitwise NOT 演算
// @param[in] src オペランド
// @return src のビットごとに否定したもの
VlValue
bit_negate(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( ~src.bitvector_value() );
}

// @relates VlValue
// @brief bitwise AND 演算
// @param[in] src1, src2 オペランド
// @return src1 と src2 をビットごとに AND したもの
VlValue
bit_and(const VlValue& src1,
	const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() & src2.bitvector_value() );
}

// @relates VlValue
// @brief bitwise OR 演算
// @param[in] src1, src2 オペランド
// @return src1 と src2 をビットごとに OR したもの
VlValue
bit_or(const VlValue& src1,
       const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() | src2.bitvector_value() );
}

// @relates VlValue
// @brief bitwise XOR 演算
// @param[in] src1, src2 オペランド
// @return src1 と src2 をビットごとに XOR したもの
VlValue
bit_xor(const VlValue& src1,
	const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() ^ src2.bitvector_value() );
}

// @relates VlValue
// @brief bitwise XNOR 演算
// @param[in] src1, src2 オペランド
// @return src1 と src2 をビットごとに XNOR したもの
VlValue
bit_xnor(const VlValue& src1,
	 const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( ~(src1.bitvector_value() ^ src2.bitvector_value()) );
}

// @brief リダクションAND
// @return 演算結果を返す．
// @note すべてのビットの AND を計算する
VlValue
reduction_and(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src.bitvector_value().reduction_and() );
}

// @brief リダクションOR
// @return 演算結果を返す．
// @note すべてのビットの OR を計算する
VlValue
reduction_or(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src.bitvector_value().reduction_or() );
}

// @brief リダクションXOR
// @return 演算結果を返す．
// @note すべてのビットの XOR を計算する
VlValue
reduction_xor(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src.bitvector_value().reduction_xor() );
}

// @brief リダクションNAND
// @return 演算結果を返す．
// @note すべてのビットの NAND を計算する
VlValue
reduction_nand(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src.bitvector_value().reduction_nand() );
}

// @brief リダクションNOR
// @return 演算結果を返す．
// @note すべてのビットの NOR を計算する
VlValue
reduction_nor(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src.bitvector_value().reduction_nor() );
}

// @brief リダクションXNOR
// @return 演算結果を返す．
// @note すべてのビットの XNOR を計算する
VlValue
reduction_xnor(const VlValue& src)
{
  if ( src.is_error() ) {
    return VlValue();
  }
  if ( !src.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src.bitvector_value().reduction_xnor() );
}

// @relates VlValue
// @brief 論理左シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (VlValue)
// @return src1 を src2 だけ論理左シフトしたもの
VlValue
operator<<(const VlValue& src1,
	   const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() << src2.bitvector_value() );
}

// @relates VlValue
// @brief 論理左シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (ymuint32)
// @return src1 を src2 だけ論理左シフトしたもの
VlValue
operator<<(const VlValue& src1,
	   ymuint32 src2)
{
  if ( src1.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() << src2 );
}

// @relates VlValue
// @brief 論理右シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (VlValue)
// @return src1 を src2 だけ論理右シフトしたもの
VlValue
operator>>(const VlValue& src1,
	   const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() >> src2.bitvector_value() );
}

// @relates VlValue
// @brief 論理右シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (ymuint32)
// @return src1 を src2 だけ論理右シフトしたもの
VlValue
operator>>(const VlValue& src1,
	   ymuint32 src2)
{
  if ( src1.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( src1.bitvector_value() >> src2 );
}

// @relates VlValue
// @brief 算術左シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (VlValue)
// @return src1 を src2 だけ算術左シフトしたもの
// 算術左シフト
VlValue
alshift(const VlValue& src1,
	const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( alshift(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief 算術左シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (ymuint32)
// @return src1 を src2 だけ算術左シフトしたもの
VlValue
alshift(const VlValue& src1,
	ymuint32 src2)
{
  if ( src1.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( alshift(src1.bitvector_value(), src2) );
}

// @relates VlValue
// @brief 算術右シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (VlValue)
// @return src1 を src2 だけ算術右シフトしたもの
VlValue
arshift(const VlValue& src1,
	const VlValue& src2)
{
  if ( src1.is_error() || src2.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() || !src2.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( arshift(src1.bitvector_value(), src2.bitvector_value()) );
}

// @relates VlValue
// @brief 算術右シフト
// @param[in] src1 元の値
// @param[in] src2 シフト量 (ymuint32)
// @return src1 を src2 だけ算術右シフトしたもの
VlValue
arshift(const VlValue& src1,
	ymuint32 src2)
{
  if ( src1.is_error() ) {
    return VlValue();
  }
  if ( !src1.is_bitvector_conv() ) {
    return VlValue();
  }
  return VlValue( arshift(src1.bitvector_value(), src2) );
}

// @relates VlValue
// @brief 条件演算
// @param[in] src1 条件
// @param[in] src2 src1 が真の時に選ばれる値
// @param[in] src3 src1 が偽の時に選ばれる値
// @return 演算結果
VlValue
ite(const VlValue& src1,
    const VlValue& src2,
    const VlValue& src3)
{
  if ( src1.logic_value().to_bool() ) {
    return src2;
  }
  else {
    return src3;
  }
}

// @relates VlValue
// @brief 条件演算
// @param[in] src1 条件
// @param[in] src2 src1 が真の時に選ばれる値
// @param[in] src3 src1 が偽の時に選ばれる値
// @return 演算結果
VlValue
ite(const VlScalarVal& src1,
    const VlValue& src2,
    const VlValue& src3)
{
  if ( src1.to_bool() ) {
    return src2;
  }
  else {
    return src3;
  }
}

// @relates VlValue
// @brief 連結演算
// @param[in] src_list 連結する値のリスト
// @return 連結した結果を返す．
VlValue
concat(const list<VlValue>& src_list)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  for (list<VlValue>::const_iterator p = src_list.begin();
       p != src_list.end(); ++ p) {
    const VlValue& v = *p;
    if ( !v.is_bitvector_conv() ) {
      return VlValue();
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue( concat(bv_array) );
}

// @relates VlValue
// @brief 連結演算
// @param[in] src_list 連結する値のリスト
// @return 連結した結果を返す．
VlValue
concat(const vector<VlValue>& src_list)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  for (vector<VlValue>::const_iterator p = src_list.begin();
       p != src_list.end(); ++ p) {
    const VlValue& v = *p;
    if ( !v.is_bitvector_conv() ) {
      return VlValue();
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue( concat(bv_array) );
}

// @relates VlValue
// @brief 繰り返し連結演算
// @param[in] src_list 連結する値のリスト
// @return src_list[1~n-1] の内容を src_list[0] 回繰り返して連結したもの
VlValue
multi_concat(const list<VlValue>& src_list)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  list<VlValue>::const_iterator p = src_list.begin();
  const VlValue& v0 = *p;
  if ( !v0.is_bitvector_conv() ) {
    return VlValue();
  }
  for (++ p; p != src_list.end(); ++ p) {
    const VlValue& v = *p;
    if ( v.is_bitvector_conv() ) {
      return VlValue();
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue( multi_concat(v0.bitvector_value(), bv_array) );
}

// @relates VlValue
// @brief 繰り返し連結演算
// @param[in] rep 繰り返し数
// @param[in] src_list 連結する値のリスト
// @return src_list[1~n-1] の内容を src_list[0] 回繰り返して連結したもの
VlValue
multi_concat(const vector<VlValue>& src_list)
{
  vector<BitVector> bv_array;
  bv_array.reserve(src_list.size());
  vector<VlValue>::const_iterator p = src_list.begin();
  const VlValue& v0 = *p;
  if ( !v0.is_bitvector_conv() ) {
    return VlValue();
  }
  for (++ p; p != src_list.end(); ++ p) {
    const VlValue& v = *p;
    if ( v.is_bitvector_conv() ) {
      return VlValue();
    }
    bv_array.push_back(v.bitvector_value());
  }
  return VlValue( multi_concat(v0.bitvector_value(), bv_array) );
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueError
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueError::VlValueError()
{
}

// @brief デストラクタ
VlValueError::~VlValueError()
{
}

// @brief 型を返す．
VlValue::tType
VlValueError::type() const
{
  return VlValue::kErrorType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueError::is_int_conv() const
{
  return false;
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueError::is_uint_conv() const
{
  return false;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueError::is_real_conv() const
{
  return false;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueError::is_time_conv() const
{
  return false;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueError::is_bitvector_conv() const
{
  return false;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueError::int_value() const
{
  return 0;
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueError::uint_value() const
{
  return 0;
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueError::scalar_value() const
{
  return VlScalarVal::x();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueError::logic_value() const
{
  return VlScalarVal::x();
}

// @brief 実数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
double
VlValueError::real_value() const
{
  return 0.0;
}

// @brief time 型の値を返す．
VlTime
VlValueError::time_value() const
{
  return VlTime();
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueError::bitvector_value(const VlValueType& req_type) const
{
  return BitVector();
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueInt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueInt::VlValueInt(int val) :
  mVal(val)
{
}

// @brief デストラクタ
VlValueInt::~VlValueInt()
{
}

// @brief 型を返す．
VlValue::tType
VlValueInt::type() const
{
  return VlValue::kIntType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueInt::is_int_conv() const
{
  return true;
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueInt::is_uint_conv() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueInt::is_real_conv() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueInt::is_time_conv() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueInt::is_bitvector_conv() const
{
  return true;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueInt::int_value() const
{
  return mVal;
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueInt::uint_value() const
{
  return static_cast<ymuint>(mVal);
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueInt::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueInt::logic_value() const
{
  if ( mVal != 0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
double
VlValueInt::real_value() const
{
  return static_cast<double>(mVal);
}

// @brief time 型の値を返す．
VlTime
VlValueInt::time_value() const
{
  return VlTime(uint_value());
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueInt::bitvector_value(const VlValueType& req_type) const
{
  return BitVector(mVal).coerce(req_type);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueUint
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueUint::VlValueUint(ymuint val) :
  mVal(val)
{
}

// @brief デストラクタ
VlValueUint::~VlValueUint()
{
}

// @brief 型を返す．
VlValue::tType
VlValueUint::type() const
{
  return VlValue::kUintType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueUint::is_int_conv() const
{
  return true;
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueUint::is_uint_conv() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueUint::is_real_conv() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueUint::is_time_conv() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueUint::is_bitvector_conv() const
{
  return true;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueUint::int_value() const
{
  return static_cast<int>(mVal);
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueUint::uint_value() const
{
  return mVal;
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueUint::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueUint::logic_value() const
{
  if ( mVal != 0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
double
VlValueUint::real_value() const
{
  return static_cast<double>(mVal);
}

// @brief time 型の値を返す．
VlTime
VlValueUint::time_value() const
{
  return VlTime(mVal);
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueUint::bitvector_value(const VlValueType& req_type) const
{
  return BitVector(mVal).coerce(req_type);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueScalar
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueScalar::VlValueScalar(const VlScalarVal& val) :
  mVal(val)
{
}

// @brief デストラクタ
VlValueScalar::~VlValueScalar()
{
}

// @brief 型を返す．
VlValue::tType
VlValueScalar::type() const
{
  return VlValue::kScalarType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueScalar::is_int_conv() const
{
  return !mVal.is_xz();
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueScalar::is_uint_conv() const
{
  return is_int_conv();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueScalar::is_real_conv() const
{
  return is_int_conv();
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueScalar::is_time_conv() const
{
  return is_int_conv();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueScalar::is_bitvector_conv() const
{
  return true;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueScalar::int_value() const
{
  return mVal.to_int();
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueScalar::uint_value() const
{
  return static_cast<ymuint>(int_value());
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueScalar::scalar_value() const
{
  return mVal;
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueScalar::logic_value() const
{
  if ( mVal.is_z() ) {
    return VlScalarVal::x();
  }
  return mVal;
}

// @brief 実数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
double
VlValueScalar::real_value() const
{
  return static_cast<double>(int_value());
}

// @brief time 型の値を返す．
VlTime
VlValueScalar::time_value() const
{
  return VlTime(uint_value());
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueScalar::bitvector_value(const VlValueType& req_type) const
{
  return BitVector(mVal).coerce(req_type);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueReal
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueReal::VlValueReal(double val) :
  mVal(val)
{
}

// @brief デストラクタ
VlValueReal::~VlValueReal()
{
}

// @brief 型を返す．
VlValue::tType
VlValueReal::type() const
{
  return VlValue::kRealType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueReal::is_int_conv() const
{
  return true;
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueReal::is_uint_conv() const
{
  return true;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueReal::is_real_conv() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueReal::is_time_conv() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueReal::is_bitvector_conv() const
{
  return false;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueReal::int_value() const
{
  return static_cast<int>(mVal);
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueReal::uint_value() const
{
  return static_cast<ymuint>(mVal);
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueReal::scalar_value() const
{
  return VlScalarVal(mVal);
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueReal::logic_value() const
{
  if ( mVal != 0.0 ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
double
VlValueReal::real_value() const
{
  return mVal;
}

// @brief time 型の値を返す．
VlTime
VlValueReal::time_value() const
{
  return VlTime(mVal);
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueReal::bitvector_value(const VlValueType& req_type) const
{
  return BitVector();
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueTime
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueTime::VlValueTime(VlTime val) :
  mVal(val)
{
}

// @brief デストラクタ
VlValueTime::~VlValueTime()
{
}

// @brief 型を返す．
VlValue::tType
VlValueTime::type() const
{
  return VlValue::kTimeType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueTime::is_int_conv() const
{
  return mVal.value() <= 0x7FFFFFFFUL;
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueTime::is_uint_conv() const
{
  return mVal.value() <= 0xFFFFFFFFUL;
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueTime::is_real_conv() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueTime::is_time_conv() const
{
  return true;
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueTime::is_bitvector_conv() const
{
  return true;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueTime::int_value() const
{
  return static_cast<int>(mVal.to_uint());
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueTime::uint_value() const
{
  return mVal.to_uint();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueTime::scalar_value() const
{
  return VlScalarVal(mVal.low());
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueTime::logic_value() const
{
  if ( mVal.value() != 0UL ) {
    return VlScalarVal::one();
  }
  return VlScalarVal::zero();
}

// @brief 実数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
double
VlValueTime::real_value() const
{
  return mVal.to_real();
}

// @brief time 型の値を返す．
VlTime
VlValueTime::time_value() const
{
  return mVal;
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueTime::bitvector_value(const VlValueType& req_type) const
{
  return BitVector(mVal).coerce(req_type);
}


//////////////////////////////////////////////////////////////////////
// クラス VlValueBitVector
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlValueBitVector::VlValueBitVector(const BitVector& val) :
  mVal(val)
{
}

// @brief デストラクタ
VlValueBitVector::~VlValueBitVector()
{
}

// @brief 型を返す．
VlValue::tType
VlValueBitVector::type() const
{
  return VlValue::kBitVectorType;
}

// @brief 整数型に変換可能な時に true を返す．
bool
VlValueBitVector::is_int_conv() const
{
  return mVal.is_int();
}

// @brief ymuint 型に変換可能な時に true を返す．
bool
VlValueBitVector::is_uint_conv() const
{
  return mVal.is_uint32();
}

// @brief 実数型に変換可能な時に true を返す．
bool
VlValueBitVector::is_real_conv() const
{
  return true;
}

// @brief time 型に変換可能な時に true を返す．
bool
VlValueBitVector::is_time_conv() const
{
  return mVal.is_time();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
bool
VlValueBitVector::is_bitvector_conv() const
{
  return true;
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
int
VlValueBitVector::int_value() const
{
  return mVal.to_int();
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
ymuint
VlValueBitVector::uint_value() const
{
  return mVal.to_uint32();
}

// @brief スカラー型の値を返す．
VlScalarVal
VlValueBitVector::scalar_value() const
{
  return mVal.to_scalar();
}

// @brief 論理型の値を返す．
VlScalarVal
VlValueBitVector::logic_value() const
{
  return mVal.to_logic();
}

// @brief 実数型の値を返す．
// @note 値が実数型に変換できない時の値は不定
double
VlValueBitVector::real_value() const
{
  return mVal.to_real();
}

// @brief time 型の値を返す．
VlTime
VlValueBitVector::time_value() const
{
  return mVal.to_time();
}

// @brief ビットベクタ型の値を返す．
// @param[in] req_type 要求されるデータの型
BitVector
VlValueBitVector::bitvector_value(const VlValueType& req_type) const
{
  return BitVector(mVal).coerce(req_type);
}

END_NAMESPACE_YM_VERILOG
