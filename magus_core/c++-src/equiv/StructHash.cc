
/// @file StructHash.cc
/// @brief StructHash の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "StructHash.h"
#include "FraigNode.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_EQUIV

//////////////////////////////////////////////////////////////////////
// StructHash
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StructHash::StructHash()
{
}

// @brief デストラクタ
StructHash::~StructHash()
{
}

// @brief 同じ構造を持つノードを探す．
// @param[in] handle1, handle2 ファンインのハンドル
// @param[out] ans 答のハンドル
// @retval true ノードが見つかった．
// @retval false ノードが見つからなかった．
bool
StructHash::find(FraigHandle handle1,
		 FraigHandle handle2,
		 FraigHandle& ans)
{
  SizeType pos = hash_func(handle1, handle2) % mHashSize;
  for ( auto node1 = mTable[pos]; node1; node1 = node1->mLink1 ) {
    if ( node1->fanin0_handle() == handle1 &&
	 node1->fanin1_handle() == handle2 ) {
      // 同じノードがあった．
      ans = node1->rep_handle();
      return true;
    }
  }
  return false;
}

// @brief ノードを追加する．
// @param[in] node 追加するノード
void
StructHash::add(FraigNode* node)
{
  if ( mNum >= mNextLimit ) {
    // テーブルを拡大して再ハッシュする．
    auto old_table = mTable;
    auto old_size = mHashSize;
    alloc_table(old_size * 2);
    for ( int pos: Range(old_size) ) {
      for ( auto node1 = old_table[pos]; node1; ) {
	auto tmp = node1;
	node1 = tmp->mLink1;
	SizeType pos1 = hash_func(tmp->fanin0_handle(), tmp->fanin1_handle()) % mHashSize;
	tmp->mLink1 = mTable[pos1];
	mTable[pos1] = tmp;
      }
    }
    delete [] old_table;
  }

  // 構造ハッシュ表に登録する．
  FraigHandle handle1 = node->fanin0_handle();
  FraigHandle handle2 = node->fanin1_handle();
  SizeType pos = hash_func(handle1, handle2) % mHashSize;
  node->mLink1 = mTable[pos];
  mTable[pos] = node;
  ++ mNum;
}

END_NAMESPACE_EQUIV
