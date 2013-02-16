#ifndef PYLIBRARY_H
#define PYLIBRARY_H

/// @file PyLibrary.h
/// @brief PyLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyLibrary PyLibrary.h "PyLibrary.h"
/// @brief CellLibraryObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyLibrary
{
public:

  /// @brief コンストラクタ
  PyLibrary();

  /// @brief デストラクタ
  ~PyLibrary();


public:

  /// @brief Cell のポインタから CellObject を得る．
  PyObject*
  get_Cell(const Cell* cell);

  /// @brief CellGroup のポインタから CellGroupObject を得る．
  PyObject*
  get_CellGroup(const CellGroup* cell_group);

  /// @brief CellClass のポインタから CellClassObject を得る．
  PyObject*
  get_CellClass(const CellClass* cell_class);

  /// @brief CellLutTemplate のポインタから CellLutTemplateObject を得る．
  PyObject*
  get_CellLutTemplate(const CellLutTemplate* cell_lut_template);

  /// @brief Cell


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ympuint から PyObject* を返す．
  /// @note なければエラーとなる．
  PyObject*
  get_obj(ympuint ptr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 任意のポインタから PyObject* を取り出すためのハッシュ表
  hash_map<ympuint, PyObject*> mObjMap;

};

END_NAMESPACE_YM

#endif // PYLIBRARY_H
