#ifndef YM_VERILOG_VLVALUE_H
#define YM_VERILOG_VLVALUE_H

/// @file ym_verilog/VlValue.h
/// @brief VlValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"
#include "ym_utils/RefCount.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlValueRep;

//////////////////////////////////////////////////////////////////////
/// @class VlValue VlValue.h "ym_verilog/VlValue.h"
/// @brief 値を表すクラス
/// ElbValue が実際の処理を行う．
//////////////////////////////////////////////////////////////////////
class VlValue
{
public:
  //////////////////////////////////////////////////////////////////////
  // 保持している値の種類
  //////////////////////////////////////////////////////////////////////

  enum tType {
    kIntType,
    kUintType,
    kScalarType,
    kRealType,
    kTimeType,
    kBitVectorType,
    kErrorType
  };

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  /// @param[in] 値は不定
  VlValue();

  /// @brief コピーコンストラクタ
  VlValue(const VlValue& src);

  /// @brief 整数値からのコンストラクタ
  explicit
  VlValue(int val);

  /// @brief ymuint からのコンストラクタ
  explicit
  VlValue(ymuint val);

  /// @brief スカラー値からのコンストラクタ
  explicit
  VlValue(const VlScalarVal& val);

  /// @brief time からのコンストラクタ
  explicit
  VlValue(VlTime val);

  /// @brief 実数からのコンストラクタ
  explicit
  VlValue(double val);

  /// @brief ビットベクタからのコンストラクタ
  explicit
  VlValue(const BitVector& val);

  /// @brief 型変換を伴うコンストラクタ
  VlValue(const VlValue& src,
	  const VlValueType& value_type);

  /// @brief デストラクタ
  ~VlValue();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数値を設定する．
  void
  set(int val);

  /// @brief ymuint の値をセットする．
  void
  set(ymuint val);

  /// @brief スカラー値をセットする．
  void
  set(const VlScalarVal& val);

  /// @brief time の値をセットする．
  void
  set(VlTime val);

  /// @brief 実数値をセットする．
  void
  set(double val);

  /// @brief ビットベクタの値をセットする．
  void
  set(const BitVector& val);


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  tType
  type() const;

  /// @brief 整数型の時に true を返す．
  bool
  is_int_type() const;

  /// @brief 符号なし整数型の時に true 返す．
  bool
  is_uint_type() const;

  /// @brief 実数型の時に true を返す．
  bool
  is_real_type() const;

  /// @brief スカラー型の時に true を返す．
  bool
  is_scalar_type() const;

  /// @brief time 型の時に true を返す．
  bool
  is_time_type() const;

  /// @brief ビットベクタ型の時に true を返す．
  bool
  is_bitvector_type() const;

  /// @brief エラー状態の時に true を返す．
  bool
  is_error() const;

  /// @brief 整数型に変換可能な時に true を返す．
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  bool
  is_bitvector_conv() const;

  /// @brief 符号付きの型の時に true を返す．
  bool
  is_signed() const;

  /// @brief ビットサイズを返す．
  ymuint
  bit_size() const;

  /// @brief 値の型を返す．
  VlValueType
  value_type() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  /// @note スカラー型には常に変換可能
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が実数型に変換できない時の値は不定
  double
  real_value() const;

  /// @brief time 型の値を返す．
  /// @note 値が time 型に変換できない時の値は不定
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  BitVector
  bitvector_value(const VlValueType& req_type = VlValueType()) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値を持つ実体
  SmartPtr<VlValueRep> mRep;

};


//////////////////////////////////////////////////////////////////////
// VlValue の演算
//////////////////////////////////////////////////////////////////////

/// @relates VlValue
/// @brief 単項のマイナス(complement)演算子
/// @return 2の補数を返す．
///
/// 2の補数を求める．
/// パタンとして2の補数を求めるだけで
/// 符号の有無は変わらない．
VlValue
operator-(const VlValue& src);

