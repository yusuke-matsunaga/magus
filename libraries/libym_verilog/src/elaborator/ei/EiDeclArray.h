#ifndef EIDECLARRAY_H
#define EIDECLARRAY_H

/// @file EiDeclArray.h
/// @brief EiDeclArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "EiRange.h"

#include "verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclArray EiDeclArray.h "EiDeclArray.h"
/// @brief 配列型の ElbDecl
//////////////////////////////////////////////////////////////////////
class EiDeclArray :
  public ElbDeclArray
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  EiDeclArray(ElbDeclHead* head,
	      const PtNamedBase* pt_item,
	      ymuint dim_size,
	      EiRange* range_array);

  /// @brief デストラクタ
  virtual
  ~EiDeclArray();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @param[out] offset インデックスに対するオフセット値
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  virtual
  bool
  calc_bit_offset(int index,
		  ymuint& offset) const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const VlDelay*
  delay() const;

  /// @brief 配列型オブジェクトの時に true を返す．
  virtual
  bool
  is_array() const;

  /// @brief 多次元の配列型オブジェクトの時に true を返す．
  virtual
  bool
  is_multi_array() const;

  /// @brief 配列型オブジェクトの場合の次元数の取得
  virtual
  ymuint
  dimension() const;

  /// @brief 範囲の取得
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  virtual
  const VlRange*
  range(ymuint pos) const;

  /// @brief 配列の要素数の取得
  virtual
  ymuint
  array_size() const;

  /// @brief 1次元配列の場合にインデックスからオフセットを計算する．
  /// @param[in] index インデックス
  /// @param[out] offset index に対するオフセット値
  /// @retval true index が範囲内だった．
  /// @retval false index が範囲外だった．
  virtual
  bool
  calc_array_offset(int index,
		    ymuint& offset) const;

  /// @brief 他次元配列の場合にインデックスのリストからオフセットを計算する．
  /// @param[in] index_list インデックスのリスト
  /// @param[out] offset index_list に対するオフセット値
  /// @retval true オフセットが正しく計算できた．
  /// @retval false index_list のいずれかの値が範囲外だった．
  virtual
  bool
  calc_array_offset(const vector<int>& index_list,
		    ymuint& offset) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の型を返す．
  virtual
  tVpiObjType
  elem_type() const;

  /// @brief インデックスからオフセットを計算する．
  /// @param[in] index_array インデックス式の配列
  ymuint
  calc_offset(const vector<ElbExpr*>& index_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbDeclHead* mHead;

  // パース木の宣言要素
  const PtNamedBase* mPtItem;

  // dimension の配列
  EiRangeArray mRangeList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayN EiDeclArray.h "EiDeclArray.h"
/// @brief 値を持たない EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayN :
  public EiDeclArray
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  EiDeclArrayN(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint dim_size,
	       EiRange* range_array);

  /// @brief デストラクタ
  virtual
  ~EiDeclArrayN();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_scalar(ymuint offset) const;

  /// @brief スカラー値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_scalar(ymuint offset,
	     const VlScalarVal& val);

  /// @brief 論理値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_logic(ymuint offset) const;

  /// @brief real 型の値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  double
  get_real(ymuint offset) const;

  /// @brief real 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_real(ymuint offset,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(ymuint offset,
		BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_bitvector(ymuint offset,
		const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(ymuint offset,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(ymuint offset,
		int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  get_partselect(ymuint offset,
		 int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(ymuint offset,
		 int left,
		 int right,
		 const BitVector& val);

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayS EiDeclArray.h "EiDeclArray.h"
/// @brief スカラー値を持つ EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayS :
  public EiDeclArray
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  /// @param[in] varray 値を納める配列
  EiDeclArrayS(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint dim_size,
	       EiRange* range_array,
	       VlScalarVal* varray);

  /// @brief デストラクタ
  virtual
  ~EiDeclArrayS();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_scalar(ymuint offset) const;

  /// @brief スカラー値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_scalar(ymuint offset,
	     const VlScalarVal& val);

  /// @brief 論理値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_logic(ymuint offset) const;

  /// @brief real 型の値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  double
  get_real(ymuint offset) const;

  /// @brief real 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_real(ymuint offset,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(ymuint offset,
		BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_bitvector(ymuint offset,
		const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(ymuint offset,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(ymuint offset,
		int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  get_partselect(ymuint offset,
		 int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(ymuint offset,
		 int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VlScalarVal* mValArray;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayR EiDeclArray.h "EiDeclArray.h"
/// @brief 実数値を持つ EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayR :
  public EiDeclArray
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  /// @param[in] varray 値を納める配列
  EiDeclArrayR(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint dim_size,
	       EiRange* range_array,
	       double* varray);

  /// @brief デストラクタ
  virtual
  ~EiDeclArrayR();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_scalar(ymuint offset) const;

  /// @brief スカラー値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_scalar(ymuint offset,
	     const VlScalarVal& val);

  /// @brief 論理値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_logic(ymuint offset) const;

  /// @brief real 型の値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  double
  get_real(ymuint offset) const;

  /// @brief real 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_real(ymuint offset,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(ymuint offset,
		BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_bitvector(ymuint offset,
		const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(ymuint offset,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(ymuint offset,
		int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  get_partselect(ymuint offset,
		 int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(ymuint offset,
		 int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double* mValArray;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayV EiDeclArray.h "EiDeclArray.h"
/// @brief ビットベクタ値を持つ EiDeclArray
//////////////////////////////////////////////////////////////////////
class EiDeclArrayV :
  public EiDeclArray
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  /// @param[in] dim_size 次元数
  /// @param[in] range_array 範囲の配列
  /// @param[in] varray 値を納める配列
  EiDeclArrayV(ElbDeclHead* head,
	       const PtNamedBase* pt_item,
	       ymuint dim_size,
	       EiRange* range_array,
	       BitVector* varray);

  /// @brief デストラクタ
  virtual
  ~EiDeclArrayV();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スカラー値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_scalar(ymuint offset) const;

  /// @brief スカラー値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_scalar(ymuint offset,
	     const VlScalarVal& val);

  /// @brief 論理値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  VlScalarVal
  get_logic(ymuint offset) const;

  /// @brief real 型の値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  double
  get_real(ymuint offset) const;

  /// @brief real 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_real(ymuint offset,
	   double val);

  /// @brief bitvector 型の値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(ymuint offset,
		BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_bitvector(ymuint offset,
		const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(ymuint offset,
		int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(ymuint offset,
		int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  get_partselect(ymuint offset,
		 int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(ymuint offset,
		 int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  BitVector* mValArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLARRAY_H
