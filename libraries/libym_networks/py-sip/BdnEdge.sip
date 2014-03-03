
/// @file BdnEdge.sip
/// @brief BdnEdge の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnEdge
{
%TypeHeaderCode
#include "networks/BdnNode.h"
using nsYm::BdnEdge;
%End

public:

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const BdnNode*
  from() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const BdnNode*
  to() const;

  /// @brief 出力側のノードの何番目の入力かを示す．
  unsigned int
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;


private:

  BdnEdge();

  ~BdnEdge();

};
