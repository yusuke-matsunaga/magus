#ifndef VLVALUEREP_H
#define VLVALUEREP_H

/// @file VlValueRep.h
/// @brief VlValueRep (とその派生クラス)のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/VlValue.h"
#include "YmVerilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlValueError VlValueRep.h "VlValueRep.h"
/// @brief エラーを表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueError :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueError();

  /// @brief デストラクタ
  virtual
  ~VlValueError();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueInt VlValueRep.h "VlValueRep.h"
/// @brief 整数型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueInt :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueInt(int val);

  /// @brief デストラクタ
  virtual
  ~VlValueInt();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueUint VlValueRep.h "VlValueRep.h"
/// @brief 符号なし整数型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueUint :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueUint(ymuint val);

  /// @brief デストラクタ
  virtual
  ~VlValueUint();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueScalar VlValueRep.h "VlValueRep.h"
/// @brief スカラー型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueScalar :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueScalar(const VlScalarVal& val);

  /// @brief デストラクタ
  virtual
  ~VlValueScalar();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VlScalarVal mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueReal VlValueRep.h "VlValueRep.h"
/// @brief 実数型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueReal :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueReal(double val);

  /// @brief デストラクタ
  virtual
  ~VlValueReal();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueTime VlValueRep.h "VlValueRep.h"
/// @brief time 型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueTime :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueTime(VlTime val);

  /// @brief デストラクタ
  virtual
  ~VlValueTime();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VlTime mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class VlValueBitVector VlValueRep.h "VlValueRep.h"
/// @brief ビットベクタ型を表す VlValueRep の派生クラス
//////////////////////////////////////////////////////////////////////
class VlValueBitVector :
  public VlValueRep
{
public:

  /// @brief コンストラクタ
  VlValueBitVector(const BitVector& val);

  /// @brief デストラクタ
  virtual
  ~VlValueBitVector();


private:
  //////////////////////////////////////////////////////////////////////
  // VlValueRep の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  VlValue::tType
  type() const;

  /// @brief 整数型に変換可能な時に true を返す．
  virtual
  bool
  is_int_conv() const;

  /// @brief ymuint 型に変換可能な時に true を返す．
  virtual
  bool
  is_uint_conv() const;

  /// @brief 実数型に変換可能な時に true を返す．
  virtual
  bool
  is_real_conv() const;

  /// @brief time 型に変換可能な時に true を返す．
  virtual
  bool
  is_time_conv() const;

  /// @brief ビットベクタ型に変換可能な時に true を返す．
  virtual
  bool
  is_bitvector_conv() const;

  /// @brief 整数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  int
  int_value() const;

  /// @brief ymuint 型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  ymuint
  uint_value() const;

  /// @brief スカラー型の値を返す．
  virtual
  VlScalarVal
  scalar_value() const;

  /// @brief 論理型の値を返す．
  virtual
  VlScalarVal
  logic_value() const;

  /// @brief 実数型の値を返す．
  /// @note 値が整数型に変換できない時の値は不定
  virtual
  double
  real_value() const;

  /// @brief time 型の値を返す．
  virtual
  VlTime
  time_value() const;

  /// @brief ビットベクタ型の値を返す．
  /// @param[in] req_type 要求されるデータの型
  virtual
  BitVector
  bitvector_value(const VlValueType& req_type) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  BitVector mVal;

};

END_NAMESPACE_YM_VERILOG

#endif // VLVALUEREP_H
