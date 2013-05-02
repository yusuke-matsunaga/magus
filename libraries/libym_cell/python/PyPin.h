#ifndef PYPIN_H
#define PYPIN_H

/// @file PyPin.h
/// @brief PyPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyPin PyPin.h "PyPin.h"
/// @brief CellPinObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyPin
{
public:

  /// @brief コンストラクタ
  PyPin(const CellPin* pin);

  /// @brief デストラクタ
  virtual
  ~PyPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief CellPin を得る．
  const CellPin*
  pin();

  /// @brief ピン番号を得る．
  PyObject*
  id();

  /// @brief ピン名を得る．
  PyObject*
  name();


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
  virutal
  PyObject*
  min_transition();


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

  // もとの CellPin
  const CellPin* mPin;

  // ピン番号
  PyObject* mId;

  // ピン名
  PyObject* mName;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellPin を得る．
inline
const CellPin*
PyPin::pin()
{
  return mPin;
}

// @brief ピン番号を得る．
inline
PyObject*
PyPin::id()
{
  return mId;
}

// @brief ピン名を得る．
inline
PyObject*
PyPin::name()
{
  return mName;
}


END_NAMESPACE_YM

#endif // PYPIN_H