/// @relates VlValue
/// @brief 加算
/// @param[in] src1, src2 オペランド
/// @return src1 + src2 を返す
VlValue
operator+(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief 減算
/// @param[in] src1, src2 オペランド
/// @return src1 - src2 を返す
VlValue
operator-(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief 乗算
/// @param[in] src1, src2 オペランド
/// @return src1 * src2 を返す
VlValue
operator*(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief 除算
/// @param[in] src1, src2 オペランド
/// @return src1 / src2 を返す
VlValue
operator/(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief 剰余算
/// @param[in] src1, src2 オペランド
/// @return src1 % src2 を返す
VlValue
operator%(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief 巾乗
/// @param[in] src1, src2 オペランド
/// @return src1 の src2 乗を返す
VlValue
power(const VlValue& src1,
      const VlValue& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Relational operators (IEEE1364-2001 4.1.7)
/// 結果が x になる場合があるので bool ではなく VlValue を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// ともに符号付きの場合には符号付き数として比較する．
///
/// そうでなければ符号なし数として比較する．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief less than 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 < src2 の時
/// @retval kVpiScalar0 src1 >= src2 の時
/// @retval kVpiScalarX 比較不能の時
VlValue
lt(const VlValue& src1,
   const VlValue& src2);

/// @relates VlValue
/// @brief less than 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 < src2 を返す
bool
operator<(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief greater than 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 > src2 の時
/// @retval kVpiScalar0 src1 <= src2 の時
/// @retval kVpiScalarX 比較不能の時
VlValue
gt(const VlValue& src1,
   const VlValue& src2);

/// @relates VlValue
/// @brief greater than 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 > src2 を返す
bool
operator>(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief less than or equal 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 <= src2 の時
/// @retval kVpiScalar0 src1 > src2 の時
/// @retval kVpiScalarX 比較不能の時
VlValue
le(const VlValue& src1,
   const VlValue& src2);

/// @relates VlValue
/// @brief less than or equal 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 <= src2 を返す
bool
operator<=(const VlValue& src1,
	   const VlValue& src2);

/// @relates VlValue
/// @brief greater than or equal 比較演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 >= src2 の時
/// @retval kVpiScalar0 src1 < src2 の時
/// @retval kVpiscalarX 比較不能の時
VlValue
ge(const VlValue& src1,
   const VlValue& src2);

/// @relates VlValue
/// @brief greater than or equal 比較演算 (bool)
/// @param[in] src1, src2 オペランド
/// @return src1 >= src2 を返す
bool
operator>=(const VlValue& src1,
	   const VlValue& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Equality operators (IEEE1364-2001 4.1.8)
/// 結果が x になる場合があるので bool ではなく VlValue を返す．
///
/// 2つのオペランドのサイズが異なっていたら長い方に合わせる．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief 等価比較演算子
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 == src2 の時
/// @retval kVpiScalar0 src1 != src2 の時
/// @retval kVpiScalarX 比較不能の時
VlValue
eq(const VlValue& src1,
   const VlValue& src2);

/// @relates VlValue
/// @brief x が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @param[in] src1, src2 オペランド
/// @return 等価と見なせるとき true を返す．
VlValue
eq_with_x(const VlValue& src1,
	  const VlValue& src2);

/// @relates VlValue
/// @brief x および z が 0 および 1 と等価と見なせるとした場合の等価比較演算子
/// @param[in] src1, src2 オペランド
/// @return 等価と見なせるとき true を返す．
VlValue
eq_with_xz(const VlValue& src1,
	   const VlValue& src2);

/// @relates VlValue
/// @brief 等価比較演算子 (bool)
/// @param[in] src1, src2 オペランド
/// @return 1 src1 == src2 を返す．
bool
operator==(const VlValue& src1,
	   const VlValue& src2);

/// @relates VlValue
/// @brief 非等価比較演算子
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar1 src1 != src2 の時
/// @retval kVpiScalar0 src1 == src2 の時
/// @retval kVpiScalarX 比較不能の時
VlValue
ne(const VlValue& src1,
   const VlValue& src2);

/// @relates VlValue
/// @brief 非等価比較演算子 (bool)
/// @param[in] src1, src2 オペランド
/// @retval 1 src1 != src2 を返す．
bool
operator!=(const VlValue& src1,
	   const VlValue& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Logical operators (scalar) (IEEE1364-2001 4.1.9)
/// 結果が x になる場合があるので bool ではなく VlValue を返す．
///
/// オペランドがスカラーで無い場合には強制的にスカラーに変換する．
/// 具体的には最下位ビットをそのままスカラー値だと思う．
///
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief NOT演算
/// @param[in] src オペランド
/// @retval kVpiScalar0 src が真の時
/// @retval kVpiScalar1 src が偽の時
/// @retval kVpiScalarX 計算不能の時
VlValue
log_not(const VlValue& src);

/// @relates VlValue
/// @brief AND演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar0 src1 と src2 のどちらか一方が偽の時
/// @retval kVpiScalar1 src1 と src2 がともに真の時
/// @retval kVpiScalarX 計算不能の時
VlValue
log_and(const VlValue& src1,
	const VlValue& src2);

/// @relates VlValue
/// @brief OR演算
/// @param[in] src1, src2 オペランド
/// @retval kVpiScalar0 src1 と src2 がともに偽の時
/// @retval kVpiScalar1 src1 と src2 のどちらか一方が真の時
/// @retval kVpiScalarX 計算不能の時
VlValue
log_or(const VlValue& src1,
       const VlValue& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Bit-wise logical operators (IEEE1364-2001 4.1.10)
/// 2つのオペランドの長さが合わないときは長い方に合わせる．
/// この場合は常に0が埋められる．
///
/// 結果の型は以下のとおり
/// - 符号:
///   - ともに signed -> signed
///   - 上記以外      -> unsigned
/// - size:
///   - unsized のオペランドは integer とおなじとみなす．
///   - 2つのオペランドの size の大きい方に合わせる．
///   - ただし，どちらも unsized なら結果も unsized
/// - 基数は2
/// @{

/// @relates VlValue
/// @brief bitwise NOT 演算
/// @param[in] src オペランド
/// @return src のビットごとに否定したもの
VlValue
bit_negate(const VlValue& src);

/// @relates VlValue
/// @brief bitwise AND 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに AND したもの
VlValue
bit_and(const VlValue& src1,
	const VlValue& src2);

/// @relates VlValue
/// @brief bitwise OR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに OR したもの
VlValue
bit_or(const VlValue& src1,
       const VlValue& src2);

/// @relates VlValue
/// @brief bitwise XOR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに XOR したもの
VlValue
bit_xor(const VlValue& src1,
	const VlValue& src2);

/// @relates VlValue
/// @brief bitwise XNOR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 をビットごとに XNOR したもの
VlValue
bit_xnor(const VlValue& src1,
	 const VlValue& src2);

/// @}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @name Reduction operators (IEEE1364-2001 4.1.11)
/// 結果の型は以下のとおり
/// - 符号なし
/// - サイズあり(1)
/// - 基数は2
/// @{

/// @brief リダクションAND
/// @return 演算結果を返す．
/// @note すべてのビットの AND を計算する
VlValue
reduction_and(const VlValue& src);

/// @brief リダクションOR
/// @return 演算結果を返す．
/// @note すべてのビットの OR を計算する
VlValue
reduction_or(const VlValue& src);

/// @brief リダクションXOR
/// @return 演算結果を返す．
/// @note すべてのビットの XOR を計算する
VlValue
reduction_xor(const VlValue& src);

/// @brief リダクションNAND
/// @return 演算結果を返す．
/// @note すべてのビットの NAND を計算する
VlValue
reduction_nand(const VlValue& src);

/// @brief リダクションNOR
/// @return 演算結果を返す．
/// @note すべてのビットの NOR を計算する
VlValue
reduction_nor(const VlValue& src);

/// @brief リダクションXNOR
/// @return 演算結果を返す．
/// @note すべてのビットの XNOR を計算する
VlValue
reduction_xnor(const VlValue& src);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Shift operators (IEEE1364-2001 4.1.12)
/// 結果の型は以下のとおり
/// - 符号   : 第1オペランドと同じ
/// - サイズ : 第1オペランドと同じ
/// - 基数   : 第1オペランドと同じ
/// @{

/// @relates VlValue
/// @brief 論理左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (VlValue)
/// @return src1 を src2 だけ論理左シフトしたもの
VlValue
operator<<(const VlValue& src1,
	   const VlValue& src2);

/// @relates VlValue
/// @brief 論理左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ論理左シフトしたもの
VlValue
operator<<(const VlValue& src1,
	   ymuint32 src2);

/// @relates VlValue
/// @brief 論理右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (VlValue)
/// @return src1 を src2 だけ論理右シフトしたもの
VlValue
operator>>(const VlValue& src1,
	   const VlValue& src2);

/// @relates VlValue
/// @brief 論理右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ論理右シフトしたもの
VlValue
operator>>(const VlValue& src1,
	   ymuint32 src2);

/// @relates VlValue
/// @brief 算術左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (VlValue)
/// @return src1 を src2 だけ算術左シフトしたもの
/// 算術左シフト
VlValue
alshift(const VlValue& src1,
	const VlValue& src2);

/// @relates VlValue
/// @brief 算術左シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ算術左シフトしたもの
VlValue
alshift(const VlValue& src1,
	ymuint32 src2);

/// @relates VlValue
/// @brief 算術右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (VlValue)
/// @return src1 を src2 だけ算術右シフトしたもの
VlValue
arshift(const VlValue& src1,
	const VlValue& src2);

/// @relates VlValue
/// @brief 算術右シフト
/// @param[in] src1 元の値
/// @param[in] src2 シフト量 (ymuint32)
/// @return src1 を src2 だけ算術右シフトしたもの
VlValue
arshift(const VlValue& src1,
	ymuint32 src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Conditional operators (IEEE1364-2001 4.1.13)
/// 第1オペランドは強制的にスカラーに変換される．
///
/// 結果の型は以下のとおり
/// - 符号:
///   - 第2,第3オペランドがともに signed -> signed
///   - 上記以外 -> unsigned
/// - サイズ:
///   - 第2,第3オペランドの長い方に合わせる．
///   - ただし unsized なら integer と同じと見なす．
///   - ともに unsized なら 結果も unsized
/// - 基数:
///   - 第1オペランドが 1 の時には第2オペランドの基数
///   - 第1オペランドが 0 の時には第3オペランドの基数
///   - 第1オペランドが x, z の時に第2オペランドと
///     第3オペランドの基数が異なっていたら2とする．
/// @{

/// @relates VlValue
/// @brief 条件演算
/// @param[in] src1 条件
/// @param[in] src2 src1 が真の時に選ばれる値
/// @param[in] src3 src1 が偽の時に選ばれる値
/// @return 演算結果
VlValue
ite(const VlValue& src1,
    const VlValue& src2,
    const VlValue& src3);

/// @relates VlValue
/// @brief 条件演算
/// @param[in] src1 条件
/// @param[in] src2 src1 が真の時に選ばれる値
/// @param[in] src3 src1 が偽の時に選ばれる値
/// @return 演算結果
VlValue
ite(const VlScalarVal& src1,
    const VlValue& src2,
    const VlValue& src3);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @name Concatenation (IEEE1364-2001 4.1.14)
/// 結果の型は以下の通り
/// - 符号: 常に unsigned
/// - サイズ: 連結するオペランドのサイズの和．
///           もしもオペランドに unsized な値が含まれていたらエラー
///           (今はintegerと見なす)
/// - 基数: 常に2
/// @{

/// @relates VlValue
/// @brief 連結演算
/// @param[in] src_list 連結する値のリスト
/// @return 連結した結果を返す．
VlValue
concat(const list<VlValue>& src_list);

/// @relates VlValue
/// @brief 連結演算
/// @param[in] src_list 連結する値のリスト
/// @return 連結した結果を返す．
VlValue
concat(const vector<VlValue>& src_list);

/// @relates VlValue
/// @brief 繰り返し連結演算
/// @param[in] src_list 連結する値のリスト
/// @return src_list[1~n-1] の内容を src_list[0] 回繰り返して連結したもの
VlValue
multi_concat(const list<VlValue>& src_list);

/// @relates VlValue
/// @brief 繰り返し連結演算
/// @param[in] rep 繰り返し数
/// @param[in] src_list 連結する値のリスト
/// @return src_list[1~n-1] の内容を src_list[0] 回繰り返して連結したもの
VlValue
multi_concat(const vector<VlValue>& src_list);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @class VlValueRep VlValue.h "ym_verilog/VlValue.h"
/// @brief VlValue の実体を表す仮想クラス
//////////////////////////////////////////////////////////////////////
class VlValueRep :
  public RefCount
{
  friend class VlValue;
  friend class SmartPtr<VlValueRep>;

private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const = 0;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const = 0;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const = 0;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const = 0;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const = 0;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const = 0;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const = 0;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const = 0;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const = 0;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const = 0;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const = 0;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const = 0;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 型を返す．
inline
VlValue::tType
VlValue::type() const
{
  return mRep->type();
}

// @brief 整数型の時に true を返す．
inline
bool
VlValue::is_int_type() const
{
  return type() == kIntType;
}

// @brief 符号なし整数型の時に true 返す．
inline
bool
VlValue::is_uint_type() const
{
  return type() == kUintType;
}

// @brief 実数型の時に true を返す．
inline
bool
VlValue::is_real_type() const
{
  return type() == kRealType;
}

// @brief スカラー型の時に true を返す．
inline
bool
VlValue::is_scalar_type() const
{
  return type() == kScalarType;
}

// @brief time 型の時に true を返す．
inline
bool
VlValue::is_time_type() const
{
  return type() == kTimeType;
}

// @brief ビットベクタ型の時に true を返す．
inline
bool
VlValue::is_bitvector_type() const
{
  return type() == kBitVectorType;
}

// @brief エラー状態の時に true を返す．
inline
bool
VlValue::is_error() const
{
  return type() == kErrorType;
}

// @brief 整数型に変換可能な時に true を返す．
inline
bool
VlValue::is_int_conv() const
{
  return mRep->is_int_conv();
}

// @brief ymuint 型に変換可能な時に true を返す．
inline
bool
VlValue::is_uint_conv() const
{
  return mRep->is_uint_conv();
}

// @brief 実数型に変換可能な時に true を返す．
inline
bool
VlValue::is_real_conv() const
{
  return mRep->is_real_conv();
}

// @brief time 型に変換可能な時に true を返す．
inline
bool
VlValue::is_time_conv() const
{
  return mRep->is_time_conv();
}

// @brief ビットベクタ型に変換可能な時に true を返す．
inline
bool
VlValue::is_bitvector_conv() const
{
  return mRep->is_bitvector_conv();
}

// @brief 符号付きの型の時に true を返す．
inline
bool
VlValue::is_signed() const
{
  switch ( type() ) {
  case kIntType:
  case kRealType:
    return true;

  case kUintType:
  case kScalarType:
  case kTimeType:
  case kErrorType:
    return false;

  case kBitVectorType:
    return bitvector_value().is_signed();
  }
  assert_not_reached(__FILE__, __LINE__);
  return false;
}

// @brief ビットサイズを返す．
inline
ymuint
VlValue::bit_size() const
{
  switch ( type() ) {
  case kIntType:
  case kUintType:
    return kVpiSizeInteger;

  case kRealType:
    return kVpiSizeReal;

  case kScalarType:
    return 1;

  case kTimeType:
    return kVpiSizeTime;

  case kBitVectorType:
    return bitvector_value().size();

  case kErrorType:
    return 0;
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 値の型を返す．
inline
VlValueType
VlValue::value_type() const
{
  switch ( type() ) {
  case kIntType:
    return VlValueType::int_type();

  case kUintType:
    return VlValueType(false, true, kVpiSizeInteger);

  case kRealType:
    return VlValueType::real_type();

  case kScalarType:
    return VlValueType(false, true, 1);

  case kTimeType:
    return VlValueType::time_type();

  case kBitVectorType:
    return bitvector_value().value_type();

  case kErrorType:
    return VlValueType();
  }
  assert_not_reached(__FILE__, __LINE__);
  return VlValueType();
}

// @brief 整数型の値を返す．
// @note 値が整数型に変換できない時の値は不定
inline
int
VlValue::int_value() const
{
  return mRep->int_value();
}

// @brief ymuint 型の値を返す．
// @note 値が整数型に変換できない時の値は不定
inline
ymuint
VlValue::uint_value() const
{
  return mRep->uint_value();
}

// @brief スカラー型の値を返す．
// @note スカラー型には常に変換可能
inline
VlScalarVal
VlValue::scalar_value() const
{
  return mRep->scalar_value();
}

// @brief 論理型の値を返す．
inline
VlScalarVal
VlValue::logic_value() const
{
  return mRep->logic_value();
}

// @brief 実数型の値を返す．
// @note 値が実数型に変換できない時の値は不定
inline
double
VlValue::real_value() const
{
  return mRep->real_value();
}

// @brief time 型の値を返す．
// @note 値が time 型に変換できない時の値は不定
inline
VlTime
VlValue::time_value() const
{
  return mRep->time_value();
}

// @brief ビットベクタ型の値を返す．
// @param[out] bitvector 結果を格納する変数
// @param[in] req_type 要求されるデータの型
inline
BitVector
VlValue::bitvector_value(const VlValueType& req_type) const
{
  return mRep->bitvector_value(req_type);
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VLVALUE_H
