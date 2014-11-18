﻿#ifndef YMUTILS_GCSOLVER_H
#define YMUTILS_GCSOLVER_H

/// @file YmUtils/GcSolver.h
/// @brief GcSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class GcNode;

//////////////////////////////////////////////////////////////////////
/// @class GcSolver GcSolver.h "YmUtils/GcSolver.h"
/// @brief 彩色問題を解くためのクラス
//////////////////////////////////////////////////////////////////////
class GcSolver
{
public:

  /// @brief コンストラクタ
  GcSolver();

  /// @brief デストラクタ
  ~GcSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を指定して初期化する．
  void
  init(ymuint num);

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief 2つのノードを接続する．
  /// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
  void
  connect(ymuint id1,
	  ymuint id2);

  /// @brief 彩色する．
  /// @param[in] color_group 同じ色のノード番号のリストの配列
  /// @return 彩色数を返す．
  ymuint
  coloring(vector<vector<ymuint> >& color_group);

  /// @brief グラフの内容を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに彩色して情報を更新する．
  void
  color_node(GcNode* node,
	     ymuint col);

  /// @brief saturation degree を再計算する．
  void
  update_sat_degree(GcNode* node,
		    ymuint color);

  /// @brief mCountArray をクリアする．
  void
  clear_count();


private:
  //////////////////////////////////////////////////////////////////////
  // ヒープ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヒープにノードを追加する．
  void
  push_node(GcNode* node);

  /// @brief ヒープからノードを取り出す．
  GcNode*
  pop_node();

  /// @brief ヒープ中の配列にノードをセットする．
  void
  set_node(ymuint pos,
	   GcNode* node);

  /// @brief ノードの比較関数
  static
  int
  compare(GcNode* node1,
	  GcNode* node2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの実体の配列
  GcNode* mNodeArray;

  // ノードのヒープ
  GcNode** mNodeHeap;

  // ヒープの末尾
  ymuint32 mHeapNum;

  // 現在の最大色番号
  ymuint32 mMaxColor;

  // sat degree 計算用の配列
  vector<bool> mCountArray;

};

END_NAMESPACE_YM

#endif // YMUTILS_GCSOLVER_H