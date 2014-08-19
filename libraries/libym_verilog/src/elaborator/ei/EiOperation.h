#ifndef EIOPERATION_H
#define EIOPERATION_H

/// @file EiOperation.h
/// @brief EiOperation のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiOperation EiOperation.h "EiOperation.h"
/// @brief 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiOperation :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  EiOperation(const PtExpr* pt_expr);

  /// @brief デストラクタ
  virtual
  ~EiOperation();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の時に true を返す．
  virtual
  bool
  is_operation() const;

  /// @brief 演算子のタイプを返す．
  virtual
  tVlOpType
  op_type() const;

};

END_NAMESPACE_YM_VERILOG

#endif // EIOPERATION_H
