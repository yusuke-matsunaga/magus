#ifndef YM_CELL_CELLPAT2GRAPH_H
#define YM_CELL_CELLPAT2GRAPH_H

/// @file ym_cell/CellPat2Graph.h
/// @brief CellPat2Graph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellPat2Graph CellPat2Graph.h "ym_cell/CellPat2Graph.h"
/// @brief パタングラフを表すクラス
///
/// 実際には根の反転属性と枝のリストしか持っていない．
/// その他の情報は CellPatMgr から補完する．
//////////////////////////////////////////////////////////////////////
class CellPat2Graph
{
public:

  /// @brief コンストラクタ
  CellPat2Graph() { }

  /// @brief デストラクタ
  virtual
  ~CellPat2Graph() { }


public:

  /// @brief 代表関数番号を返す．
  virtual
  ymuint
  rep_id() const = 0;

  /// @brief 根のノード番号を返す．
  virtual
  ymuint
  root_id() const = 0;

  /// @brief 根の反転属性を返す．
  virtual
  bool
  root_inv() const = 0;

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const = 0;

  /// @brief 枝数を返す．
  virtual
  ymuint
  edge_num() const = 0;

  /// @brief 枝(の番号)を返す．
  /// @param[in] pos 位置 ( 0 <= pos < edge_num() )
  virtual
  ymuint
  edge(ymuint pos) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLPAT2GRAPH_H
