#ifndef VERILOG_VL_VLTASKFUNC_H
#define VERILOG_VL_VLTASKFUNC_H

/// @file YmVerilog/vl/VlTaskFunc.h
/// @brief VlTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlNamedObj.h"
#include "YmVerilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlTaskFunc VlTaskFunc.h "YmVerilog/vlVlTaskFunc.h"
/// @brief elaboration 中の task/function を表すクラス
/// IEEE Std 1364-2001 26.6.18 Task, function declaration
//////////////////////////////////////////////////////////////////////
class VlTaskFunc :
  public VlNamedObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlTaskFunc() { }


public:
  //////////////////////////////////////////////////////////////////////
  // Task/Function に共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 宣言されていたら true を返す．
  virtual
  bool
  automatic() const = 0;

  /// @brief 入出力数を得る．
  virtual
  ymuint
  io_num() const = 0;

  /// @brief 入出力の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < io_num() )
  virtual
  const VlIODecl*
  io(ymuint pos) const = 0;

  /// @brief 本体のステートメントを得る．
  virtual
  const VlStmt*
  stmt() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Function の仮想関数
  // Task の場合には意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  virtual
  tVpiFuncType
  func_type() const = 0;

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

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VLTASKFUNC_H
