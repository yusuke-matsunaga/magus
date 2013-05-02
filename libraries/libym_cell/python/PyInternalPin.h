#ifndef PYINTERNALPIN_H
#define PYINTERNALPIN_H

/// @file PyInternalPin.h
/// @brief PyInternalPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyInternalPin PyInternalPin.h "PyInternalPin.h"
/// @brief 内部ピン用の PyPin
//////////////////////////////////////////////////////////////////////
class PyInternalPin :
  public PyPin
{
public:

  /// @brief コンストラクタ
  PyInternalPin(const CellPin* pin);

  /// @brief デストラクタ
  virtual
  ~PyInternalPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部ピン番号を返す．
  virtual
  PyObject*
  internal_id();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部ピン番号
  PyObject* mInternalId;

};

END_NAMESPACE_YM

#endif // PYINTERNALPIN_H
