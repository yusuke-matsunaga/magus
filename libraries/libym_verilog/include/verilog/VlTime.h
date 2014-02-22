#ifndef VERILOG_VLTIME_H
#define VERILOG_VLTIME_H

/// @file verilog/VlTime.h
/// @brief VlTime のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/verilog.h"
#include "verilog/VlScalarVal.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
/// @class VlTime VlTime.h "ym_verilog/VlTime.h"
/// @brief シミュレーション時刻を表すクラス
//////////////////////////////////////////////////////////////////////
class VlTime
{
public:

  /// @brief 空のコンストラクタ
  /// @note 値は不定
  VlTime();

  /// @brief 2つの値を指定するコンストラクタ
  /// @param[in] l 下位32ビットの値
  /// @param[in] h 上位32ビットの値
  VlTime(PLI_UINT32 l,
	 PLI_UINT32 h);

  /// @brief 符号なし整数からの変換コンストラクタ
  /// @param[in] val 値
  explicit
  VlTime(ymuint val);

  /// @brief double からの変換コンストラクタ
  /// @param[in] val 値
  explicit
  VlTime(double val);

  /// @brief コピーコンストラクタ
  VlTime(const VlTime& src);

  /// @brief 代入演算子
  VlTime
  operator=(VlTime src);

  /// @brief 加算つき代入演算子
  VlTime
  operator+=(VlTime src);

  /// @brief デストラクタ
  ~VlTime();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの値を指定する．
  /// @param[in] l 下位32ビットの値
  /// @param[in] h 上位32ビットの値
  void
  set(PLI_UINT32 l,
      PLI_UINT32 h);

  /// @brief 符号なし整数の値を設定する．
  void
  set(ymuint val);

  /// @brief double の値を設定する．
  void
  set(double val);


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 64ビットの値を取り出す．
  ymuint64
  value() const;

  /// @brief 下位32ビットの値を取り出す．
  PLI_UINT32
  low() const;

  /// @brief 上位32ビットの値を取り出す．
  PLI_UINT32
  high() const;

  /// @brief 符号なし整数への変換
  /// @note 符号なし整数に入りきらない値の時はおかしな値になる．
  ymuint
  to_uint() const;

  /// @brief 論理値への変換
  /// @retval kVpiScalar0 値が 0
  /// @retval kVpiScalar1 値が 0 以外
  VlScalarVal
  to_logic() const;

  /// @brief double への変換
  double
  to_real() const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  // ハッシュ関数
  ymuint32
  hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 64ビットの値
  ymuint64 mValue;

};


//////////////////////////////////////////////////////////////////////
/// @name VlTime に関連する関数
/// @{

/// @brief 加算
/// @param[in] op1, op2 オペランド
/// @return op1 + op2
VlTime
operator+(VlTime op1,
	  VlTime op2);

/// @brief 等価比較
/// @param[in] op1, op2 オペランド
/// @return op1 と op2 が等しいとき true を返す．
bool
operator==(VlTime op1,
	   VlTime op2);

/// @brief 小なり
/// @param[in] op1, op2 オペランド
/// @return op1 < op2 のとき true を返す．
bool
operator<(VlTime op1,
	  VlTime op2);

/// @brief 小なりまたは等しい
/// @param[in] op1, op2 オペランド
/// @return op1 <= op2 のとき true を返す．
bool
operator<=(VlTime op1,
	   VlTime op2);

/// @brief 大なり
/// @param[in] op1, op2 オペランド
/// @return op1 > op2 のとき true を返す．
bool
operator>(VlTime op1,
	  VlTime op2);

