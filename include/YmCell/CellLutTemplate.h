#ifndef CELL_CELLLUTTEMPLATE_H
#define CELL_CELLLUTTEMPLATE_H

/// @file YmCell/CellLutTemplate.h
/// @brief CellLutTemplate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/cell_nsdef.h"
#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLutTemplate CellLutTemplate.h "YmCell/CellLutTemplate.h"
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

END_NAMESPACE_YM_CELL

#endif // CELL_CELLLUTTEMPLATE_H
