#ifndef YM_CELL_CELLCLASS_H
#define YM_CELL_CELLCLASS_H

/// @file libym_cell/CellClass.h
/// @brief CellClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_logic/npn_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellClass CellClass.h "ym_cell/CellClass.h"
/// @brief NPN同値なセルグループの集合を表すクラス
//////////////////////////////////////////////////////////////////////
class CellClass
{
public:

  /// @brief コンストラクタ
  CellClass() { }

  /// @brief デストラクタ
  virtual
  ~CellClass() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 一般的な情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  /// @note CellLibrary::npn_class(id) で返されるオブジェクトの id() は id となる．
  virtual
  ymuint
  id() const = 0;

  /// @brief 同位体変換の個数を得る．
  /// @note 恒等変換は含まない．
  virtual
  ymuint
  idmap_num() const = 0;

  /// @brief 同位体変換を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < idmap_num() )
  virtual
  const NpnMapM&
  idmap(ymuint pos) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに属しているセルグループの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CellGroup の数を返す．
  virtual
  ymuint
  group_num() const = 0;

  /// @brief CellGroup を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  virtual
  const CellGroup*
  cell_group(ymuint pos) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLCLASS_H