/// @brief 大なりまたは等しい
/// @param[in] op1, op2 オペランド
/// @return op1 >= op2 のとき true を返す．
bool
operator>=(VlTime op1,
	   VlTime op2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 値は不定
inline
VlTime::VlTime() :
  mValue(0UL)
{
}

// @brief 2つの値を指定するコンストラクタ
// @param[in] l 下位32ビットの値
// @param[in] h 上位32ビットの値
inline
VlTime::VlTime(PLI_UINT32 l,
	       PLI_UINT32 h)
{
  ymuint64 tmp = h;
  mValue = (tmp << 32) | l;
}

// @brief 符号なし整数からの変換コンストラクタ
inline
VlTime::VlTime(ymuint val) :
  mValue(val)
{
}

// @brief double からの変換コンストラクタ
inline
VlTime::VlTime(double val)
{
  set(val);
}

// @brief コピーコンストラクタ
inline
VlTime::VlTime(const VlTime& src) :
  mValue(src.mValue)
{
}

// @brief 代入演算子
inline
VlTime
VlTime::operator=(VlTime src)
{
  mValue = src.mValue;
  return *this;
}

// @brief デストラクタ
inline
VlTime::~VlTime()
{
}

// @brief 2つの値を指定する．
// @param[in] l 下位32ビットの値
// @param[in] h 上位32ビットの値
inline
void
VlTime::set(PLI_UINT32 l,
	    PLI_UINT32 h)
{
  ymuint64 tmp = h;
  mValue = (tmp << 32) | l;
}

// @brief 符号なし整数の値を設定する．
inline
void
VlTime::set(ymuint val)
{
  mValue = val;
}

// @brief double の値を設定する．
inline
void
VlTime::set(double val)
{
  if ( val < 0.0 ) {
    // 負数は0にする．
    mValue = 0UL;
    return;
  }
  double val1 = rint(val);
  mValue = static_cast<ymuint64>(val1);
}

// @brief 64ビットの値を取り出す．
inline
ymuint64
VlTime::value() const
{
  return mValue;
}

// @brief 下位32ビットの値を取り出す．
inline
PLI_UINT32
VlTime::low() const
{
  return mValue & 0xFFFFFFFFUL;
}

// @brief 上位32ビットの値を取り出す．
inline
PLI_UINT32
VlTime::high() const
{
  return (mValue >> 32);
}

// @brief 符号なし整数への変換
// @note 符号なし整数に入りきらない値の時はおかしな値になる．
inline
ymuint
VlTime::to_uint() const
{
  return static_cast<ymuint32>(mValue);
}

// @brief 論理値への変換
// @retval kVpiScalar0 値が 0
// @retval kVpiScalar1 値が 0 以外
inline
VlScalarVal
VlTime::to_logic() const
{
  if ( mValue != 0UL ) {
    return VlScalarVal::one();
  }
  else {
    return VlScalarVal::zero();
  }
}

// @brief double への変換
inline
double
VlTime::to_real() const
{
  return static_cast<double>(mValue);
}

// ハッシュ関数
inline
ymuint32
VlTime::hash() const
{
  return (mValue * mValue) >> 24;
}

// 加算付き代入
inline
VlTime
VlTime::operator+=(VlTime op2)
{
  mValue += op2.mValue;
  return *this;
}

// 加算
inline
VlTime
operator+(VlTime op1,
	  VlTime op2)
{
  return VlTime(op1).operator+=(op2);
}

// 等価比較
inline
bool
operator==(VlTime op1,
	   VlTime op2)
{
  if ( op1.value() == op2.value() ) {
    return true;
  }
  else {
    return false;
  }
}

// 小なり
inline
bool
operator<(VlTime op1,
	  VlTime op2)
{
  if ( op1.value() < op2.value() ) {
    return true;
  }
  else {
    return false;
  }
}

// 小なりまたは等しい
inline
bool
operator<=(VlTime op1,
	   VlTime op2)
{
  return !(op2 < op1);
}

// 大なり
inline
bool
operator>(VlTime op1,
	  VlTime op2)
{
  return op2 < op1;
}

// 大なりまたは等しい
inline
bool
operator>=(VlTime op1,
	   VlTime op2)
{
  return !(op1 < op2);
}

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_HASH

// VlTimeをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsVerilog::VlTime>
{
  ymuint32
  operator()(nsYm::nsVerilog::VlTime time) const
  {
    return time.hash();
  }
};

END_NAMESPACE_HASH

#endif // VERILOG_VLTIME_H
