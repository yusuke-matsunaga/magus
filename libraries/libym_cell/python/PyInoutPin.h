#ifndef PYINOUTPIN_H
#define PYINOUTPIN_H

/// @file PyInoutPin.h
/// @brief PyInoutPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyOutputPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyInoutPin PyInoutPin.h "PyInoutPin.h"
/// @brief 入出力ピン用の PyPin
//////////////////////////////////////////////////////////////////////
class PyInoutPin :
  public PyOutputPin
{
public:

  /// @brief コンストラクタ
  PyInoutPin(const CellPin* pin);

  /// @brief デストラクタ
  virtual
  ~PyInoutPin();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン番号
  PyObject* mInputId;

  // 負荷容量
  PyObject* mCapacitance;

  // 立ち上がりの負荷容量
  PyObject* mRiseCapacitance;

  // 立ち下がりの負荷容量
  PyObject* mFallCapacitance;

};

END_NAMESPACE_YM

#endif // PYINOUTPIN_H
