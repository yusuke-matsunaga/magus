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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // V1 側のノード
  BmNode* mV1;

  // V2 側のノード
  BmNode* mV2;

};

END_NAMESPACE_YM_IGF

#endif // BMEDGE_H
