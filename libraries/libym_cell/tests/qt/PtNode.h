#ifndef PTNODE_H
#define PTNODE_H

/// @file libym_cell/tests/dotlib_ptview/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include "dotlib/dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h
/// @brief dotlib のパース木を表示するためのモデル
//////////////////////////////////////////////////////////////////////
class PtNode
{
  friend class ParseTreeModel;

public:

  /// @brief コンストラクタ
  PtNode();

  /// @brief デストラクタ
  virtual
  ~PtNode();


public:

  /// @brief 親のインデックスを返す．
  QModelIndex
  parent_index() const;

  /// @brief 子供の数を返す．
  virtual
  int
  child_num() const = 0;

  /// @brief 子供を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
  virtual
  PtNode*
  child(int pos) const = 0;

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const = 0;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  QModelIndex mParentIndex;

};

END_NAMESPACE_YM_DOTLIB

#endif // PTNODE_H
