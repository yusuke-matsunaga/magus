#ifndef LIBYM_VERILOG_ELB_ELBDECL_H
#define LIBYM_VERILOG_ELB_ELBDECL_H

/// @file libym_verilog/elaborator/include/ElbDecl.h
/// @brief ElbDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlDeclArray.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDeclHead.h ElbDecl.h "ElbDecl.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class ElbDeclHead
{
protected:

  /// @brief コンストラクタ
  ElbDeclHead();

  /// @brief デストラクタ
  virtual
  ~ElbDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclHead の仮想関数
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

  /// @brief 範囲指定を持つとき true を返す．
  virtual
  bool
  has_range() const = 0;

  /// @brief 範囲の MSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の値を返す．
  /// @note 範囲を持たないときの値は不定
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のMSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  left_range_string() const = 0;

  /// @brief 範囲のLSBを表す文字列の取得
  /// @note 範囲を持たない時の値は不定
  virtual
  string
  right_range_string() const = 0;

  /// @brief left_range >= right_range の時に true を返す．
  virtual
  bool
  is_big_endian() const = 0;

  /// @brief left_range <= right_range の時に true を返す．
  virtual
  bool
  is_little_endian() const = 0;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @retval index に対するオフセット index が範囲内に入っている時．
  /// @retval -1 index が範囲外の時
  virtual
  int
  bit_offset(int index) const = 0;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  /// @note このクラスでは kVpiVarNone を返す．
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  /// @note このクラスでは kVpiNone を返す．
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  /// @note このクラスでは kVpiNsNone を返す．
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  /// @note このクラスでは kVpiNoStrength を返す．
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
  ElbDelay*
  delay() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の設定
  /// @note このクラスではなにもしない．
  virtual
  void
  set_delay(ElbDelay* delay);

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDecl ElbDecl.h "ElbDecl.h"
/// @brief 名前付きの宣言要素を表すクラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
/// IEEE Std 1364-2001 26.6.8 Variables
/// IEEE Std 1364-2001 26.6.11 Named event
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDecl :
  public VlDecl
{
  friend class CellDecl;

protected:

  /// @brief コンストラクタ
  ElbDecl();

  /// @brief デストラクタ
  virtual
  ~ElbDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数値を持つ型のときに true を返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_consttype() const;

  /// @brief localparam のときに true 返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_local_param() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  virtual
  void
  set_signed() = 0;

  /// @brief 初期値の設定
  /// @param[in] expr 初期値
  /// @note このクラスでは何もしない．
  virtual
  void
  set_init(ElbExpr* expr);


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  const ElbDecl*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbDecl* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbDeclArray ElbDecl.h "ElbDecl.h"
/// @brief 名前付きの宣言要素を表すクラス
/// IEEE Std 1364-2001 26.6.6 Nets and net arrays
/// IEEE Std 1364-2001 26.6.7 Regs and reg arrays
/// IEEE Std 1364-2001 26.6.8 Variables
/// IEEE Std 1364-2001 26.6.11 Named event
/// IEEE Std 1364-2001 26.6.12 Parameter, specparam
//////////////////////////////////////////////////////////////////////
class ElbDeclArray :
  public VlDeclArray
{
  friend class CellDeclArray;

protected:

  /// @brief コンストラクタ
  ElbDeclArray();

  /// @brief デストラクタ
  virtual
  ~ElbDeclArray();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の型を返す．
  virtual
  tVpiObjType
  elem_type() const = 0;

  /// @brief スカラー値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  tVpiScalarVal
  get_scalar(ymuint offset) const = 0;

  /// @brief スカラー値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_scalar(ymuint offset,
	     tVpiScalarVal val) = 0;

  /// @brief 論理値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  tVpiScalarVal
  get_logic(ymuint offset) const = 0;

  /// @brief real 型の値を返す．
  /// @param[in] offset 要素のオフセット
  virtual
  double
  get_real(ymuint offset) const = 0;

  /// @brief real 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_real(ymuint offset,
	   double val) = 0;

  /// @brief bitvector 型の値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(ymuint offset,
		BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const = 0;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] val 値
  virtual
  void
  set_bitvector(ymuint offset,
		const BitVector& val) = 0;

  /// @brief ビット選択値を返す．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(ymuint offset,
		int index) const = 0;

  /// @brief ビット値を設定する．
  /// @param[in] offset 要素のオフセット
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(ymuint offset,
		int index,
		tVpiScalarVal val) = 0;

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
		 BitVector& val) const = 0;

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
		 const BitVector& val) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  const ElbDeclArray*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbDeclArray* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を返す．
inline
const ElbDecl*
ElbDecl::next() const
{
  return mNext;
}

// @brief 次の要素を返す．
inline
const ElbDeclArray*
ElbDeclArray::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELB_ELBDECL_H
