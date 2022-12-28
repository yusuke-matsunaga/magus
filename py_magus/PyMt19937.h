#ifndef PYMT19937_H
#define PYMT19937_H

/// @file PyMt19937.h
/// @brief PyMt19937 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "PyBase.h"
#include <random>


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class PyMt19937 PyMt19937.h "PyMt19937.h"
/// @brief mt19937 を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PyMt19937 :
  public PyBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief mt19937 を表す PyObject から mt19937 を取り出す．
  /// @return 変換が成功したら true を返す．
  ///
  /// エラーの場合にはPython例外がセットされる．
  static
  bool
  FromPyObject(
    PyObject* obj,         ///< [in] Color を表す PyObject
    std::mt19937*& mt19937 ///< [out] 変換された mt19937 を格納する変数
  );

  /// @brief PyObject が mt19937 タイプか調べる．
  static
  bool
  _check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief mt19937 を表す PyObject から mt19937 を取り出す．
  /// @return mt19937 を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  std::mt19937*
  _get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief mt19937 を表す PyObject に値を設定する．
  ///
  /// Check(obj) == true であると仮定している．
  static
  void
  _put(
    PyObject* obj,          ///< [in] 対象の PyObject
    const std::mt19937& ban ///< [in] 設定する値
  );

  /// @brief mt19937 を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_MAGUS

#endif // PYMT19937_H
