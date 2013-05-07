#ifndef PYOUTPUTPIN_H
#define PYOUTPUTPIN_H

/// @file PyOutputPin.h
/// @brief PyOutputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyPin.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyOutputPin PyOutputPin.h "PyOutputPin.h"
/// @brief 出力ピン用の PyPin
//////////////////////////////////////////////////////////////////////
class PyOutputPin :
  public PyPin
{
public:

  /// @brief コンストラクタ
  PyOutputPin(const CellPin* pin);

  /// @brief デストラクタ
  virtual
  ~PyOutputPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力ピン番号を返す．
  virtual
  PyObject*
  output_id();

  /// @brief 機能を表す論理式を返す．
  virtual
  PyObject*
  function();

  /// @brief three_state 論理式を返す．
  virtual
  PyObject*
  three_state();

  /// @brief 最大ファンアウト容量を返す．
  virtual
  PyObject*
  max_fanout();

  /// @brief 最小ファンアウト容量を返す．
  virtual
  PyObject*
  min_fanout();

  /// @brief 最大負荷容量を返す．
  virtual
  PyObject*
  max_capacitance();

  /// @brief 最小負荷容量を返す．
  virtual
  PyObject*
  min_capacitance();

  /// @brief 最大遷移時間を返す．
  virtual
  PyObject*
  max_transition();

  /// @brief 最小遷移時間を返す．
  virtual
  PyObject*
  min_transition();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 機能を表す論理式
  PyObject* mFunction;

  // three_state 条件を表す論理式
  PyObject* mThreeState;

};

END_NAMESPACE_YM

#endif // PYOUTPUTPIN_H
