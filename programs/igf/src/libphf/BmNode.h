#ifndef BMNODE_H
#define BMNODE_H

/// @file BmNode.h
/// @brief BmNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class BmEdge;

//////////////////////////////////////////////////////////////////////
/// @class BmNode BmNode.h "BmNode.h"
/// @brief BmGraph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class BmNode
{
  friend class BmGraph;

public:

  /// @brief コンストラクタ
  BmNode();

  /// @brief デストラクタ
  ~BmNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をセットする．
  /// @param[in] type 種類 ( 0 or 1 )
  /// @param[in] id ID番号
  void
  set(ymuint type,
      ymuint id);

  /// @brief 種類を返す．
  ymuint
  type() const;

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 接続している枝を返す．
  ymuint
  edge_num() const;

  /// @brief 枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
  BmEdge*
  edge(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号と種類(1bit)をパックしたもの
  ymuint32 mIdType;

  // 枝のリスト
  vector<BmEdge*> mEdgeList;

};

END_NAMESPACE_YM_IGF

#endif // BMNODE_H
