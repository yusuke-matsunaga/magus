
/// @file CellMap.cc
/// @brief CellMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.

#include <json/json.h>
#include "CellMap.h"
#include "AreaCover.h"
#include "SbjGraph.h"
#include "Bn2Sbj.h"


BEGIN_NAMESPACE_MAGUS

// @brief 面積最小化 DAG covering のヒューリスティック関数
BnNetwork
CellMap::area_map(
  const BnNetwork& src_network,
  const ClibCellLibrary& cell_library,
  const string option
)
{
  SbjGraph sbjgraph;
  Bn2Sbj::convert(src_network, sbjgraph);

  Json::Reader reader;
  Json::Value js_obj;
  reader.parse(option, js_obj);

  CELLMAP_NAMESPACE::AreaCover area_cover;
  return area_cover(sbjgraph, cell_library, js_obj);
}

END_NAMESPACE_MAGUS
