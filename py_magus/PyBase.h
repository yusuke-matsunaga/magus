#ifndef PYBASE_H
#define PYBASE_H

/// @file PyBase.h
/// @brief PyBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "magus.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class PyBase PyBase.h "PyBase.h"
/// @brief Python 拡張用の基底クラス
//////////////////////////////////////////////////////////////////////
class PyBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型オブジェクトを登録する．
  static
  bool
  reg_type_obj(
    PyObject* m,       ///< [in] モジュールオブジェクト
    const char* name,  ///< [in] 名前
    PyObject* type_obj ///< [in] 型オブジェクト
  )
  {
    Py_INCREF(type_obj);
    if ( PyModule_AddObject(m, name, type_obj) < 0 ) {
      Py_DECREF(type_obj);
      return false;
    }
    return true;
  }

  /// @brief PyObject から文字列を取り出す．
  /// @return 変換が成功したら true を返す．
  ///
  /// 変換が失敗しても Python 例外を設定しない．
  static
  bool
  StringFromPyObject(
    PyObject* obj, ///< [in] 対象のオブジェクト
    string& val    ///< [out] 変換した値を格納するオブジェクト
  )
  {
    if ( PyUnicode_Check(obj) ) {
      auto obj2 = PyUnicode_EncodeLocale(obj, nullptr);
      val = string{PyBytes_AsString(obj2)};
      return true;
    }
    return false;
  }

};

END_NAMESPACE_MAGUS

#endif // PYBASE_H
