#ifndef PYCELL_H
#define PYCELL_H

/// @file PyCell.h
/// @brief PyCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyCell PyCell.h "Pycell.h"
/// @brief CellObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyCell
{
public:

  /// @brief コンストラクタ
  PyCell(const Cell* cell);

  /// @brief デストラクタ
  ~PyCell();


public:

  /// @brief CellPin のポインタから CellPinObject を得る．
  PyObject*
  get_CellPin(const CellPin* pin);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ympuint から PyObject* を返す．
  /// @note なければ NULL となる．
  PyObject*
  get_obj(ympuint ptr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 任意のポインタから PyObject* を取り出すためのハッシュ表
  hash_map<ympuint, PyObject*> mObjMap;

  // ID 番号を表すオブジェクト
  PyObject* mId;

  // 名前を表すオブジェクト
  PyObject* mName;

  // area を表すオブジェクト
  PyObject* mArea;

};

END_NAMESPACE_YM

#endif // PYCELL_H
