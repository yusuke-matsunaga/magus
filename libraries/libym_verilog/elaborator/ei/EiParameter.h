#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIPARAMETER_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIPARAMETER_H

/// @file libym_verilog/elaborator/ei/EiParameter.h
/// @brief EiParameter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParameter.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParameter.h"
#include "EiRange.h"

#include "ym_verilog/VlValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamHead.h EiParameter.h "EiParameter.h"
/// @brief parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamHead :
  public ElbParamHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  EiParamHead(const VlNamedObj* parent,
	      const PtDeclHead* pt_head);

  /// @brief デストラクタ
  virtual
  ~EiParamHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParamHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlNamedObj*
  parent() const;

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

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief データ型の取得
  virtual
  tVpiVarType
  data_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // パース木の宣言ヘッダ
  const PtDeclHead* mPtHead;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiParamHeadV EiParameter.h "EiParameter.h"
/// @brief 範囲付きの EiParamHead
//////////////////////////////////////////////////////////////////////
class EiParamHeadV :
  public EiParamHead
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head パース木の宣言ヘッダ
  /// @param[in] left 範囲の左側の式
  /// @param[in] right 範囲の右側の式
  /// @param[in] left_val 範囲の左側の値
  /// @param[in] right_val 範囲の右側の値
  EiParamHeadV(const VlNamedObj* parent,
	       const PtDeclHead* pt_head,
	       const PtExpr* left,
	       const PtExpr* right,
	       int left_val,
	       int right_val);

  /// @brief デストラクタ
  virtual
  ~EiParamHeadV();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

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

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  EiRangeImpl mRange;

};


//////////////////////////////////////////////////////////////////////
/// @class EiParameter EiParameter.h "EiParameter.h"
/// @brief ElbParameter の実装クラス
//////////////////////////////////////////////////////////////////////
class EiParameter :
  public ElbParameter
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiParameter(ElbParamHead* head,
	      const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiParameter();


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
  tVpiValueType
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

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは false を返す．
  virtual
  bool
  is_local_param() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbParameter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  virtual
  VlValue
  get_value() const;

  /// @brief 値の設定
  /// @param[in] expr 値を表す式
  /// @param[in] value 値
  virtual
  void
  set_expr(const PtExpr* expr,
	   const VlValue& value);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbParamHead* mHead;

  // パース木の宣言要素
  const PtNamedBase* mPtItem;

  // 式
  const PtExpr* mExpr;

  // 値
  VlValue mValue;

};


//////////////////////////////////////////////////////////////////////
/// @class EiLocalParam EiParameter.h "EiParameter.h"
/// @brief ElbParameter の実装クラス (localparam版)
//////////////////////////////////////////////////////////////////////
class EiLocalParam :
  public EiParameter
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] head ヘッダ
  /// @param[in] pt_item パース木の宣言要素
  EiLocalParam(ElbParamHead* head,
	       const PtNamedBase* pt_item);

  /// @brief デストラクタ
  virtual
  ~EiLocalParam();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief localparam のときに true 返す．
  /// @note このクラスでは true を返す．
  virtual
  bool
  is_local_param() const;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIPARAMETER_H
