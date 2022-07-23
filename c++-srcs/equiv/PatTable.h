#ifndef PATTABLE_H
#define PATTABLE_H

/// @file PatTable.h
/// @brief PatTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigNode.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class PatHash PatTable.h "PatTable.h"
/// @brief FraigNode のパタンハッシュ関数
//////////////////////////////////////////////////////////////////////
class PatHash
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief FraigNode のパタンハッシュ関数
  SizeType
  operator()(
    const FraigNode* node ///< [in] 対象のノード
  ) const
  {
    return node->pat_hash();
  }
};


//////////////////////////////////////////////////////////////////////
/// @class PatEq PatTable.h "PatTable.h"
/// @brief FraigNode のパタンハッシュ用等価比較関数
//////////////////////////////////////////////////////////////////////
class PatEq
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief FraigNode のパタン比較関数
  bool
  operator()(
    const FraigNode* node1, ///< [in] ノード1
    const FraigNode* node2  ///< [in] ノード2
  ) const
  {
    bool inv = node1->pat_hash_inv() ^ node2->pat_hash_inv();
    auto p1 = node1->pat();
    auto e1 = node1->pat_end();
    auto p2 = node2->pat();
    if ( inv ) {
      for ( ; p1 != e1; ++ p1, ++ p2 ) {
	if ( *p1 != ~(*p2) ) {
	  return false;
	}
      }
    }
    else {
      for ( ; p1 != e1; ++ p1, ++ p2 ) {
	if ( *p1 != *p2 ) {
	  return false;
	}
      }
    }
    return true;
  }
};

/// @brief FraigNode のパタンハッシュテーブル
using PatTable = std::unordered_multiset<FraigNode*, PatHash, PatEq>;

END_NAMESPACE_FRAIG

#endif // PATTABLE_H
