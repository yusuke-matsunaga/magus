#ifndef BMEDGE_H
#define BMEDGE_H

/// @file BmEdge.h
/// @brief BmEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class BmNode;

//////////////////////////////////////////////////////////////////////
/// @class BmEdge BmEdge.h "BmEdge.h"
/// @brief BmGraph の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class BmEdge
{
public:

  /// @brief コンストラクタ
  /// @param[in] v1 V1 側のノード
  /// @param[in] v2 V2 側のノード
  BmEdge(BmNode* v1,
	 BmNode* v2);

  /// @brief デストラクタ
  ~BmEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief V1 側のノードを返す．
  BmNode*
  v1();

  /// @brief V2 側のノードを返す．
  BmNode*
  v2();

  /// @brief flow を設定する．
  void
  set_flow(ymuint flow);

  /// @brief flow を返す．
  /// @note 答えは 0 か 1
  ymuint
  flow();

  /// @brief residual を返す．
  /// @note 答えは 0 か 1
  ymuint
  residual();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // V1 側のノード
  BmNode* mV1;

  // V2 側のノード
  BmNode* mV2;

  // flow を組にしたもの
  ymuint32 mFlow;

  // visited flag
  bool mVisited;

};

END_NAMESPACE_YM_IGF

#endif // BMEDGE_H
