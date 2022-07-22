
/// @file PatHash.cc
/// @brief PatHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "PatHash.h"
#include "FraigNode.h"
#include "FraigMgrImpl.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
// PatHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatHash::PatHash()
{
}

// @brief デストラクタ
PatHash::~PatHash()
{
}

// @brief 同じパタン造を持つノードを探す．
bool
PatHash::find(
  FraigNode* node,
  FraigMgrImpl& mgr,
  FraigHandle& ans
)
{
  bool go_on;
  do { // do-while() は今風の C++ のスコープに合っていない．
    go_on = false;
    SizeType pos = node->pat_hash() % mHashSize;
    bool inv0 = node->pat_hash_inv();
    for ( FraigNode* node1 = mTable[pos];
	  node1; node1 = node1->mLink2 ) {
      bool inv = node1->pat_hash_inv() ^ inv0;
      bool retry = false;
      bool stat = mgr.compare_node(node1, node, inv, retry);
      if ( stat ) {
	ans = FraigHandle(node1, inv);
	return true;
      }
      if ( retry ) {
	go_on = true;
	break;
      }
    }
  } while ( go_on );

  return false;
}

// @brief ノードを追加する．
void
PatHash::add(
  FraigNode* node
)
{
  if ( mNum >= mNextLimit ) {
    // テーブルを拡大して再ハッシュする．
    auto old_table = mTable;
    auto old_size = mHashSize;
    alloc_table(old_size * 2);
    for ( int pos: Range(old_size) ) {
      for ( auto node1 = old_table[pos]; node1; ) {
	auto tmp = node1;
	node1 = tmp->mLink2;
	SizeType pos1 = tmp->pat_hash() % mHashSize;
	tmp->mLink2 = mTable[pos1];
	mTable[pos1] = tmp;
      }
    }
    delete [] old_table;
  }

  // ハッシュ表に登録する．
  SizeType pos1 = node->pat_hash() % mHashSize;
  node->mLink2 = mTable[pos1];
  mTable[pos1] = node;
  ++ mNum;
}

END_NAMESPACE_FRAIG
