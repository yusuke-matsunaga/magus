#ifndef FLOWNODEHEAP_H
#define FLOWNODEHEAP_H

/// @file FlowNodeHeap.h
/// @brief FlowNodeHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

class FlowNode;

//////////////////////////////////////////////////////////////////////
/// @class FlowNodeHeap FlowNodeHeap.h "FlowNodeHeap.h"
/// @brief FlowNode 用のヒープ木
//////////////////////////////////////////////////////////////////////
class FlowNodeHeap
{
public:

  /// @brief コンストラクタ
  FlowNodeHeap();

  /// @brief デストラクタ
  ~FlowNodeHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空にする．
  void
  clear();

  /// @brief 空の時に true を返す．
  bool
  empty() const;

  /// @brief 要素を追加する．
  void
  put(FlowNode* node);

  /// @brief 最小要素を取り出す．
  FlowNode*
  getmin();

  /// @brief 要素を適切な位置に移動する．
  void
  move(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の位置の要素を下に移動する．
  void
  move_down(ymuint pos);

  /// @brief 引数の位置の要素を上に移動する．
  void
  move_up(ymuint pos);

  /// @brief 配列にセットする．
  void
  set_pos(FlowNode* node,
	  ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの配列
  vector<FlowNode*> mArray;

};

END_NAMESPACE_YM

#endif // FLOWNODEHEAP_H
