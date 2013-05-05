#ifndef PYLUTTEMPLATE_H
#define PYLUTTEMPLATE_H

/// @file PyLutTemplate.h
/// @brief PyLutTemplate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyLutTemplate PyLutTemplate.h "PyLutTemplate.h"
/// @brief CellLutTemplate を表すための補助クラス
//////////////////////////////////////////////////////////////////////
class PyLutTemplate
{
public:

  /// @brief コンストラクタ
  PyLutTemplate(const CellLutTemplate* lut_template);

  /// @brief デストラクタ
  ~PyLutTemplate();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CellLutTemplate を返す．
  const CellLutTemplate*
  lut_template();

  /// @brief 名前を返す．
  PyObject*
  name();

  /// @brief 変数型を返す．
  /// @param[in] var 変数番号
  PyObject*
  variable_type(ymuint var);

  /// @brief デフォルトインデックス値を返す．
  PyObject*
  index(ymuint var,
	ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CellLutTemplate
  const CellLutTemplate* mLutTemplate;

  // 名前
  PyObject* mName;

  // 変数型の配列
  PyObject** mVariableTypeArray;

  // デフォルトインデックス値の配列
  PyObject*** mIndexArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellLutTemplate を返す．
inline
const CellLutTemplate*
PyLutTemplate::lut_template()
{
  return mLutTemplate;
}

// @brief 名前を返す．
inline
PyObject*
PyLutTemplate::name()
{
  return mName;
}

END_NAMESPACE_YM

#endif // PYLUTTEMPLATE_H
