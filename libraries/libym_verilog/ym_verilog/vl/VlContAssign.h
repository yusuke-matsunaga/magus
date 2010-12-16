#ifndef YM_VERILOG_VL_VLCONTASSIGN_H
#define YM_VERILOG_VL_VLCONTASSIGN_H

/// @file ym_verilog/vl/VlContAssign.h
/// @brief VlContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlContAssign.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlContAssign VlContAssign.h <ym_verilog/vl/VlContAssign.h>
/// @brief 継続的代入文を表すクラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class VlContAssign :
  public VlObj
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlContAssign() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  virtual
  const VlModule*
  module() const = 0;

  /// @brief 0の強さを返す．
  virtual
  tVpiStrength
  drive0() const = 0;

  /// @brief 1の強さを返す．
  virtual
  tVpiStrength
  drive1() const = 0;

  /// @brief 遅延を表す式を返す．
  virtual
  const VlDelay*
  delay() const = 0;

  /// @brief ネット宣言中の assignment の時に true を返す．
  virtual
  bool
  has_net_decl_assign() const = 0;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const = 0;

  /// @brief 左辺を返す．
  virtual
  const VlExpr*
  lhs() const = 0;

  /// @brief 左辺式の要素数の取得
  /// @note 通常は1だが，連結演算子の場合はその子供の数となる．
  /// @note ただし，連結演算の入れ子はすべて平坦化して考える．
  virtual
  ymuint
  lhs_elem_num() const = 0;

  /// @brief 左辺式の要素の取得
  /// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
  /// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
  virtual
  const VlExpr*
  lhs_elem(ymuint pos) const = 0;

  /// @brief 右辺を返す．
  virtual
  const VlExpr*
  rhs() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VL_VLCONTASSIGN_H
