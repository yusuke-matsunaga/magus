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

END_NAMESPACE_YM

#endif // PYLUTTEMPLATE_H
