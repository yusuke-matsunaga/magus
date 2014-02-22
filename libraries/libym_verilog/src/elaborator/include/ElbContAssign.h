#ifndef ELBCONTASSIGN_H
#define ELBCONTASSIGN_H

/// @file ElbContAssign.h
/// @brief ElbContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlContAssign.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbCaHead ElbContAssign.h "ElbContAssign.h"
/// @brief 継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class ElbCaHead
{
protected:

  /// @brief コンストラクタ
  ElbCaHead();

  /// @brief デストラクタ
  virtual
  ~ElbCaHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbCaHead の仮想関数
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
  ElbDelay*
  delay() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbContAssign ElbContAssign.h "ElbContAssign.h"
/// @brief 継続的代入文を表すクラス
/// IEEE Std 1364-2001 26.6.24 Continuous assignment
//////////////////////////////////////////////////////////////////////
class ElbContAssign :
  public VlContAssign
{
  friend class CellContAssign;

protected:

  /// @brief コンストラクタ
  ElbContAssign();

  /// @brief デストラクタ
  virtual
  ~ElbContAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbContAssign の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  const ElbContAssign*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すポインタ
  ElbContAssign* mNext;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を得る．
inline
const ElbContAssign*
ElbContAssign::next() const
{
  return mNext;
}

END_NAMESPACE_YM_VERILOG

#endif // ELBCONTASSIGN_H
