#ifndef VERILOG_VL_VLIODECL_H
#define VERILOG_VL_VLIODECL_H

/// @file verilog/vl/VlIODecl.h
/// @brief VlIODecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlObj.h"
#include "verilog/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlIODecl VlIODecl.h "ym_verilog/vl/VlIODecl.h"
/// @brief 入出力宣言要素を表すクラス
//////////////////////////////////////////////////////////////////////
class VlIODecl :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlIODecl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlIODecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const char*
  name() const = 0;

  /// @brief 方向を返す．
  /// @retval kVlInput 入力
  /// @retval kVlOutput 出力
  /// @retval kVlInout 入出力
  virtual
  tVlDirection
  direction() const = 0;

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

  /// @brief MSB の値を返す．
  /// @note 範囲を持たない時の値は不定
  virtual
  int
  left_range_val() const = 0;

  /// @brief LSB の値を返す．
  /// @note 範囲を持たない時の値は不定
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

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

  /// @brief 対応する宣言要素返す．
  virtual
  const VlDecl*
  decl() const = 0;

  /// @brief 親のモジュールの取得
  /// @note モジュールのIOの時にのみ意味を持つ．
  virtual
  const VlModule*
  module() const = 0;

  /// @brief 親の UDP の取得
  /// @note UDPのIOの時にのみ意味を持つ．
  virtual
  const VlUdpDefn*
  udp_defn() const = 0;

  /// @brief 親のタスクの取得
  /// @note タスクのIOの時にのみ意味を持つ．
  virtual
  const VlTaskFunc*
  task() const = 0;

  /// @brief 親の関数の取得
  /// @note 関数のIOの時にのみ意味を持つ．
  virtual
  const VlTaskFunc*
  function() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLIODECL_H
