#ifndef PYINPUTPIN_H
#define PYINPUTPIN_H

/// @file PyInputPin.h
/// @brief PyInputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyInputPin PyInputPin.h "PyInputPin.h"
/// @brief 入力ピン用の PyPin
//////////////////////////////////////////////////////////////////////
class PyInputPin :
  public PyPin
{
public:

  /// @brief コンストラクタ
  PyInputPin(const CellPin* pin);

  /// @brief デストラクタ
  virtual
  ~PyInputPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ピン番号を得る．
  virtual
  PyObject*
  input_id();

  /// @brief 負荷容量を返す．
  virtual
  PyObject*
  capacitance();

  /// @brief 立ち上がりの負荷容量を返す．
  virtual
  PyObject*
  rise_capacitance();

  /// @brief 立ち下がりの負荷容量を返す．
  virtual
  PyObject*
  fall_capacitance();

};

END_NAMESPACE_YM

#endif // PYINPUTPIN_H
