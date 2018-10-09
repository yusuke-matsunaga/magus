
/// @file MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_LUTMAP

// コンストラクタ
MapRecord::MapRecord()
{
}

// @brief コピーコンストラクタ
MapRecord::MapRecord(const MapRecord& src) :
  mCutArray(src.mCutArray)
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
// @param[in] sbjgraph サブジェクトグラフ
void
MapRecord::init(const SbjGraph& sbjgraph)
{
  mCutArray.clear();
  mCutArray.resize(sbjgraph.node_num(), nullptr);
}

END_NAMESPACE_LUTMAP
