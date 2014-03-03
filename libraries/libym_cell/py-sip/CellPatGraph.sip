
/// @file CellPatGraph.sip
/// @brief CellPatGraph の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellPatGraph /Abstract/
{
%TypeHeaderCode
#include "cell/CellPatGraph.h"
using nsYm::CellPatGraph;
%End

public:

  /// @brief 代表関数番号を返す．
  unsigned int
  rep_id() const;

  /// @brief 根のノード番号を返す．
  unsigned int
  root_id() const;

  /// @brief 根の反転属性を返す．
  bool
  root_inv() const;

  /// @brief 入力数を返す．
  unsigned int
  input_num() const;

  /// @brief 枝数を返す．
  unsigned int
  edge_num() const;

  /// @brief 枝(の番号)を返す．
  /// @param[in] pos 位置 ( 0 <= pos < edge_num() )
  unsigned int
  edge(unsigned int pos) const;


private:

  ~CellPatGraph();

};