#ifndef VERILOG_VL_VLDECL_H
#define VERILOG_VL_VLDECL_H

/// @file YmVerilog/vl/VlDecl.h
/// @brief VlDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlDeclBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlDecl VlDecl.h "ym_YmVerilog/vl/VlDecl.h"
/// @brief エラボレーション中の名前付きオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlDecl :
  public VlDeclBase
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDecl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数値を持つ型のときに true を返す．
  virtual
  bool
  is_consttype() const = 0;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval NULL 設定がない場合
  virtual
  const VlExpr*
  init_value() const = 0;

  /// @brief localparam のときに true 返す．
  virtual
  bool
  is_local_param() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLDECL_H
