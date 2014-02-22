#ifndef ELBATTRIBUTE_H
#define ELBATTRIBUTE_H

/// @file ElbAttribute.h
/// @brief ElbAttribute のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlAttribute.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbAttribute ElbAttribute.h "ElbAttribute.h"
/// @brief VlAttribute の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbAttribute :
  public VlAttribute
{
protected:

  /// @brief コンストラクタ
  ElbAttribute() { }

  /// @brief デストラクタ
  virtual
  ~ElbAttribute() { }

};


//////////////////////////////////////////////////////////////////////
/// @class ElbAttrList ElbAttribute.h "ElbAttribute.h"
/// @brief ElbAttribute のリスト(配列)
//////////////////////////////////////////////////////////////////////
class ElbAttrList :
  public VlAttrList
{
protected:

  /// @brief コンストラクタ
  ElbAttrList() { }

  /// @brief デストラクタ
  ~ElbAttrList() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbAttrList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を設定する．
  /// @param[in] pos 位置番号
  /// @param[in] pt_as パース木の定義要素
  /// @param[in] expr 値を表す式
  /// @param[in] def 定義側の属性のとき true とするフラグ
  virtual
  void
  set(ymuint pos,
      const PtAttrSpec* pt_as,
      ElbExpr* expr,
      bool def) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBATTRIBUTE_H
