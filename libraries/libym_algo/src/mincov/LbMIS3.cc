
/// @file LbMIS3.cc
/// @brief LbMIS3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS3.h"
#include "McMatrix.h"
#include "YmAlgo/Graph.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMIS3
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
ymuint32
LbMIS3::operator()(const McMatrix& matrix)
{
  if ( matrix.row_num() == 0 ) {
    return 0;
  }

  // MIS を用いた下限
  // 各行に対応する Node というオブジェクトを作る．
  // ndoe_array[row_pos] に row_pos の行の Node が入る．
  // top から Node::mNext を使ってリンクとリストを作る．
  SimpleAlloc alloc;
  ymuint rs = matrix.row_size();
  ymuint rn = matrix.row_num();

  Graph graph(rs);

  void* p = alloc.get_memory(sizeof(ymuint) * rs);
  ymuint* row_map = new (p) ymuint[rs];
  for (ymuint i = 0; i < rs; ++ i) {
    row_map[i] = 0;
  }

  ymuint idx = 0;
  for (const McRowHead* row1 = matrix.row_front();
       !matrix.is_row_end(row1); row1 = row1->next()) {
    ymuint row_pos = row1->pos();
    row_map[row_pos] = idx;
    ++ idx;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  for (const McRowHead* row1 = matrix.row_front();
       !matrix.is_row_end(row1); row1 = row1->next()) {
    // マークを消す．
    // 結構めんどくさいけど効率はいい
    for (const McCell* cell1 = row1->front();
	 !row1->is_end(cell1); cell1 = cell1->row_next()) {
      const McColHead* col1 = matrix.col(cell1->col_pos());
      for (const McCell* cell2 = col1->front();
	   !col1->is_end(cell2); cell2 = cell2->col_next()) {
	ymuint row_pos = cell2->row_pos();
	matrix.row(row_pos)->mWork = 0;
      }
    }
    // マークを用いて隣接関係を作る．
    ymuint row_pos1 = row1->pos();
    ymuint id1 = row_map[row_pos1];
    ymuint row_list_idx = 0;
    for (const McCell* cell1 = row1->front();
	 !row1->is_end(cell1); cell1 = cell1->row_next()) {
      const McColHead* col1 = matrix.col(cell1->col_pos());
      for (const McCell* cell2 = col1->front();
	   !col1->is_end(cell2); cell2 = cell2->col_next()) {
	ymuint row_pos2 = cell2->row_pos();
	if ( matrix.row(row_pos2)->mWork == 0 ) {
	  matrix.row(row_pos2)->mWork = 1;
	  ymuint id2 = row_map[row_pos2];
	  graph.connect(id1, id2);
	}
      }
    }
  }

  // 各行を被覆する列の最小コストを求める．
  for (const McRowHead* row1 = matrix.row_front();
       !matrix.is_row_end(row1); row1 = row1->next()) {
    ymuint row_pos = row1->pos();
    ymuint id = row_map[row_pos];
    ymuint32 min_cost = UINT_MAX;
    for (const McCell* cell = row1->front();
	 !row1->is_end(cell); cell = cell->row_next()) {
      ymuint32 cpos = cell->col_pos();
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
  }

  vector<ymuint> node_set;
  max_clique(graph, node_set);

  ymuint cost = 0;

  return cost;
}

END_NAMESPACE_YM_MINCOV
