#ifndef YM_VERILOG_VL_VLPRIMITIVE_H
#define YM_VERILOG_VL_VLPRIMITIVE_H

/// @file ym_verilog/vl/VlPrimitive.h
/// @brief VlPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPrimitive.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlNamedObj.h"
#include "ym_verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlPrimArray VlPrimitive.h "ym_verilog/vl/VlPrimitive.h"
/// @brief gate/UDP instance の配列を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class VlPrimArray :
  public VlNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const = 0;

  /// @brief UDP 定義を返す．
  virtual
  const VlUdpDefn*
  udp_defn() const = 0;

  /// @brief 0 の強さを得る．
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief 1 の強さを得る．
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief 遅延式を得る．
  virtual
  const VlDelay*
  delay() const = 0;

  /// @brief 範囲の MSB の値を返す．
  virtual
  int
  left_range_val() const = 0;

  /// @brief 範囲の LSB の値を返す．
  virtual
  int
  right_range_val() const = 0;

  /// @brief 範囲のMSBを表す文字列の取得
  virtual
  string
  left_range_string() const = 0;

  /// @brief 範囲のLSBを表す文字列の取得
  virtual
  string
  right_range_string() const = 0;

  /// @brief 要素数を返す．
  virtual
  ymuint
  elem_num() const = 0;

  /// @brief 要素のプリミティブを返す．
  /// @param[in] offset 位置番号 ( 0 <= offset < elem_num() )
  virtual
  const VlPrimitive*
  elem_by_offset(ymuint offset) const = 0;

  /// @brief 要素を返す．
  /// @param[in] index インデックス
  virtual
  const VlPrimitive*
  elem_by_index(int index) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VlPrimitive VlPrimitive.h "ym_verilog/vl/VlPrimitive.h"
/// @brief gate/UDP instance を表すクラス
//////////////////////////////////////////////////////////////////////
class VlPrimitive :
  public VlNamedObj
{
public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  virtual
  tVpiPrimType
  prim_type() const = 0;

  /// @brief プリミティブの定義名を返す．
  virtual
  const char*
  def_name() const = 0;

  /// @brief UDP 定義を返す．
  /// @note prim_type() が kVpiSeqPrim と kVpiConbPrim の場合のみ意味を持つ．
  virtual
  const VlUdpDefn*
  udp_defn() const = 0;

  /// @brief 0 の強さを得る．
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief 1 の強さを得る．
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief 遅延式を得る．
  virtual
  const VlDelay*
  delay() const = 0;

  /// @brief ポート数を得る．
  virtual
  ymuint
  port_num() const = 0;

  /// @brief ポート端子を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < port_num() )
  virtual
  const VlPrimTerm*
  prim_term(ymuint pos) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class VlPrimTerm VlPrimitive.h "ym_verilog/vl/VlPrimitive.h"
/// @brief プリミティブインスタンスのポート端子を表すクラス
/// IEEE Std 1364-2001 26.6.13 Primitive, prim term
//////////////////////////////////////////////////////////////////////
class VlPrimTerm :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlPrimTerm() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のプリミティブを返す．
  virtual
  const VlPrimitive*
  primitive() const = 0;

  /// @brief 入出力の種類を返す．
  virtual
  tVlDirection
  direction() const = 0;

  /// @brief 端子番号を返す．
  virtual
  ymuint
  term_index() const = 0;

  /// @brief 接続しているネットを表す式を返す．
  virtual
  const VlExpr*
  expr() const = 0;

};


END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLPRIMITIVE_H
