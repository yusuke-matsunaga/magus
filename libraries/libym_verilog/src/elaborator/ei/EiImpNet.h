#ifndef EIIMPNET_H
#define EIIMPNET_H

/// @file EiImpNet.h
/// @brief EiImpNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"
#include "YmVerilog/VlScalarVal.h"
#include "YmVerilog/pt/PtP.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiImpNet EiImpNet.h "EiImpNet.h"
/// @brief 暗黙のネットを表すクラス
//////////////////////////////////////////////////////////////////////
class EiImpNet :
  public ElbDecl
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木のプライマリ式
  EiImpNet(const VlNamedObj* parent,
	   const PtExpr* pt_expr,
	   tVpiNetType net_type);

  /// @brief デストラクタ
  virtual
  ~EiImpNet();


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
  /// @note このクラスでは false を返す．
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
  /// @note このクラスは 1 を返す．
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
  /// @retval データ型 パラメータや変数の場合
  /// @retval kVpiVarNone 上記以外
  /// @note デフォルト値としてこのクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note デフォルト値としてこのクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note デフォルト値としてこのクラスでは kVpiVsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note デフォルト値としてこのクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  /// @note デフォルト値としてこのクラスでは NULL を返す．
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
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  virtual
  void
  set_signed();

  /// @brief スカラー値を返す．
  virtual
  VlScalarVal
  get_scalar() const;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(const VlScalarVal& val);

  /// @brief 論理値を返す．
  virtual
  VlScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		const VlValueType& req_type = VlValueType()) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  VlScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		const VlScalarVal& val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木のプライマリ式
  const PtExpr* mPtExpr;

  // ネット型
  tVpiNetType mNetType;

  // 値
  VlScalarVal mVal;

};

END_NAMESPACE_YM_VERILOG

#endif // EIIMPNET_H
