#ifndef YM_CELL_CELLLUT_H
#define YM_CELL_CELLLUT_H

/// @file　ym_cell/CellLut.h
/// @brief CellLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLutTemplate CellLut.h "ym_cell/CellLut.h"
/// @brief ルックアップテーブルのテンプレートを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLutTemplate
{
protected:

  /// @brief デストラクタ
  virtual
  ~CellLutTemplate() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  const char*
  name() const = 0;

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const = 0;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  tCellVarType
  variable_type(ymuint32 var) const = 0;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const = 0;

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class CellLut CellLut.h "ym_cell/CellLut.h"
/// @brief ルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CellLut
{
protected:

  /// @brief デストラクタ
  virtual
  ~CellLut() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const = 0;

  /// @brief テンプレート名の取得
  virtual
  const char*
  template_name() const = 0;

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const = 0;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  tCellVarType
  variable_type(ymuint32 var) const = 0;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const = 0;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const = 0;

  /// @brief 格子点の値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  grid_value(const vector<ymuint32>& pos_array) const = 0;

  /// @brief 値の取得
  /// @param[in] val_array 入力の値の配列
  /// @note val_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<double>& val_array) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLUT_H
