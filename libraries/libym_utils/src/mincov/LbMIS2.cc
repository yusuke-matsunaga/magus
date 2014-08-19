
/// @file LbMIS2.cc
/// @brief LbMIS2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMIS2.h"
#include "McMatrix.h"
#include "MisNode.h"
#include "MisNodeHeap.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMIS2
//////////////////////////////////////////////////////////////////////

// @brief 下限を求める．
// @param[in] matrix 対象の行列
// @return 下限値
ymuint32
LbMIS2::operator()(const McMatrix& matrix)
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

  MisNodeHeap node_heap(rs);

  void* p = alloc.get_memory(sizeof(MisNode*) * rs);
  MisNode** node_array = new (p) MisNode*[rs];
  for (ymuint i = 0; i < rs; ++ i) {
    node_array[i] = NULL;
  }

  ymuint idx = 0;
  for (const McRowHead* row1 = matrix.row_front();
       !matrix.is_row_end(row1); row1 = row1->next()) {
    ymuint row_pos = row1->pos();
    MisNode* node = node_heap.node(idx);
    ++ idx;
    node->set(row_pos);
    node_array[row_pos] = node;
  }

  // 同じ列を共有する行の関係を Node::mAdjLink で表す．
  // node1 と列を共有する行の Node が node1->mAdjLink[0:node1->mAdjNum -1]
  // に入る．
  // node1->mNum も node1->mAdjNum で初期化される．
  void* r = alloc.get_memory(sizeof(ymuint32) * rn);
  ymuint32* row_list = new (r) ymuint32[rn];
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
    ymuint row_list_idx = 0;
    for (const McCell* cell1 = row1->front();
	 !row1->is_end(cell1); cell1 = cell1->row_next()) {
      const McColHead* col1 = matrix.col(cell1->col_pos());
      for (const McCell* cell2 = col1->front();
	   !col1->is_end(cell2); cell2 = cell2->col_next()) {
	ymuint row_pos2 = cell2->row_pos();
	if ( matrix.row(row_pos2)->mWork == 0 ) {
	  matrix.row(row_pos2)->mWork = 1;
	  row_list[row_list_idx] = row_pos2;
	  ++ row_list_idx;
	}
      }
    }
    MisNode* node1 = node_array[row_pos1];
    void* p = alloc.get_memory(sizeof(MisNode*) * row_list_idx);
    MisNode** adj_link = new (p) MisNode*[row_list_idx];
    for (ymuint i = 0; i < row_list_idx; ++ i) {
      MisNode* node2 = node_array[row_list[i]];
      adj_link[i] = node2;
    }
    node1->set_adj_link(row_list_idx, adj_link);

    node_heap.put(node1);
  }

  // 未処理の MisNode のうち Node::mNum が最小のものを取り出し，解に加える．
  // 実際にはその行を被覆する列の最小コストを解に足す．
  ymuint32 cost = 0;
  while ( !node_heap.empty() ) {
    MisNode* best_node = node_heap.get_min();

    // best_node に対応する行を被覆する列の最小コストを求める．
    ymuint32 min_cost = UINT_MAX;
    const McRowHead* row = matrix.row(best_node->row_pos());
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint32 cpos = cell->col_pos();
      if ( min_cost > matrix.col_cost(cpos) ) {
	min_cost = matrix.col_cost(cpos);
      }
    }
    cost += min_cost;

    for (ymuint i = 0; i < best_node->adj_size(); ++ i) {
      // best_node に隣接しているノードも処理済みとする．
      MisNode* node2 = best_node->adj_node(i);
      if ( !node2->deleted() ) {
	node_heap.delete_node(node2);
	// さらにこのノードに隣接しているノードの mNum を減らす．
	for (ymuint j = 0; j < node2->adj_size(); ++ j) {
	  MisNode* node3 = node2->adj_node(j);
	  if ( !node3->deleted() ) {
	    node3->dec_adj_num();
	    node_heap.update(node3);
	  }
	}
      }
    }
  }

  return cost;
}

END_NAMESPACE_YM_MINCOV
